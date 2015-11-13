MEMORY
{
	RAM (rwx) :
		ORIGIN = 0,
		LENGTH = 0
}

start_paddr = 3M;

SECTIONS
{
	. = start_paddr;
	.text . :{
		*(.text)
	}
	.rodata . :{
		*(.rodata)
	}
	.data . :{
		*(.data)
	}
	.bss . :{
		*(.bss)
		*(COMMON)
	}
}
