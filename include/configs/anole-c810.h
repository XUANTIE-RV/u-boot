/*
* Copyright (C) 2017-2020 Alibaba Group Holding Limited
*
* SPDX-License-Identifier: GPL-2.0+
*/

#ifndef __CONFIG_H
#define __CONFIG_H

#include <linux/sizes.h>


// #define DEBUG


#define SPL_SSEG0_BASEADDRESS       0x40000000
#define SSEG0_BASEADDRESS           0x40000000
#define SSEG1_BASEADDRESS           0xe0000000


#define CONFIG_NR_DRAM_BANKS 1
#define CONFIG_BOARD_EARLY_INIT_R
#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_SYS_HZ 1000

/*
 * LSP : Timer UART
 * HSP : SPI0 EMMC
 */
#ifdef CONFIG_IS_ASIC
#define CPU_DEFAULT_FREQ        1000000000
#define HSP_DEFAULT_FREQ        250000000
#define LSP_DEFAULT_FREQ        62500000
#else
#define CPU_DEFAULT_FREQ        50000000
#define HSP_DEFAULT_FREQ        50000000
#define LSP_DEFAULT_FREQ        50000000
#endif

#define LSP_CLOCK  (LSP_DEFAULT_FREQ / 1000000)
#define CONFIG_SYS_CLK_FREQ CPU_DEFAULT_FREQ

/* Physical Memory Map */
#define PHYS_SRAM_1                 0xfe400000
#define PHYS_SDRAM_1                0x40000000
#define PHYS_SDRAM_1_SIZE           0x18000000 /* 384M */
#define CONFIG_SYS_TEXT_BASE        0x57800000
#define CONFIG_SYS_LOAD_ADDR        0x80000000
#define CONFIG_SYS_INIT_SP_ADDR     (CONFIG_SYS_TEXT_BASE  + 0x80000 - 0x8)
#define CONFIG_SYS_MALLOC_LEN       SZ_2M
#define CONFIG_SYS_BOOTM_LEN        SZ_32M
#define CONFIG_SYS_SDRAM_BASE       PHYS_SDRAM_1
#define CONFIG_SYS_MONITOR_BASE     CONFIG_SYS_TEXT_BASE

#define CONFIG_SPL_STACK  (PHYS_SRAM_1 + 0x8000 - 0x8)
#define CONFIG_SPL_MAX_FOOTPRINT    0x4000


#ifdef CONFIG_SPL_BUILD
#define INTC_BASEADDRESS            0xfe3f0000
#define TIMER_BASEADDRESS           0xfff70000
#define ERAGON_MMC0_BASE            0xfffb0000
#else
#define INTC_BASEADDRESS            0xbe3f0000
#define TIMER_BASEADDRESS           0xbff70000
#define ERAGON_MMC0_BASE            0xbffb0000
#endif

#define UBOOT_INTERNAL_VERSION "0.3"
#define CONFIG_C810_CSKY
#define CONFIG_BOARD_PRINTF_SUPPORT
#define CONFIG_BOARD_CONSOLE_SUPPORT
#define CONFIG_BOARD_MMC_SUPPORT
#define CONFIG_BOARD_SPIFLASH_SUPPORT
#define CONFIG_SUPPORT_EMMC_BOOT
#define HAVE_BLOCK_DEVICE
#define TIMER_ID 0
#define TIME_IRQ 4
#define CONFIG_BAUDRATE             115200
#define TIMEOUT_COUNT_TIMER         TIMER0
#define TIMEOUT_VALUE               500
#define CONFIG_SYS_CBSIZE           256   /* This is Console Buffer size */
#define CONFIG_SYS_MAXARGS          16

/* NAND configuration */
#ifdef CONFIG_CMD_NAND
#define CONFIG_NAND_ERAGON          1
#define CONFIG_SYS_ERAGON_NAND_HWECC
#define CONFIG_SYS_MAX_NAND_DEVICE  1
#define CONFIG_SYS_NAND_BASE        0x4e000000
#endif

/* Network Configuration */
#define CONFIG_DW_ALTDESCRIPTOR
#define CONFIG_MII              1
#define CONFIG_PHY_MARVELL      1
#define CONFIG_NET_RETRY_COUNT  20


#define PHYS_FLASH_1                0x00200000 /* Flash Bank #0 */
#define CONFIG_SYS_FLASH_BASE       PHYS_FLASH_1
#define CONFIG_SYS_MAX_FLASH_BANKS  1
#define CONFIG_ENV_ADDR             (CONFIG_SYS_FLASH_BASE + 0x070000)
#define CONFIG_SYS_MMC_ENV_DEV      0


/* Image address in Flash */
#define FLASH_SPL_READ_ADDR         0x100000 /* 1M */
#define FLASH_SPL_SIZE              0x010000 /* 64K */
#define FLASH_FDT_READ_ADDR         0x130000 /* 1M+192K */
#define FLASH_FDT_SIZE              0x010000
#define FLASH_UBOOT_READ_ADDR       0x140000 /* 1M+256K */
#define FLASH_UBOOT_SIZE            0x080000

/* Environment options */

#define CONFIG_EXTRA_ENV_SETTINGS \
    "dtb_load_addr_virt=0x8f000000\0" \
    "dtb_load_addr_phys=0x4f000000\0"  \
    "linux_load_addr_virt=0x90000000\0"  \
    "linux_load_addr_phys=0x50000000\0"

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
    "bootm ${linux_load_addr_virt} "


#endif /* __CONFIG_H */
