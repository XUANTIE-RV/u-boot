/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <asm/types.h>
#include "ddr.h"

//FPGA mode
#define FPGA

int init_ddr(void)
{
#ifdef FPGA

    // release ddr
    // set *0xFFC36140=0x3ff
    u32* ddr_reg = (u32*)0xffc36140;
    *ddr_reg = 0x3ff;

#else //ASIC

#endif
    return 0;
}
