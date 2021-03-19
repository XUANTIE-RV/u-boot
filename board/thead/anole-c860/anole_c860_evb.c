/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <cpu_func.h>
#include "../common/mini_printf.h"
#include "spl/ddr.h"
#include "hardware.h"


extern void clock_init(void);


#define WDT_BASEADDR        (u32)(0xbff78000)

#define WDT_LOCK            0x00
#define WDT_OT_LOAD_H       0x04
#define WDT_OT_LOAD_L       0x08
#define WDT_EN              0x10

#define WDG_ENABLE          (0x5ada7200)
#define WDG_DISABLE         (0x1)

#define SLAVE_ENTRY_BASEADDR    (u32)(0xbfff0000)
#define SLAVE_RESET_CONTROL     (u32)(0xbff48068)
#define SLAVE_ENABLE_CONTROL    (u32)(0xbe830068)


DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_SPL_BUILD
void sdram_init(void)
{
	init_ddr();
}
#endif

int dram_init(void)
{
	gd->ram_size = PHYS_SDRAM_1_SIZE;
	return 0;
}

void board_lmb_reserve(struct lmb *lmb)
{
	lmb_add(lmb, 0x80000000, 0x20000000);
}

void show_boot_progress(int val) {}

int board_early_init_f(void)
{
	timer_init();
	clock_init();
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
	return 0;
}

int disable_slave_cpu(void)
{
	writel(0, SLAVE_RESET_CONTROL);
	return 0;
}

int set_slave_cpu_entry(phys_addr_t entry)
{
	// set slave jump addr
	writel(entry, SLAVE_ENTRY_BASEADDR);
	flush_cache(SLAVE_ENTRY_BASEADDR, 0x100);
	return 0;
}

int enable_slave_cpu(void)
{
	printf("reset slave cpu\n");
	writel(2, SLAVE_ENABLE_CONTROL);
	writel(1, SLAVE_RESET_CONTROL);
	return 0;
}

int board_prep_linux(bootm_headers_t *images)
{
	// anole_c860
	// # Setup CPU features regs
	// # Enable MMU, Icache, Dcache, Return Stack, BPB, BTB, IBTB ...
	// set $cr31 = 0x670c
	// set $cr18 = 0x1586d
	asm volatile("mtcr %0, cr<31, 0>\n" : : "r"(0x670c));
	asm volatile("mtcr %0, cr<18, 0>\n" : : "r"(0x1586d));

	// # Invalid L2 cache by 'exec l2cache.iall'
	// set *0x8f000000=0x9820c100
	// set $pc=0x8f000000
	// si
	asm volatile (".long 0x9820c100");

	// # Enable L2 cache
	// set $cr23 = 0xe0010009
	asm volatile("mtcr %0, cr<23, 0>\n" : : "r"(0xe0010009));

	return 0;
}

void board_reset(void)
{
	/* Reset the cpu by setting up the watchdog timer */
	writel(WDG_ENABLE, WDT_BASEADDR + WDT_LOCK);
	writel(0, WDT_BASEADDR + WDT_EN);
	writel(WDG_DISABLE, WDT_BASEADDR + WDT_LOCK);

	writel(WDG_ENABLE, WDT_BASEADDR + WDT_LOCK);
	writel(0, WDT_BASEADDR + WDT_OT_LOAD_H);
	writel(1000, WDT_BASEADDR + WDT_OT_LOAD_L);
	writel(1, WDT_BASEADDR + WDT_EN);
	writel(WDG_DISABLE, WDT_BASEADDR + WDT_LOCK);

	mdelay(500);
}
