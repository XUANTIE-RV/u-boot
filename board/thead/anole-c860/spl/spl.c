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


extern void sys_clk_config(int cpu_freq, int ddr_freq);
extern s32 uart_open(u32 uart_addrbase);
extern void sdram_init(void);

extern int check_multi_bin_magic(phys_addr_t img_baseaddr);
extern int clear_multi_bin_magic(phys_addr_t img_baseaddr);
extern int load_from_multi_bin(phys_addr_t img_baseaddr, phys_addr_t fdt_baseaddr, phys_addr_t uboot_baseaddr);


#define UBOOT_IMG_BASEADDR (u32)(0xfe500000)

static void print_some_freq(void)
{
    u32 read = 0;

    mini_printf("\n\n---- Welcome to Anole EVB_BOARD T-HEAD ----\n");
    read = *(volatile unsigned int*)(0xfff77120);
    mini_printf("CPU_CLK = %dMHz\n",read/1000);
    read = *(volatile unsigned int*)(0xfff77124);
    mini_printf("AXI_CLK = %dMHz\n",read/1000);
    read = *(volatile unsigned int*)(0xfff77130);
    mini_printf("AHB_CLK = %dMHz\n",read/1000);
    read = *(volatile unsigned int*)(0xfff7712c);
    mini_printf("DDR_CK = %d MT\n",read/1000*4);
    mini_printf("GMAC = RGMII MODE\n");
}

void board_init_f(ulong dummy)
{
#ifdef DDRINIT_CLOCK_FREQ_MT
    int ddr_freq = DDRINIT_CLOCK_FREQ_MT;
#else
    int ddr_freq = 2000;
#endif

    /* initialize clocks */
    sys_clk_config(1000, ddr_freq);

    /* Clear global data */
    uart_open(CONSOLE_UART_BASE);
    mini_printf("Welcome to SPL!\n");

    sdram_init();
    print_some_freq();
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
    u8 image_loaded;
    void (*image_entry)(u32, phys_addr_t);
    /* Because of the relocation of uboot, the address of uboot in DDR will change.
    So we prepare the uboot at the address which is calculated by uboot itself.
    Different DDR address and size will create different uboot address. */
    phys_addr_t uboot_baseaddr = CONFIG_SYS_TEXT_BASE;
    phys_addr_t fdt_baseaddr = uboot_baseaddr - 0x10000;

    mini_printf("The U-Boot-spl start.\n");
    mini_printf("U-Boot version is 2020.01, internal version is %s\n", UBOOT_INTERNAL_VERSION);

    if (check_multi_bin_magic(UBOOT_IMG_BASEADDR) == 0) {
        om_judge = 0;
    } else {
        om_judge = 1;
    }

    image_loaded = 0;
    switch (om_judge) {
    case 0x0:
        mini_printf("This is ram mode.\n");
        ret = load_from_multi_bin(UBOOT_IMG_BASEADDR, fdt_baseaddr, uboot_baseaddr);
        if (ret != 0) {
            mini_printf("load from ram failed.\n");
            break;
        }

        clear_multi_bin_magic(UBOOT_IMG_BASEADDR);
        image_loaded = 1;
        break;

    default:
        mini_printf("This is eMMC mode.\n");
        ret = emmc_host_init(NULL);
        if (ret != 0) {
            mini_printf("The eMMC is not exist.\n");
            break;
        }
        mini_printf("eMMC init ready.\n");

        mini_printf("load image, uboot_baseaddr: %x, fdt_baseaddr: %x\n", (u32)uboot_baseaddr, (u32)fdt_baseaddr);
        emmc_load_image(FLASH_UBOOT_READ_ADDR, FLASH_UBOOT_SIZE, uboot_baseaddr);
        emmc_load_image(FLASH_FDT_READ_ADDR, FLASH_FDT_SIZE, fdt_baseaddr);
        image_loaded = 1;
        break;
    }

    if (image_loaded) {
        image_entry = (void (*)(u32, phys_addr_t))(*((u32*)uboot_baseaddr));
        mini_printf("Jump to image_entry: %x\n", (u32)image_entry);
        image_entry(0, fdt_baseaddr);
    }

    // always loop
    while (1);
}
