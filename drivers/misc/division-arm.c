/*
 * Copyright (C) 2015 David Gao <davidgao1001@gmail.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

/*
 * Provide integer division and modulus as Cortex-A9 does not have its own.
 * These functions should not be called directly.
 */


/*
 * TODO
 * by Piji You on 15/12/5.
 * design:
 * 1.	udivmod, follow the origin implementation, fix for overflow issue
 * 2.	udiv, call udivmod & operation on registers r0, r1, not sure if it works
 * 3.	idivmod, see the function, round to zero
 * 4.	idiv, call idivmod
 * 5.	idiv0, not fully handled, how to call it?
 * 6.	need uidiv0?
 * 7.	64bit div, don't give a shit
 */

signed int __aeabi_idiv0(signed int numerator, signed int denominator);
inline unsigned int ABS(signed int x) {return x < 0 ? -x : x;}

unsigned int __aeabi_uidivmod(unsigned int numerator, unsigned int denominator)
{
//	unsigned int tmp = 1;
//	unsigned int shift = denominator << 1;
//	unsigned int quot = 0;
//	while (shift <= numerator) {
//		denominator <<= 1;
//		shift <<= 1;
//		tmp <<= 1;
//	}
//	while (tmp > 0) {
//		if (numerator > denominator){
//			quot += tmp;
//			numerator -= denominator;
//		}
//		denominator >>= 1;
//		tmp >>= 1;
//	}
//	asm volatile (
//		"mov r1, %[rem]"::[rem] "r" (numerator)
//	);
//	return quot;

	unsigned int i = 1;
	unsigned int shift = denominator << 1;
	unsigned int quot = 0;

	if (denominator == 0) return __aeabi_idiv0(numerator, denominator);
//	update, prevent shift overflow
	for (i = 1; shift < 0x80000000 && shift <= numerator; i <<= 1, denominator <<= 1)
		shift <<= 1;

	for (; i > 0; i >>= 1, denominator >>= 1)
		while (numerator >= denominator)
		{
			numerator -= denominator;
			quot += i;
		};

	asm volatile
	(
		"mov r0, %0\n"
		"mov r1, %1\n"
		:
		:"r"(quot), "r"(numerator)
	);
	return quot;
}

unsigned int __aeabi_uidiv(unsigned int numerator, unsigned int denominator)
{
//	is it ok?
	return __aeabi_uidivmod(numerator, denominator);
}

signed int __aeabi_idivmod(signed int numerator, signed int denominator)
{
//	result should round to zero? c99, c++0x
//	-2147483648 / -1 = -2147483648

	if (denominator == 0) return __aeabi_idiv0(numerator, denominator);

	signed int result = __aeabi_uidivmod(ABS(numerator), ABS(denominator));

	if (numerator >= 0 && denominator > 0);
	if (numerator < 0 && denominator > 0) result = -result;
	if (numerator >= 0 && denominator < 0) result = -result;
	if (numerator < 0 && denominator < 0);

	signed int remain = numerator - result * denominator;
	asm volatile
	(
		"mov r0, %0\n"
		"mov r1, %1\n"
		:
		:"r"(result), "r"(remain)
	);
	return 0;
}

signed int __aeabi_idiv(signed int numerator, signed int denominator)
{
	return __aeabi_idivmod(numerator, denominator);
}

// exception idiv0, should also implement uidiv0?
signed int __aeabi_idiv0(signed int numerator, signed int denominator)
{
	return -1;
}
