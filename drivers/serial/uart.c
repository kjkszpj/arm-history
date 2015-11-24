/*
 * Copyright (C) 2015 Gan Quan <coin2028@hotmail.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <drivers/serial/uart.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>

void uart_spin_puts(const char *str)
{
	for (; *str != '\0'; ++str)
		uart_spin_putbyte((unsigned char)*str);
}

void puthex(u32 num)
{
	int i;
	const char table[] = "0123456789ABCDEF";
	char buf[15] = "00000000\r\n\0";
	uart_spin_puts(buf);
	for (i = 28; i >= 0; i -= 4){
		buf[(28 - i) >> 2] = table[(num >> i) & 0xF];
	}
	uart_spin_puts(buf);
}

