/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <linux/types.h>
#include <common.h>

void invalidate_dcache_range(unsigned long addr, unsigned long stop)
{
    int value = 0x32;

    __asm__ __volatile__("mtcr %0,cr17\n\t": :"r" (value));
}

void flush_cache(unsigned long addr, unsigned long size)
{
    int value = 0x33;

    __asm__ __volatile__("mtcr %0,cr17\n\t": :"r" (value));
}
void flush_dcache_range(unsigned long start, unsigned long stop)
{
    int value = 0x33;

    __asm__ __volatile__("mtcr %0,cr17\n\t": :"r" (value));
}
