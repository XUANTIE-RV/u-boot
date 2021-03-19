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


// #define DEBUG_RAM_IMAGE


extern s32 uart_open(u32 uart_addrbase);
extern void sdram_init(void);

#ifdef CONFIG_IS_ASIC
static void print_some_freq(void)
{
	u32 read = 0;

	mini_printf("\n\n---- Welcome to ICE EVB_BOARD T-HEAD ----\n\n");
        read = *(volatile unsigned int*)(0xfff77120);
	mini_printf("CPU_CLK = %dMHz\n",read/1000);
        read = *(volatile unsigned int*)(0xfff77124);
	mini_printf("AXI_CLK = %dMHz\n",read/1000);
        read = *(volatile unsigned int*)(0xfff77130);
	mini_printf("AHB_CLK = %dMHz\n",read/1000);
        read = *(volatile unsigned int*)(0xfff77140);
	mini_printf("NPU_CLK = %dMHz\n",read/1000);
        read = *(volatile unsigned int*)(0xfff7712c);
        read = *(volatile unsigned int*)(0xfff7712c);
	mini_printf("DDR_CK = %d MT\n",read/1000*4);
	mini_printf("GMAC = RGMII MODE\n");
}
#endif

void board_init_f(ulong dummy)
{
#ifdef CONFIG_IS_ASIC
    int ddr_freq = 1600;
    int cpu_freq = 1200;

    sys_clk_config(cpu_freq, ddr_freq);
#endif
    uart_open(CONSOLE_UART_BASE);
    mini_printf("Welcome to SPL!\n");
#ifdef CONFIG_IS_ASIC
    print_some_freq();
#endif
    init_ddr();
	asm volatile("mtcr %0, cr<21, 1>\n" : : "r"(0x100));
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
    case 0x0:
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

	asm volatile ("bkpt\n");
    while (1);
}
