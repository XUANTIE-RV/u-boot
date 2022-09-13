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

#ifdef CONFIG_IS_ASIC
#define CPU_DEFAULT_FREQ  1000000000
#define HSP_DEFAULT_FREQ  250000000
#define LSP_DEFAULT_FREQ  62500000
#else
#define CPU_DEFAULT_FREQ  50000000
#define HSP_DEFAULT_FREQ  25000000
#define LSP_DEFAULT_FREQ  50000000
#endif

#define LSP_CLOCK  (LSP_DEFAULT_FREQ / 1000000)
#define CONFIG_SYS_CLK_FREQ CPU_DEFAULT_FREQ

/* Physical Memory Map */
#define PHYS_SRAM_1                 0xfe400000
#define PHYS_SDRAM_1                0x0
#define PHYS_SDRAM_1_SIZE           0x18000000 /* 384M */
#define CONFIG_SYS_TEXT_BASE        0x17800000
#define CONFIG_SYS_LOAD_ADDR        0x80000000
#define CONFIG_SYS_SRAM_BASE        PHYS_SRAM_1
#define CONFIG_SYS_INIT_SP_ADDR     (CONFIG_SYS_TEXT_BASE  + 0x80000 - 0x8)
#define CONFIG_SYS_MALLOC_LEN       SZ_2M
#define CONFIG_SYS_BOOTM_LEN        SZ_64M
#define CONFIG_SYS_MEMTEST_START    0x30000000    /* memtest works on */
#define CONFIG_SYS_MEMTEST_END      0x33F00000    /* 63 MB in DRAM */
#define CONFIG_SYS_MONITOR_BASE     CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_CACHELINE_SIZE   128

#define CONFIG_SPL_STACK  (CONFIG_SYS_SRAM_BASE + 0x8000 - 0x8)
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

#define UBOOT_INTERNAL_VERSION "0.2"
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
#define CONFIG_SYS_NAND_BASE        0x4E000000
#endif

/* Network Configuration */
#define CONFIG_DW_ALTDESCRIPTOR
#define CONFIG_RGMII            1
#define CONFIG_PHY_MARVELL      1
#define CONFIG_NET_RETRY_COUNT  20


#define PHYS_FLASH_1                0x00000000 /* Flash Bank #0 */
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


#ifdef CONFIG_IS_ASIC
#define TFTP_LOAD_DTB "tftpboot ${dtb_load_addr_virt} c860/ice_evb.dtb ; "
#define TFTP_LOAD_SLAVE_DTB "tftpboot ${dtb_load_addr_virt} c810/ice_ck810_evb.dtb ; "
#else
#define TFTP_LOAD_DTB "tftpboot ${dtb_load_addr_virt} c860/ice.dtb ; "
#define TFTP_LOAD_SLAVE_DTB "tftpboot ${dtb_load_addr_virt} c810/ice_ck810.dtb ; "
#endif

#define CONFIG_EXTRA_ENV_SETTINGS \
	"spl_start_sector=0x0\0" \
	"spl_size_sector=0x140\0" \
    "fdt_start_sector=0x980\0"     /* fdt start sector = FLASH_FDT_READ_ADDR / 0x200 */ \
    "fdt_size_sector=0x20\0" \
    "uboot_start_sector=0xa00\0"   /* uboot start sector = FLASH_UBOOT_READ_ADDR / 0x200 */ \
    "uboot_size_sector=0x400\0" \
    "dtb_start_sector=0x21000\0"   /* dtb start sector */ \
    "dtb_size_sector=0x1000\0"    /* dtb size in sector -> 2MB */ \
    "linux_start_sector=0x22000\0" /* linux start sector */  \
    "linux_size_sector=0xa000\0" /* linux size in sector -> 20MB */ \
    "ramdisk_start_sector=0x2c000\0" /* ramdisk start sector */ \
    "ramdisk_size_sector=0x10000\0" /* ramdisk size in sector -> 32MB */ \
    "slave_spl_start_sector=0x1000\0" /* uboot spl slave start sector */ \
    "slave_fdt_start_sector=0x1180\0"   /* slave fdt start sector */ \
    "slave_uboot_start_sector=0x1200\0"  /* slave uboot start sector */ \
    "slave_dtb_start_sector=0x41000\0" \
    "slave_linux_start_sector=0x42000\0" \
    "sram_addr_virt=0xbe400000\0"  /* PHYS_SRAM_1 */ \
    "slave_spl_load_addr_virt=0x8e000000\0" \
    "slave_spl_load_addr_phys=0x0e000000\0" \
    "dtb_load_addr_virt=0x81f00000\0" \
    "dtb_load_addr_phys=0x01f00000\0"  \
    "linux_load_addr_virt=0x80000000\0"  \
    "linux_load_addr_phys=0x00000000\0" \
    "ramdisk_load_addr_virt=0x82000000\0" \
    "ramdisk_load_addr_phys=0x02000000\0" \
    "avail_addr_virt=0x80000000\0" \
    "avail_addr_phy=0x00000000\0" \
	"abc=" \
		"tftpboot ${avail_addr_virt} kernelimg;" \
		"mmc write ${avail_addr_phy} 0x21000 0x1b000;" \
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
        "setenv dtb_size_sector ${fw_sz} ; " \
        "saveenv ; " \
        "\0" \
    "update_linux=" \
        "tftpboot ${linux_load_addr_virt} c860/uImage ; " \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc write ${linux_load_addr_phys} ${linux_start_sector} ${fw_sz} ; " \
        "setenv linux_size_sector ${fw_sz} ; " \
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
        "mmc dev 0 0 ; "  /* restore to USER PARTITION */ \
        "cp ${slave_spl_load_addr_virt} ${sram_addr_virt} 0x8000;" \
        "bootslave ${sram_addr_virt}; " \
        "\0"

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
        "mmc read ${dtb_load_addr_phys} ${dtb_start_sector} ${dtb_size_sector} ; " \
        "mmc read ${linux_load_addr_phys} ${linux_start_sector} ${linux_size_sector} ; " \
        "mmc read ${ramdisk_load_addr_phys} ${ramdisk_start_sector} ${ramdisk_size_sector} ; " \
        "bootm ${linux_load_addr_virt} - ${dtb_load_addr_virt}"


#endif /* __CONFIG_H */
