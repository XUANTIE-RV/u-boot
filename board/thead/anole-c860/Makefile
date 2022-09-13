#
# Copyright (C) 2017-2020 Alibaba Group Holding Limited
#
# SPDX-License-Identifier: GPL-2.0+
#

obj-y := anole_c860_evb.o
obj-y += clock/clock.o

ifdef CONFIG_SPL_BUILD
ifdef CONFIG_SYS_ANOLE_DDR4
obj-y += spl/ddr4/ddr.o
obj-y += spl/ddr4/ddr_phy.o
else
obj-y += spl/lpddr4/ddr.o
obj-y += spl/lpddr4/ddr_phy.o
endif
obj-y += spl/clock.o
obj-y += spl/multi_bin.o
obj-y += spl/spl.o
endif
