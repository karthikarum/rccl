#include <sys/types.h>
#include <unistd.h>

#include "pollara/pollaradvsymbols.h"

#ifdef NCCL_BUILD_POLLARADV
/* Pollaradv linking mode. Symbols are pointers to linked Pollara Direct Verbs */

#define ASSIGN_SYM(container, symbol, name) container->name= &symbol;

ncclResult_t buildPollaradvSymbols(struct ncclPollaradvSymbols* pollaradvSymbols) {
  ASSIGN_SYM(pollaradvSymbols, ionic_dv_qp_set_gda, pollaradv_internal_qp_set_gda);
  ASSIGN_SYM(pollaradvSymbols, ionic_dv_pd_set_udma_mask, pollaradv_internal_pd_set_udma_mask);
  return ncclSuccess;
}

#else
/* Pollaradv dynamic loading mode. Symbols are loaded from shared objects. */

#include <dlfcn.h>
#include "core.h"

// IONICDV Library versioning
#define IONIC_VERSION "IONIC_1.0"

ncclResult_t buildPollaradvSymbols(struct ncclPollaradvSymbols* pollaradvSymbols) {
  static void* pollaradvhandle = NULL;
  void* tmp;
  void** cast;

  pollaradvhandle = dlopen("libionic.so", RTLD_NOW);
  if (!pollaradvhandle) {
    pollaradvhandle = dlopen("libionic.so.1", RTLD_NOW);
    if (!pollaradvhandle) {
      INFO(NCCL_INIT, "Failed to open libionic.so[.1]");
      goto teardown;
    }
  }

#define LOAD_SYM(handle, symbol, funcptr) do {           \
    cast = (void**)&funcptr;                             \
    tmp = dlvsym(handle, symbol, IONIC_VERSION);       \
    if (tmp == NULL) {                                   \
      WARN("dlvsym failed on %s - %s version %s", symbol, dlerror(), IONIC_VERSION);  \
      goto teardown;                                     \
    } else {                                             \
      WARN("dlvsym loaded successfully for %s - version %s", symbol, IONIC_VERSION);  \
    }                                                    \
    *cast = tmp;                                         \
  } while (0)

// Attempt to load a specific symbol version - fail silently
#define LOAD_SYM_VERSION(handle, symbol, funcptr, version) do {  \
    cast = (void**)&funcptr;                                     \
    *cast = dlvsym(handle, symbol, version);                     \
    if (*cast == NULL) {                                         \
      INFO(NCCL_NET, "dlvsym failed on %s - %s version %s", symbol, dlerror(), version);  \
    }                                                            \
  } while (0)

  LOAD_SYM(pollaradvhandle, "ionic_dv_qp_set_gda", pollaradvSymbols->pollaradv_internal_qp_set_gda);
  LOAD_SYM(pollaradvhandle, "ionic_dv_pd_set_udma_mask", pollaradvSymbols->pollaradv_internal_pd_set_udma_mask);
  INFO(NCCL_INIT, "Loaded dlvsym from libionic.so[.1]");

  return ncclSuccess;

teardown:
  pollaradvSymbols->pollaradv_internal_qp_set_gda = NULL;
  pollaradvSymbols->pollaradv_internal_pd_set_udma_mask = NULL;

  if (pollaradvSymbols != NULL) dlclose(pollaradvSymbols);
  return ncclSystemError;
}

#endif
