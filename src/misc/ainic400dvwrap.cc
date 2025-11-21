#include "ainic400/ainic400dvwrap.h"
#include <sys/types.h>
#include <unistd.h>
#include "param.h"

#include "ainic400/ainic400dvcore.h"
#include "ainic400/ainic400dvsymbols.h"

static pthread_once_t initOnceControl = PTHREAD_ONCE_INIT;
static ncclResult_t initResult;
struct ncclAinic400dvSymbols ainic400dvSymbols;

extern int64_t rcclParamAinic400();

ncclResult_t wrap_ainic400dv_symbols(void) {
#if defined(__HIP_PLATFORM_AMD__) || defined(__HIPCC__)
  if (rcclParamAinic400() == 1) {
    pthread_once(&initOnceControl,
                 [](){ initResult = buildAinic400dvSymbols(&ainic400dvSymbols); });
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

#define AINIC400DV_INT_CHECK_RET_ERRNO(container, internal_name, call, success_retval, name) \
  CHECK_NOT_NULL(container, internal_name); \
  int ret = container.call; \
  if (ret != success_retval) { \
    INFO(NCCL_NET, "Call to " name " failed with error %s errno %d", strerror(ret), ret); \
    return ncclSystemError; \
  } else { \
    INFO(NCCL_NET, "Call to " name " success with ret %d", ret); \
  } \
  return ncclSuccess;

ncclResult_t wrap_ainic400dv_qp_set_gda(struct ibv_qp *qp, bool enable_send, bool enable_recv) {
  if (ainic400dvSymbols.ainic400dv_internal_qp_set_gda == NULL) {
    errno = EOPNOTSUPP;
    return ncclSystemError;
  }
  AINIC400DV_INT_CHECK_RET_ERRNO(ainic400dvSymbols, ainic400dv_internal_qp_set_gda, ainic400dv_internal_qp_set_gda(qp, enable_send, enable_recv), 0, "ionic_dv_qp_set_gda");
}

ncclResult_t wrap_ainic400dv_pd_set_udma_mask(struct ibv_pd *ibpd, uint8_t udma_mask) {
  if (ainic400dvSymbols.ainic400dv_internal_pd_set_udma_mask == NULL) {
    errno = EOPNOTSUPP;
    return ncclSystemError;
  }
  AINIC400DV_INT_CHECK_RET_ERRNO(ainic400dvSymbols, ainic400dv_internal_pd_set_udma_mask, ainic400dv_internal_pd_set_udma_mask(ibpd, udma_mask), 0, "ionic_dv_pd_set_udma_mask");
}
