SECTIONS
{
    . = 3M;

	.text . : {
	    *entry_program.o(.text)
		*(.text)
	}

	.rodata . : {
		*(.rodata)
	}

	.data . : {
		*(.data)
	}

	.bss . : {
		*(.bss)
		*(COMMON)
	}
}

ENTRY(entry_program)
