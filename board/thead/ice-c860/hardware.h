/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __ASM_ARCH_HARDWARE_H__
#define __ASM_ARCH_HARDWARE_H__

#ifdef CONFIG_SPL_BUILD

#define GPIOA_BASEADDR       (u32)(0xfff71000)
#define GPIOB_BASEADDR       (u32)(0xfff72000)
#define GMAC_BASEADDR        (u32)(0xfffc0000)
#define EMMC_BASEADDR        (u32)(0xfffb0000)
#define OM_BASEADDR          (u32)(0xfff72050)  /* GPIOB_BASE+0x50 */
#define SPI0_BASEADDR        (u32)(0xfff75000)
#define UART0_BASEADDR       (u32)(0xfff73000)
#define UART1_BASEADDR       (u32)(0xfff73400)
#define SYSREG_BASEADDR      (u32)(0xffff0000)
#define SLAVE_RESET_CONTROL  (u32)(0xfe830068)

#else

#define GPIOA_BASEADDR       (u32)(0xbff71000)
#define GPIOB_BASEADDR       (u32)(0xbff72000)
#define GMAC_BASEADDR        (u32)(0xbffc0000)
#define EMMC_BASEADDR        (u32)(0xbffb0000)
#define OM_BASEADDR          (u32)(0x1fbd0100)
#define SPI0_BASEADDR        (u32)(0xbff75000)
#define UART0_BASEADDR       (u32)(0xbff73000)
#define UART1_BASEADDR       (u32)(0xbff73400)
#define SYSREG_BASEADDR      (u32)(0xbfff0000)
#define SLAVE_RESET_CONTROL  (u32)(0xbe830068)

#endif /* CONFIG_SPL_BUILD */

#define CONSOLE_UART_BASE   UART0_BASEADDR

#endif /* __ASM_ARCH_HARDWARE_H__ */
