/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include "../hardware.h"
#include "../../common/datatype.h"
#include "om.h"

/*
 * get the om pin value
 *
 */
s8 get_boot_select(void)
{
    int8_t *bootsel = (s8 *)OM_BASEADDR;

    return ((*bootsel) & 0x3);
}

