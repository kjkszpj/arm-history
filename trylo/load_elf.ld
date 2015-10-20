MEMORY
{
	RAM (rwx) :
		ORIGIN = 0,
		LENGTH = 0
}

preload_size = 0x100000;


SECTIONS
{
	. = preload_size;
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
