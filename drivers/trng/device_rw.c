/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */
#include "device_rw.h"
#include "soc.h"
#include "rambus_log.h"

#define eip_readl(addr) \
    (*(volatile uint32_t *) (addr))

#define eip_writel(addr, v) ((*(volatile uint32_t *) (addr)) = (v))

uint32_t Device_Read32(const Device_Handle_t Device,
                       const unsigned int    ByteOffset)
{
        uint64_t addr  = (uint64_t)Device + (uint64_t)ByteOffset;
        volatile uint32_t v = eip_readl(addr);

        return v;
}

void Device_Write32(const Device_Handle_t Device, const unsigned int ByteOffset,
                    const uint32_t Value)
{
        uint64_t addr  = (uint64_t)Device + (uint64_t)ByteOffset;
        eip_writel(addr, Value);
}
