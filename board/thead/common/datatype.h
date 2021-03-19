/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __DATATYPE_H__
#define __DATATYPE_H__
#include <linux/types.h>
#include <asm/types.h>

#ifndef NULL
#define NULL  0x00
#endif

#ifndef TRUE
#define TRUE  0x01
#endif
#ifndef FALSE
#define FALSE 0x00
#endif

#ifndef true
#define true  0x01
#endif
#ifndef false
#define false 0x00
#endif

#ifndef SUCCESS
#define SUCCESS  0
#endif
#ifndef FAILURE
#define FAILURE  -1
#endif

#define STATUS_ERR  1
#define STATUS_OK   0

typedef struct {
	u16 year;
	u8  month;
	u8  day;
	u8  weekday;
	u8  hour;
	u8  min;
	u8  sec;
} __attribute__((packed)) RTCTIME, *PRTCTIME;


#if defined(DEBUG)
#define Debug     printf
#else
#define Debug
#endif

#define  IN
#define  OUT
#define INOUT

#endif  // __DATATYPE_H__
