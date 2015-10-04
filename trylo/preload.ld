MEMORY
{
	RAM (rwx) :
		ORIGIN = 0,
		LENGTH = 0
}

preload_size = 0;


SECTIONS
{
	. = 0;
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

ENTRY(foo)
