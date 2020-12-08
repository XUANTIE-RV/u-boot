/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <asm/asm.h>
#include <asm/io.h>
#include <asm/types.h>
#include <lmb.h>
#include <cpu_func.h>
#include <stdio.h>
#include <common.h>

void board_lmb_reserve(struct lmb *lmb)
{
	lmb_add(lmb, (phys_addr_t)0x0, (phys_addr_t)0x20000000);
}

static void gmac_hw_init(void)
{
	/* release phy by gpio0_0 */
	writel(0x1, (void *)0x3fff71000);
	udelay(10000);

	/* RGMII mode */
	writel(0x20202020, (void *)0x3fe8302cc);
	writel(0x07072020, (void *)0x3fe8302cf);
	writel(0x07070707, (void *)0x3fe8302d4);

	/* PHY Mode Sel */
	writel(0x1, (void *)0x3fe83025c);

	/* Enable TX_CLK PAD */
	writel(0x0, (void *)0x3fe83031c);

	/* Enable pll_div, 1000/4=250MHz */
	writel(0x4, (void *)0x3fff770cc);
	writel(0x4, (void *)0x3fff770cc);
	writel(0x80000004, (void *)0x3fff770cc);
	writel(0x80000004, (void *)0x3fff770cc);

	writel(0xa, (void *)0x3fff770d0);
	writel(0xa, (void *)0x3fff770d0);
	writel(0x8000000a, (void *)0x3fff770d0);
	writel(0x8000000a, (void *)0x3fff770d0);

	writel(0x28, (void *)0x3fff770d8);
	writel(0x28, (void *)0x3fff770d8);
	writel(0x80000028, (void *)0x3fff770d8);
	writel(0x80000028, (void *)0x3fff770d8);

	writel(0x1f, (void *)0x3fff770c4);
	writel(0x1f, (void *)0x3fff770c8);

	writel(0x13dc, (void *)0x3fff770c0);
}

static void usb_hw_init(void)
{
	unsigned int usb_clk_cfg = readl((void *)0x3fff770f0);

	usb_clk_cfg |= (1 << 17);
	usb_clk_cfg |= (1 << 16);
	usb_clk_cfg |= (1 << 7);
	usb_clk_cfg |= (1 << 15);

	writel(usb_clk_cfg, (void *)0x3fff770f0);
	udelay(10);

	writel(0x1, (void *)0x3fe83009c);
	udelay(10);

	writel(0x1, (void *)0x3fff78048);
	udelay(10);
}

static void gpu_config(void)
{
	writel(readl((void *)0x3fff77070) & ~((1 << 21) | (1 << 20)), (void *)0x3fff77070);
	udelay(100);

	writel(0x00011212, (void *)0x3fff7709c);
	udelay(100);

	writel((1 << 21) | (1 << 20), (void *)0x3fff77070);

	writel(0x0, (void *)0x3fff780a4);
	writel(0x0, (void *)0x3fff78094);
	writel(0x0, (void *)0x3fff780c0);
	udelay(100);

	writel(0x1, (void *)0x3fff78094);
	writel(0x1, (void *)0x3fff780c0);
	writel(0x1, (void *)0x3fff780a4);
	udelay(1000);

	printf("GPU ChipDate is:0x%08x\n", readl((void *)0x3fff27028));
	printf("GPU Frequency is:%dKHz\n", readl((void *)0x3fff7713c));
}

static void npu_config(void)
{
	writel(0xff, (void *)0x3fff78044);
	udelay(100);

	printf("NPU ChipDate is:0x%08x\n", readl((void *)0x3fff20028));
}

static void dpu_config(void)
{
	writel(0x10611212, (void *)0x3fff77098);
	writel(readl((void *)0x3fff77070) | (1 << 23 | 1 << 22), (void *)0x3fff77070);
	writel(0x11611212, (void *)0x3fff77098);
	writel(readl((void *)0x3fff78090) | (1 << 0), (void *)0x3fff78090);
	writel(readl((void *)0x3fff780a0) | (1 << 0), (void *)0x3fff780a0);
	writel(readl((void *)0x3fff7809c) | (1 << 0), (void *)0x3fff7809c);
	writel(readl((void *)0x3fe830700) | (1 << 2), (void *)0x3fe830700);
	writel(readl((void *)0x3fff72000) | (1 << 2), (void *)0x3fff72000);
	writel(readl((void *)0x3fff72004) | (1 << 2), (void *)0x3fff72004);

	printf("DPU ChipDate is:0x%08x\n", readl((void *)0x3fff28028));
}

static void vpu_hw_init(void)
{
	writel(readl((void *)0x3fff77074) | 0xf, (void *)0x3fff77074);
	writel(readl((void *)0x3fff78064) | (1 << 0), (void *)0x3fff78064);
}

static void dmac_hw_init(void)
{
	writel(readl((void *)0x3fff77094) | (1 << 11), (void *)0x3fff77094);
	writel(readl((void *)0x3fff77070) | (1 << 13), (void *)0x3fff77070);
	writel(readl((void *)0x3fff78050) | (1 << 0), (void *)0x3fff78050);
	writel(readl((void *)0x3fff780c4) | (1 << 0), (void *)0x3fff780c4);
}

static void sd_hw_init(void)
{
	writel(readl((void *)0x3fe83033c) | (1 << 22), (void *)0x3fe83033c);
	writel(readl((void *)0x3fff72004) | (1 << 15), (void *)0x3fff72004);
	writel(readl((void *)0x3fff72000) & ~(1 << 15), (void *)0x3fff72000);
	writel(0x0c800c80, (void *)0x3fff77078);
	writel(readl((void *)0x3fff78058) | (1 << 16), (void *)0x3fff78058);
}

static void spi_hw_init(void)
{
	writel(readl((void *)0x3fe830334) | (1 << 2) | (1 << 4), (void *)0x3fe830334);
	writel(readl((void *)0x3fe830334) | (1 << 12) | (1 << 14), (void *)0x3fe830334);
	writel(readl((void *)0x3fe830330) | (1 << 22), (void *)0x3fe830330);
}

int board_init(void)
{
	gmac_hw_init();
	usb_hw_init();
	gpu_config();
	npu_config();
	dpu_config();
	vpu_hw_init();
	dmac_hw_init();
	sd_hw_init();
	spi_hw_init();

	return 0;
}
