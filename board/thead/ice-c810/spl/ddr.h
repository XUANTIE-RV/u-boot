/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */


#ifndef __ASM_ARCH_DDR_H__
#define __ASM_ARCH_DDR_H__

int init_ddr(void);

//block name: UMCTL2_REGS
#define MSTR        0x0
#define STAT        0x4
#define MSTR1       0x8
#define MRCTRL0     0x10
#define MRCTRL1     0x14
#define MRSTAT      0x18
#define MRCTRL2     0x1c
#define DERATEEN    0x20
#define DERATEINT   0x24
#define PWRCTL      0x30
#define PWRTMG      0x34
#define HWLPCTL     0x38
#define RFSHCTL0    0x50
#define RFSHCTL1    0x54
#define RFSHCTL2    0x58
#define RFSHCTL3    0x60
#define RFSHTMG     0x64
#define ECCCFG0     0x70
#define ECCCFG1     0x74
#define ECCSTAT     0x78
#define ECCCLR      0x7c
#define ECCERRCNT   0x80
#define ECCCADDR0   0x84
#define ECCCADDR1   0x88
#define ECCCSYN0    0x8c
#define ECCCSYN1    0x90
#define ECCCSYN2    0x94
#define ECCBITMASK0 0x98
#define ECCBITMASK1 0x9c
#define ECCBITMASK2 0xa0
#define ECCUADDR0   0xa4
#define ECCUADDR1   0xa8
#define ECCUSYN0    0xac
#define ECCUSYN1    0xb0
#define ECCUSYN2    0xb4
#define ECCPOISONADDR0  0xb8
#define ECCPOISONADDR1  0xbc
#define CRCPARCTL0  0xc0
#define CRCPARCTL1  0xc4
#define CRCPARCTL2  0xc8
#define CRCPARSTAT  0xcc
#define INIT0       0xd0
#define INIT1       0xd4
#define INIT2       0xd8
#define INIT3       0xdc
#define INIT4       0xe0
#define INIT5       0xe4
#define INIT6       0xe8
#define INIT7       0xec
#define DIMMCTL     0xf0
#define RANKCTL     0xf4
#define DRAMTMG0    0x100
#define DRAMTMG1    0x104
#define DRAMTMG2    0x108
#define DRAMTMG3    0x10c
#define DRAMTMG4    0x110
#define DRAMTMG5    0x114
#define DRAMTMG6    0x118
#define DRAMTMG7    0x11c
#define DRAMTMG8    0x120
#define DRAMTMG9    0x124
#define DRAMTMG10   0x128
#define DRAMTMG11   0x12c
#define DRAMTMG12   0x130
#define DRAMTMG13   0x134
#define DRAMTMG14   0x138
#define DRAMTMG15   0x13c
#define MRAMTMG0    0x170
#define MRAMTMG1    0x174
#define MRAMTMG4    0x178
#define MRAMTMG9    0x17c
#define ZQCTL0      0x180
#define ZQCTL1      0x184
#define ZQCTL2      0x188
#define ZQSTAT      0x18c
#define DFITMG0     0x190
#define DFITMG1     0x194
#define DFILPCFG0   0x198
#define DFILPCFG1   0x19c
#define DFIUPD0     0x1a0
#define DFIUPD1     0x1a4
#define DFIUPD2     0x1a8
#define DFIMISC     0x1b0
#define DFITMG2     0x1b4
#define DFITMG3     0x1b8
#define DBICTL      0x1c0
#define DFIPHYMSTR  0x1c4
#define ADDRMAP0    0x200
#define ADDRMAP1    0x204
#define ADDRMAP2    0x208
#define ADDRMAP3    0x20c
#define ADDRMAP4    0x210
#define ADDRMAP5    0x214
#define ADDRMAP6    0x218
#define ADDRMAP7    0x21c
#define ADDRMAP8    0x220
#define ADDRMAP9    0x224
#define ADDRMAP10   0x228
#define ADDRMAP11   0x22c
#define ODTCFG      0x240
#define ODTMAP      0x244
#define SCHED       0x250
#define SCHED1      0x254
#define PERFHPR1    0x25c
#define PERFLPR1    0x264
#define PERFWR1     0x26c
#define PERFVPR1    0x274
#define PERFVPW1    0x278
#define DQMAP0      0x280
#define DQMAP1      0x284
#define DQMAP2      0x288
#define DQMAP3      0x28c
#define DQMAP4      0x290
#define DQMAP5      0x294
#define DBG0        0x300
#define DBG1        0x304
#define DBGCAM      0x308
#define DBGCMD      0x30c
#define DBGSTAT     0x310
#define SWCTL       0x320
#define SWSTAT      0x324
#define OCPARCFG0   0x330
#define OCPARCFG1   0x334
#define OCPARCFG2   0x338
#define OCPARCFG3   0x33c
#define OCPARSTAT0  0x340
#define OCPARSTAT1  0x344
#define OCPARWLOG0  0x348
#define OCPARWLOG1  0x34c
#define OCPARWLOG2  0x350
#define OCPARAWLOG0 0x354
#define OCPARAWLOG1 0x358
#define OCPARRLOG0  0x35c
#define OCPARRLOG1  0x360
#define OCPARARLOG0 0x364
#define OCPARARLOG1 0x368
#define POISONCFG   0x36c
#define POISONSTAT  0x370

//block name: UMCTL2_MP registers
#define PSTAT       0x3fc
#define PCCFG       0x400
//#define PCFGR_n     0x404
//#define PCFGW_n     0x408
//#define PCFGC_n     0x40c
#define PCFGIDMASKCH    0x410
#define PCFGIDVALUECH   0x414
//#define PCTRL_n     0x490
//#define PCFGQOS0_n  0x494
//#define PCFGQOS1_n  0x498
//#define PCFGWQOS0_n 0x49c
//#define PCFGWQOS1_n 0x4a0
//#define SARBASEn    0xf04
//#define SARSIZEn    0xf08
#define SBRCTL      0xf24
#define SBRSTAT     0xf28
#define SBRWDATA0   0xf2c
#define SBRWDATA1   0xf30
#define PDCH        0xf34

//block name: UMCTL2_REGS_DCH1 registers
//not used for now

//other used registers that are not in previous blocks
#define PCFGR_0     0x404
#define PCFGW_0     0x408
#define PCTRL_0     0x490
#define PCFGQOS0_0  0x494
#define PCFGQOS1_0  0x498
#define PCFGWQOS0_0 0x49c
#define PCFGWQOS1_0 0x4a0
#define PCFGR_1     0x4b4
#define PCFGW_1     0x4b8
#define PCTRL_1     0x540
#define PCFGQOS0_1  0x544
#define PCFGQOS1_1  0x548
#define PCFGWQOS0_1 0x54c
#define PCFGWQOS1_1 0x550
#define PCFGR_2     0x564
#define PCFGW_2     0x568
#define PCTRL_2     0x5f0
#define PCFGQOS0_2  0x5f4
#define PCFGQOS1_2  0x5f8
#define PCFGWQOS0_2 0x5fc
#define PCFGWQOS1_2 0x600
#define PCFGR_3     0x614
#define PCFGW_3     0x618
#define PCTRL_3     0x6a0
#define PCFGQOS0_3  0x6a4
#define PCFGQOS1_3  0x6a8
#define PCFGWQOS0_3 0x6ac
#define PCFGWQOS1_3 0x6b0
#define PCFGR_4     0x6c4
#define PCFGW_4     0x6c8
#define PCTRL_4     0x750
#define PCFGQOS0_4  0x754
#define PCFGQOS1_4  0x758
#define PCFGWQOS0_4 0x75c
#define PCFGWQOS1_4 0x760
#define PCFGR_5     0x774
#define PCFGW_5     0x778
#define PCTRL_5     0x800
#define PCFGQOS0_5  0x804
#define PCFGQOS1_5  0x808
#define PCFGWQOS0_5 0x80c
#define PCFGWQOS1_5 0x810

//pub registers
#define PUB_RIDR        0x000
#define PUB_PIR         0x001
#define PUB_PGCR0       0x002
#define PUB_PGCR1       0x003
#define PUB_PGSR0       0x004
#define PUB_PGSR1       0x005
#define PUB_PLLCR       0x006
#define PUB_PTR0        0x007
#define PUB_PTR1        0x008
#define PUB_PTR2        0x009
#define PUB_PTR3        0x00a
#define PUB_PTR4        0x00b
#define PUB_ACMDLR      0x00c
#define PUB_ACBDLR      0x00d
#define PUB_ACIOCR      0x00e
#define PUB_DXCCR       0x00f
#define PUB_DSGCR       0x010
#define PUB_DCR         0x011
#define PUB_DTPR0       0x012
#define PUB_DTPR1       0x013
#define PUB_DTPR2       0x014
#define PUB_MR0         0x015
#define PUB_MR1         0x016
#define PUB_MR2         0x017
#define PUB_MR3         0x018
#define PUB_ODTCR       0x019
#define PUB_DTCR        0x01a
#define PUB_DTAR0       0x01b
#define PUB_DTAR1       0x01c
#define PUB_DTAR2       0x01d
#define PUB_DTAR3       0x01e
#define PUB_DTDR0       0x01f
#define PUB_DTDR1       0x020
#define PUB_DTEDR0      0x021
#define PUB_DTEDR1      0x022
#define PUB_PGCR2       0x023
#define PUB_RDIMMGCR0   0x02c
#define PUB_RDIMMGCR1   0x02d
#define PUB_RDIMMCR0    0x02e
#define PUB_RDIMMCR1    0x02f
#define PUB_DCUAR       0x030
#define PUB_DCUDR       0x031
#define PUB_DCURR       0x032
#define PUB_DCULR       0x033
#define PUB_DCUGCR      0x034
#define PUB_DCUTPR      0x035
#define PUB_DCUSR0      0x036
#define PUB_DCUSR1      0x037
#define PUB_BISTRR      0x040
#define PUB_BISTWCR     0x041
#define PUB_BISTMSKR0   0x042
#define PUB_BISTMSKR1   0x043
#define PUB_BISTMSKR2   0x044
#define PUB_BISTLSR     0x045
#define PUB_BISTAR0     0x046
#define PUB_BISTAR1     0x047
#define PUB_BISTAR2     0x048
#define PUB_BISTUDPR    0x049
#define PUB_BISTGSR     0x04a
#define PUB_BISTWER     0x04b
#define PUB_BISTBER0    0x04c
#define PUB_BISTBER1    0x04d
#define PUB_BISTBER2    0x04e
#define PUB_BISTBER3    0x04f
#define PUB_BISTWCSR    0x050
#define PUB_BISTFWR0    0x051
#define PUB_BISTFWR1    0x052
#define PUB_BISTFWR2    0x053
#define PUB_AACR        0x05d
#define PUB_GPR0        0x05e
#define PUB_GPR1        0x05f
#define PUB_ZQ0CR0      0x060
#define PUB_ZQ0CR1      0x061
#define PUB_ZQ0SR0      0x062
#define PUB_ZQ0SR1      0x063
#define PUB_ZQ1CR0      0x064
#define PUB_ZQ1CR1      0x065
#define PUB_ZQ1SR0      0x066
#define PUB_ZQ1SR1      0x067
#define PUB_ZQ2CR0      0x068
#define PUB_ZQ2CR1      0x069
#define PUB_DX0BDLR0    0x073
#define PUB_DX0BDLR1    0x074
#define PUB_DX0BDLR2    0x075
#define PUB_DX0BDLR3    0x076
#define PUB_DX0BDLR4    0x077
#define PUB_DX0LCDLR0   0x078
#define PUB_DX0LCDLR1   0x079
#define PUB_DX0LCDLR2   0x07a
#define PUB_DX0MDLR     0x07b
#define PUB_DX1BDLR0    0x083
#define PUB_DX1BDLR1    0x084
#define PUB_DX1BDLR2    0x085
#define PUB_DX1BDLR3    0x086
#define PUB_DX1BDLR4    0x087
#define PUB_DX1LCDLR0   0x088
#define PUB_DX1LCDLR1   0x089
#define PUB_DX1LCDLR2   0x08a
#define PUB_DX1MDLR     0x08b
#define PUB_DX2GCR      0x090
#define PUB_DX2BDLR0    0x093
#define PUB_DX2BDLR1    0x094
#define PUB_DX2BDLR2    0x095
#define PUB_DX2BDLR3    0x096
#define PUB_DX2BDLR4    0x097
#define PUB_DX2LCDLR0   0x098
#define PUB_DX2LCDLR1   0x099
#define PUB_DX2LCDLR2   0x09a
#define PUB_DX2MDLR     0x09b
#define PUB_DX3GCR      0x0a0
#define PUB_DX3BDLR0    0x0a3
#define PUB_DX3BDLR1    0x0a4
#define PUB_DX3BDLR2    0x0a5
#define PUB_DX3BDLR3    0x0a6
#define PUB_DX3BDLR4    0x0a7
#define PUB_DX3LCDLR0   0x0a8
#define PUB_DX3LCDLR1   0x0a9
#define PUB_DX3LCDLR2   0x0aa
#define PUB_DX3MDLR     0x0ab


#ifdef CACHE_EN
    #define SSEG0_BADDR 0x80000000
    #define SSEG1_BADDR 0xa0000000
#else
    #define SSEG0_BADDR 0x00000000
    #define SSEG1_BADDR 0x00000000
#endif

#ifndef __CLIB_DEFINE_H__
#define __CLIB_DEFINE_H__


#define _CHIP_CTRL_BADDR (0x1fbd0000+SSEG1_BADDR)
#define _POWM_BADDR (0x1fbd0800+SSEG1_BADDR)
#define _MAILBOX_BADDR (0x1fbd4000+SSEG1_BADDR)

#define _DMAC0_BADDR (0x1fb19000+SSEG1_BADDR)
#define _SDHC_BADDR (0x1fb12000+SSEG1_BADDR)
#define _EMMC_BADDR (0x1fb13000+SSEG1_BADDR)
#define _USBOTG_BADDR (0x1fb00000+SSEG1_BADDR)
#define _MAC_BADDR (0x1fb10000+SSEG1_BADDR)

#define _SHA_BADDR (0x1fb80000+SSEG1_BADDR)
#define _AES_BADDR (0x1fb82000+SSEG1_BADDR)
#define _TDES_BADDR (0x1fb83000+SSEG1_BADDR)
#define _RSA_BADDR (0x1fb81000+SSEG1_BADDR)
#define _CRC_BADDR (0x1fb84000+SSEG1_BADDR)

#define _WDT_BADDR (0x1fba0000+SSEG1_BADDR)
#define _RTC_BADDR (0x1fba1000+SSEG1_BADDR)
#define _TIM_BADDR (0x1fba2000+SSEG1_BADDR)
#define _INTC_BADDR (0x1fba3000+SSEG1_BADDR)
#define _UART2_BADDR (0x1fba4000+SSEG1_BADDR)
#define _UART3_BADDR (0x1fba5000+SSEG1_BADDR)
#define _UART4_BADDR (0x1fba6000+SSEG1_BADDR)
#define _I2C2_BADDR (0x1fba7000+SSEG1_BADDR)
#define _I2C3_BADDR (0x1fba8000+SSEG1_BADDR)
#define _I2C4_BADDR (0x1fba9000+SSEG1_BADDR)
#define _PWM_BADDR (0x1fbaa000+SSEG1_BADDR)
#define _SPI1_BADDR (0x1fbab000+SSEG1_BADDR)
#define _GPIO0_BADDR (0x1fbac000+SSEG1_BADDR)
#define _GPIO1_BADDR (0x1fbad000+SSEG1_BADDR)
#define _GPIO2_BADDR (0x1fbae000+SSEG1_BADDR)
#define _GPIO3_BADDR (0x1fbaf000+SSEG1_BADDR)
#define _TIM_2_BADDR (0x1fbb0000+SSEG1_BADDR)

#define _I2S0_BADDR (0x1fb90000+SSEG1_BADDR)
#define _I2S1_BADDR (0x1fb91000+SSEG1_BADDR)
#define _DMAC1_BADDR (0x1fb94000+SSEG1_BADDR)

#define _UART0_BADDR (0x1fb14000+SSEG1_BADDR)
#define _UART1_BADDR (0x1fb15000+SSEG1_BADDR)
#define _SPI0_BADDR  (0x1fb18000+SSEG1_BADDR)
#define _I2C0_BADDR  (0x1fb16000+SSEG1_BADDR)
#define _I2C1_BADDR  (0x1fb17000+SSEG1_BADDR)
#define _DDR_MC_BADDR  (0x1fbc0000+SSEG1_BADDR)
#define _DDR_PHY_BADDR (0x1fbc1000+SSEG1_BADDR)

#define _LCDC_BADDR (0x1fa00000+SSEG1_BADDR)
#define _VPU_BADDR  (0x1fa10000+SSEG1_BADDR)
#define _HDMI_BADDR (0x1fa20000+SSEG1_BADDR)

#define _SMS_BADDR (0x1fc00000+SSEG0_BADDR)

#define _DDR3_1_BADDR (0x00000000+SSEG0_BADDR)

#define _SMS_BANK0_BADDR 0x0
#define _SMS_BANK0_EADDR 0x0fffc
#define _SMS_BANK1_BADDR 0x10000
#define _SMS_BANK1_EADDR 0x1fffc
#define _SMS_BANK2_BADDR 0x20000
#define _SMS_BANK2_EADDR 0x2fffc
#define _SMS_REG_BADDR 0x10000

#define _UART0_RX_DMAC_PORT 0
#define _UART0_TX_DMAC_PORT 1
#define _UART1_RX_DMAC_PORT 2
#define _UART1_TX_DMAC_PORT 3
#define _SPI0_DMA_RX_DMAC_PORT 4
#define _SPI0_DMA_TX_DMAC_PORT 5
#define _I2C0_RX_DMAC_PORT 6
#define _I2C0_TX_DMAC_PORT 7
#define _I2C1_RX_DMAC_PORT 8
#define _I2C1_TX_DMAC_PORT 9

#define _POWM_AUDIO_PORT0 0
#define _POWM_SPU_PORT0 1
#define _POWM_LSP_PORT0 2
#define _POWM_PX_PORT0 3
#define _POWM_DX_PORT0 4
#define _POWM_BB_PORT0 5
#define _POWM_CFG_PORT0 6
#define _POWM_LS232e_PORT0 7

#define _POWM_SHA_PORT1 0
#define _POWM_RSA_PORT1 1
#define _POWM_AES_PORT1 2
#define _POWM_DES_PORT1 3
#define _POWM_CRC_PORT1 4
#define _POWM_DMAC0_PORT1 5
#define _POWM_DMAC1_PORT1 6
#define _POWM_USBOTG_PORT1 7
#define _POWM_GMAC_PORT1 8
#define _POWM_SDHC_PORT1 9
#define _POWM_eMMC_PORT1 10
#define _POWM_HBOOT_PORT1 11
#define _POWM_SRAM_PORT1 12
#define _POWM_VPU_PORT1 13
#define _POWM_LCDC_PORT1 14
#define _POWM_HDMI_PORT1 15
#define _POWM_DDRAXI0_PORT1 16
#define _POWM_DDRAXI1_PORT1 17
#define _POWM_DDRAXI2_PORT1 18
#define _POWM_DDRAXI3_PORT1 19
#define _POWM_DDRCTRL_PORT1 20
#define _POWM_DDRPHY_PORT1 21
#define _POWM_MAILBOX_PORT1 22

#define _POWM_I2C0_PORT2 0
#define _POWM_I2C1_PORT2 1
#define _POWM_I2C2_PORT2 2
#define _POWM_I2C3_PORT2 3
#define _POWM_I2C4_PORT2 4
#define _POWM_GPIO0_PORT2 5
#define _POWM_GPIO1_PORT2 6
#define _POWM_GPIO2_PORT2 7
#define _POWM_GPIO3_PORT2 8
#define _POWM_SPI0_PORT2 9
#define _POWM_SPI1_PORT2 10
#define _POWM_PWM_PORT2 11
#define _POWM_WDT_PORT2 12
#define _POWM_RTC_PORT2 13
#define _POWM_TIM_PORT2 14
#define _POWM_UART0_PORT2 15
#define _POWM_UART1_PORT2 16
#define _POWM_UART2_PORT2 17
#define _POWM_UART3_PORT2 18
#define _POWM_UART4_PORT2 19
#define _POWM_I2S0_PORT2 20
#define _POWM_I2S1_PORT2 21
#define _POWM_INTC_PORT2 24
#define _POWM_DDRCTRL_PORT2 25
#define _POWM_DDRPHY_PORT2 26
#define _POWM_MEM_PORT2 27
#define _POWM_TIM_2_PORT2 28


#define _RSTGEN_CFG_PORT0 8
#define _RSTGEN_DX_PORT0 7
#define _RSTGEN_BB_PORT0 6
#define _RSTGEN_PX_PORT0 5
#define _RSTGEN_LSP_PORT0 4
#define _RSTGEN_SPU_PORT0 3
#define _RSTGEN_AUD_PORT0 2
#define _RSTGEN_SW0_PORT0 1
#define _RSTGEN_SW1_PORT0 0

#define _RSTGEN_MAILBOX_PORT1 22
#define _RSTGEN_DDRPHY_PORT1 21
#define _RSTGEN_DDRCTL_PORT1 20
#define _RSTGEN_DDRAXI3_PORT1 19
#define _RSTGEN_DDRAXI2_PORT1 18
#define _RSTGEN_DDRAXI1_PORT1 17
#define _RSTGEN_DDRAXI0_PORT1 16
#define _RSTGEN_HDMI_PORT1 15
#define _RSTGEN_LCDC_PORT1 14
#define _RSTGEN_VPU_PORT1 13
#define _RSTGEN_SMS_PORT1 12
#define _RSTGEN_HBOOT_PORT1 11
#define _RSTGEN_EMMC_PORT1 10
#define _RSTGEN_SDHC_PORT1 9
#define _RSTGEN_MAC_PORT1 8
#define _RSTGEN_USB_PORT1 7
#define _RSTGEN_DMAC1_PORT1 6
#define _RSTGEN_DMAC0_PORT1 5
#define _RSTGEN_SHA_PORT1 4
#define _RSTGEN_RSA_PORT1 3
#define _RSTGEN_DES_PORT1 2
#define _RSTGEN_CRC_PORT1 1
#define _RSTGEN_AES_PORT1 0

#define _RSTGEN_TIM_2_PORT2 28
#define _RSTGEN_MEM_PORT2 27
#define _RSTGEN_DDRPHY_PORT2 26
#define _RSTGEN_DDRCTRL_PORT2 25
#define _RSTGEN_UART4_PORT2 23
#define _RSTGEN_UART3_PORT2 22
#define _RSTGEN_UART2_PORT2 21
#define _RSTGEN_UART1_PORT2 20
#define _RSTGEN_UART0_PORT2 19
#define _RSTGEN_TIM_PORT2 18
#define _RSTGEN_SPI1_PORT2 17
#define _RSTGEN_SPI0_PORT2 16
#define _RSTGEN_RTC_PORT2 15
#define _RSTGEN_PWM_PORT2 14
#define _RSTGEN_INTC_PORT2 13
#define _RSTGEN_I2S1_PORT2 11
#define _RSTGEN_I2S0_PORT2 10
#define _RSTGEN_I2C4_PORT2 9
#define _RSTGEN_I2C3_PORT2 8
#define _RSTGEN_I2C2_PORT2 7
#define _RSTGEN_I2C1_PORT2 6
#define _RSTGEN_I2C0_PORT2 5
#define _RSTGEN_GPIO3_PORT2 4
#define _RSTGEN_GPIO2_PORT2 3
#define _RSTGEN_GPIO1_PORT2 2
#define _RSTGEN_GPIO0_PORT2 1
#define _RSTGEN_WDT_PORT2 0

#define COM_MEM_BADDR 0x1fc13000
#define _C_CASE_PASS_FLAG 0x1
#define _C_CASE_FAIL_FLAG 0x1324ffbb
#define _DMEM_BADDR COM_MEM_BADDR

#define _GPIO0_INT_NUM 0
#define _GPIO1_INT_NUM 1
#define _GPIO2_INT_NUM 2
#define _GPIO3_INT_NUM 3
#define _TIMER1_INT_NUM 4
#define _TIMER2_INT_NUM 5
#define _TIMER3_INT_NUM 6
#define _WDT_INT_NUM 7
#define _RTC_INT_NUM 8
#define _UART0_INT_NUM 9
#define _UART1_INT_NUM 10
#define _UART2_INT_NUM 11
#define _UART3_INT_NUM 12
#define _UART4_INT_NUM 13
#define _SPIM0_INT_NUM 14
#define _SPIM1_INT_NUM 15
#define _I2C0_INT_NUM 16
#define _I2C1_INT_NUM 17
#define _I2C2_INT_NUM 18
#define _I2C3_INT_NUM 19
#define _I2C4_INT_NUM 20
#define _AES_INT_NUM 21
#define _TDES_INT_NUM 22
#define _RSA_INT_NUM 23
#define _SHA_INT_NUM 24
#define _DMAC0_INT_NUM 25
#define _DMAC1_INT_NUM 26
#define _POWM_INT_NUM 27
#define _PWM0_0_INT_NUM 28
#define _PWM0_1_INT_NUM 29
#define _PWM0_2_INT_NUM 30
#define _PWMFAULT_INT_NUM 31
#define _SDHC_INT_NUM 32
#define _eMMC_INT_NUM 33
#define _MAC_INT_NUM 34
#define _USBOTG_INT_NUM 35
#define _I2S0_INT_NUM 36
#define _I2S1_INT_NUM 37
#define _LCDC_NUM 40
#define _VPU_INT_NUM 41
#define _BPD_INT_NUM 42
#define _HDMI_INT_NUM 43
#define _TIMER_2_0_INT_NUM  44
#define _TIMER_2_1_INT_NUM  45
#define _TIMER_2_2_INT_NUM  46
#define _CK807_MAILBOX_INT_NUM  47
#define _LS232e_MAILBOX_INT_NUM  47

#define  _FLAG_DDR_INIT (_DMEM_BADDR+0x500)
#define  _RETENTION_CTRL_REG (_CHIP_CTRL_BADDR+0x70)


#define CHIP_CTRL_REG(offset)   *((volatile int *)(_CHIP_CTRL_BADDR + (offset)))
#define POWM_REG(offset)     *((volatile int *)(POWM_BADDR+(offset)))
#define INTC_REG(offset)            *((volatile int *)(INTC_BADDR+(offset)))
#define TIM1_REG(offset)            *((volatile int *)(_TIM_BADDR+(offset)))
#define TIM2_REG(offset)            *((volatile int *)(_TIM_2_BADDR+(offset)))

#define DDR_ACCESS(offset)   *((volatile int *)(_DDR3_1_BADDR + (offset)))
#define SRAM_ACCESS(offset)   *((volatile int *)(_SMS_BADDR + (offset)))

#define DMEM_REG(offset)   *((volatile int *)(_DMEM_BADDR + (offset)))

#define DDR_REG(offset)   *((volatile int *)(_DDR_MC_BADDR + (offset)))
#define PUB_REG(offset)   *((volatile int *)(_DDR_PHY_BADDR + (offset * 4)))

#endif

#endif /* __ASM_ARCH_DDR_H__ */
