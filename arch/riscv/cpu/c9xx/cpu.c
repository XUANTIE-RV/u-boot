/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <asm/types.h>
#include <asm/asm.h>
#include <common.h>
#include <cpu_func.h>

/*
 * cleanup_before_linux() is called just before we call linux
 * it prepares the processor for linux
 *
 * we disable interrupt and caches.
 */
int cleanup_before_linux(void)
{
	cache_flush();

	return 0;
}

#define sync_is()   asm volatile (".long 0x01b0000b")

void flush_dcache_range(unsigned long start, unsigned long end)
{
	register unsigned long i asm("a0") = start & ~(CONFIG_SYS_CACHELINE_SIZE - 1);

	for (; i < end; i += CONFIG_SYS_CACHELINE_SIZE)
		asm volatile(".long 0x0295000b");  /* dcache.cpa a0 */

	sync_is();
}

void invalidate_dcache_range(unsigned long start, unsigned long end)
{
	register unsigned long i asm("a0") = start & ~(CONFIG_SYS_CACHELINE_SIZE - 1);

	for (; i < end; i += CONFIG_SYS_CACHELINE_SIZE)
		asm volatile(".long 0x02b5000b");  /* dcache.cipa a0 */

	sync_is();
}
