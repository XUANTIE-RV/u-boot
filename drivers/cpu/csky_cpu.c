/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <cpu.h>
#include <dm.h>
#include <errno.h>
#include <dm/device-internal.h>
#include <dm/lists.h>

DECLARE_GLOBAL_DATA_PTR;

static int csky_cpu_get_desc(struct udevice *dev, char *buf, int size)
{
    const char *isa;

    isa = dev_read_string(dev, "csky,isa");
    if (size < (strlen(isa) + 1))
        return -ENOSPC;

    strcpy(buf, isa);

    return 0;
}

static int csky_cpu_get_info(struct udevice *dev, struct cpu_info *info)
{
    dev_read_u32(dev, "clock-frequency", (u32 *)&info->cpu_freq);

    return 0;
}

static int csky_cpu_get_count(struct udevice *dev)
{
    ofnode node;
    int num = 0;

    ofnode_for_each_subnode(node, dev_ofnode(dev->parent)) {
        const char *device_type;

        /* skip if hart is marked as not available in the device tree */
        if (!ofnode_is_available(node))
            continue;

        device_type = ofnode_read_string(node, "device_type");
        if (!device_type)
            continue;
        if (strcmp(device_type, "cpu") == 0)
            num++;
    }

    return num;
}

static const struct cpu_ops csky_cpu_ops = {
    .get_desc   = csky_cpu_get_desc,
    .get_info   = csky_cpu_get_info,
    .get_count  = csky_cpu_get_count,
};

static const struct udevice_id csky_cpu_ids[] = {
    { .compatible = "csky" },
    { }
};

U_BOOT_DRIVER(csky_cpu) = {
    .name = "csky_cpu",
    .id = UCLASS_CPU,
    .of_match = csky_cpu_ids,
    .ops = &csky_cpu_ops,
    .flags = DM_FLAG_PRE_RELOC,
};
