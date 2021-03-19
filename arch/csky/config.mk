#
# Copyright (C) 2017-2020 Alibaba Group Holding Limited
#
# SPDX-License-Identifier: GPL-2.0+
#

ENDIANNESS = -EL
PLATFORM_RELFLAGS += -mlittle-endian

PLATFORM_LDFLAGS  += $(ENDIANNESS)
PLATFORM_RELFLAGS += -fno-strict-aliasing -fomit-frame-pointer
PLATFORM_RELFLAGS += -frename-registers
LDFLAGS_FINAL     += --gc-sections
OBJCOPYFLAGS      += -j .text -j .rodata -j .data -j .u_boot_list
OBJCOPYFLAGS      += -j .dtb.init.rodata
PLATFORM_CPPFLAGS += -EL
