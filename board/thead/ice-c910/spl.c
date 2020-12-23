/*
* Copyright (C) 2017-2020 Alibaba Group Holding Limited
*
* SPDX-License-Identifier: GPL-2.0+
*/

#include <linux/types.h>
#include <common.h>
#include <console.h>
#include <asm/csr.h>
#include <asm/io.h>
#include <spl.h>
#include <asm/spl.h>

extern char init_ddr[];
extern void cpu_clk_config(int cpu_freq);
extern void ddr_clk_config(int ddr_freq);
extern void show_sys_clk(void);

void setup_dev_pmp(void)
{
	/* peripherals: 0x3 f000 0000 ~ 0x4 0000 0000 napot rw */
	csr_write(pmpaddr2, 0x3f0000000 >> 2 | ((0x10000000 - 1) >> 3));
	csr_write(pmpcfg0, 0x88980000001b1f1d);
}

void setup_ddr_pmp(void)
{
	/* ddr: 0x0 00000000 ~ 0x1 00000000 */
	csr_write(pmpaddr0, 0x0 >> 2 | ((0x100000000 - 1) >> 3));

	/* Leave pmpaddr1 for SDRAM on which this code is running */
	/* Plus, this was set in bootrom */

	csr_write(pmpaddr3, 0x0);
	csr_write(pmpaddr4, 0x0);
	csr_write(pmpaddr5, 0x0);

	csr_write(pmpcfg0, 0x88980000001b1f1f);
}

void cpu_performance_enable(void)
{
#define CSR_MCOR         0x7c2
#define CSR_MHCR         0x7c1
#define CSR_MCCR2        0x7c3
#define CSR_MHINT        0x7c5
#define CSR_MXSTATUS     0x7c0

	csr_write(CSR_MCOR, 0x70013);
	csr_write(CSR_MCCR2, 0xe0410009);
	csr_write(CSR_MHCR, 0x11ff);
	csr_write(CSR_MXSTATUS, 0x638000);
	csr_write(CSR_MHINT, 0x16e30c);
}

void board_init_f(ulong dummy)
{
	int ddr_freq = 1600;
	int cpu_freq = 1200;
	int ret;
	void (*ddr_initialize)(void);

	setup_dev_pmp();
	cpu_clk_config(cpu_freq);

	ret = spl_early_init();
	if (ret) {
		printf("spl_early_init() failed: %d\n", ret);
		hang();
	}
	arch_cpu_init_dm();
	preloader_console_init();

	ddr_clk_config(ddr_freq);
	ddr_initialize = (void (*)(void))(init_ddr);
	ddr_initialize();
	show_sys_clk();
	setup_ddr_pmp();
}

void board_boot_order(u32 *spl_boot_list)
{
	if ((readl((void *)0x3fff72050) & 0x3) == 3) {
		debug("Wait here for JTAG/GDB connecting\n");
		asm volatile ("ebreak");
	} else {
		spl_boot_list[0] = BOOT_DEVICE_MMC1;
		cpu_performance_enable();
	}
}
