/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <asm/mmu.h>
#include <config.h>

#define PAGE_SIZE (16 * 1024 * 1024)

void init_mmu(void)
{
    int32_t i;
    page_attr_t attr;
    attr.global = 0;
    attr.valid = 1;
    attr.writeable = 1;
    attr.cacheable = 1;
    attr.is_secure = 0;
    attr.strong_order = 0;
    attr.bufferable = 0;
    uint32_t base_addr = PHYS_SDRAM_1, page_count;
    page_count = (PHYS_SDRAM_1_SIZE + PAGE_SIZE - 1) / PAGE_SIZE;
    base_addr = (base_addr + (PAGE_SIZE - 1)) / PAGE_SIZE * PAGE_SIZE;
    csky_mmu_set_pagesize(PAGE_SIZE_16M);
    for (i = 0; i < page_count; i++)
        csky_mmu_set_tlb(base_addr + i * PAGE_SIZE, base_addr + i * PAGE_SIZE, i, attr);
    csky_mmu_enable();
}
