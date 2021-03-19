/*
* Copyright (C) 2017-2020 Alibaba Group Holding Limited
*
* SPDX-License-Identifier: GPL-2.0+
*/

#include <common.h>

int check_for_good(char *name, long addr)
{
	printf("%s is good: 0x%lx\n", name, addr);

	return 0;
}
