/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __ASM_ARCH_HARDWARE_H__
#define __ASM_ARCH_HARDWARE_H__

#ifdef CONFIG_SPL_BUILD

#define GMAC_BASEADDR        (u32)(0xfffc0000)
#define EMMC_BASEADDR        (u32)(0xfffb0000)
#define SPI0_BASEADDR        (u32)(0xfff75000)
#define UART0_BASEADDR       (u32)(0xfff73000)
#define UART1_BASEADDR       (u32)(0xfff73400)

#else

#define GMAC_BASEADDR        (u32)(0xbffc0000)
#define EMMC_BASEADDR        (u32)(0xbffb0000)
#define SPI0_BASEADDR        (u32)(0xbff75000)
#define UART0_BASEADDR       (u32)(0xbff73000)
#define UART1_BASEADDR       (u32)(0xbff73400)

#endif /* CONFIG_SPL_BUILD */

#define CONSOLE_UART_BASE   UART1_BASEADDR

#endif /* __ASM_ARCH_HARDWARE_H__ */
