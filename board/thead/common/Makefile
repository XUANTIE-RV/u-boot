#
# Copyright (C) 2017-2020 Alibaba Group Holding Limited
#
# SPDX-License-Identifier: GPL-2.0+
#

obj-y += board_common.o


ifneq ($(CONFIG_SPL_BUILD)$(CONFIG_PPL_BUILD),)

ifdef CONFIG_BOARD_MMC_SUPPORT
obj-y += mmc/emmc.o
obj-y += mmc/emmc_interface.o
obj-y += mmc/emmc_callback.o
endif
obj-$(CONFIG_BOARD_CONSOLE_SUPPORT) += uart/uart.o
obj-$(CONFIG_BOARD_PRINTF_SUPPORT) += mini_printf.o
ifdef CONFIG_BOARD_SPIFLASH_SUPPORT
obj-y += spi/spi.o
obj-y += spiflash/spiflash.o
endif

CFLAGS_emmc_interface.o += -I$(srctree)/board/$(BOARDDIR)
CFLAGS_mini_printf.o    += -I$(srctree)/board/$(BOARDDIR)
CFLAGS_spi.o            += -I$(srctree)/board/$(BOARDDIR)
CFLAGS_spiflash.o       += -I$(srctree)/board/$(BOARDDIR)

endif

ifeq ($(CONFIG_TARGET_LIGHT_C910),)
obj-$(CONFIG_CMD_BOOT_SLAVE) += boot/bootslave.o
endif

