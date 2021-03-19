/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <serial.h>
#include "../uart.h"
#include "uart.h"

/****************************************************************************/

s32 uart_open(phys_addr_t uart_addrbase)
{
	volatile UART_REG *info = (UART_REG *)uart_addrbase;
	s32 divisor;

	/*baudrate=(seriak clock freq)/(16*divisor).*/
	divisor = ((LSP_DEFAULT_FREQ / 115200) >> 4);
	info->uart_LCR |= LCR_SET_DLAB;
	/* DLL and DLH is lower 8-bits and higher 8-bits of divisor.*/
	info->uart_DLL = divisor & 0xff;
	info->uart_DLH = (divisor >> 8) & 0xff;
	/*
	 * The DLAB must be cleared after the baudrate is setted
	 * to access other registers.
	 */
	info->uart_LCR &= (~LCR_SET_DLAB);
	/* paraty */
	info->uart_FCR = 0x1;
	info->uart_LCR =  LCR_WORD_SIZE_8;
	info->uart_IER |= (IER_RDA_INT_ENABLE | IER_THRE_INT_ENABLE);

	return SUCCESS;
}

/* This function is used to close the uart
 * clear the callback function
 * free the irq
 * return: SUCCESS or FAILURE
 */
s32 uart_close(phys_addr_t uart_addrbase)
{
	volatile UART_REG *info = (UART_REG *)uart_addrbase;

	info->uart_IER &= ~IER_RDA_INT_ENABLE;
	info->uart_IER &= !(IER_RDA_INT_ENABLE | IER_THRE_INT_ENABLE);
	return SUCCESS;
}

/* This function is used to transmit character,in query mode or interrupt mode.
 * Parameters:
 *       uartid--a basepointer, could be one of UART0, UART1, UART2 or UART3.
 *   brxquery--it indicates the receive mode: true - query mode, false -
 *   interrupt mode
 * Return: SUCCESS or FAILURE.
 */
s32 uart_put_char(phys_addr_t uart_addrbase, u8 ch)
{
	volatile UART_REG *info = (UART_REG *)uart_addrbase;

	while ((!(info->uart_LSR & LSR_TRANS_EMPTY)));
	info->uart_THR = ch;
	return SUCCESS;
}
