#ifndef NCCL_POLLARADV_WRAP_H_
#define NCCL_POLLARADV_WRAP_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#ifdef NCCL_BUILD_POLLARADV
#include <infiniband/ionic_dv.h>
#else
#include "pollara/pollaradvcore.h"
#endif

#include "core.h"
#include "ibvwrap.h"
#include <sys/types.h>
#include <unistd.h>

ncclResult_t wrap_pollaradv_symbols(void);
/* NCCL wrappers of Pollara ionic direct verbs functions */
ncclResult_t wrap_pollaradv_qp_set_gda(struct ibv_qp *ibqp, bool enable_send, bool enable_recv);
ncclResult_t wrap_pollaradv_pd_set_udma_mask(struct ibv_pd *ibpd, uint8_t udma_mask);

#endif // NCCL_POLLARADV_WRAP_H_
