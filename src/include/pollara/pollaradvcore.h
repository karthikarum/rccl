#ifndef NCCL_POLLARADV_CORE_H_
#define NCCL_POLLARADV_CORE_H_

/* Basic POLLARA ionic direct verbs structs.
 * Needed to dynamically load POLLARA ionic direct verbs functions without
 * explicit including of POLLARA direct verbs header.
 */

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include "ibvwrap.h"

enum pollaradv_reg_udma_mask {
    IONIC_UDMA_MASK_LOW    = 1,
    IONIC_UDMA_MASK_HIGH   = 2
};

#endif  // NCCL_POLLARADV_CORE_H_
