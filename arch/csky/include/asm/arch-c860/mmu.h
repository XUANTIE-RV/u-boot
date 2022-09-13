/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __MMU_H_
#define __MMU_H_

.macro  csky_mmu_disable
movi    a3, 0x0
mtcr    a3, cr18

.endm

.macro create_page_table vma
/*
* invalid the jTLB
*/
bgeni   a0, 26
mtcr    a0, cr<8, 15>

/* Virtual address */
lrw     a3, \vma
mtcr    a3, cr<4, 15> //MEH

/* page size 16M */
bmaski  a2, 12
lsli    a2, 13
mtcr    a2, cr<6, 15> //MPR

/* MEL0 */
movi    a1, 0x1e     //c = 011, D = 1, V = 1, G = 0, pfn = 0x0
or      a1, a3       // set pfn
mtcr    a1, cr<2, 15> // MEL0

/* MEL1 */
movi    a2, 0x1e     // C = 011, D = 1, V = 1, G = 0, pfn = 0x0
or      a2, a3       // set pfn

mtcr    a1, cr<2, 15> // MEL0

/* MEL1 */
movi    a2, 0x1e     // C = 011, D = 1, V = 1, G = 0, pfn = 0x0
or      a2, a3       // set pfn
bseti   a2, 24       // add size of page
mtcr    a2, cr<3, 15> // MEL1

/* rand write */
bgeni   a0, 28
mtcr    a0, cr<8, 15> //MCIR
.endm


.macro  csky_mmu_enable
mfcr    a3, cr18
bseti   a3, 0        // bit 0~1, mmu enable
bclri   a3, 1        //
bseti   a3, 2        // enable IE
bseti   a3, 3        // enable DE
bseti   a3, 4        // write back
bseti   a3, 5        // return stack
bseti   a3, 6        // jumping predict
mtcr    a3, cr18

.endm
#endif
