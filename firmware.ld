

/*
 * Copyright (C) 2015 David Gao <davidgao1001@gmail.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

/* BOARD-SPECIFIC BEGIN */

/* Total ONBOARD memory */

/* ram_size = 512M */

/* BOARD-SPECIFIC END */


/* CHIP-SPECIFIC BEGIN */

/* size of address section to be filtered by SCU */
/* We are in NO-SCU state now, but RAM is ready. */

/* scu_sect_size = 1M */

/* CHIP-SPECIFIC END */



/* preload_size = 1M */

MEMORY
{
	FIRMWARE (rwx) :
		ORIGIN = 512M - 1M,
		LENGTH = 1M
}

SECTIONS
{
	.vector : {
		*vector.o(.text)
	}
	.text : {
		*(.text)
	}
	.rodata : {
		*(.rodata)
	}
	.data : {
		*(.data)
	}
	.bss : {
		*(.bss)
	}
}

ENTRY(firmware_vector)
