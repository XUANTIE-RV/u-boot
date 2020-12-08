/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

int riscv_send_ipi(int hart)
{
	return 0;
}

int riscv_clear_ipi(int hart)
{
	return 0;
}

int riscv_get_ipi(int hart, int *pending)
{
	return 0;
}
