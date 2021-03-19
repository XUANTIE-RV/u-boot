/*
* Copyright (C) 2017-2020 Alibaba Group Holding Limited
*
* SPDX-License-Identifier: GPL-2.0+
*/

#include <common.h>
#include <mmc.h>
#include <miiphy.h>
#include <netdev.h>
#include "../common/mini_printf.h"
#include "hardware.h"

DECLARE_GLOBAL_DATA_PTR;


int dram_init(void)
{
	gd->ram_size = PHYS_SDRAM_1_SIZE;
	return 0;
}
void show_boot_progress(int val) {}

int board_early_init_f(void)
{
	return 0;
}

int dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1 + 0x14000000;
	gd->bd->bi_dram[0].size = 0x2000000;

	return 0;
}

int board_early_init_r(void)
{
	timer_init();
	return 0;
}

int board_prep_linux(bootm_headers_t *images)
{
	// anole_c810
	// # CPU cache settings	
	// set $cr31 = 0xfff
	// set $cr17 = (1<<4) | 0x3
	asm volatile("mtcr %0, cr<31, 0>\n" : : "r"(0xfff));
	asm volatile("mtcr %0, cr<17, 0>\n" : : "r"((1<<4) | 0x3));

	// # Enable L1 cache and MMU and other CPU features
	// set $cr18 = 0x187d
	asm volatile("mtcr %0, cr<18, 0>\n" : : "r"(0x187d));

	return 0;
}
