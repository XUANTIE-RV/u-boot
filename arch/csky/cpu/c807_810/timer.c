/*
* Copyright (C) 2017-2020 Alibaba Group Holding Limited
*
* SPDX-License-Identifier: GPL-2.0+
*/

#include <common.h>
#include <fdtdec.h>
#include <asm/io.h>
#include <asm/arch/timer.h>

DECLARE_GLOBAL_DATA_PTR;

#define SYS_TIMER           ((PCKPStruct_TIMER)TIMER_BASEADDRESS)
#define TIMER_CLOCK         (LSP_CLOCK * 1000 * 1000)
#define COUNT_TO_USEC(x)    ((x) / LSP_CLOCK)
#define USEC_TO_COUNT(x)    ((x) * LSP_CLOCK)
#define TICKS_PER_HZ        (TIMER_CLOCK / CONFIG_SYS_HZ)
#define TICKS_TO_HZ(x)      ((x) / TICKS_PER_HZ)
#define TIMER_LOAD_VAL      0xffffffff

static ulong read_timer(void)
{
    ulong time;
    time = SYS_TIMER->TxCurrentValue;
    return ~time;
}
/* init timer register */
int timer_init(void)
{
    gd->arch.tbl = 0;
    gd->arch.tbu = 0;
    /* load time (FULL) */
    SYS_TIMER->TxLoadCount = TIMER_LOAD_VAL;

    /*in user-defined running mode*/
    SYS_TIMER->TxControl |= TIMER_TXCONTROL_MODE;

    /* enable the corresponding timer */
    SYS_TIMER->TxControl &= ~(TIMER_TXCONTROL_ENABLE);
    SYS_TIMER->TxControl |= TIMER_TXCONTROL_ENABLE;
    return 0;
}

ulong get_timer_masked(void)
{
    /* current tick value */
    ulong now = TICKS_TO_HZ(read_timer());

    if (now >= gd->arch.lastinc)    /* normal (non rollover) */
        gd->arch.tbl += (now - gd->arch.lastinc);
    else {
        /* rollover */
        gd->arch.tbl += (TICKS_TO_HZ(TIMER_LOAD_VAL)
                - gd->arch.lastinc) + now;
    }
    gd->arch.lastinc = now;

    return gd->arch.tbl;
}

/* timer without interrupts */
ulong get_timer(ulong base)
{
    return get_timer_masked() - base;
}

/* delay x useconds */
void __udelay(unsigned long usec)
{
    long tmo = USEC_TO_COUNT(usec);
    ulong now, last = read_timer();

    while (tmo > 0) {
        now = read_timer();
        if (now > last)    /* normal (non rollover) */
            tmo -= now - last;
        else        /* rollover */
            tmo -= TIMER_LOAD_VAL - last + now;
        last = now;
    }
}

/*
* This function is derived from PowerPC code (read timebase as long long).
* On CSKY it just returns the timer value.
*/
unsigned long long get_ticks(void)
{
    return get_timer(0);
}

/*
* This function is derived from PowerPC code (timebase clock frequency).
* On CSKY it returns the number of timer ticks per second.
*/
ulong get_tbclk(void)
{
    return CONFIG_SYS_HZ;
}

unsigned long timer_get_us(void)
{
    return (get_timer(0) * 1000);
}

