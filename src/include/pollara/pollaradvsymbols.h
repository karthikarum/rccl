#ifndef NCCL_POLLARADV_SYMBOLS_H_
#define NCCL_POLLARADV_SYMBOLS_H_

#ifdef NCCL_BUILD_POLLARADV
#include <infiniband/ionic_dv.h>
#else
#include "pollara/pollaradvcore.h"
#endif

#include "nccl.h"

/* POLLARA ionic Direct Verbs Function Pointers*/
struct ncclPollaradvSymbols  {
  int (*pollaradv_internal_qp_set_gda)(struct ibv_qp *qp, bool enable_send, bool enable_recv);
  int (*pollaradv_internal_pd_set_udma_mask)(struct ibv_pd *ibpd, uint8_t udma_mask);
};

/* Constructs POLLARA ionic direct verbs symbols per rdma-core linking or dynamic loading mode */
ncclResult_t buildPollaradvSymbols(struct ncclPollaradvSymbols* pollaradvSymbols);

#endif  // NCCL_POLLARADV_SYMBOLS_H_
