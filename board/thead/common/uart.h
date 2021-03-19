/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __ASM_ARCH_UART_H__
#define __ASM_ARCH_UART_H__

#include <linux/types.h>
#include "datatype.h"
#include "uart/uart.h"


/* UART registers addr definition */
#define UART_RBR            0x00    /* Receive Buffer Register (32 bits, R) */
#define UART_THR            0x00    /* Transmit Holding Register (32 bits, W) */
#define UART_DLL            0x00    /* Divisor Latch(Low)  (32 bits, R/W) */
#define UART_IER            0x01    /* Interrupt Enable Register (32 bits, R/W) */
#define UART_DLH            0x01    /* Divisor Latch(High) (32 bits, R/W) */
#define UART_IIR            0x02    /* Interrupt Identity Register (32 bits, R) */
#define UART_FCR            0x02    /* fifo Countrol Register (32 bits, W) */
#define UART_LCR            0x03    /* Line Control Register (32 bits, R/W) */
#define UART_MCR            0x04    /* Modem Control Register (32 bits, W) */
#define UART_LSR            0x05    /* Line Status Register (32 bits, R) */
#define UART_MSR            0x06    /* Modem Status Register (32 bits, R/W) */
#define UART_USR            0x1f    /* UART Status Register (32 bits, R/W) */


#define UART_BUSY_TIMEOUT      1000000
#define UART_RECEIVE_TIMEOUT   1000
#define UART_TRANSMIT_TIMEOUT  1000


/* UART register bit definitions */

#define USR_UART_BUSY           0x01
#define LSR_DATA_READY          0x01
#define LSR_THR_EMPTY           0x20
#define IER_RDA_INT_ENABLE      0x01
#define IER_THRE_INT_ENABLE     0x02
#define IIR_NO_ISQ_PEND         0x01

#define LCR_SET_DLAB            0x80   /* enable r/w DLR to set the baud rate */
#define LCR_PARITY_ENABLE       0x08   /* parity enabled */
#define LCR_PARITY_EVEN         0x10   /* Even parity enabled */
#define LCR_PARITY_ODD          0xef   /* Odd parity enabled */
#define LCR_WORD_SIZE_5         0xfc   /* the data length is 5 bits */
#define LCR_WORD_SIZE_6         0x01   /* the data length is 6 bits */
#define LCR_WORD_SIZE_7         0x02   /* the data length is 7 bits */
#define LCR_WORD_SIZE_8         0x03   /* the data length is 8 bits */
#define LCR_STOP_BIT1           0xfb   /* 1 stop bit */
#define LCR_STOP_BIT2           0x04   /* 1.5 stop bit */

#define LSR_PFE              0x80
#define LSR_TEMT             0x40
#define LSR_THRE             0x40
#define LSR_BI               0x10
#define LSR_FE               0x08
#define LSR_PE               0x04
#define LSR_OE               0x02
#define LSR_DR               0x01
#define LSR_TRANS_EMPTY      0x20


/*config the uart */

typedef struct UART_Info_t {
	volatile u32* addr;
} CKStruct_UartInfo, *PCKStruct_UartInfo;

/* Struct of Uart REG*/
typedef struct UART_REG {
	union {
		volatile u32 uart_RBR;
		volatile u32 uart_THR;
		volatile u32 uart_DLL;
	};
	union {
		volatile u32 uart_IER;
		volatile u32 uart_DLH;
	};
	union {
		volatile u32 uart_IIR;
		volatile u32 uart_FCR;
	};
	volatile u32 uart_LCR;
	volatile u32 uart_MCR;
	volatile u32 uart_LSR;
	volatile u32 uart_MSR;
	volatile u32 uart_RSV[24];
	volatile u32 uart_USR;
} UART_REG;

#endif /* __ASM_ARCH_UART_H__ */
