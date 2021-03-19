/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <cpu_func.h>

void invalidate_icache_all(void)
{
}

__weak void flush_dcache_all(void)
{
}

void invalidate_icache_range(unsigned long start, unsigned long end)
{
    invalidate_icache_all();
}

void cache_flush(void)
{
    invalidate_icache_all();
    flush_dcache_all();
}

__weak void icache_enable(void)
{
}

__weak void icache_disable(void)
{
}

__weak int icache_status(void)
{
    return 0;
}

__weak void dcache_enable(void)
{
}

__weak void dcache_disable(void)
{
}

__weak int dcache_status(void)
{
    return 0;
}
