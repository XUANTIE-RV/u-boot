/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <linux/sizes.h>

#define CONFIG_SYS_SPL_MALLOC_START     0x3fe430000
#define CONFIG_SYS_SPL_MALLOC_SIZE      0x00000a000
#define CONFIG_SPL_STACK                0x3fe43d000
#define CONFIG_SPL_BSS_START_ADDR       0x3fe43d000
#define CONFIG_SPL_BSS_MAX_SIZE         0x000002000

#define CONFIG_SYS_MONITOR_LEN                         (1024 * 1024) /* Assumed U-Boot size */
#define CONFIG_SYS_MMCSD_RAW_MODE_EMMC_BOOT_PARTITION  1
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_USE_SECTOR
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR        0x178

#define CONFIG_SYS_INIT_SP_ADDR     (CONFIG_SYS_TEXT_BASE + SZ_1M)
#define CONFIG_SYS_LOAD_ADDR        (CONFIG_SYS_TEXT_BASE + SZ_1M)
#define CONFIG_SYS_MALLOC_LEN       SZ_8M
#define CONFIG_SYS_BOOTM_LEN        SZ_64M
#define CONFIG_SYS_CACHELINE_SIZE   64

#define CONFIG_DW_ALTDESCRIPTOR
#define CONFIG_PHY_MARVELL      1
#define CONFIG_NET_RETRY_COUNT  20

#define CONFIG_SYS_FLASH_BASE       0x0
#define CONFIG_SYS_MAX_FLASH_BANKS  1
#define CONFIG_ENV_ADDR             (CONFIG_SYS_FLASH_BASE + CONFIG_ENV_OFFSET)
#define CONFIG_SYS_MMC_ENV_DEV      0

#define CONFIG_EXTRA_ENV_SETTINGS \
	"opensbi_addr=0x0\0" \
	"dtb_addr=0x40000000\0" \
	"kernel_addr=0x00200000\0" \
	"avail_addr=0x10000000\0" \
	"boot_vector=0\0" \
	"set_bootargs=setenv bootargs console=ttyS0,115200 root=PARTUUID=$uuid_rootfs rootfstype=ext4 rdinit=/sbin/init rootwait rw earlyprintk clk_ignore_unused loglevel=7 c910_mmu_v1 eth=$ethaddr\0" \
	"bootcmd_load=ext4load mmc 0:2 $opensbi_addr fw_jump.bin; ext4load mmc 0:2 $dtb_addr hw.dtb; ext4load mmc 0:2 $kernel_addr uImage\0" \
	"bootcmd=run bootcmd_load; run set_bootargs; bootm $kernel_addr - $dtb_addr\0" \
	"\0"

#endif /* __CONFIG_H */
