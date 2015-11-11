MEMORY
{
	RAM (rwx) :
		ORIGIN = 0,
		LENGTH = 0
}

start_addr = 0x200000;

SECTIONS
{
	. = start_addr;
	.text . : {
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

ENTRY(main)
