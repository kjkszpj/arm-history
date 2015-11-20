#include <config.h>

// TODO, current cache policy of page table is?
int l1cache_init()
{
	asm volatile
	(
		"LDR r0, #0"
		"MCR p15, 0, r0, c8, c7, 0\n"
		"MCR p15, 0, r0, c7, c5, 0\n"
		"MCR p15, 0, r0, c7, c5, 6\n"
		// TODO, what? should be done for all the sets/ways
		"MCR p15, 0, r11, c7, c14, 2\n"
		// TODO initialize mmu
		"MRC p15, 0, r0, c1, c0, 0\n"
		"ORR r0, #0x1004\n"
		"MCR p15, 0, r0, c1, c0, 0\n"
		"DSB\n"
		"ISB\n"
		:
		:
		:"r0"
	);
}