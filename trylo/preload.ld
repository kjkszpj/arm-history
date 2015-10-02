MEMORY
{
	RAM (rwx) :
		ORIGIN = 1M,
		LENGTH = 256M - 1M
}

preload_size = 1M;

SECTIONS
{
	. = ORIGIN(RAM) + LENGTH(RAM) - preload_size;
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
