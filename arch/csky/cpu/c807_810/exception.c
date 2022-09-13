/*
* Copyright (C) 2017-2020 Alibaba Group Holding Limited
*
* SPDX-License-Identifier: GPL-2.0+
*/

#include <serial.h>
#include <common.h>

#define CSKY_VECTOR_SYS         31
#define CSKY_VECTOR_RESET       0
#define CSKY_VECTOR_AUTOVEC     10
#define CSKY_VECTOR_FASTAUTOVEC 11
#define CSKY_VECTOR_TLBMISS     14

extern  volatile unsigned int csky_vsr_table[128];
extern void hw_vsr_default_exception(void);
extern void hw_vsr_autovec(void);
extern void hw_vsr_fastautovec(void);
extern void hw_vsr_reset(void);
extern void hw_vsr_tlbmiss(void);
extern int mini_printf(const char *fmt, ...);

static char *exception_names[] =
{
    "RESET PC", "BUS ERROR", "ADDRESS ERROR", "ZERO DIVIDE",
    "ILLEGAL INSTRUCTION",  "PRIVILEGE VIOLATION",
    "TRACE", "BREAK POINT ERROR", "FATAL ERROR" , "Idly4 ERROR",
    "" , "" ,  "UNASSIGNED RESERVED HAI", "UNASSIGNED RESERVED FP",
    "" , "" , "TRAP #0" , "TRAP #1", "TRAP #2", "TRAP #3",
    "UNASSIGNED RESERVED 20", "UNASSIGNED RESERVED 21",
    "UNASSIGNED RESERVED 22", "UNASSIGNED RESERVED 23",
    "UNASSIGNED RESERVED 24", "UNASSIGNED RESERVED 25",
    "UNASSIGNED RESERVED 26", "UNASSIGNED RESERVED 27",
    "UNASSIGNED RESERVED 28", "UNASSIGNED RESERVED 29",
    "UNASSIGNED RESERVED 30",  "SYSTEM DESCRIPTION POINT",
};

/*
* CKCORE_SAVED_CONTEXT -- Saved by a normal interrput or exception
*/
typedef struct
{
    unsigned int pc;
    unsigned int psr;
    unsigned int r0;
    unsigned int r1;
    unsigned int r2;
    unsigned int r3;
    unsigned int r4;
    unsigned int r5;
    unsigned int r6;
    unsigned int r7;
    unsigned int r8;
    unsigned int r9;
    unsigned int r10;
    unsigned int r11;
    unsigned int r12;
    unsigned int r13;
    unsigned int r15;
    unsigned int r16;
    unsigned int r17;
    unsigned int r18;
    unsigned int r19;
    unsigned int r20;
    unsigned int r21;
    unsigned int r22;
    unsigned int r23;
    unsigned int r24;
    unsigned int r25;
    unsigned int r26;
    unsigned int r27;
    unsigned int r28;
    unsigned int r29;
    unsigned int r30;
    unsigned int r31;
}Ckcore_SavedRegisters;

extern void csky_default_exception_handler(int vector, Ckcore_SavedRegisters *regs);

/******************************************************************
csky_default_exception_handler -- The default exception handler
INPUT:
vector : the exception vector number

regs : the registers that have restored

RETURN VALUE: None

******************************************************************/

void csky_default_exception_handler(int vector, Ckcore_SavedRegisters *regs)
{
#ifndef CONFIG_SPL_BUILD
    printf("Exception: %s\n", exception_names[vector]);
#else
    mini_printf("Exception: %s\n", exception_names[vector]);
#endif
}

/*********************************************************
csky_exception_init -- Initialize the vector table

INPUT: None

RETURN VALUE: None

*********************************************************/

int csky_exception_init (void)
{
    int i;

    for (i = 0; i <= CSKY_VECTOR_SYS; i++)
    {
        csky_vsr_table[i] =(unsigned int) hw_vsr_default_exception;
    }
    csky_vsr_table[CSKY_VECTOR_RESET] = (unsigned int) hw_vsr_reset;
    csky_vsr_table[CSKY_VECTOR_AUTOVEC] = (unsigned int) hw_vsr_autovec;
    csky_vsr_table[CSKY_VECTOR_FASTAUTOVEC] =
        (unsigned int) hw_vsr_fastautovec | 0x1;
#ifdef CONFIG_CKCPU_MMU
    csky_vsr_table[CSKY_VECTOR_TLBMISS] = (unsigned int) hw_vsr_tlbmiss;
#endif
    return 0;
}

