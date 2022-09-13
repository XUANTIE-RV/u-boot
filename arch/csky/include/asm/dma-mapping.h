/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __ASM_CSKY_DMA_MAPPING_H
#define __ASM_CSKY_DMA_MAPPING_H

#include <linux/dma-direction.h>

#define dma_mapping_error(x, y)    0

static inline void *dma_alloc_coherent(size_t len, unsigned long *handle)
{
    return NULL;
}

static inline void dma_free_coherent(void *addr)
{
    free(addr);
}

static inline unsigned long dma_map_single(volatile void *vaddr, size_t len,
                    enum dma_data_direction dir)
{
    return (unsigned long)vaddr;
}

static inline void dma_unmap_single(volatile void *vaddr, size_t len,
                    unsigned long paddr)
{
}

#endif /* __ASM_CSKY_DMA_MAPPING_H */
