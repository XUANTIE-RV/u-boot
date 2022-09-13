/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include "config.h"

.global hw_vsr_default_exception
.global hw_vsr_autovec
.global hw_vsr_fastautovec
.global hw_vsr_reset
.global hw_vsr_tlbmiss
.extern csky_default_exception_handler
.extern ck_irq_service

        .text
        .balign 4
hw_vsr_default_exception:
        subi    sp, 124             /* Preserve the entire state. */
        stm     r0-r13, (sp)
        addi    sp, 56
        stm     r15-r31, (sp)
        subi    sp, 56

        subi    sp, 8
        mfcr    r2, epsr
        stw     r2, (sp,4)         /* save epsr registwr */
        mfcr    r2, epc
        stw     r2, (sp,0)         /* save epc register */

        mfcr    r0, psr            /* Get psr register */
        lsri    r0, 16             /* Get vector in 7 bits */
        sextb   r0                 /* Fill upper bytes with zero */
        mov     r1, sp             /* pass stack point to r3*/

        jsri    csky_default_exception_handler
        /* exception   VSR.*/

        ldw     r2, (sp,0)
        mtcr    r2, epc            /* restore the epc */
        ldw     r2, (sp,4)
        mtcr    r2, epsr           /* restore the epsr */
        addi    sp, 8
        ldm     r0-r13,(sp)        /* restore all the regs */
        addi    sp,56
        ldm     r15-r31,(sp)
        addi    sp,68

        rte

/*********************************************************************/
/*      Normal interrupt vector handler.                             */
/*********************************************************************/
        .text
        .balign 4
hw_vsr_autovec:
        subi    sp, 124             /* Preserve the entire state. */
        stm     r0-r13, (sp)
        addi    sp, 56
        stm     r15-r31, (sp)
        subi    sp, 56

        subi    sp, 8
        mfcr    r2, epsr
        stw     r2, (sp,4)         /* save epsr registwr */
        mfcr    r2, epc
        stw     r2, (sp,0)         /* save epc register */

        /* read the interrupt vector number from interrupt status register */
        lrw     r4, INTC_BASEADDRESS
        ldw     r0, (r4, 0x0)
        movi    r10,0x7f
        and     r0, r10
        subi    r0, 0x20

        jsri    ck_irq_service

        ldw     r2, (sp,0)
        mtcr    r2, epc           /* restore the epc */
        ldw     r2, (sp,4)
        mtcr    r2, epsr          /* restore the epsr */
        addi    sp, 8
        ldm     r0-r13,(sp)       /* restore all the regs */
        addi    sp,56
        ldm     r15-r31,(sp)
        addi    sp,68
        rte

/****************************************************************************/
/*      Fast interrupt vector handler.                                      */
/****************************************************************************/
        .text
        .balign 4
hw_vsr_fastautovec:
        /*read the interrupt vector number from interrupt status register */
        lrw     r4, INTC_BASEADDRESS
        ldw     r0, (r4, 0x0)
        movi    r10,0x7f
        and     r0, r10

        jsri    ck_irq_service


        rfi                      /*   Restore the psr and pc     */

/****************************************************************************/
/*      reset interrupt vector handler.                                     */
/****************************************************************************/
        .text
        .balign 4
hw_vsr_reset:

        jsri    _start

        rte                      /*   Restore the psr and pc     */
