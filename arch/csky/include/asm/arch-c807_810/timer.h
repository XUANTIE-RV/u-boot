/*
* Copyright (C) 2017-2020 Alibaba Group Holding Limited
*
* SPDX-License-Identifier: GPL-2.0+
*/

#ifndef _E_TIMER_H_
#define _E_TIMER_H_

#include <asm/types.h>

typedef struct CKS_TIMER
{
    volatile u32    TxLoadCount;    /*  Timer Value to be loaded into Timer */
    volatile u32    TxCurrentValue; /*  Timer Current Value of Timer */
    volatile u32    TxControl;      /* Timer Control Register for Timer */
    volatile u32    TxEOI;          /* Timer Clears the interrupt from Timer */
    volatile u32    TxIntStatus;    /* Timer0 Contains the interrupt status for Timer*/

} CKStruct_TIMER,* PCKPStruct_TIMER;


/*
*  define the bits for TxControl
*/
#define TIMER_TXCONTROL_ENABLE      (1UL << 0)
#define TIMER_TXCONTROL_MODE        (1UL << 1)
#define TIMER_TXCONTROL_INTMASK     (1UL << 2)


typedef struct CKS_TIMER_CON
{
    volatile u32    IntStatus;  /*  Contains the interrupt status of all
                    *  timers in the component.
                    */
    volatile u32    EOI;        /*  Returns all zeroes (0) and clears all
                    *  active interrupts.
                    */
    volatile u32    RawIntStatus;   /*  Contains the unmasked interrupt status
                    *  of all timers in the component.
                    */

} CKStruct_TIMER_CON,* PCKStruct_TIMER_CON;
#endif /* _E_TIMER_H_ */

