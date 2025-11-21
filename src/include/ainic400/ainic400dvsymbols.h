#ifndef NCCL_AINIC400DV_SYMBOLS_H_
#define NCCL_AINIC400DV_SYMBOLS_H_

#include "ainic400/ainic400dvcore.h"
#include "nccl.h"

/* AINIC400 ionic Direct Verbs Function Pointers*/
struct ncclAinic400dvSymbols  {
  int (*ainic400dv_internal_qp_set_gda)(struct ibv_qp *qp, bool enable_send, bool enable_recv);
  int (*ainic400dv_internal_pd_set_udma_mask)(struct ibv_pd *ibpd, uint8_t udma_mask);
};

/* Constructs AINIC400 ionic direct verbs symbols per rdma-core linking or dynamic loading mode */
ncclResult_t buildAinic400dvSymbols(struct ncclAinic400dvSymbols* ainic400dvSymbols);

#endif  // NCCL_AINIC400DV_SYMBOLS_H_
