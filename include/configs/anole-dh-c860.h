/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <linux/sizes.h>


// #define DEBUG


#define SSEG0_BASEADDRESS           0x0
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
#define DWMMC_MAX_FREQ          25000000
#else
#define CPU_DEFAULT_FREQ        50000000
#define HSP_DEFAULT_FREQ        50000000
#define LSP_DEFAULT_FREQ        50000000
#endif

#define LSP_CLOCK  (LSP_DEFAULT_FREQ / 1000000)
#define CONFIG_SYS_CLK_FREQ CPU_DEFAULT_FREQ

/* Physical Memory Map */
#define PHYS_SRAM_1                 0xfe400000
#define PHYS_SDRAM_1                0x0
#define PHYS_SDRAM_1_SIZE           0x18000000 /* 384M */
#define CONFIG_SYS_TEXT_BASE        0x17800000
#define CONFIG_SYS_LOAD_ADDR        0x80000000
#define CONFIG_SYS_INIT_SP_ADDR     (CONFIG_SYS_TEXT_BASE  + 0x80000 - 0x8)
#define CONFIG_SYS_MALLOC_LEN       SZ_4M
#define CONFIG_SYS_BOOTM_LEN        SZ_64M
#define CONFIG_SYS_SDRAM_BASE       PHYS_SDRAM_1
#define CONFIG_SYS_MONITOR_BASE     CONFIG_SYS_TEXT_BASE

#define CONFIG_SPL_STACK  (PHYS_SRAM_1 + 0x100000 - 0x8)
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

#define UBOOT_INTERNAL_VERSION "0.4"
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
#define DDRINIT_CLOCK_FREQ_MT       2000

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
#define CONFIG_PHY_REALTEK      1
#define CONFIG_NET_RETRY_COUNT  20


#define PHYS_FLASH_1                0x00000000 /* Flash Bank #0 */
#define CONFIG_SYS_FLASH_BASE       PHYS_FLASH_1
#define CONFIG_SYS_MAX_FLASH_BANKS  1
#define CONFIG_ENV_ADDR             (CONFIG_SYS_FLASH_BASE + 0x070000)
#define CONFIG_SYS_MMC_ENV_DEV      0


/* Image address in Flash */
#define FLASH_SPL_READ_ADDR         0x0
#define FLASH_SPL_SIZE              0x20000  /* 128K */
#define FLASH_FDT_READ_ADDR         0x30000  /* 192K */
#define FLASH_FDT_SIZE              0x10000
#define FLASH_UBOOT_READ_ADDR       0x40000  /* 256K */
#define FLASH_UBOOT_SIZE            0x80000


#ifdef CONFIG_IS_ASIC
#define TFTP_LOAD_DTB "tftpboot ${dtb_load_addr_virt} c860/anole_dh_evb.dtb ; "
#define TFTP_LOAD_SLAVE_DTB "tftpboot ${dtb_load_addr_virt} c810/anole_evb_ck810.dtb ; "
#else
#define TFTP_LOAD_DTB "tftpboot ${dtb_load_addr_virt} c860/anole_dh.dtb ; "
#define TFTP_LOAD_SLAVE_DTB "tftpboot ${dtb_load_addr_virt} c810/anole_ck810.dtb ; "
#endif

#define CONFIG_EXTRA_ENV_SETTINGS \
    "fdt_start_sector=0x180\0"     /* fdt start sector = FLASH_FDT_READ_ADDR / 0x200 */ \
    "uboot_start_sector=0x200\0"   /* uboot start sector = CONFIG_FLASH_UBOOT_READ_ADDR / 0x200 */ \
    "dtb_start_sector=0x1000\0"    /* dtb start sector -> 2MB */ \
    "dtb_size_sectors=0x1000\0"    /* dtb size in sectors -> 2MB */ \
    "linux_start_sector=0x2000\0"  /* linux start sector -> 4MB */  \
    "linux_size_sectors=0x1e000\0" /* linux size in sectors -> 60MB */ \
    "slave_spl_start_sector=0x800\0" /* uboot spl slave start sector */ \
    "slave_fdt_start_sector=0x980\0"   /* slave fdt start sector */ \
    "slave_uboot_start_sector=0xa00\0" /* slave uboot start sector */ \
    "slave_dtb_start_sector=0x21000\0" \
    "slave_dtb_size_sectors=0x1000\0" \
    "slave_linux_start_sector=0x22000\0" \
    "slave_linux_size_sectors=0xa000\0" \
    "sram_addr_virt=0xbe400000\0"  /* PHYS_SRAM_1 */ \
    "slave_spl_load_addr_virt=0x8e000000\0" \
    "slave_spl_load_addr_phys=0x0e000000\0" \
    "slave_fdt_load_addr_phys=0x577f0000\0" \
    "slave_uboot_load_addr_phys=0x57800000\0" \
    "dtb_load_addr_virt=0x8f000000\0" \
    "dtb_load_addr_phys=0x0f000000\0"  \
    "linux_load_addr_virt=0x90000000\0"  \
    "linux_load_addr_phys=0x10000000\0" \
    "slave_dtb_load_addr_phys=0x4f000000\0" \
    "slave_linux_load_addr_phys=0x50000000\0" \
    "update_spl=" \
        "tftpboot ${dtb_load_addr_virt} c860/mboot.fimg ; " \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc dev 0 1 ; "  /* uboot -> eMMC BOOT PARTITION #2 */ \
        "mmc write ${dtb_load_addr_phys} 0 ${fw_sz} ; " \
        "mmc dev 0 0 ; "  /* restore to USER PARTITION */ \
        "\0" \
    "update_fdt=" \
        "tftpboot ${dtb_load_addr_virt} c860/dt.dtb ; " \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc dev 0 1 ; "  /* uboot -> eMMC BOOT PARTITION #1 */ \
        "mmc write ${dtb_load_addr_phys} ${fdt_start_sector} ${fw_sz} ; " \
        "mmc dev 0 0 ; "  /* restore to USER PARTITION */ \
        "\0" \
    "update_uboot=" \
        "tftpboot ${dtb_load_addr_virt} c860/u-boot.bin ; " \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc dev 0 1 ; "  /* uboot -> eMMC BOOT PARTITION #1 */ \
        "mmc write ${dtb_load_addr_phys} ${uboot_start_sector} ${fw_sz} ; " \
        "mmc dev 0 0 ; "  /* restore to USER PARTITION */ \
        "\0" \
    "update_dtb=" \
        TFTP_LOAD_DTB \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc write ${dtb_load_addr_phys} ${dtb_start_sector} ${fw_sz} ; " \
        "setenv dtb_size_sectors ${fw_sz} ; " \
        "saveenv ; " \
        "\0" \
    "update_linux=" \
        "tftpboot ${linux_load_addr_virt} c860/uImage ; " \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc write ${linux_load_addr_phys} ${linux_start_sector} ${fw_sz} ; " \
        "setenv linux_size_sectors ${fw_sz} ; " \
        "saveenv ; " \
        "\0" \
    "update_slave_spl=" \
        "tftpboot ${dtb_load_addr_virt} c810/u-boot-spl.bin ; " \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc dev 0 1 ; "  /* uboot -> eMMC BOOT PARTITION #1 */ \
        "mmc write ${dtb_load_addr_phys} ${slave_spl_start_sector} ${fw_sz} ; " \
        "mmc dev 0 0 ; "  /* restore to USER PARTITION */ \
        "\0" \
    "update_slave_fdt=" \
        "tftpboot ${dtb_load_addr_virt} c810/dt.dtb ; " \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc dev 0 1 ; "  /* uboot -> eMMC BOOT PARTITION #1 */ \
        "mmc write ${dtb_load_addr_phys} ${slave_fdt_start_sector} ${fw_sz} ; " \
        "mmc dev 0 0 ; "  /* restore to USER PARTITION */ \
        "\0" \
    "update_slave_uboot=" \
        "tftpboot ${dtb_load_addr_virt} c810/u-boot.bin ; " \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc dev 0 1 ; "  /* uboot -> eMMC BOOT PARTITION #1 */ \
        "mmc write ${dtb_load_addr_phys} ${slave_uboot_start_sector} ${fw_sz} ; " \
        "mmc dev 0 0 ; "  /* restore to USER PARTITION */ \
        "\0" \
    "update_slave_dtb=" \
        TFTP_LOAD_SLAVE_DTB \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc write ${dtb_load_addr_phys} ${slave_dtb_start_sector} ${fw_sz} ; " \
        "\0" \
    "update_slave_linux=" \
        "tftpboot ${linux_load_addr_virt} c810/uImage ; " \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc write ${linux_load_addr_phys} ${slave_linux_start_sector} ${fw_sz} ; " \
        "\0" \
    "boot_slave=" \
        "mmc dev 0 1 ; "  /* uboot -> eMMC BOOT PARTITION #1 */ \
        "mmc read ${slave_spl_load_addr_phys} ${slave_spl_start_sector} 0x40;" \
        "mmc read ${slave_fdt_load_addr_phys} ${slave_fdt_start_sector} 0x80;" \
        "mmc read ${slave_uboot_load_addr_phys} ${slave_uboot_start_sector} 0x400;" \
        "mmc dev 0 0 ; "  /* restore to USER PARTITION */ \
        "mmc read ${slave_dtb_load_addr_phys} ${slave_dtb_start_sector} ${slave_dtb_size_sectors} ; " \
        "mmc read ${slave_linux_load_addr_phys} ${slave_linux_start_sector} ${slave_linux_size_sectors} ; " \
        "cp ${slave_spl_load_addr_virt} ${sram_addr_virt} 0x8000;" \
        "bootslave ${sram_addr_virt}; " \
        "\0"

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
        "mmc read ${dtb_load_addr_phys} ${dtb_start_sector} ${dtb_size_sectors} ; " \
        "mmc read ${linux_load_addr_phys} ${linux_start_sector} ${linux_size_sectors} ; " \
        "run boot_slave; " \
        "bootm ${linux_load_addr_virt} "


#endif /* __CONFIG_H */
