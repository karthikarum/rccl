#include <sys/types.h>
#include <unistd.h>

#include "ainic400/ainic400dvsymbols.h"

/* Ainic400dv dynamic loading mode. Symbols are loaded from shared objects. */
#include <dlfcn.h>
#include "core.h"

// IONICDV Library versioning
#define IONIC_VERSION "IONIC_1.0"

ncclResult_t buildAinic400dvSymbols(struct ncclAinic400dvSymbols* ainic400dvSymbols) {
  static void* ainic400dvhandle = NULL;
  void* tmp;
  void** cast;

  ainic400dvhandle = dlopen("libionic.so", RTLD_NOW);
  if (!ainic400dvhandle) {
    ainic400dvhandle = dlopen("libionic.so.1", RTLD_NOW);
    if (!ainic400dvhandle) {
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

  LOAD_SYM(ainic400dvhandle, "ionic_dv_qp_set_gda", ainic400dvSymbols->ainic400dv_internal_qp_set_gda);
  LOAD_SYM(ainic400dvhandle, "ionic_dv_pd_set_udma_mask", ainic400dvSymbols->ainic400dv_internal_pd_set_udma_mask);
  INFO(NCCL_INIT, "Loaded dlvsym from libionic.so[.1]");

  return ncclSuccess;

teardown:
  ainic400dvSymbols->ainic400dv_internal_qp_set_gda = NULL;
  ainic400dvSymbols->ainic400dv_internal_pd_set_udma_mask = NULL;

  if (ainic400dvSymbols != NULL) dlclose(ainic400dvSymbols);
  return ncclSystemError;
}
