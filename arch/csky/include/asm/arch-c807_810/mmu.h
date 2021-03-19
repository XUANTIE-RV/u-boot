/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __MMU_H_
#define __MMU_H_

.macro  csky_mmu_disable
        movi    a3, 0x0
        mtcr    a3,cr18

.endm

#endif
