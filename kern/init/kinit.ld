MEMORY
{
	RAM (rwx) :
		ORIGIN = 0,
		LENGTH = 512M
}

start_vaddr = 2048M+2M;

SECTIONS
{
	. = start_vaddr;
	.text . :{
	    *enter_kernel_space.o(.text)
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
