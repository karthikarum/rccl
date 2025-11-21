#ifndef NCCL_AINIC400DV_WRAP_H_
#define NCCL_AINIC400DV_WRAP_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include "ainic400/ainic400dvcore.h"
#include "core.h"
#include "ibvwrap.h"
#include <sys/types.h>
#include <unistd.h>

ncclResult_t wrap_ainic400dv_symbols(void);
/* NCCL wrappers of AINIC400 ionic direct verbs functions */
ncclResult_t wrap_ainic400dv_qp_set_gda(struct ibv_qp *ibqp, bool enable_send, bool enable_recv);
ncclResult_t wrap_ainic400dv_pd_set_udma_mask(struct ibv_pd *ibpd, uint8_t udma_mask);

#endif // NCCL_AINIC400DV_WRAP_H_
