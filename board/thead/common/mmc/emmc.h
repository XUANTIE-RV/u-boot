/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __ASM_ARCH_EMMC_H
#define __ASM_ARCH_EMMC_H

#include "../datatype.h"

#define MMC_DEBUG 0
#define PDEBUG(fmt, ...)

/* Judge the version of emmc */
#define SD_VERSION_SD   (1 << 31)
#define MMC_VERSION_MMC (1 << 30)

#define MAKE_SDMMC_VERSION(a, b, c) \
    ((((uint32_t)(a)) << 16) | ((uint32_t)(b) << 8) | (uint32_t)(c))
#define MAKE_SD_VERSION(a, b, c) \
    (SD_VERSION_SD | MAKE_SDMMC_VERSION(a, b, c))
#define MAKE_MMC_VERSION(a, b, c) \
    (MMC_VERSION_MMC | MAKE_SDMMC_VERSION(a, b, c))

#define MMC_VERSION_UNKNOWN MAKE_MMC_VERSION(0, 0, 0)
#define MMC_VERSION_1_2     MAKE_MMC_VERSION(1, 2, 0)
#define MMC_VERSION_1_4     MAKE_MMC_VERSION(1, 4, 0)
#define MMC_VERSION_2_2     MAKE_MMC_VERSION(2, 2, 0)
#define MMC_VERSION_3       MAKE_MMC_VERSION(3, 0, 0)
#define MMC_VERSION_4       MAKE_MMC_VERSION(4, 0, 0)
#define MMC_VERSION_4_1     MAKE_MMC_VERSION(4, 1, 0)
#define MMC_VERSION_4_2     MAKE_MMC_VERSION(4, 2, 0)
#define MMC_VERSION_4_3     MAKE_MMC_VERSION(4, 3, 0)
#define MMC_VERSION_4_41    MAKE_MMC_VERSION(4, 4, 1)
#define MMC_VERSION_4_5     MAKE_MMC_VERSION(4, 5, 0)
#define MMC_VERSION_5_0     MAKE_MMC_VERSION(5, 0, 0)
#define MMC_VERSION_5_1     MAKE_MMC_VERSION(5, 1, 0)

/* Retry counts */
#define CMD1_RETRY_COUNT   10   /*changed from 50 Just to be cautious--Manju */
#define ACMD41_RETRY_COUNT 1000 /*changed from 50 Just to be cautious--Manju */
#define CMD2_RETRY_COUNT   1000 /*changed from 50 Just to be cautious--Manju */
#define CMD5_RETRY_COUNT   1000 /*changed from 50 Just to be cautious--Manju */

/**
  * Flag in emmc_read_write_bytes custom command instructing the function
  * not to put the card into the trans state before firing the command.
  */
#define CUSTCOM_DONT_TRANS  0x10000000

/**
  * Flag in emmc_read_write_bytes custom command instructing the function to
  * use CMD23 to set the block number before firing the data command.
  */
#define CUSTCOM_DO_CMD23    0x20000000

/**
  * Flag in emmc_read_write_bytes custom command instructing the function not to
  * to go to standby state after the data transfer.
  */
#define CUSTCOM_DONTSTDBY   0x40000000

/**
  * Flag in emmc_read_write_bytes custom command instructing the function not to
  * issue a CMD16 for setting the blocksize before the data command is fired.
  */
#define CUSTCOM_DONT_CMD16  0x01000000

/**
  * Flag in emmc_read_write_bytes custom command instructing the function
  * that the transfer is a stream transfer
  */
#define CUSTCOM_STREAM_RW   0x02000000

/**
  * Flag in emmc_read_write_bytes custom command instructing the function
  * not to populate the BLKSIZ register expecting calling function will do the
  * needful.
  */
#define CUSTCOM_DONT_BLKSIZ 0x80000000

#define CUSTCOM_COMMAND_MSK 0x00000fff
#define CUSTOM_BLKSIZE_MSK  0x00fff000

#define CUSTOM_BLKSIZE_SHIFT    12

#define CUSTOM_BLKSIZE(x)   (((x&CUSTOM_BLKSIZE_MSK))>>CUSTOM_BLKSIZE_SHIFT)

/*
  * Macro to access arrays to dwords and extract specified bits.
  * These macros are are used to access the 4 dword CSD register. We employ this form of
  * access since we maintain the csd as a 4 dword entity. These macros provide a readable
  * way to access a section of the CSD without having to unfold them and store them
  * separately.
  *
  * (z[y DIV 32] ROLL-RIGHT-BY (x MOD 32 )) AND  ((2^NUM_OF_BITS_TO_EXTRACT)-1)
  * @param	x start bit
  * @param	y end bit
  * @param	z Array of dwords
  */
#define GET_BITS_BETWEEN(x,y,z)     ((((z)[y>>5])>>(x&0x1f))&((1<<(y-x+1))-1))
#define SET_BITS_BETWEEN(v,x,y,z)   ((z)[y>>5])|=((v&((1<<(y-x+1))-1))<<(x&0x1f))
#define CLEAR_BITS_BETWEEN(v,x,y,z) ((z)[y>>5])&=(~((v&((1<<(y-x+1))-1))<<(x&0x1f)))

/*************** Begin CSD Register Defines **************************/
#define CSD_TAAC(x)     GET_BITS_BETWEEN(112,119,x)
#define CSD_NSAC(x)     GET_BITS_BETWEEN(104,111,x)

#define CSD_READ_BL_LEN(x)  GET_BITS_BETWEEN(80,83,x)

#define CSD_ERASE_BLK_EN(x)     GET_BITS_BETWEEN(46,46,x)
#define CSD_SECTOR_SIZE(x)      GET_BITS_BETWEEN(39,45,x)
#define CSD_WRT_BL_LEN(x)       GET_BITS_BETWEEN(22,25,x)
#define CSD_WRT_BL_PARTIAL(x)   GET_BITS_BETWEEN(21,21,x)
#define CSD_C_SIZE_MULT(x)      GET_BITS_BETWEEN(47,49,x)

#define CSD_C_SIZE(x)           CSD_C_SIZE_INLINE(x)

static inline u32 CSD_C_SIZE_INLINE(u32 *csd_array)
{
	u32 bits_62_to_63, bits_64_to_73;
	bits_62_to_63 = GET_BITS_BETWEEN(62, 63, csd_array);
	bits_64_to_73 = GET_BITS_BETWEEN(64, 73, csd_array);
	return (bits_62_to_63 | (bits_64_to_73 << 2));
}

/*************** End  CSD Register Defines **************************/

/*********************Begin Ext CSD Structure ****************************/
#define ECSD_S_CMD_SET(x)               (x)[504]
#define ECSD_MIN_PERF_W_8_52(x)         (x)[210]
#define ECSD_MIN_PERF_R_8_52(x)         (x)[209]
#define ECSD_MIN_PERF_W_8_26_4_52(x)    (x)[208]
#define ECSD_MIN_PERF_R_8_26_4_52(x)    (x)[207]
#define ECSD_MIN_PERF_W_4_26(x)         (x)[206]
#define ECSD_MIN_PERF_R_4_26(x)         (x)[205]
#define ECSD_PWR_CL_26_360(x)           (x)[203]
#define ECSD_PWR_CL_52_360(x)           (x)[202]
#define ECSD_PWR_CL_26_195(x)           (x)[201]
#define ECSD_PWR_CL_52_195(x)           (x)[200]
#define ECSD_CARD_TYPE(x)               (x)[196]
#define ECSD_CSD_STRUCTURE(x)           (x)[194]
#define ECSD_EXT_CSD_REV(x)             (x)[192]
#define ECSD_CMD_SET(x)                 (x)[191]
#define ECSD_CMD_SET_REV(x)             (x)[189]
#define ECSD_POWER_CLASS(x)             (x)[187]
#define ECSD_HS_TIMING(x)               (x)[185]

#define ECSD_BOOT_CONFIG(x)             (x)[179]
#define BOOT_ACK                        0x00000040   //If 1 Boot ack will be sent while booting, else boot ack will not be sent
#define BOOT_PARTITION_ENABLE_MSK       0x00000038   // Mask for BOOT_PARTION_ENABLE in BOOT_CONFIG
#define BOOT_PARTITION_1                0x00000008   //Boot Partition 1 Selected/Enabled
#define BOOT_PARTITION_2                0x00000010   //Boot Partition 2 Selected/Enabled
#define BOOT_PARTITION_USER             0x00000038   //User area selected for booting
#define BOOT_PARTITION_ACCESS_MAK       0x00000007   //Read Write access mask
#define BOOT_PARTITION_ACCESS_1_RW      0x00000001   //read write access to boot partition #1
#define BOOT_PARTITION_ACCESS_2_RW      0x00000002   //read write access to boot partition #2

#define ECSD_BOOT_BUS_WIDTH(x)          (x)[177]
#define BOOT_BUS_WIDTH_1                0x00000000
#define BOOT_BUS_WIDTH_4                0x00000001
#define BOOT_BUS_WIDTH_8                0x00000002
#define RESET_BOOT_BUS_WIDTH_1
#define ECSD_BOOT_SIZE_MULTI(x)         (x)[226]
#define ECSD_RPMB_SIZE_MULTI(x)         (x)[168]

#define ECSD_BOOT_INFO(x)               (x)[228]
#define ALT_BOOT_MODE                   0x00000001 //If 1 Device/card supports alternate boot mode. If 0, doesnot supports alternate boot mode

#define ARG_BOOTCONFIG_INDEX            0x00B30000  //179 is coded in 23:16 of CMD6 argument
#define ARG_BOOTCONFIG_ACCESS_WRITE     0x03000000  //Write is coded in 25:24 of CMD6 argumnet

#define ECSD_SECTOR_COUNT(x)        (  ((x)[215]<<24) | ((x)[214]<<16) | ((x)[213]<<8) | ((x)[212]) )

#define ARG_BUSWIDTH_INDEX            0x00B70000  //183 is coded in 23:16 of CMD6 argument
#define ARG_BUSWIDTH_ACCESS_WRITE     0x03000000  //Write is coded in 25:24 of CMD6 argumnet

/**********************End CSD Structure ****************************/
/*
One has to maintain proper state information of the card in order to have proper
functionality. Please refer MMC specs for the state information. Note irq is not
considered as the state. btst(Bus test state) of mmc spec is not considered in the driver
*/

typedef void (*emmc_postproc_callback)(void *, u32 *);
typedef void (*emmc_preproc_callback)(u32, u32, u32 *, u32 *);

typedef enum {
	EMMC_BOOT_PARTITION_1    = 0,
	EMMC_BOOT_PARTITION_2,
	EMMC_BOOT_PARTITION_USER
} emmc_area_e;
/*
Card information structure. One structure is maintained for each and every
card supported by the ip. Since the MMC is the basic type, this structure is mainly
based on MMC card types and the extra fields are added as per the requirement
*/
typedef enum {
	CARD_STATE_EMPTY    = -1,
	CARD_STATE_IDLE     = 0,
	CARD_STATE_READY    = 1,
	CARD_STATE_IDENT    = 2,
	CARD_STATE_STBY     = 3,
	CARD_STATE_TRAN     = 4,
	CARD_STATE_DATA     = 5,
	CARD_STATE_RCV      = 6,
	CARD_STATE_PRG      = 7,
	CARD_STATE_DIS      = 8,
	CARD_STATE_INA      = 9
} card_state_e;

/**
  * This structure contains the information for the IP which is being used.
  * It maintains certain state information for the IP and also so the border
  * values for the IP which will need to be referred at a later time.
  */
typedef struct {
	u32 total_cards;               /* The total cards on the system                */
	u32 fifo_depth;                /* The fifo depth of the IP                      */

	u32 num_of_cards;              /* Total number of cards the IP has been
                                    * configured for                                */
} emmc_status_info_t;

/**
  * The data pertaining to the single command active on the bus.
  * This data is maintained for a single command which is active on the bus. Only one
  * instance of this structure is needed at one point of time. This structure carries
  * all state information regarding the command which is operational on the bus.
  */
typedef struct {

	/** The error status of the command.
	  * 0 means that there is no error.
	  */
	u32 error_status;

	/** The array of dwords which stores the response for the  command.
	 *  If set to NULL, the response is discarded,
	 */
	u32 *resp_buffer;

	/** The data buffer for a data command. It is ignored for
	 *  non data commands. used in Slave mode of operation
	 */
	u8 *data_buffer;

	/** The state of the command in progress. */
	u32 cmd_status;

	/** The number of blocks of to be read/written. */
	u32 num_of_blocks;

	/** The number of bytes already read/written. */
	u32 num_bytes_read;

	/** The slot in which the target card is inserted in. */
	u32 slot_num;

	/** This flag is set if a data command got aborted. */

	/** A bus corruption had occured during the data transfer. */
	u32 bus_corruption_occured;

	/** The block size for the current data exchange */
	u32 blksize;

	u32 command_index;
} current_task_status_t;

/*
The supported card enumeration types. At present following cards are supported.
Note that once the HSMMC ad HSSD cards come in to picture the enum elements may
increase
*/
typedef enum {
	SD_TYPE,
	MMC_TYPE,
	MMC_4_3_TYPE,
	NONE_TYPE,
	ERRTYPE
} card_type_e;

#define CCCR_LENGTH     0x14

typedef struct {
	card_type_e    card_type;
	card_state_e card_state;
	union {
		u32 csd_dwords[4];
		u8 csd_bytes[16];
	} csd_union;
	union {
		u32 cid_dwords[4];
		u8 cid_bytes[16];
	} cid_union;
#ifdef CONDIF_SUPPORT_EMMC_EXTCSD
	union {
		u32 extcsd_dwords[128];
		u8 extcsd_bytes[512];
	} extcsd_union;
#endif
	union {
		u32 scr_dwords[2];
		u8 scr_bytes[8];
	} scr_union;

	u8 the_cccr_bytes[CCCR_LENGTH];

	u32 the_rca;
	u32 card_write_blksize;
	u32 card_read_blksize;
	u32 orig_card_write_blksize;
	u32 orig_card_read_blksize;
	u32 card_size;
	u32 card_boot_size;
	u32 card_rpmb_size;
	u32 divider_val;
	u32 version;
} card_info_t;

#define the_cid	cid_union.cid_dwords
#define the_csd csd_union.csd_dwords
#define the_cid_bytes cid_union.cid_bytes
#define the_csd_bytes csd_union.csd_bytes
#ifdef CONDIF_SUPPORT_EMMC_EXTCSD
#define the_extcsd_bytes extcsd_union.extcsd_bytes
#define the_extcsd extcsd_union.extcsd_dwords
#endif
#define the_scr scr_union.scr_dwords
#define the_scr_bytes scr_union.scr_bytes

/* Error Codes */

#define ERRCARDNOTFOUND         5
#define ERRCMDRETRIESOVER       6
#define ERRHARDWARE             11

#define ERRNOERROR              0
#define ERRCARDNOTCONN          1
#define ERRCMDNOTSUPP           2
#define ERRINVALIDCARDNUM       3
#define ERRRESPTIMEOUT          4
#define ERRCARDNOTFOUND         5
#define ERRCMDINPROGRESS        7
#define ERRNOTSUPPORTED         8
#define ERRRESPRECEP            9

#define ERRENUMERATE            10

#define ERRHARDWARE             11
#define ERRNOMEMORY             12
#define ERRFSMSTATE             14
#define ERRADDRESSRANGE         15
#define ERRADDRESSMISALIGN      16
#define ERRBLOCKLEN             17
#define ERRERASESEQERR          18
#define ERRERASEPARAM           19
#define ERRPROT                 20
#define ERRCARDLOCKED           21
#define ERRCRC                  22
#define ERRILLEGALCOMMAND       23
#define ERRECCFAILED            24
#define ERRCCERR                25
#define ERRUNKNOWN              26
#define ERRUNDERRUN             27
#define ERROVERRUN              28
#define ERRCSDOVERWRITE         29
#define ERRERASERESET           30
#define ERRDATATIMEOUT          31
#define ERRUNDERWRITE           32
#define ERROVERREAD             33
#define ERRENDBITERR            34
#define ERRDCRC                 35
#define ERRSTARTBIT             36
#define ERRTIMEROUT             37
#define ERRCARDNOTREADY         38
#define ERRBADFUNC              39
#define ERRPARAM                40
#define ERRNOTFOUND             41
#define ERRWRTPRT               42

#define R1CS_CURRENT_STATE_MASK         0x00001e00

#define R1CS_CURRENT_STATE(x)           (((x)&R1CS_CURRENT_STATE_MASK)>>9)
/* Define Card status bits (R1) */
#define R1CS_ADDRESS_OUT_OF_RANGE       0x80000000
#define R1CS_ADDRESS_MISALIGN           0x40000000
#define R1CS_BLOCK_LEN_ERR              0x20000000
#define R1CS_ERASE_SEQ_ERR              0x10000000
#define R1CS_ERASE_PARAM                0x08000000
#define R1CS_WP_VIOLATION               0x04000000
#define R1CS_CARD_IS_LOCKED             0x02000000
#define R1CS_LCK_UNLCK_FAILED           0x01000000
#define R1CS_COM_CRC_ERROR              0x00800000
#define R1CS_ILLEGAL_COMMAND            0x00400000
#define R1CS_CARD_ECC_FAILED            0x00200000
#define R1CS_CC_ERROR                   0x00100000
#define R1CS_ERROR                      0x00080000
#define R1CS_UNDERRUN                   0x00040000
#define R1CS_OVERRUN                    0x00020000
#define R1CS_CSD_OVERWRITE              0x00010000
#define R1CS_WP_ERASE_SKIP              0x00008000
#define R1CS_RESERVED_0                 0x00004000
#define R1CS_ERASE_RESET                0x00002000
#define R1CS_CURRENT_STATE_MASK         0x00001e00
#define R1CS_READY_FOR_DATA             0x00000100
#define R1CS_SWITCH_ERROR               0x00000080
#define R1CS_RESERVED_1                 0x00000040
#define R1CS_APP_CMD                    0x00000020
#define R1CS_RESERVED_2                 0x00000010
#define R1CS_APP_SPECIFIC_MASK          0x0000000c
#define R1CS_MANUFAC_TEST_MASK          0x00000003

#define R1CS_ERROR_OCCURED_MAP          0xfdffa080

#define R1CS_CURRENT_STATE(x)           (((x)&R1CS_CURRENT_STATE_MASK)>>9)

#define GET_FIFO_COUNT(x)               (((x)&0x3ffe0000)>>17)
#define GET_R6_RCA(x)                   (((x)&0xffff0000)>>16)
#define STATUS_FIFO_EMPTY               0x00000004
#define STATUS_FIFO_FULL                0x00000008

#define READY_FOR_DATA_RETRIES          20

/* Misc defines */
#define VOLT_SWITCH_TIMEOUT_5   1    /* 1 Second is the time kept for Voltage switching to Happen*/
#define VOLT_SWITCH_TIMEOUT_1   1    /* 1 Second is the time kept for Voltage switching to Happen*/
#define CMD_RESP_TIMEOUT        11   /*25 changed from 25 to 11 => wait for 11 seconds: CEATA time out is 10 seconds*/
#define CMD_MAX_RETRIES         1000 /*changed from 100 to be consistent with all polling iterations -- Manju */

/* Operation Conditions Register (OCR) Register Definition */
#define OCR_POWER_UP_STATUS             0x80000000
#define OCR_ACCESSMODE_SECTOR           0x40000000 /*This is to indicate the secor addressing for MMC4.2 High capacity MMC cards */
#define OCR_RESERVED_1                  0x7f000000
#define OCR_27TO36                      0x00ff8000
#define OCR_20TO26                      0x00007f00
#define OCR_165TO195                    0x00000010
#define OCR_RESERVED_2                  0x0000007f
#define MMC_MOBILE_VOLTAGE              OCR_165TO195

#define OCR_CCS                         0x40000000  /*This is sent by card to indicate it is high capcity SD card*/
#define OCR_HCS                         OCR_CCS     /*This is sent by host querying whether card is high capacity?*/

#define OCR_FB                          0x20000000  /* Fast Boot bit reserved for eSD */
#define OCR_XPC                         0x10000000  /* OCR_XPC used to check on SDXC Power Control. If 0 => Power Saving, If 1 => Maximum Performance */
#define OCR_S18R                        0x01000000  /* Switching to 1.8V Request 0 => Use current Signal Voltage, 1 => Switch to 1.8V Signal Voltage */

#define R4_RESP_ERROR_BIT               0x00010000
#define CMD39_WRITE_REG                 0x00008000

#define R5_IO_ERR_BITS  0x0000cd00

#define R5_IO_CRC_ERR   0x00008000
#define R5_IO_BAD_CMD   0x00004000
#define R5_IO_GEN_ERR   0x00000800
#define R5_IO_FUNC_ERR  0x00000200
#define R5_IO_OUT_RANGE 0x00000100

u32 emmc_host_init(card_info_t *emmc_card_info);
u32 emmc_send_clock_only_cmd(void);
u32 emmc_execute_command(u32 cmd_register, u32 arg_register);
s32 emmc_init(void);
u32 emmc_enumerate_the_card(u32 slot_num);
u32 emmc_check_r1_resp(u32 the_response);
u32 emmc_put_in_trans_state(u32 slot);
void emmc_emmc_read(u8 slot_id, u32 from, u32 len, u8 *buf);
void emmc_emmc_write(u8 slot_id, u32 to, u32 len, const u8 *buf);
u32 emmc_write_out_data(current_task_status_t *the_task_status, u32 the_interrupt_status);
u32 emmc_read_in_data(current_task_status_t *the_task_status, u32 the_interrupt_status);

#endif /* __ASM_ARCH_EMMC_H  */

