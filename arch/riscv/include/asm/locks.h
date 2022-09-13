/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2019 Western Digital Corporation or its affiliates.
 *
 * Authors:
 *   Anup Patel <anup.patel@wdc.com>
 */

#ifndef __RISCV_LOCKS_H__
#define __RISCV_LOCKS_H__

typedef struct {
	volatile long lock;
} arch_spinlock_t;

#define __RISCV_SPIN_UNLOCKED 0

#define SPIN_LOCK_INIT(_lptr) (_lptr)->lock = __RISCV_SPIN_UNLOCKED

#define SPIN_LOCK_INITIALIZER                  \
	{                                      \
		.lock = __RISCV_SPIN_UNLOCKED, \
	}

int spin_lock_check(arch_spinlock_t *lock);

int spin_trylock(arch_spinlock_t *lock);

void arch_spin_lock(arch_spinlock_t *lock);

void arch_spin_unlock(arch_spinlock_t *lock);

#endif
