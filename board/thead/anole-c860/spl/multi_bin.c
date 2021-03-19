/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <linux/types.h>
#include <common.h>
#include "../../common/mini_printf.h"


typedef struct {
    u8  magic[8];
    u8  count;
    u8  reserved[7];
} multibin_header_t;

typedef struct {
    u32 offset;
    u32 length;
    u32 reserved[2];
} multibin_info_t;

typedef struct {
    multibin_header_t   header;
    multibin_info_t     bin[8];
} multibin_file_header_t;



int check_multi_bin_magic(phys_addr_t img_baseaddr)
{
    multibin_file_header_t* file_header = (multibin_file_header_t*)img_baseaddr;
    u8* magic = file_header->header.magic;
    if (magic[0] != 'M' || magic[1] != 'B' || magic[2] != 'I' || magic[3] != 'N'
        || magic[4] != 0x20 || magic[5] != 0x20 || magic[6] != 0x03 || magic[7] != 0x30) {
        return -1;
    }

    return 0;
}

int clear_multi_bin_magic(phys_addr_t img_baseaddr)
{
    multibin_file_header_t* file_header = (multibin_file_header_t*)img_baseaddr;
    memset(file_header->header.magic, 0, 8);
    return 0;
}

static void ram_load_image(u32 offset, u32 size, phys_addr_t baseaddr)
{
    u8* src = (u8*)offset;
    u8* dst = (u8*)baseaddr;
    mini_printf("load image(0x%x,0x%x) to addr(0x%lx)\n", offset, size, baseaddr);
    memcpy(dst, src, size);
}

int load_from_multi_bin(phys_addr_t img_baseaddr, phys_addr_t fdt_baseaddr, phys_addr_t uboot_baseaddr)
{
    multibin_file_header_t* file_header = (multibin_file_header_t*)img_baseaddr;

    if (file_header->header.count < 2) {
        mini_printf("image count less than 2!\n");
        return -1;
    }

    multibin_info_t* fdt_info = &file_header->bin[0];
    multibin_info_t* uboot_info = &file_header->bin[1];

    ram_load_image(img_baseaddr + fdt_info->offset, fdt_info->length, fdt_baseaddr);
    ram_load_image(img_baseaddr + uboot_info->offset, uboot_info->length, uboot_baseaddr);

    return 0;
}

