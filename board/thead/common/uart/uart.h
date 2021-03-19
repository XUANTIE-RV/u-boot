/*
* Copyright (C) 2017-2020 Alibaba Group Holding Limited
*
* SPDX-License-Identifier: GPL-2.0+
*/

#ifndef __UART_H__
#define __UART_H__

#include <asm/types.h>
#include "../datatype.h"
/************************************
 * (8 data bitbs, ODD, 1 stop bits)
 ***********************************/

typedef enum {
	B1200  = 1200,
	B2400  = 2400,
	B4800  = 4800,
	B9600  = 9600,
	B14400 = 14400,
	B19200 = 19200,
	B56000 = 56000,
	B38400 = 38400,
	B57600 = 57600,
	B115200 = 115200
} UART_BAUDRATE;

#define BAUDRATE   19200


typedef enum {
	UART0 = 0,
	UART1 = 1,
	UART2 = 2,
	UART3 = 3
} UART_DEVICE;

typedef enum {
	WORD_SIZE_5 = 0,
	WORD_SIZE_6 = 1,
	WORD_SIZE_7 = 2,
	WORD_SIZE_8 = 3
} UART_WORDSIZE;

typedef enum {
	ODD = 0,
	EVEN = 1,
	NONE = 2
} UART_PARITY;

typedef enum {
	LCR_STOP_BIT_1 = 0,
	LCR_STOP_BIT_2 = 1
} UART_STOPBIT;


/////////////////////////////////////////////////////////////////
/* open the uart :
* set the callback function --- handler(void);
* intilize the serial port,sending and receiving buffer;
* intilize irqhandler ;
* register irqhandler
* return: SUCCESS or FAILURE
*/
s32 uart_open(phys_addr_t uart_addrbase);

/* This function is used to close the uart
* clear the callback function
* free the irq
* return: SUCCESS or FAILURE
*/
s32 uart_close(phys_addr_t uart_addrbase);

/* This function is used to get character,in query mode or interrupt mode.
* Parameters:
*   uartid--a basepointer, could be one of UART0, UART1, UART2 or UART3.
*   brxquery--it indicates the receive mode: TRUE - query mode, FALSE -
*   interrupt mode
* return: SUCCESS or FAILURE
*/
s32 uart_get_char(phys_addr_t uart_addrbase,  u8 *ch);

/* This function is used to transmit character,in query mode or interrupt mode.
* Parameters:
*   uartid--a basepointer, could be one of UART0, UART1, UART2 or UART3.
*   brxquery--it indicates the receive mode: TRUE - query mode, FALSE -
*   interrupt mode
* Return: SUCCESS or FAILURE.
*/
s32 uart_put_char(phys_addr_t uart_addrbase, u8 ch);

#endif
