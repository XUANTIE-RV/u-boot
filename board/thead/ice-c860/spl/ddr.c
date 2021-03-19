/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <asm/types.h>
#include "ddr.h"

void init_ddr(void)
{
    u32* ddr_reg = (u32*)0xfff78040;
    *ddr_reg = 0xffff;
}
