/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __INTERRUPT_H_
#define __INTERRUPT_H_

#include <asm/types.h>

/*
* define the registers structure of the interrupt controller
*/

typedef struct CKS_INTC {
    union {
        volatile u16    ICR;
        volatile u16    ISR;
    };
    volatile u32    Rev0;
    volatile u32    IFR;
    volatile u32    IPR;
    volatile u32    NIER;
    volatile u32    NIPR;
    volatile u32    FIER;
    volatile u32    FIPR;
    volatile u32    Rev[8];
    volatile u32    PR[8];
} CKStruct_INTC, *PCKStruct_INTC;

/*
*  Bit Definition for the PIC Interrupt control register
*/
#define ICR_AVE   0x80000000  /* Select vectored interrupt */
#define ICR_FVE   0x40000000  /* Unique vector number for fast vectored*/
#define ICR_ME    0x20000000  /* Interrupt masking enabled */
#define ICR_MFI   0x10000000  /* Fast interrupt requests masked by MASK value */


void enable_normalirq(int irq);
void disable_normalirq(int irq);
int disable_inerrupts(void);

#endif /* __INTERRUPT_H_ */
