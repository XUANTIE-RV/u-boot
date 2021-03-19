/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __MMU_H__
#define __MMU_H__
#include <common.h>
#include <asm/types.h>

void init_mmu(void);

/**
 \brief  Consortium definition for accessing mmu entry of high physical address register(MPR, CR<6,15>).
*/
typedef union {
    struct {
        uint32_t _reserved0: 13;              /*!< bit:   0.. 12   Reserved */
        uint32_t page_mask: 12;               /*!< bit:  13.. 24   Page mask */
        uint32_t _reserved1: 7;               /*!< bit:  25.. 31   Reserved */
    } b;
    uint32_t w;
} MPR_Type;

/**
 \brief  Consortium definition for accessing mmu entry of high physical address register(MEH, CR<4,15>).
*/
typedef union {
    struct {
        uint32_t ASID : 8;                    /*!< bit:   0.. 7   ASID */
        uint32_t _reserved : 4;               /*!< bit:   7.. 10  Reserved */
        uint32_t VPN : 20;                    /*!< bit:  11.. 31  Virtual page number */
    } b;
    uint32_t w;
} MEH_Type;

/* MEH Register Definitions */
#define MEH_VPN_Pos    12                           /*!< MEH: VPN Position */
#define MEH_VPN_Msk    (0xFFFFFUL << MEH_VPN_Pos)   /*!< MEH: VPN Mask */

#define MEH_ASID_Pos   0                            /*!< MEH: ASID Position */
#define MEH_ASID_Msk   (0xFFUL << MEH_ASID_Pos)     /*!< MEH: ASID Mask */

/**
 \brief  Consortium definition for accessing mmu entry of high physical address register(MEL, CR<2,15> and CR<3,15>).
*/
typedef union {
    struct {
        uint32_t G: 1;                        /*!< bit:       0   Global enbale bit */
        uint32_t V: 1;                        /*!< bit:       1   TLB mapping valid bit */
        uint32_t D: 1;                        /*!< bit:       2   TLB Page dirty bit */
        uint32_t C: 1;                        /*!< bit:       3   TLB Page cacheable bit */
        uint32_t SEC: 1;                      /*!< bit:       4   TLB Page security bit */
        uint32_t SO: 1;                       /*!< bit:       2   Strong order enable bit */
        uint32_t B: 1;                        /*!< bit:       2   TLB Page bufferable bit */
        uint32_t _reserved: 5;                /*!< bit:   7.. 11  Reserved */
        uint32_t PFN: 20;                     /*!< bit:  12.. 31  Physical frame number */
    } b;
    uint32_t w;
} MEL_Type;


/**
 \brief  Consortium definition for accessing mmu entry of high physical address register(CR<8,15>).
*/
typedef union {
    struct {
        uint32_t ASID: 8;                     /*!< bit:   0.. 7   ASID */
        uint32_t _reserved: 17;               /*!< bit:   8.. 24  Reserved */
        uint32_t TLBINV_INDEX: 1;             /*!< bit:       25  TLBINV_INDEX */
        uint32_t TLBINV_ALL: 1;               /*!< bit:       26  TLBINV_ALL */
        uint32_t TLBINV: 1;                   /*!< bit:       27  TLBINV */
        uint32_t TLBWR: 1;                    /*!< bit:       28  TLBWR */
        uint32_t TLBWI: 1;                    /*!< bit:       29  TLBWI */
        uint32_t TLBR: 1;                     /*!< bit:       30  TLBR */
        uint32_t TLBP: 1;                     /*!< bit:       31  TLBP */
    } b;
    uint32_t w;
} MCIR_Type;

/* MCIR Register Definitions */
#define MCIR_TLBP_Pos             31                               /*!< MCIR: TLBP Position */
#define MCIR_TLBP_Msk             (0x1UL << MCIR_TLBP_Pos)         /*!< MCIR: TLBP Mask */

#define MCIR_TLBR_Pos             30                               /*!< MCIR: TLBR Position */
#define MCIR_TLBR_Msk             (0x1UL << MCIR_TLBR_Pos)         /*!< MCIR: TLBR Mask */

#define MCIR_TLBWI_Pos            29                               /*!< MCIR: TLBWI Position */
#define MCIR_TLBWI_Msk            (0x1UL << MCIR_TLBWI_Pos)        /*!< MCIR: TLBWI Mask */

#define MCIR_TLBWR_Pos            28                               /*!< MCIR: TLBWR Position */
#define MCIR_TLBWR_Msk            (0x1UL << MCIR_TLBWR_Pos)        /*!< MCIR: TLBWR Mask */

#define MCIR_TLBINV_Pos           27                               /*!< MCIR: TLBINV Position */
#define MCIR_TLBINV_Msk           (0x1UL << MCIR_TLBINV_Pos)       /*!< MCIR: TLBINV Mask */

#define MCIR_TLBINV_ALL_Pos       26                               /*!< MCIR: TLBINV_ALL Position */
#define MCIR_TLBINV_ALL_Msk       (0x1UL << MCIR_TLBINV_ALL_Pos)   /*!< MCIR: TLBINV_ALL Mask */

#define MCIR_TLBINV_INDEX_Pos     25                               /*!< MCIR: TLBINV_INDEX Position */
#define MCIR_TLBINV_INDEX_Msk     (0x1UL << MCIR_TLBINV_INDEX_Pos) /*!< MCIR: TLBINV_INDEX Mask */

#define MCIR_ASID_Pos             0                                /*!< MCIR: ASID Position */
#define MCIR_ASID_Msk             (0xFFUL << MCIR_ASID_Pos)        /*!< MCIR: ASID Mask */

/**
 \brief   Search from the highest bit that the very first bit which's value is 1.
\param [in]    value  Value to  bit search.
\return               if the highest bit' value is 1,  return 0, and if lowest bit's value is 1, return 31, otherwise return 32.
*/
inline uint32_t __FF0(uint32_t value)
{
    uint32_t ret;
    asm volatile("ff0 %0, %1" : "=r"(ret) : "r"(value));
    return ret;
}

/**
 \brief   Get CCR
\details Returns the current value of the CCR.
\return               CCR Register value
*/
inline uint32_t __get_CCR(void)
{
    register uint32_t result;
    asm volatile("mfcr %0, cr<18, 0>\n"  : "=r"(result));
    return (result);
}

/**
 \brief   Set CCR
\details Assigns the given value to the CCR.
\param [in]    ccr  CCR value to set
*/
inline void __set_CCR(uint32_t ccr)
{
    asm volatile("mtcr %0, cr<18, 0>\n" : : "r"(ccr));
}

/**
 \brief   Get MPR
\details Returns the content of the MPR Register.
\return               MPR Register value
*/
inline uint32_t __get_MPR(void)
{
    uint32_t result;
    asm volatile("mfcr %0, cr<6, 15>" : "=r"(result));
    return (result);
}

/**
 \brief   Set MPR
\details Writes the given value to the MPR Register.
\param [in]    mpr  MPR Register value to set
*/
inline void __set_MPR(uint32_t mpr)
{
    asm volatile("mtcr %0, cr<6, 15>" : : "r"(mpr));
}

/**
 \brief   Get MEH
\details Returns the content of the MEH Register.
\return               MEH Register value
*/
inline uint32_t __get_MEH(void)
{
    uint32_t result;
    asm volatile("mfcr %0, cr<4, 15>" : "=r"(result));
    return (result);
}

/**
 \brief   Set MEH
\details Writes the given value to the MEH Register.
\param [in]    meh  MEH Register value to set
*/
inline void __set_MEH(uint32_t meh)
{
    asm volatile("mtcr %0, cr<4, 15>" : : "r"(meh));
}

/**
 \brief   Get MCIR
\details Returns the content of the MCIR Register.
\return               MCIR Register value
*/
inline uint32_t __get_MCIR(void)
{
    uint32_t result;
    asm volatile("mfcr %0, cr<8, 15>" : "=r"(result));
    return (result);
}

/**
 \brief   Set MCIR
\details Writes the given value to the MCIR Register.
\param [in]    mcir  MCIR Register value to set
*/
inline void __set_MCIR(uint32_t mcir)
{
    asm volatile("mtcr %0, cr<8, 15>" : : "r"(mcir));
}

/**
 \brief   Get MIR
\details Returns the content of the MIR Register.
\return               MIR Register value
*/
inline uint32_t __get_MIR(void)
{
    uint32_t result;
    asm volatile("mfcr %0, cr<0, 15>" : "=r"(result));
    return (result);
}

/**
 \brief   Set MIR
\details Writes the given value to the MIR Register.
\param [in]    mir  MIR Register value to set
*/
inline void __set_MIR(uint32_t mir)
{
    asm volatile("mtcr %0, cr<0, 15>" : : "r"(mir));
}

/**
 \brief   Get MEL0
\details Returns the content of the MEL0 Register.
\return               MEL0 Register value
*/
inline uint32_t __get_MEL0(void)
{
    uint32_t result;
    asm volatile("mfcr %0, cr<2, 15>" : "=r"(result));
    return (result);
}

/**
 \brief   Set MEL0
\details Writes the given value to the MEL0 Register.
\param [in]    mel0  MEL0 Register value to set
*/
inline void __set_MEL0(uint32_t mel0)
{
    asm volatile("mtcr %0, cr<2, 15>" : : "r"(mel0));
}

/**
 \brief   Get MEL1
\details Returns the content of the MEL1 Register.
\return               MEL1 Register value
*/
inline uint32_t __get_MEL1(void)
{
    uint32_t result;
    asm volatile("mfcr %0, cr<3, 15>" : "=r"(result));
    return (result);
}

/**
 \brief   Set MEL1
\details Writes the given value to the MEL1 Register.
\param [in]    mel1  MEL1 Register value to set
*/
inline void __set_MEL1(uint32_t mel1)
{
    asm volatile("mtcr %0, cr<3, 15>" : : "r"(mel1));
}

/* CCR Register Definitions */
#define CCR_IPE_Pos     15u                           /*!< CCR: IPE Position */
#define CCR_IPE_Msk     (0x1UL << CCR_IPE_Pos)        /*!< CCR: IPE Mask */

#define CCR_BSTE_Pos    14u                           /*!< CCR: BSTE Position */
#define CCR_BSTE_Msk    (0x1UL << CCR_BSTE_Pos)       /*!< CCR: BSTE Mask */

#define CCR_E_V2_Pos    13U                           /*!< CCR: E_V2 Position */
#define CCR_E_V2_Msk    (0x1UL << CCR_E_V2_Pos)       /*!< CCR: E_V2 Mask */

#define CCR_WA_Pos      12u                           /*!< CCR: WA Position */
#define CCR_WA_Msk      (0x1UL << CCR_WA_Pos)         /*!< CCR: WA Mask */

#define CCR_SCK_Pos     8U                            /*!< CCR: SCK Position */
#define CCR_SCK_Msk     (0x3UL << CCR_SCK_Pos)        /*!< CCR: SCK Mask */

#define CCR_BE_Pos      7U                            /*!< CCR: BE Position */
#define CCR_BE_Msk      (0x1UL << CCR_BE_Pos)         /*!< CCR: BE Mask */

#define CCR_Z_Pos       6U                            /*!< CCR: Z Position */
#define CCR_Z_Msk       (0x1UL << CCR_BE_Pos)         /*!< CCR: Z Mask */

#define CCR_RS_Pos      5U                            /*!< CCR: RS Position */
#define CCR_RS_Msk      (0x1UL << CCR_BE_Pos)         /*!< CCR: RS Mask */

#define CCR_WB_Pos      4U                            /*!< CCR: WB Position */
#define CCR_WB_Msk      (0x1UL << CCR_BE_Pos)         /*!< CCR: WB Mask */

#define CCR_DE_Pos      3U                            /*!< CCR: DE Position */
#define CCR_DE_Msk      (0x1UL << CCR_BE_Pos)         /*!< CCR: DE Mask */

#define CCR_IE_Pos      2U                            /*!< CCR: IE Position */
#define CCR_IE_Msk      (0x1UL << CCR_BE_Pos)         /*!< CCR: IE Mask */

#define CCR_MP_Pos      0U                            /*!< CCR: MP Position */
#define CCR_MP_Msk      (0x3UL << CCR_MP_Pos)         /*!< CCR: MP Mask */

/* MEL Register Definitions */
#define MEL_PFN_Pos     12                            /*!< MEL: PFN Position */
#define MEL_PFN_Msk     (0xFFFFFUL << MEL_PFN_Pos)    /*!< MEL: PFN Mask */

#define MEL_B_Pos       6                             /*!< MEL: B Position */
#define MEL_B_Msk       (0x1UL << MEL_B_Pos)          /*!< MEL: B Mask */

#define MEL_SO_Pos      5                             /*!< MEL: SO Position */
#define MEL_SO_Msk      (0x1UL << MEL_SO_Pos)         /*!< MEL: SO Mask */

#define MEL_SEC_Pos     4                             /*!< MEL: SEC Position */
#define MEL_SEC_Msk     (0x1UL << MEL_SEC_Pos)        /*!< MEL: SEC Mask */

#define MEL_C_Pos       3                             /*!< MEL: C Position */
#define MEL_C_Msk       (0x1UL << MEL_C_Pos)          /*!< MEL: C Mask */

#define MEL_D_Pos       2                             /*!< MEL: D Position */
#define MEL_D_Msk       (0x1UL << MIR_D_Pos)          /*!< MEL: D Mask */

#define MEL_V_Pos       1                             /*!< MEL: V Position */
#define MEL_V_Msk       (0x1UL << MIR_V_Pos)          /*!< MEL: V Mask */

#define MEL_G_Pos       0                             /*!< MEL: G Position */
#define MEL_G_Msk       (0x1UL << MIR_G_Pos)          /*!< MEL: G Mask */

/**
 \brief  Consortium definition for accessing mmu index register(MIR,CR<0,15>).
*/
typedef union {
    struct {
        uint32_t Index: 10;         /*!< bit:   0.. 9   TLB index */
        uint32_t _reserved: 20;     /*!< bit:  10.. 29  Reserved */
        uint32_t TF: 1;             /*!< bit:       30  TLB fatal error */
        uint32_t P: 1;              /*!< bit:       31  TLBP instruction */
    } b;
    uint32_t w;
} MIR_Type;

/* MIR Register Definitions */
#define MIR_P_Pos       31                            /*!< PRSR: P(TLBP instruction) Position */
#define MIR_P_Msk       (0x1UL << MIR_P_Pos)          /*!< PRSR: P(TLBP instruction) Mask */

#define MIR_TF_Pos      30                            /*!< PRSR: Tfatal Position */
#define MIR_TF_Msk      (0x1UL << MIR_TF_Pos)         /*!< PRSR: Tfatal Mask */

#define MIR_Index_Pos   0                             /*!< PRSR: Index Position */
#define MIR_Index_Msk   (0x3ffUL << MIR_Index_Pos)    /*!< PRSR: Index Mask */

typedef struct {
    uint32_t global: 1;        /* tlb page global access */
    uint32_t valid: 1;         /* tlb page valid */
    uint32_t writeable: 1;     /* tlb page writeable */
    uint32_t cacheable: 1;     /* tlb page cacheable */
    uint32_t is_secure: 1;     /* tlb page security access */
    uint32_t strong_order: 1;  /* tlb sequence of accessing data on tlb page is corresponding to the program flow? */
    uint32_t bufferable: 1;    /* tlb page bufferable */
} page_attr_t;

typedef enum {
    PAGE_SIZE_4KB   = 0x000,
    PAGE_SIZE_16KB  = 0x003,
    PAGE_SIZE_64KB  = 0x00F,
    PAGE_SIZE_256KB = 0x03F,
    PAGE_SIZE_1MB   = 0x0FF,
    PAGE_SIZE_4MB   = 0x3FF,
    PAGE_SIZE_16M   = 0xFFF,
} page_size_e;

/**
 \brief  enable mmu
\details
*/
inline void csky_mmu_enable(void)
{
    __set_CCR(__get_CCR() | (1u << CCR_MP_Pos));
    __set_CCR(__get_CCR() | 0x00000008); // d cache
    __set_CCR(__get_CCR() | 0x00000004); // i cache
}

/**
 \brief disable mmu
\details
*/
inline void csky_mmu_disable(void)
{
    __set_CCR(__get_CCR() & (~(1u << CCR_MP_Pos)));
}

/**
 \brief  create page with feature.
\details
\param [in]  vaddr     virtual address.
\param [in]  paddr     physical address.
\param [in]  asid      address sapce id (default: 0).
\param [in]  attr      \ref page_attr_t. tlb page attribute.
*/
inline void csky_mmu_set_tlb(uint32_t vaddr, uint32_t paddr, uint32_t asid, page_attr_t attr)
{
    MPR_Type pgmask;
    MEH_Type meh;
    MEL_Type mel;
    uint32_t vaddr_bit;
    uint32_t page_feature = 0;

    page_feature |= attr.global << MEL_G_Pos | attr.valid << MEL_V_Pos |
                    attr.writeable << MEL_D_Pos | attr.cacheable << MEL_C_Pos |
                    attr.is_secure << MEL_SEC_Pos | attr.strong_order << MEL_SO_Pos |
                    attr.bufferable << MEL_B_Pos;
    pgmask.w = __get_MPR();
    vaddr_bit = 44 - __FF0(~((uint32_t)pgmask.b.page_mask));
    meh.b.ASID = (uint8_t)asid;
    meh.b.VPN  = (vaddr & ((~pgmask.w | 0xFE000000) & 0xFFFFE000)) >> MEH_VPN_Pos;
    __set_MEH(meh.w);
    __set_MCIR(1u << MCIR_TLBP_Pos);
    mel.w = ((paddr & ~(pgmask.b.page_mask << 12)) | page_feature);
    if (vaddr & (1 << vaddr_bit)) {
        __set_MEL1(mel.w);
    } else {
        __set_MEL0(mel.w);
    }

    if (__get_MIR() & (1 << MIR_P_Pos)) {
        __set_MCIR(1u << MCIR_TLBWR_Pos);
    } else {
        __set_MCIR(1u << MCIR_TLBWI_Pos);
    }
}

/**
 \brief  enble mmu
\details
\param [in]  size  tlb page size.
*/
inline void csky_mmu_set_pagesize(page_size_e size)
{
    MPR_Type pgmask;
    pgmask.b.page_mask = size;
    __set_MPR(pgmask.w);
}

/**
 \brief  read MEH, MEL0, MEL1 by tlb index.
\details
\param [in]    index  tlb index(0, 1, 2, ...)
\param [out]   meh    pointer to variable for retrieving MEH.
\param [out]   mel0   pointer to variable for retrieving MEL0.
\param [out]   mel1   pointer to variable for retrieving MEL1.
*/
inline void csky_mmu_read_by_index(uint32_t index, uint32_t *meh, uint32_t *mel0, uint32_t *mel1)
{
    MIR_Type mir;

    if (meh == NULL || mel0 == NULL || mel1 == NULL) {
        return;
    }

    mir.b.Index = index;
    __set_MIR(mir.w);
    __set_MCIR(1u << MCIR_TLBR_Pos);

    *meh = __get_MEH();
    *mel0 = __get_MEL0();
    *mel1 = __get_MEL1();
}

/**
 \brief  flush all mmu tlb.
\details
*/
inline void csky_mmu_invalid_tlb_all(void)
{
    __set_MCIR(1u << MCIR_TLBINV_ALL_Pos);
}

/**
 \brief  flush mmu tlb by index.
\details
*/
inline void csky_mmu_invalid_tlb_by_index(uint32_t index)
{
    MIR_Type mir;

    mir.b.Index = index;
    __set_MIR(mir.w);
    __set_MCIR(1u << MCIR_TLBINV_INDEX_Pos);
}

/**
 \brief  flush mmu tlb by virtual address.
\details
*/
inline void csky_mmu_invalid_tlb_by_vaddr(uint32_t vaddr, uint32_t asid)
{
    __set_MEH(vaddr | (asid & MEH_ASID_Msk));
    __set_MCIR(1u << MCIR_TLBP_Pos);

    if (__get_MIR() & (1 << MIR_P_Pos)) {
        return;
    } else {
        __set_MCIR(1u << MCIR_TLBINV_INDEX_Pos);
    }
}
#endif /* __MMU_H__ */

