/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <mmc.h>
#include <miiphy.h>
#include <netdev.h>
#include "gpio/gpio.h"
#include "../common/mini_printf.h"
#include "hardware.h"

DECLARE_GLOBAL_DATA_PTR;

int dram_init(void)
{
    gd->ram_size = PHYS_SDRAM_1_SIZE;
    return 0;
}
void show_boot_progress(int val)
{
}

int board_early_init_f(void)
{
    return 0;
}

int dram_init_banksize(void)
{
    gd->bd->bi_dram[0].start = PHYS_SDRAM_1 + 0x14000000;
    gd->bd->bi_dram[0].size = 0x4000000;

    return 0;
}

int board_early_init_r(void)
{
    timer_init();
    return 0;
}

