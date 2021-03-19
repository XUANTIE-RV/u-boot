/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <image.h>
#include <u-boot/zlib.h>
#include <asm/byteorder.h>
#include <linux/libfdt.h>
#include <mapmem.h>
#include <fdt_support.h>
#include <linux/compiler.h>
#include <bootm.h>
#include <vxworks.h>
#include <asm/arch/interrupt.h>
#include <irq_func.h>
#include <cpu_func.h>
DECLARE_GLOBAL_DATA_PTR;


__weak int board_prep_linux(bootm_headers_t *images) { return 0; }


/* Main Entry point for csky bootm implementation
*
* Modeled after the powerpc implementation
* DIFFERENCE: Instead of calling prep and go at the end
* they are called if subcommand is equal 0.
*/
int do_bootm_linux(int flag, int argc, char * const argv[],
                bootm_headers_t *images)
{
    void (*theKernel)(int magic, void * params);
    char *tmp;
    unsigned int dtb_load_addr = 0;

    if ((tmp = env_get("dtb_load_addr_virt")) != NULL) {
        dtb_load_addr = simple_strtoul(tmp, NULL, 16);
        printf("dtb_load_addr: 0x%X\n", dtb_load_addr);
    } else {
        printf("\nError! No dtb address found! Stop here...\n");
        while (1);
    }

    theKernel = (void (*)(int, void *))images->ep;
    printf("\nStarting kernel ... \n\n");

    disable_interrupts();
    flush_cache(0, 0);
#ifdef CONFIG_ERAGON3
    /* Open the D cache */
    asm volatile(
        "mfcr   r4, cr18\n"
        "bseti  r4, 3   \n"
        "mtcr   r4, cr18\n"
        :
        :
        : "r4"
    );
#endif
    board_prep_linux(images);
    theKernel(0x20150401, (void *)dtb_load_addr);
    return 1;
}
