MEMORY
{
	RAM (rwx) :
		ORIGIN = 0,
		LENGTH = 0
}

start_vaddr = 2048M;

SECTIONS
{
	. = start_vaddr;
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
