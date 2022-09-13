/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

OUTPUT_ARCH(csky)
ENTRY(_start)
SECTIONS
{
    .text ALIGN(0x4):
    {
        __text_start = .;
        *start.o(*.exptable)
        *(.text)
        *(.text*)
        __text_end = .;
    }

    .rodata ALIGN(0x4):
    {
        __rodata_start = .;
        *(.rodata)
        *(.rodata*)
        __rodata_end = .;
    }

    .data ALIGN(0x4):
    {
        __data_start = .;
        *(.data)
        __data_end = .;
    }

    .got ALIGN(4):
    {
        _got_start = .;
        *(.got)
        . = ALIGN(4);
        _got_end = .;
    }

    . = ALIGN(4);
    .u_boot_list : {
        KEEP(*(SORT(.u_boot_list*)));
    }
    __init_end = .;

    .bss ALIGN(0x4):
    {
        __bss_start = .;
        *(.sbss)
        *(.scommon)
        *(.bss)
        *(COMMON)
        . = ALIGN(4);
        __bss_end = . ;
    }
    __end = .;
    _end = .;
}
