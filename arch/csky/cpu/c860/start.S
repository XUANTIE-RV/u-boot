/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <config.h>
#include <asm-offsets.h>
#include <linux/linkage.h>
#include <asm/arch/mmu.h>

.export hw_vsr_autovec
.export csky_vsr_table      /* Vector table base address. */
.globl  _start
.import start_cskyboot
.import hw_vsr_default_exception
.import __stack
.import __fstack
.data
/* Vector table space. */
.section .exptable
.align 10
csky_vsr_table:
.long _start
.rept 31
.long hw_vsr_default_exception
.endr
.rept 64
.long hw_vsr_autovec
.endr


.text
_start:

    /* Init psr value */
    mfcr    r3, psr
    bseti   r3, 8
    bseti   r3, 31
    mtcr    r3, psr

    /*
     * Setup initial vector base table for interrupts and exceptions
     */
    lrw     r2, csky_vsr_table
    mtcr    r2, vbr

    /* Initial CKCPU MMU & Cache */
    csky_mmu_disable

    /* Invalid Instruction and data cache content */
    mfcr    r4, cr17
    bseti   r4, 0
    bseti   r4, 1
    bseti   r4, 4
    mtcr    r4, cr17

    mfcr    r4, cr31
    bseti   r4, 2
    bseti   r4, 8
    mtcr    r4, cr31

    jbsr    _main

__exit:
    bkpt
    br      __exit

