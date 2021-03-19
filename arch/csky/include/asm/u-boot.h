/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef _U_BOOT_H_
#define _U_BOOT_H_    1

/* Use the generic board which requires a unified bd_info */
#include <asm-generic/u-boot.h>

/* For image.h:image_check_target_arch() */
#define IH_ARCH_DEFAULT IH_ARCH_CSKY

#endif    /* _U_BOOT_H_ */
