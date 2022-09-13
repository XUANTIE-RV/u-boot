/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __THEAD_HARDWARE_H__
#define __THEAD_HARDWARE_H__

#ifdef CONFIG_SPL_BUILD

#define GPIOA_BASEADDR       (u32)(0xf7004000)
#define GPIOB_BASEADDR       (u32)(0xf7008000)
#define GMAC_BASEADDR        (u32)(0xf7070000)
#define EMMC_BASEADDR        (u32)(0xf7080000)
#define OM_BASEADDR          (u32)(0xffc37010) // SYSREG_BASE + 0x10
#define SPI0_BASEADDR        (u32)(0xf700c000)
#define UART0_BASEADDR       (u32)(0xf7014000)
#define UART1_BASEADDR       (u32)(0xf7018000)

#else

#define GPIOA_BASEADDR       (u32)(0xb7004000)
#define GPIOB_BASEADDR       (u32)(0xb7008000)
#define GMAC_BASEADDR        (u32)(0xb7070000)
#define EMMC_BASEADDR        (u32)(0xb7080000)
#define OM_BASEADDR          (u32)(0xbfc37010) // SYSREG_BASE + 0x10
#define SPI0_BASEADDR        (u32)(0xb700c000)
#define UART0_BASEADDR       (u32)(0xb7014000)
#define UART1_BASEADDR       (u32)(0xb7018000)

#endif /* CONFIG_SPL_BUILD */

#define CONSOLE_UART_BASE   UART1_BASEADDR

#endif /* __THEAD_HARDWARE_H__ */
