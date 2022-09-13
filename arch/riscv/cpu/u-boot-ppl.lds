
/*
 * Copyright (C) 2020 C-SKY Microsystems
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

MEMORY { .ppl_mem : ORIGIN = IMAGE_TEXT_BASE, LENGTH = IMAGE_MAX_SIZE }
MEMORY { .bss_mem : ORIGIN = CONFIG_PPL_BSS_START_ADDR, \
            LENGTH = CONFIG_PPL_BSS_MAX_SIZE }

OUTPUT_ARCH("riscv")
ENTRY(_start)

SECTIONS
{
    . = ALIGN(4);
    .text : {
        arch/riscv/cpu/start.o	(.text)
        *(.text*)
    } > .ppl_mem

    . = ALIGN(4);
    .rodata : {
        *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
    } > .ppl_mem

    . = ALIGN(4);
    .data : {
        *(.data*)
    } > .ppl_mem
    . = ALIGN(4);

    .got : {
        __got_start = .;
        *(.got.plt) *(.got)
        __got_end = .;
    } > .ppl_mem

    . = ALIGN(4);

    .dynsym : {
        __dyn_sym_start = .;
        *(.dynsym)
        __dyn_sym_end = .;
    } > .ppl_mem

    . = ALIGN(4);

    _end = .;

    .bss : {
        __bss_start = .;
        *(.bss*)
        . = ALIGN(8);
        __bss_end = .;
    } > .bss_mem
}
