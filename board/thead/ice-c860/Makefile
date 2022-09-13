#
# Copyright (C) 2017-2020 Alibaba Group Holding Limited
#
# SPDX-License-Identifier: GPL-2.0+
#

obj-y := ice_fpga.o
obj-y += gpio/gpio.o

ifdef CONFIG_IS_ASIC
obj-y += clock/clock.o
endif

ifdef CONFIG_SPL_BUILD
ifdef CONFIG_IS_ASIC
obj-y += spl/clock.o
obj-y += spl/lpddr4/ddr.o
obj-y += spl/lpddr4/ddr_phy.o
else
obj-y += spl/ddr.o
endif
obj-y += spl/spl.o
obj-y += om/om.o
endif
