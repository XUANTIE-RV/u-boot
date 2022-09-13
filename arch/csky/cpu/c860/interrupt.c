/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <irq_func.h>
#include <asm/arch/interrupt.h>

#define CONFIG_SYS_NUM_IRQS        32

#define PCK_INTC ((PCKStruct_INTC)INTC_BASEADDRESS)

void mask_irq(int irq);
void unmask_irq(int irq);

static void enter_critical(int *psr);
static void exit_critical(int psr);


struct IRQ_Handler {
    interrupt_handler_t *handler;
    void *arg;
};

/*
* PCK_INTC: The base address of interrupt controller registers
*/
volatile CKStruct_INTC *icrp = PCK_INTC;

static struct IRQ_Handler ckirq_handlers[CONFIG_SYS_NUM_IRQS] = { {0} };

int interrupt_init(void)
{
#if 0
    /* initialize PR0-PR31, big endian */
    icrp->PR[0] = 0x00010203;
    icrp->PR[1] = 0x04050607;
    icrp->PR[2] = 0x08090a0b;
    icrp->PR[3] = 0x0c0d0e0f;
    icrp->PR[4] = 0x10111213;
    icrp->PR[5] = 0x14151617;
    icrp->PR[6] = 0x18191a1b;
    icrp->PR[7] = 0x1c1d1e1f;

    icrp->ICR |= ICR_AVE;
    mask_irq(31);        /* first mask all the interrupt */
    enable_interrupts();
#endif
    return 0;
}

/*
* Install an interrupt handler
*/
void irq_install_handler(int irq, interrupt_handler_t *handler, void *arg)
{
    int status;

    if (irq < 0 || irq >= CONFIG_SYS_NUM_IRQS) {
        printf("irq_install_handler: bad irq number %d\n", irq);
        return;
    }

    status = disable_interrupts();

    ckirq_handlers[irq].handler = handler;
    ckirq_handlers[irq].arg = arg;

    enable_normalirq(irq);

    unmask_irq(irq);

    if (status) {
        enable_interrupts();
    }
}

/*
* Free an interrupt handler
*/
void irq_free_handler(int irq)
{
    int status;

    if (irq < 0 || irq >= CONFIG_SYS_NUM_IRQS) {
        printf("irq_free_handler: bad irq number %d\n", irq);
        return;
    }

    status = disable_interrupts();

    disable_normalirq(irq);

    mask_irq(irq);

    ckirq_handlers[irq].handler = NULL;
    ckirq_handlers[irq].arg = NULL;

    if (status) {
        enable_interrupts();
    }
}

void enable_interrupts(void)
{
    asm("psrset ee,ie");
}

int disable_interrupts(void)
{
    asm("psrclr ie");
    return 1;
}

/*
* This function enables a priority level of normal interrupt.
* irq: A priority of normal interrupt which between 0 to 31.
*/
void enable_normalirq(int irq)
{
    int psrbk;

    enter_critical(&psrbk);
    icrp->NIER |= (1 << irq);
    exit_critical(psrbk);
}

/*
* This function disables a priority level of normal interrupt.
* irq: A priority of normal interrupt which between 0 to 31.
*/
void disable_normalirq(int irq)
{
    int psrbk;

    enter_critical(&psrbk);
    icrp->NIER &= ~(1 << irq);
    exit_critical(psrbk);
}

/*
* This function enables normal interrupt masking.
*
* irq: The priority level that would be masked,
* leading to mask those priority levels below primask.
*/
void mask_irq(int irq)
{
}

/*
* This function disables normal interrupt masking.
*
* irq: The priority level that would be unmasked.
*/
void unmask_irq(int irq)
{
}

/*
* Execute the interrupt service.
*
*irq :
*RETURN VALUE: None
*/
void ck_irq_service(int irq)
{
    if (irq < 0 || irq >= CONFIG_SYS_NUM_IRQS) {
        printf("ck_irq_service: bad irq number %d\n", irq);
        return;
    }

    if (ckirq_handlers[irq].handler) {
        mask_irq(irq);

        ckirq_handlers[irq].handler(ckirq_handlers[irq].arg);

        unmask_irq(irq);
    }
}

/*
* This function should be called before executing critical code which should not be
*interrupted by other interrupts
*
*INPUT: psr, to store the value of psr
*RETURN VALUE: None
*/
static void enter_critical(int *psr)
{
    asm volatile("mfcr    %0, psr\n\r"
                "psrclr  ie, fe"
                : "=r"(*psr));
}

/*This function should be called after exiting critical area.
*
*INPUT: psr, contain the backup value of psr
*RETURN VALUE: None
*/
static void exit_critical(int psr)
{
    asm volatile("mtcr   %0, psr"
                :
                :"r"(psr));
}
