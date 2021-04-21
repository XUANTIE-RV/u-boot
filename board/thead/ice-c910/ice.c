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
}

static void npu_config(void)
{
	writel(0xff, (void *)0x3fff78044);
	udelay(100);
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
}

static void dpu_start(void)
{
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001518));
	writel(0x00071900, (void *)(0x3fff28000 + 0x00000000));
	writel(0x00070900, (void *)(0x3fff28000 + 0x00000000));
	writel(0x032f02d0, (void *)(0x3fff28000 + 0x00001430));
	writel(0x05190500, (void *)(0x3fff28000 + 0x00001440));
	writel(0x418102f8, (void *)(0x3fff28000 + 0x00001438));
	writel(0x4287850b, (void *)(0x3fff28000 + 0x00001448));
	writel(0x00000080, (void *)(0x3fff28000 + 0x00001488));
	writel(0x00000005, (void *)(0x3fff28000 + 0x000014b8));
	writel(0x00000111, (void *)(0x3fff28000 + 0x00001418));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001530));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001538));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001800));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001808));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001818));
	writel(0x028002d0, (void *)(0x3fff28000 + 0x00001810));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001508));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001510));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001528));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001a18));
	writel(0x00000001, (void *)(0x3fff28000 + 0x00001480));
	writel(0xcc000000, (void *)(0x3fff28000 + 0x000014f0));
	writel(0xcc000000, (void *)(0x3fff28000 + 0x000014e8));
	writel(0x00030000, (void *)(0x3fff28000 + 0x000014f8));
	writel(0x00000b40, (void *)(0x3fff28000 + 0x00001500));
	writel(0x00000b40, (void *)(0x3fff28000 + 0x00001408));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001600));
	writel(0x00000000, (void *)(0x3fff28000 + 0x000018c0));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001900));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001640));
	writel(0x00780140, (void *)(0x3fff28000 + 0x00001680));
	writel(0x00000020, (void *)(0x3fff28000 + 0x00001580));
	writel(0x00000000, (void *)(0x3fff28000 + 0x000016c0));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001700));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001940));
	writel(0x00500078, (void *)(0x3fff28000 + 0x000017c0));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001740));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001780));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001840));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001880));
	writel(0x00005f7f, (void *)(0x3fff28000 + 0x00001a40));
	writel(0x0000aa4f, (void *)(0x3fff28000 + 0x00001a80));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001ac0));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001b00));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001b40));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001b80));
	writel(0x80008000, (void *)(0x3fff28000 + 0x00001bc0));
	writel(0x00000153, (void *)(0x3fff28000 + 0x00001c00));

	writel(0x00000000, (void *)(0x3fff28000 + 0x00001518));
	writel(0x032f02d0, (void *)(0x3fff28000 + 0x00001430));
	writel(0x05190500, (void *)(0x3fff28000 + 0x00001440));
	writel(0x418102f8, (void *)(0x3fff28000 + 0x00001438));
	writel(0x4287850b, (void *)(0x3fff28000 + 0x00001448));
	writel(0x00000080, (void *)(0x3fff28000 + 0x00001488));
	writel(0x00000005, (void *)(0x3fff28000 + 0x000014b8));
	writel(0x00000111, (void *)(0x3fff28000 + 0x00001418));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001530));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001538));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001800));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001808));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001818));
	writel(0x028002d0, (void *)(0x3fff28000 + 0x00001810));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001508));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001510));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001528));
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001a18));
	writel(0x00000001, (void *)(0x3fff28000 + 0x00001480));
	writel(0x04000000, (void *)(0x3fff28000 + 0x000014f0));
	writel(0x04000000, (void *)(0x3fff28000 + 0x000014e8));
	writel(0x00030000, (void *)(0x3fff28000 + 0x000014f8));
	writel(0x00000b40, (void *)(0x3fff28000 + 0x00001500));
	writel(0x00000b40, (void *)(0x3fff28000 + 0x00001408));
	writel(0x03000000, (void *)(0x3fff28000 + 0x00001400));
	writel(0x18000011, (void *)(0x3fff28000 + 0x00001518));
	readl((void *)(0x3fff28000 + 0x00001518));
	writel(0x18000030, (void *)(0x3fff28000 + 0x00001518));
	writel(0x03000000, (void *)(0x3fff28000 + 0x00001400));
	writel(0x18002811, (void *)(0x3fff28000 + 0x00001518));

	writel(0, (void *)0x3fff72000);
	udelay(200 * 1000);
	writel(4, (void *)0x3fff72000);

	udelay(400 * 1000);
	writel(0x00000000, (void *)(0x3fff28000 + 0x00001518));
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

static void wifibt_hw_init(void)
{
#if defined(CONFIG_SDIO_WIFI_BT_HW)
	/*wifi power on */
	writel(readl((void *)0x3fff72004) | (1 << 5), (void *)0x3fff72004);
	writel(readl((void *)0x3fff72000) | (1 << 5), (void *)0x3fff72000);
	/*bt wake up */
	writel(readl((void *)0x3fff72004) | (1 << 7), (void *)0x3fff72004);
	writel(readl((void *)0x3fff72000) | (1 << 7), (void *)0x3fff72000);
	/*bt power on */
	writel(readl((void *)0x3fff72004) | (1 << 8), (void *)0x3fff72004);
	writel(readl((void *)0x3fff72000) | (1 << 8), (void *)0x3fff72000);
#endif
}

void show_sys_clk(void)
{
	unsigned int read = 0;

	read = *(volatile unsigned int*)(0x3fff77120);
	printf("CPU freq: %dMHz\n", read / 1000);

	read = *(volatile unsigned int*)(0x3fff7712c);
	printf("DDR freq: %dMT\n", read / 1000 * 4);

	read = *(volatile unsigned int*)(0x3fff77124);
	printf("AXI freq: %dMHz\n", read / 1000);

	read = *(volatile unsigned int*)(0x3fff77130);
	printf("AHB freq: %dMHz\n", read / 1000);

	read = *(volatile unsigned int*)(0x3fff77140);
	printf("NPU freq: %dMHz\n", read / 1000);

	read = *(volatile unsigned int*)(0x3fff7713c);
	printf("GPU freq: %dMHz\n", read / 1000);
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
	dpu_start();
	wifibt_hw_init();
	show_sys_clk();

	return 0;
}
