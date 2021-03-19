/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <linux/types.h>
#include <common.h>
#include <console.h>
#include <asm/io.h>
#include <spl.h>
#include <asm/spl.h>
#include "../../common/datatype.h"
#include "../../common/mini_printf.h"
#include "../../common/mmc/emmc.h"
#include "../../common/spiflash/spiflash.h"
#include "../hardware.h"


extern s32 uart_open(u32 uart_addrbase);


void board_init_f(ulong dummy)
{
    /* Clear global data */
    uart_open(CONSOLE_UART_BASE);
    mini_printf("Welcome to SPL!\n");
}

void board_init_r(gd_t *gd, ulong dummy)
{
    u32 ret;
    s8 om_judge;
    void (*load_image)(u32 offset, u32 size, phys_addr_t baseaddr);
    void (*image_entry)(u32, phys_addr_t);
    /* Because of the relocation of uboot, the address of uboot in DDR will change.
    So we prepare the uboot at the address which is calculated by uboot itself.
    Different DDR address and size will create different uboot address. */
    phys_addr_t uboot_baseaddr = CONFIG_SYS_TEXT_BASE;
    phys_addr_t fdt_baseaddr = uboot_baseaddr - 0x10000;

    mini_printf("The U-Boot-spl start.\n");
    mini_printf("U-Boot version is 2020.01, internal version is %s-beta4\n", UBOOT_INTERNAL_VERSION);

    image_entry = (void (*)(u32, phys_addr_t))(*((u32*)uboot_baseaddr));
    mini_printf("Jump to image_entry: %x\n", image_entry);
    image_entry(0, fdt_baseaddr);

    // always loop
    while (1);
}
