MEMORY
{
	RAM (rwx) :
		ORIGIN = 0,
		LENGTH = 0
}

low_paddr = 3M;
low_vaddr = 3M;
high_paddr = 2M;
high_vaddr = 2048M+2M;
int_vaddr = 0xFFFF0000;
int_paddr = 0x4F0000;

SECTIONS
{
	.low low_vaddr : AT(low_paddr)
	{
		mmu_low.elf
	}

	.interrupt_table int_vaddr : AT(int_paddr)
	{
		*interrupt_vector.o
	}

	.high high_vaddr : AT(high_paddr)
	{
		*enter_kernel_space.o
		*.o
	}
}

ENTRY(mmu_low_main)