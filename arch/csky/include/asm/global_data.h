/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef	__ASM_GBL_DATA_H
#define __ASM_GBL_DATA_H

/* Architecture-specific global data */
struct arch_global_data {
    long boot_hart;
    unsigned long timer_rate_hz;
    unsigned long tbu;
    unsigned long tbl;
    unsigned long lastinc;
};

#include <asm-generic/global_data.h>

#define DECLARE_GLOBAL_DATA_PTR register volatile gd_t *gd asm ("r28")

#endif /* __ASM_GBL_DATA_H */
