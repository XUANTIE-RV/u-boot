#
# Copyright (C) 2017-2020 Alibaba Group Holding Limited
#
# SPDX-License-Identifier: GPL-2.0+
#

cpuflags-$(CONFIG_CPU_C860)       := -mcpu=ck860

PLATFORM_CPPFLAGS += $(cpuflags-y)

head-y := arch/csky/cpu/$(CONFIG_SYS_CPU)/start.o
libs-y += arch/csky/cpu/
libs-y += arch/csky/cpu/$(CONFIG_SYS_CPU)/
libs-y += arch/csky/lib/
