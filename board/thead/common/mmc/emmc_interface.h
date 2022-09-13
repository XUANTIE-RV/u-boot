/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __ASM_ARCH_EMMC_INTERFACE_H
#define __ASM_ARCH_EMMC_INTERFACE_H
#include "../datatype.h"

/**
  * Controller Register definitions
  * This is the enumeration of the registers on the host controller. The
  * individual values for the members are the offsets of the individual
  * registers. The registers need to be updated according to IP release 2.10
  */
typedef enum {
	CTRL     = 0x0,     /** Control */
	PWREN    = 0x4,     /** Power-enable */
	CLKDIV   = 0x8,     /** Clock divider */
	CLKSRC   = 0xC,     /** Clock source */
	CLKENA   = 0x10,    /** Clock enable */
	TMOUT    = 0x14,    /** Timeout */
	CTYPE    = 0x18,    /** Card type */
	BLKSIZ   = 0x1C,    /** Block Size */
	BYTCNT   = 0x20,    /** Byte count */
	INTMSK   = 0x24,    /** Interrupt Mask */
	CMDARG   = 0x28,    /** Command Argument */
	CMD      = 0x2C,    /** Command */
	RESP0    = 0x30,    /** Response 0 */
	RESP1    = 0x34,    /** Response 1 */
	RESP2    = 0x38,    /** Response 2 */
	RESP3    = 0x3C,    /** Response 3 */
	MINTSTS  = 0x40,    /** Masked interrupt status */
	RINTSTS  = 0x44,    /** Raw interrupt status */
	STATUS   = 0x48,    /** Status */
	FIFOTH   = 0x4C,    /** FIFO threshold */
	CDETECT  = 0x50,    /** Card detect */
	WRTPRT   = 0x54,    /** Write protect */
	GPIO     = 0x58,    /** General Purpose IO */
	TCBCNT   = 0x5C,    /** Transferred CIU byte count */
	TBBCNT   = 0x60,    /** Transferred host/DMA to/from byte count */
	DEBNCE   = 0x64,    /** Card detect debounce */
	USRID    = 0x68,    /** User ID */
	VERID    = 0x6C,    /** Version ID */
	HCON     = 0x70,    /** Hardware Configuration */
	UHSREG   = 0x74,    /** Reserved */

	FIFODAT  = 0x200,   /** FIFO data read write */
} controller_reg_e ;

/* Misc Defines */
#define HCON_NUM_CARDS(x)       ((((x&0x3E)>>1))+1)
#define DEFAULT_DEBNCE_VAL      0x0FFFFFF
#define GET_FIFO_DEPTH(x)       ((((x)&0x0FFF0000)>>16)+1)
#define FIFO_WIDTH              16      /* in bytes */
#define MAX_DIVIDER_VALUE       0xff
#define CLK_ONLY_CMD            0x80200000
#define SET_RCA(x,y)            ((x)|=(y<<16))
#define SET_BITS(x,y)           ((x)|=(y))          // Set y bits in x
#define UNSET_BITS(x,y)         ((x)&=(~(y)))       // Unset y bits in x
#define CARD_PRESENT(x)         (!((emmc_read_register(CDETECT))&(1<<x)))
#define SET_CARD_NUM(x,y)       ((x)|= ((y)<<16))
#define SET_CMD_INDEX(x,y)      ((x)|= (y&0x3f))

/* Control register definitions */
#define CTRL_RESET             0x00000001
#define FIFO_RESET             0x00000002
#define DMA_RESET              0x00000004
#define INT_ENABLE             0x00000010
#define DMA_ENABLE             0x00000020
#define READ_WAIT              0x00000040
#define SEND_IRQ_RESP          0x00000080
#define ABRT_READ_DATA         0x00000100
#define SEND_CCSD              0x00000200
#define SEND_AS_CCSD           0x00000400
#define ENABLE_OD_PULLUP       0x01000000

#ifdef IDMAC_SUPPORT
#define MAX_BUFF_SIZE_IDMAC    8192
#define CTRL_USE_IDMAC         0x02000000
#define CTRL_IDMAC_RESET       0x00000004
#endif

/* Interrupt mask defines */
#define INTMSK_CDETECT          0x00000001
#define INTMSK_RESP_ERR         0x00000002
#define INTMSK_CMD_DONE         0x00000004
#define INTMSK_DAT_OVER         0x00000008
#define INTMSK_TXDR             0x00000010
#define INTMSK_RXDR             0x00000020
#define INTMSK_RCRC             0x00000040
#define INTMSK_DCRC             0x00000080
#define INTMSK_RTO              0x00000100
#define INTMSK_DTO              0x00000200
#define INTMSK_HTO              0x00000400
#define INTMSK_VSI              INTMSK_HTO   // VSI => Voltage Switch Interrupt
#define INTMSK_FRUN             0x00000800
#define INTMSK_HLE              0x00001000
#define INTMSK_SBE              0x00002000
#define INTMSK_ACD              0x00004000
#define INTMSK_EBE              0x00008000

/*SDIO interrupts are catered from bit 15 through 31*/
#define INTMSK_SDIO_INTR        0xffff0000
#define INTMSK_SDIO_CARD(x)     (1<<(16+x))
#define INTMSK_ALL_ENABLED      0xffffffff

/* CMD Register Defines */
#define CMD_VOLT_SW_BIT         0x10000000
#define CMD_RESP_EXP_BIT        0x00000040
#define CMD_RESP_LENGTH_BIT     0x00000080
#define CMD_CHECK_CRC_BIT       0x00000100
#define CMD_DATA_EXP_BIT        0x00000200
#define CMD_RW_BIT              0x00000400
#define CMD_TRANSMODE_BIT       0x00000800
#define CMD_SENT_AUTO_STOP_BIT  0x00001000
#define CMD_WAIT_PRV_DAT_BIT    0x00002000
#define CMD_ABRT_CMD_BIT        0x00004000
#define CMD_SEND_INIT_BIT       0x00008000
#define CMD_SEND_CLK_ONLY       0x00200000
#define CMD_READ_CEATA          0x00400000
#define CMD_CCS_EXPECTED        0x00800000
#define CMD_USE_HOLD            0x20000000
#define CMD_DONE_BIT            0x80000000

/* Status register bits */
#define STATUS_DAT_BUSY_BIT     0x00000200

/* The below defines should be outside #ifdef IDMAC_SUPPORT as some callbacks use variable instead of #define Macro */

/* Standard MMC commands (3.1)           type  argument     response */
/* class 1 */
#define	CMD0    0   /* MMC_GO_IDLE_STATE        bc                    */
#define CMD1    1   /* MMC_SEND_OP_COND         bcr  [31:0]  OCR  R3  */
#define CMD2    2   /* MMC_ALL_SEND_CID         bcr               R2  */
#define CMD3    3   /* MMC_SET_RELATIVE_ADDR    ac   [31:16] RCA  R1  */
#define CMD4    4   /* MMC_SET_DSR              bc   [31:16] RCA      */

#define CMD5    5   /* SDIO_SEND_OCR            ??   ??               */

#define CMD6    6   /* HSMMC_SWITCH             ac                R1  */
/* For ACMD6:SET_BUS_WIDTH  ??   ??               */

#define CMD7    7   /* MMC_SELECT_CARD          ac   [31:16] RCA  R1  */
#define CMD8    8   /* HSMMC_SEND_EXT_CSD       adtc [31:16] RCA  R1  */
#define CMD9    9   /* MMC_SEND_CSD             ac   [31:16] RCA  R2  */
#define CMD10   10  /* MMC_SEND_CID             ac   [31:16] RCA  R2  */
#define CMD11   11  /* MMC_READ_DAT_UNTIL_STOP  adtc [31:0]  dadr R1  */
#define CMD12   12  /* MMC_STOP_TRANSMISSION    ac                R1b */
#define CMD13   13  /* MMC_SEND_STATUS          ac   [31:16] RCA  R1  */
#define ACMD13  13  /* SD_STATUS                ac   [31:2] Stuff,
                                                     [1:0]Buswidth  R1 */
#define CMD14   14  /* HSMMC_BUS_TESTING        adtc [31:16] stuff R1 */
#define CMD15   15  /* MMC_GO_INACTIVE_STATE    ac   [31:16] RCA  */
#define CMD19   19  /* HSMMC_BUS_TESTING        adtc [31:16] stuff R1 */

/* class 2 */
#define CMD16   16  /* MMC_SET_BLOCKLEN         ac   [31:0] blkln R1  */
#define CMD17   17  /* MMC_READ_SINGLE_BLOCK    adtc [31:0] dtadd R1  */
#define CMD18   18  /* MMC_READ_MULTIPLE_BLOCK  adtc [31:0] dtadd R1  */

/* class 3 */
#define CMD20   20  /* MMC_WRITE_DAT_UNTIL_STOP adtc [31:0] dtadd R1  */

/* class 4 */
#define CMD23   23  /* MMC_SET_BLOCK_COUNT      adtc [31:0] dtadd R1  */
#define CMD24   24  /* MMC_WRITE_BLOCK          adtc [31:0] dtadd R1  */
#define CMD25   25  /* MMC_WRITE_MULTIPLE_BLOCK adtc              R1  */
#define CMD26   26  /* MMC_PROGRAM_CID          adtc              R1  */
#define CMD27   27  /* MMC_PROGRAM_CSD          adtc              R1  */

/* class 6 */
#define CMD28   28  /* MMC_SET_WRITE_PROT       ac   [31:0] dtadd R1b */
#define CMD29   29  /* _CLR_WRITE_PROT          ac   [31:0] dtadd R1b */
#define CMD30   30  /* MMC_SEND_WRITE_PROT      adtc [31:0] wpdtaddr R1  */

/* class 5 */
#define CMD32   32  /* SD_ERASE_GROUP_START    ac   [31:0] dtadd  R1  */
#define CMD33   33  /* SD_ERASE_GROUP_END      ac   [31:0] dtaddr R1  */

#define CMD35   35  /* MMC_ERASE_GROUP_START    ac   [31:0] dtadd  R1  */
#define CMD36   36  /* MMC_ERASE_GROUP_END      ac   [31:0] dtaddr R1  */
#define CMD38   38  /* MMC_ERASE                ac                 R1b */

/* class 9 */
#define CMD39   39  /* MMC_FAST_IO              ac   <Complex>     R4  */
#define CMD40   40  /* MMC_GO_IRQ_STATE         bcr                R5  */

#define ACMD41  41  /* SD_SEND_OP_COND          ??                 R1  */

/* class 7 */
#define CMD42   42  /* MMC_LOCK_UNLOCK          adtc               R1b */

#define ACMD51  51  /* SEND_SCR                 adtc               R1  */

#define CMD52   52  /* SDIO_RW_DIRECT           ??                 R5  */
#define CMD53   53  /* SDIO_RW_EXTENDED         ??                 R5  */

/* class 8 */
#define CMD55   55  /* MMC_APP_CMD              ac   [31:16] RCA   R1  */
#define CMD56   56  /* MMC_GEN_CMD              adtc [0] RD/WR     R1b */

// For CE-ATA Drive
#define CMD60   60
#define CMD61   61

#define SDIO_RESET  100  //To differentiate CMD52 for IORESET and other rd/wrs.
#define SDIO_ABORT  101  //To differentiate CMD52 for IO ABORT and other rd/wrs.

#define UNADD_OFFSET    200
#define UNADD_CMD7      207
#define WCMC52          252
#define WCMD53          253
#define WCMD60          260
#define WCMD61          261
#define ACMD6           206
#define SD_CMD8         208  /*This is added to support SD 2.0 (SDHC) cards*/
#define SD_CMD11        211  /*This is added to support SDXC Voltage Switching*/

u32 emmc_set_bits(controller_reg_e reg, u32 val);
u32 emmc_clear_bits(controller_reg_e reg, u32 val);
u32 emmc_set_register(controller_reg_e reg, u32 val);
u32 emmc_read_register(controller_reg_e reg);
void *emmc_get_fifo_address(void);

#endif /* __ASM_ARCH_EMMC_INTERFACE_H */

