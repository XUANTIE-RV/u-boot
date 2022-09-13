/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __THEAD_GPIO_H__
#define __THEAD_GPIO_H__

#include <linux/types.h>
#include "../../common/datatype.h"
#include "../hardware.h"
/*
* Define IRQBurst type, GPIO Port, and Port's direction. They are all enum
* type.
*/
typedef enum{
    GPIOA = GPIOA_BASEADDR,
    GPIOB = GPIOB_BASEADDR,
#ifdef GPIOC_BASEADDR
    GPIOC = GPIOC_BASEADDR,
#endif
#ifdef GPIOD_BASEADDR
    GPIOD = GPIOD_BASEADDR,
#endif
}enum_gpio_device_t;

typedef enum{
    GPIO_BESOFTWARE,
    GPIO_BEHARDWARE
}enum_gpio_mode;

/* Define the functions in gpio.c. */

/*
* Choose the software mode or hardware mode for any IO bit.
* Parameters:
*   pins: choose the bits which you want to config.
*   bhardware:
*       '1' -- the corresponding pins are software mode, or, as GPIO.
*       '0' -- the corresponding pins are hardware mode, or, used as UART, LCDC *   etc.
* return: SUCCESS or FAILURE.
*/

bool gpio_set_reuse(enum_gpio_device_t gpio_addrbase, u32 pins, enum_gpio_mode bhardware);

/*
* Define a struct that contains three registers of one Port -- SWPORT_DR,
* SWPORT_DDR, and PORT_CTL.
*/

typedef struct {
    volatile u32    SWPORT_DR;
    volatile u32    SWPORT_DDR;
    volatile u32    PORT_CTL;
} ckstruct_gpio_t, *pckstruct_gpio_t;


#endif /* __THEAD_GPIO_H__ */
