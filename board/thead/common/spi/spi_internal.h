/*
* Copyright (C) 2017-2020 Alibaba Group Holding Limited
*
* SPDX-License-Identifier: GPL-2.0+
*/

#ifndef __SPI_INTERNAL_H__
#define __SPI_INTERNAL_H__

#include "../datatype.h"
#include "spi.h"

/* some register bits macro  */
#define CK_SPI_ENABLE           0x01
#define CK_SPI_DISABLE          0x00
#define CK_SPI_TMOD_BIT8        0x0100
#define CK_SPI_TMOD_BIT9        0x0200
#define CK_SPI_POLARITY         0x80
#define CK_SPI_PHASE            0x40
#define CK_SPI_BUSY             0x01
#define CK_SPI_INT_DISABLE      0x00

/* register visit struct  */
typedef struct {
	volatile u16 CTRLR0;
	volatile u16 rev0;
	volatile u16 CTRLR1;
	volatile u16 rev1;
	volatile u8  SPIENR;
	volatile u8  rev2[7];
	volatile u32 SER;
	volatile u16 BAUDR;
	volatile u16 rev3;
	volatile u32 TXFTLR;
	volatile u32 RXFTLR;
	volatile u32 TXFLR;
	volatile u32 RXFLR;
	volatile u8  SR;
	volatile u8  rev4[3];
	volatile u32 IMR;
	volatile u32 ISR;
	volatile u32 RISR;
	volatile u8  TXOICR;
	volatile u8  rev5[3];
	volatile u8  RXOICR;
	volatile u8  rev6[3];
	volatile u8  RXUICR;
	volatile u8  rev7[3];
	volatile u8  MSTICR;
	volatile u8  rev8[3];
	volatile u8  ICR;
	volatile u8  rev9[3];
	volatile u8  DMACR;
	volatile u8  rev10[3];
	volatile u8  DMATDLR;
	volatile u8  rev11[3];
	volatile u8  DMARDLR;
	volatile u8  rev12[3];
	volatile u32 IDR;
	volatile u32 rev13;
	volatile u16 DR;
	volatile u16 rev14[17];
	volatile u8  WR;
} ckstruct_spi_reg_t, *pckstruct_spi_reg_t;

#endif /* __SPI_INTERNAL_H__ */
