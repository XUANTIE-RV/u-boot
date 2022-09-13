/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __ASM_PROC_PTRACE_H
#define __ASM_PROC_PTRACE_H

#define USR26_MODE  0x00
#define FIQ26_MODE  0x01
#define IRQ26_MODE  0x02
#define SVC26_MODE  0x03
#define USR_MODE    0x10
#define FIQ_MODE    0x11
#define IRQ_MODE    0x12
#define SVC_MODE    0x13
#define ABT_MODE    0x17
#define HYP_MODE    0x1a
#define UND_MODE    0x1b
#define SYSTEM_MODE 0x1f
#define MODE_MASK   0x1f
#define T_BIT       0x20
#define F_BIT       0x40
#define I_BIT       0x80
#define A_BIT       0x100
#define CC_V_BIT    (1 << 28)
#define CC_C_BIT    (1 << 29)
#define CC_Z_BIT    (1 << 30)
#define CC_N_BIT    (1 << 31)
#define PCMASK      0

#ifndef __ASSEMBLY__

/* this struct defines the way the registers are stored on the
stack during a system call. */

struct pt_regs {
    long uregs[18];
};
#endif	/* __ASSEMBLY__ */

#endif /* __ASM_PROC_PTRACE_H */
