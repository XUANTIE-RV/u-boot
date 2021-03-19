/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include "gpio.h"
/*
* Choose the software mode or hardware mode for any IO bit.
* Parameters:
*   pins: choose the bits which you want to config.
*   bSoftware:
*       '1' -- the corresponding pins are software mode, or, as GPIO.
*       '0' -- the corresponding pins are hardware mode, or, used as UART, LCDC *               etc.
* return: SUCCESS or FAILURE.
*/
bool gpio_set_reuse(enum_gpio_device_t gpio_addrbase, u32 pins, enum_gpio_mode bhardware)
{
    pckstruct_gpio_t reg;
#if 0
    reg = (pckstruct_gpio_t)gpio_addrbase;
    if (bhardware) {
        reg->PORT_CTL |= pins;
    } else {
        reg->PORT_CTL &= (~pins);
    }
#endif
    return SUCCESS;
}

