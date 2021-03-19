/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include "ddr.h"

//FPGA mode
#define FPGA

int init_ddr(void){
#ifdef FPGA

#else //ASIC

#endif
    return 0;
}
