#include "pollara/pollaradvwrap.h"
#include <sys/types.h>
#include <unistd.h>
#include "param.h"

#ifdef NCCL_BUILD_POLLARADV
#include <infiniband/ionic_dv.h>
#else
#include "pollara/pollaradvcore.h"
#endif
#include "pollara/pollaradvsymbols.h"

static pthread_once_t initOnceControl = PTHREAD_ONCE_INIT;
static ncclResult_t initResult;
struct ncclPollaradvSymbols pollaradvSymbols;

extern int64_t rcclParamPollaraNic();

ncclResult_t wrap_pollaradv_symbols(void) {
#if defined(__HIP_PLATFORM_AMD__) || defined(__HIPCC__)
  if (rcclParamPollaraNic() == 1) {
    pthread_once(&initOnceControl,
                 [](){ initResult = buildPollaradvSymbols(&pollaradvSymbols); });
    return initResult;
  }
#endif
  // simply return for unsupported platform/NIC.
  return ncclSuccess;
}

/* CHECK_NOT_NULL: helper macro to check for NULL symbol */
#define CHECK_NOT_NULL(container, internal_name) \
  if (container.internal_name == NULL) { \
     WARN("lib wrapper not initialized."); \
     return ncclInternalError; \
  }

#define POLLARADV_INT_CHECK_RET_ERRNO(container, internal_name, call, success_retval, name) \
  CHECK_NOT_NULL(container, internal_name); \
  int ret = container.call; \
  if (ret != success_retval) { \
    INFO(NCCL_NET, "Call to " name " failed with error %s errno %d", strerror(ret), ret); \
    return ncclSystemError; \
  } else { \
    INFO(NCCL_NET, "Call to " name " success with ret %d", ret); \
  } \
  return ncclSuccess;

ncclResult_t wrap_pollaradv_qp_set_gda(struct ibv_qp *qp, bool enable_send, bool enable_recv) {
  if (pollaradvSymbols.pollaradv_internal_qp_set_gda == NULL) {
    errno = EOPNOTSUPP;
    return ncclSystemError;
  }
  POLLARADV_INT_CHECK_RET_ERRNO(pollaradvSymbols, pollaradv_internal_qp_set_gda, pollaradv_internal_qp_set_gda(qp, enable_send, enable_recv), 0, "ionic_dv_qp_set_gda");
}

ncclResult_t wrap_pollaradv_pd_set_udma_mask(struct ibv_pd *ibpd, uint8_t udma_mask) {
  if (pollaradvSymbols.pollaradv_internal_pd_set_udma_mask == NULL) {
    errno = EOPNOTSUPP;
    return ncclSystemError;
  }
  POLLARADV_INT_CHECK_RET_ERRNO(pollaradvSymbols, pollaradv_internal_pd_set_udma_mask, pollaradv_internal_pd_set_udma_mask(ibpd, udma_mask), 0, "ionic_dv_pd_set_udma_mask");
}
