/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <linux/string.h>
#include <stdarg.h>
#include <common.h>
#include <serial.h>
#include <hardware.h>
#include "uart.h"
#include "mini_printf.h"

/******************************************************
*change the number to string
*
* uq : the number need to change
* base: the base of system(for example 2,8,10,16 as usual)
* buf: data buffer for change procedure and return result
* return value: the string  after change
*********************************************************/
static char *bf __attribute__((section(".data")));
static char zs __attribute__((section(".data")));

#ifdef CONFIG_USE_MINI_PRINTF
static char *outstr __attribute__((section(".data")));
#endif

static void out(char c)
{
	*bf++ = c;
}

static void out_dgt(char dgt)
{
	out(dgt + (dgt < 10 ? '0' : 'a' - 10));
	zs = 1;
}

static void div_out(unsigned int *num, unsigned int div)
{
	unsigned char dgt = 0;

	while (*num >= div) {
		*num -= div;
		dgt++;
	}

	if (zs || dgt > 0)
		out_dgt(dgt);
}
static char *numtostring(unsigned long uq, int base, char *buf)
{
	register char *p, *p0;
	int n = 0, i;

	p = buf;
	*buf = 0;
	do {
		*buf ++ = "0123456789abcdef"[uq % base];
		n++;
	} while (uq /= base);
	p[n] = '\0';

	p0 = ++buf;
	if (base == 16 && n < 8) {  /*If Hex, the length is fixxed with 8 digitals*/
		for (i = 0; i < 8 - n; i++) {
			p0[i] = '0';
		}
		for (; i < 8; i++) {
			p0[i] = p[8 - i - 1];
		}
		p0[8] = '\0';
	} else {
		for (i = 0; i < n; i++) {
			p0[i] = p[n - i - 1];
		}
		p0[n] = '\0';
	}
	return (p0);
}

/*
* display the char on the console
* ch: the char need to display
*/
void mini_putc(const char c)
{
	char ch = c;
	while (uart_put_char(CONSOLE_UART_BASE, ch) != SUCCESS);
}

static int puts_without_n(const char *ptr)
{
	while (*ptr != '\0') {
		if (SUCCESS == uart_put_char(CONSOLE_UART_BASE, *ptr))
			ptr++;
	}
	return 0;
}

/*
* display a string on the console
* ptr: the string need to display
*/
void mini_puts(const char *s)
{
	puts_without_n(s);
	uart_put_char(CONSOLE_UART_BASE, '\n');
}

/*
* receive a char from the console
*return value: the char received from the console
*/
#if 0
int getchar(void)
{
	char ch;
	while (uart_get_char(CONSOLE_UART_BASE, (u8 *)&ch) != SUCCESS);
	return ch;
}

#endif
/*
* print the result after translated according to the format
*/
int mini_printf(const char *fmt, ...)
{
	const char *s;
	int        value;
	void*        ptr;
	char       ch, buf[64], *pbuf;
	va_list    ap;

	va_start(ap, fmt);
	while (*fmt) {
		if (*fmt != '%') {
			if (*fmt == '\n')
				mini_putc('\r');
			mini_putc(*fmt++);
			continue;
		}
		switch (*++fmt) {
		case 's':
			s = va_arg(ap, const char *);
			puts_without_n(s);
			break;
		case 'd':
			value = va_arg(ap, int);
			if (value < 0) {
				mini_putc('-');
				value = 0 - value;
			}
			pbuf = numtostring((unsigned long)value, 10, buf);
			puts_without_n(pbuf);
			break;
		case 'x':
			value = va_arg(ap, int);
			pbuf = numtostring((unsigned long)value, 16, buf);
			puts_without_n(pbuf);
			break;
		case 'c':
			ch = (unsigned char)va_arg(ap, int);
			pbuf = &ch;
			mini_putc(*pbuf);
			break;
		case 'p':
			ptr = va_arg(ap, void *);
			pbuf = numtostring((unsigned long)ptr, 16, buf);
			puts_without_n(pbuf);
			break;
		default:
			mini_putc(*fmt);
			break;
		}
		fmt ++;
	}
	va_end(ap);
	return 0x01;
}

int _vprintf(const char *fmt, va_list va, void (*putc)(const char ch))
{
	char ch;
	char *p;
	unsigned int num;
	char buf[12];
	unsigned int div;
	while ((ch = *(fmt++))) {
		if (ch != '%') {
			putc(ch);
		} else {
			bool lz = false;
			int width = 0;
			ch = *(fmt++);
			if (ch == '0') {
				ch = *(fmt++);
				lz = 1;
			}

			if (ch >= '0' && ch <= '9') {
				width = 0;
				while (ch >= '0' && ch <= '9') {
					width = (width * 10) + ch - '0';
					ch = *fmt++;
				}
			}
			bf = buf;
			p = bf;
			p = bf;
			zs = 0;

			switch (ch) {
			case '\0':
				goto abort;
			case 'u':
			case 'd':
				num = va_arg(va, unsigned int);
				if (ch == 'd' && (int)num < 0) {
					num = -(int)num;
					out('-');
				}
				if (!num) {
					out_dgt(0);
				} else {
					for (div = 1000000000; div; div /= 10)
						div_out(&num, div);
				}
				break;
			case 'x':
				num = va_arg(va, unsigned int);
				if (!num) {
					out_dgt(0);
				} else {
					for (div = 0x10000000; div; div /= 0x10)
						div_out(&num, div);
				}
				break;
			case 'c':
				out((char)(va_arg(va, int)));
				break;
			case 's':
				p = va_arg(va, char*);
				break;
			case '%':
				out('%');
			default:
				break;
			}

			*bf = 0;
			bf = p;
			while (*bf++ && width > 0)
				width--;
			while (width-- > 0)
				putc(lz ? '0' : ' ');
			if (p) {
				while ((ch = *p++))
					putc(ch);
			}
		}
	}
abort:
	return 0;
}

#ifdef CONFIG_USE_MINI_PRINTF
static void putc_outstr(char ch)
{
	*outstr++ = ch;
}

int sprintf(char *buf, const char *fmt, ...)
{
	va_list va;
	int ret;

	va_start(va, fmt);
	outstr = buf;
	ret = _vprintf(fmt, va, putc_outstr);
	va_end(va);
	*outstr = '\0';

	return ret;
}
#endif
