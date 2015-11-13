MEMORY
{
	RAM (rwx) :
		ORIGIN = 0,
		LENGTH = 0
}

low_paddr = 3M;
low_vaddr = 3M;
high_paddr = 2M;
high_vaddr = 2048M;

SECTIONS
{
	.low low_vaddr : AT(low_paddr){
		mmu_low.elf
	}
	.high high_vaddr : AT(high_paddr){
		mmu_high.elf
	}
}

ENTRY(mmu_low_main)