/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <linux/sizes.h>

/* LSP : UART */
/* HSP : SPI0 EMMC */
#define CPU_DEFAULT_FREQ      1000000000
#define HSP_DEFAULT_FREQ      250000000
#define LSP_DEFAULT_FREQ      62500000
#define DWMMC_MAX_FREQ        25000000

#define CONFIG_SYS_INIT_SP_ADDR     (CONFIG_SYS_TEXT_BASE + SZ_1M)
#define CONFIG_SYS_LOAD_ADDR        (CONFIG_SYS_TEXT_BASE + SZ_1M)
#define CONFIG_SYS_MALLOC_LEN       SZ_8M
#define CONFIG_SYS_BOOTM_LEN        SZ_64M
#define CONFIG_SYS_CACHELINE_SIZE   64

#define CONFIG_DW_ALTDESCRIPTOR
#define CONFIG_PHY_MARVELL      1
#define CONFIG_NET_RETRY_COUNT  20

#define CONFIG_EXTRA_ENV_SETTINGS \
	"dtb_emmc_start=0x1000\0" \
	"dtb_emmc_size=0x100\0" \
	"kernel_emmc_start=0x2000\0" \
	"kernel_emmc_size=0xa000\0" \
	"dtb_addr=0x01f00000\0" \
	"kernel_addr=0x02000000\0" \
	"avail_addr=0x10000000\0" \
	"\0"

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	"mmc read ${dtb_addr} ${dtb_emmc_start} ${dtb_emmc_size} ; " \
	"mmc read ${kernel_addr} ${kernel_emmc_start} ${kernel_emmc_size} ; " \
	"bootm ${kernel_addr} - ${dtb_addr}"

#endif /* __CONFIG_H */
