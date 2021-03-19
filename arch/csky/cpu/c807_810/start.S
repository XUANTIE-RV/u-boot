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
#ifdef CONFIG_BOOT_SWITCH_WORLD
    // update psr
    mfcr    r0, psr
    mtcr    r0, epsr
    bclri   r0, 29
    bclri   r0, 8
    bclri   r0, 6
    mtcr    r0, psr
    //Invalid L1-cache include I/Dcache
    movi    r0, 0
    mtcr    r0, cr<31, 0>
    movi    r0, 0x13
    mtcr    r0, cr<17, 0>
    //Enable L1 cache and MMU and other CPU features
    lrw     r0, 0xc870
    mtcr    r0, cr<18, 0>
    lrw     r0, 0x10
    mtcr    r0, cr<24, 0>
    lrw     r0, 0x1018
    mtcr    r0, cr<23, 0>

    // WSSR.TSC = 1 to allow RTE to REE
    mfcr    r0, cr<0, 3>
    bseti   r0, 0
    mtcr    r0, cr<0, 3>
    // WRCR.RS = 0 to access NTW register
    mfcr    r0, cr<1, 3>
    bclri   r0, 0
    mtcr    r0, cr<1, 3>
    // set temp stack to NT_SSP
    lrw     r0, 0xf00000
    subi    r0, 12
    mov     r14, r0

    lrw     r1, __ntw_start
    movi    r2, 0
    bseti   r2, 31
    stw     r1, (r0, 8)
    stw     r2, (r0, 4)
    movi    r1, 0
    stw     r1, (r0, 0)

    // WRCR.RS = 1 to access TW register
    mfcr    r0, cr<1, 3>
    bseti   r0, 0
    mtcr    r0, cr<1, 3>

    // clr CPR to be void MULU issue
    mfcr    r0, cr<9, 3>
    bclri   r0, 14
    bclri   r0, 15
    mtcr    r0, cr<9, 3>

    mfcr    r0, cr<8, 3>
    bseti   r0, 1
    bseti   r0, 0
    mtcr    r0, cr<8, 3>
    rte

__ntw_start:
#endif
    /* Init psr value */
    mfcr    r3, psr
    bseti   r3, 8
    bseti   r3, 5
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

#ifndef CONFIG_TRUST_CPU
    mfcr    r4, cr31
#ifdef CONFIG_C807_CSKY
    bseti   r4, 4
#endif
#ifdef CONFIG_C810_CSKY
    bseti   r4, 2
    bseti   r4, 8
#endif
    mtcr    r4, cr31
#endif /* CONFIG_TRUST_CPU */

    jbsr    _main

__exit:
    bkpt
    br      __exit

