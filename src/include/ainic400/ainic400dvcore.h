#ifndef NCCL_AINIC400DV_CORE_H_
#define NCCL_AINIC400DV_CORE_H_

/* Basic AINIC400 ionic direct verbs structs.
 * Needed to dynamically load AINIC400 ionic direct verbs functions without
 * explicit including of AINIC400 direct verbs header.
 */

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include "ibvwrap.h"

enum ainic400dv_reg_udma_mask {
    IONIC_UDMA_MASK_LOW    = 1,
    IONIC_UDMA_MASK_HIGH   = 2
};

#endif  // NCCL_AINIC400DV_CORE_H_
