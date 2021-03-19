/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <linux/sizes.h>

#define CONFIG_SYS_SPL_MALLOC_START     0xffe0030000
#define CONFIG_SYS_SPL_MALLOC_SIZE      0x000000a000
#define CONFIG_SPL_STACK                0xffe003d000
#define CONFIG_SPL_BSS_START_ADDR       0xffe003d000
#define CONFIG_SPL_BSS_MAX_SIZE         0x0000002000

#define CONFIG_SYS_MONITOR_LEN                         (1024 * 1024) /* Assumed U-Boot size */
#define CONFIG_SYS_MMCSD_RAW_MODE_EMMC_BOOT_PARTITION  1
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_USE_SECTOR
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR        0x178

#define CONFIG_SYS_INIT_SP_ADDR     (CONFIG_SYS_TEXT_BASE + SZ_1M)
#define CONFIG_SYS_LOAD_ADDR        (CONFIG_SYS_TEXT_BASE + SZ_1M)
#define CONFIG_SYS_MALLOC_LEN       SZ_1M
#define CONFIG_SYS_BOOTM_LEN        SZ_64M
#define CONFIG_SYS_CACHELINE_SIZE   64

#define PLIC_BASE_ADDR   0xffd8000000
#define PMP_BASE_ADDR    0xffdc020000

/* Network Configuration */
#define CONFIG_DW_ALTDESCRIPTOR
#define CONFIG_RGMII            1
#define CONFIG_PHY_MARVELL      1
#define CONFIG_NET_RETRY_COUNT  20

#define CONFIG_SYS_FLASH_BASE       0x0
#define CONFIG_SYS_MAX_FLASH_BANKS  1
#define CONFIG_ENV_ADDR             (CONFIG_SYS_FLASH_BASE + CONFIG_ENV_OFFSET)
#define CONFIG_SYS_MMC_ENV_DEV      0

#define CONFIG_EXTRA_ENV_SETTINGS \
    "uboot_start=0x0\0" \
    "uboot_size=0x400\0" \
    "t_opensbi_start=0x0\0" \
    "t_opensbi_size=0x200\0" \
    "t_kernel_start=0x1000\0" \
    "t_kernel_size=0x8000\0" \
    "t_rootfs_start=0x10000\0" \
    "t_rootfs_size=0x8000\0" \
    "t_dtb_start=0x60000\0" \
    "t_dtb_size=0x20\0" \
    "nt_opensbi_start=0x30000\0" \
    "nt_opensbi_size=0x200\0" \
    "nt_kernel_start=0x31000\0" \
    "nt_kernel_size=0x8000\0" \
    "nt_rootfs_start=0x40000\0" \
    "nt_rootfs_size=0x8000\0" \
    "nt_dtb_start=0x90000\0" \
    "nt_dtb_size=0x20\0" \
    "t_opensbi_addr=0x0\0" \
    "t_kernel_addr=0x00200000\0" \
    "t_rootfs_addr=0x02000000\0" \
    "t_dtb_addr=0x01f00000\0" \
    "nt_opensbi_addr=0x80000000\0" \
    "nt_kernel_addr=0x80200000\0" \
    "nt_rootfs_addr=0x82000000\0" \
    "nt_dtb_addr=0x81f00000\0" \
        "\0"

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
       "bootm $t_kernel_addr $t_rootfs_addr $t_dtb_addr $nt_dtb_addr"

#endif /* __CONFIG_H */
/*
       "mmc read $t_opensbi_addr $t_opensbi_start $t_opensbi_size;" \
       "mmc read $t_kernel_addr $t_kernel_start $t_kernel_size;" \
       "mmc read $t_rootfs_addr $t_rootfs_start $t_rootfs_size;" \
       "mmc read $t_dtb_addr $t_dtb_start $t_dtb_size;" \
       "mmc read $nt_opensbi_addr $nt_opensbi_start $nt_opensbi_size;" \
       "mmc read $nt_kernel_addr $nt_kernel_start $nt_kernel_size;" \
       "mmc read $nt_rootfs_addr $nt_rootfs_start $nt_rootfs_size;" \
       "mmc read $nt_dtb_addr $nt_dtb_start $nt_dtb_size;" \
*/
