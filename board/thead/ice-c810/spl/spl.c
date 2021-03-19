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
#include "../om/om.h"
#include "../gpio/gpio.h"
#include "../hardware.h"

extern s32 uart_open( u32 uart_addrbase);
extern void sdram_init(void);

void board_init_f(ulong dummy)
{
    /* Clear global data */
    uart_open(CONSOLE_UART_BASE);
    sdram_init();
    mini_printf("Welcome to SPL!\n");
}

#ifdef DEBUG_RAM_IMAGE
static void ram_load_image(u32 offset, u32 size, phys_addr_t baseaddr)
{
    // please use gdb load image to ram
}
#endif

static void spiflash_load_image(u32 offset, u32 size, phys_addr_t baseaddr)
{
    int i, retlen;
    for (i = 0; i < (size + 255) / 256; i++) {
        spiflash_read(0, offset + (i * 256), baseaddr + (i * 256) , 256, &retlen);
    }
}

static void emmc_load_image(u32 offset, u32 size, phys_addr_t baseaddr)
{
    int i;
    for (i = 0; i < (size + 511) / 512; i++) {
        emmc_emmc_read(0, (offset + (i * 512)) / 0x200, 512, (u8 *)(baseaddr + (i * 512)));
    }
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
    mini_printf("U-Boot version is 2020.03, internal version is %s\n", UBOOT_INTERNAL_VERSION);

    load_image = NULL;
    om_judge = get_boot_select();
    switch (om_judge) {
#ifdef DEBUG_RAM_IMAGE
    case 0x2:
        mini_printf("This is ram mode.\n");
        load_image = ram_load_image;
        break;
#endif
    case 0x1:
        /* The mode of spi flash */
        mini_printf("This is spiflash mode.\n");
        load_image = spiflash_load_image;
        break;
    case 0x2:
        /* The mode of emmc */
        mini_printf("This is eMMC mode.\n");
        ret = emmc_host_init(NULL);
        if (ret != 0) {
            mini_printf("The eMMC is not exist.\n");
            break;
        }
        mini_printf("eMMC init ready.\n");
        load_image = emmc_load_image;
        break;
    default:
        mini_printf("OM mode is %x, please check the OM.\n", om_judge);
        break;
    }

    if (load_image) {
        load_image(FLASH_UBOOT_READ_ADDR, FLASH_UBOOT_SIZE, uboot_baseaddr);
        load_image(FLASH_FDT_READ_ADDR, FLASH_FDT_SIZE, fdt_baseaddr);

        image_entry = (void (*)(u32, phys_addr_t))(*((u32*)uboot_baseaddr));
        mini_printf("Jump to image_entry: %x, fdt_baseaddr: %x\n", image_entry, fdt_baseaddr);
        image_entry(0, fdt_baseaddr);
    }

    // always loop
    while (1);
}

