/*
 *	PLEASE refer to elf format CAREFULLY first!
 *
 *	TODO:
 *	1.	buffer address is 0x1200000 and 0x1300002, do not load segment here
 *	2.	better load to the buffer and copy to target address
 *	3.	paddr instead of vaddr
 */

#include <config.h>

#define PRELOAD_VECTOR_BASE	0x1ff00000

void (*uart_init)(void) = (void *)(PRELOAD_VECTOR_BASE + 0x4);
void (*uart_enable)(void) = (void *)(PRELOAD_VECTOR_BASE + 0x8);
void (*uart_spin_puts)(const char *) = (void *)(PRELOAD_VECTOR_BASE + 0xC);
int (*sd_dma_spin_read)(u32 pa, u16 count, u32 offset) = (void *)(PRELOAD_VECTOR_BASE + 0x14);

void mbr_bootmain(void)
{
	u32 i;

	volatile u8 *buffer = (void *) 0x1300002;
	sd_dma_spin_read((u32)buffer, 1, 0);
	u32 elf_addr = *(u32 *)(buffer + 0x1D6);

	/* get second partition */
	buffer = (void *) 0x1200000;
	sd_dma_spin_read((u32)buffer, 3, elf_addr);

	void (*kernel_entry)(void) = (void *)(*(u32 *)(buffer + 0x18));
	u32 phoff = *((u32 *)(buffer + 0x1C));
	u16 phentsize = *((u16 *)(buffer + 0x2A));
	u16 phnum = *((u16 *)(buffer + 0x2C));
	
	uart_spin_puts("DO");
	/* load according to program header */
	u32 *p_header;
	for (i = 0; i < phnum; i++)
	{
		sd_dma_spin_read((u32)buffer, 2, (u32)(elf_addr + (phoff >> 9)));
		p_header = (u32 *)(buffer + (phoff & 511));
		u32 phe_offset = p_header[1];
		u32 phe_vaddr = p_header[2];
		u32 phe_filesz = p_header[4];
		u8 offset = phe_offset & 511;
		u32 start_id = phe_offset >> 9;
		u32 end_id = ((phe_offset + (u32)phe_filesz - 1) >> 9) + 1;
		/* TODO should be big_read instead */
		sd_dma_spin_read(phe_vaddr - offset, end_id - start_id, start_id + elf_addr);
		phoff += phentsize >> 2;
	}
	/*now entering kernel*/
	kernel_entry();
	while (1);
}

