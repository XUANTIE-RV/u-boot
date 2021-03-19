/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <cpu.h>
#include <watchdog.h>
#include <command.h>
#include <netdev.h>
#include <asm/io.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_OF_PRIOR_STAGE
phys_addr_t prior_stage_fdt_address __attribute__((section(".data")));
#endif

static int csky_cpu_probe(void)
{
#ifdef CONFIG_CPU
    cpu_probe_all();
#endif

    return 0;
}

int arch_cpu_init_dm(void)
{
    return csky_cpu_probe();
}
