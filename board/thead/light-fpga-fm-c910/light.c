/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/types.h>

void gmac_clk_config(void)
{
	writel(1 << 13, (void *)(0xffe7030004));
	writel(1 << 13, (void *)(0xffe7030008));
	udelay(10000);
}

void usb_clk_config(void)
{
	writel(0x0, (void *)(0xffef01406c));
	udelay(1000);
	writel(0x7, (void *)(0xffef01406c));
}

int board_init(void)
{
#define GMAC_IOPMP   0xfff7038000
	writel(0xffffffff, (void *)(GMAC_IOPMP+0xc0));

#define USB_IOPMP   0xfff703a000
	writel(0xffffffff, (void *)(USB_IOPMP+0xc0));

#define SPI_NOR_FLASH_PINMUX 0xffef01c430
	writel(readl((void *)SPI_NOR_FLASH_PINMUX) | 0x30, (void *)SPI_NOR_FLASH_PINMUX);

#define SPI_NAND_FLASH_PINMUX 0xffef01c428
	writel(readl((void *)SPI_NAND_FLASH_PINMUX) | (3<<20), (void *)SPI_NAND_FLASH_PINMUX);

	usb_clk_config();
	gmac_clk_config();

	return 0;
}
