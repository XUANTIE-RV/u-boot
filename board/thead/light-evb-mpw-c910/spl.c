/*
* Copyright (C) 2017-2020 Alibaba Group Holding Limited
*
* SPDX-License-Identifier: GPL-2.0+
*/

#include <linux/types.h>
#include <common.h>
#include <console.h>
#include <cpu_func.h>
#include <asm/csr.h>
#include <asm/io.h>
#include <asm/barrier.h>
#include <spl.h>
#include <asm/spl.h>
#include <string.h>

extern void init_ddr(void);
extern void cpu_clk_config(int cpu_freq);
extern void ddr_clk_config(int ddr_freq);
extern void show_sys_clk(void);

/* 0: 0xff e0000000 ~ 0xff e0100000 SRAM, NAPOT rwx */
/* 1: 0xff d8000000 ~ 0xff e0000000 plic, NAPOT rw */
/* 2: 0xff e0000000 ~ 0x100 00000000 peripherals, NAPOT rw */
/* 3: 0xff ffd00000 ~ 0xff ffe00000 BROM, NAPOT rx */
/* 4: 0x0 00000000 ~ 0x1 00000000 DDR, NAPOT rwx */

/* 6: 0x00000000 ~ 0x10000000 NAPOT no access */
/* 7: 0x10000000 ~ 0x10000000000 TOR no access */
void setup_ddr_pmp(void)
{
	/* sram: 0xff e0000000 ~ 0xff e0100000 */
	csr_write(pmpaddr0, 0xffe0000000 >> 2 | ((0x100000 - 1) >> 3));

	/* ddr: 0x0 00000000 ~ 0x1 00000000 */
	csr_write(pmpaddr4, 0x0 >> 2 | ((0x100000000 - 1) >> 3));

	csr_write(pmpcfg0, 0x8898001f1d1b1b1f);
}

void cpu_performance_enable(void)
{
    csr_write(CSR_MCOR, 0x70013);
    csr_write(CSR_MCCR2, 0xe0010009);
    csr_write(CSR_MHCR, 0x11ff);
    csr_write(CSR_MXSTATUS, 0x638000);
    csr_write(CSR_MHINT, 0x16e30c);
}

void board_init_f(ulong dummy)
{
	int ret;

	cpu_clk_config(0);

	ret = spl_early_init();
	if (ret) {
		printf("spl_early_init() failed: %d\n", ret);
		hang();
	}
	arch_cpu_init_dm();
	preloader_console_init();

	ddr_clk_config(0);

	init_ddr();
	udelay(600000);
	setup_ddr_pmp();
}

void board_boot_order(u32 *spl_boot_list)
{
#define SOC_OM_ADDRBASE        0xffef018010
	switch (readl((void *)SOC_OM_ADDRBASE) & 0x7) {
    case 0:
		spl_boot_list[0] = BOOT_DEVICE_MMC1;
        break;
    case 1:
        //ret = sd_boot();
        break;
    case 2:
    case 3:
        //ret = qspi_flash_boot();
        break;
    case 4:
        //ret = usb_boot();
        break;
    case 5:
		debug("Wait here for JTAG/GDB connecting\n");
		asm volatile ("ebreak");
        break;
    case 7:
        //ret = polling_boot();
		break;
	}

	cpu_performance_enable();
}
