/*
 *	PLEASE refer to elf format CAREFULLY first!
 *
 *	Alignment 4?
 *	TODO:
 *	1.	buffer address is 0x120000 and 0x130002, do not load segment here
 *	2.	assuming ph follow after elf header, and the same time block number is NOT enough!!!
 *	3.	better load to the buffer and copy to target address
 */

#include "sys/types.h"
#define BS 512

int main(void)
{
	/*	sdb2, starting sector in mbr[0x1D5] */
	/* void (*puts)(const char *) = (void *)0x1FF0000C; */
	int (*sd_dma_spin_read)(u32 pa, u16 count, u32 offset) = (void *)0x1FF00014;
	u32 i;

	/* puts("DO"); */

	volatile u8 *buffer = (void *) 0x130002;
	sd_dma_spin_read((u32)buffer, 1, 0);
	u32 elf_addr = *(u32 *)(buffer + 0x1D6);

	/* get second partition */
	buffer = (void *) 0x120000;
	sd_dma_spin_read((u32)buffer, 3, elf_addr);

	void (*kernel_entry)(void) = (void *)(*(u32 *)(buffer + 0x18));
	u32 phoff = *((u32 *)(buffer + 0x1C));
	u16 phentsize = *((u16 *)(buffer + 0x2A));
	u16 phnum = *((u16 *)(buffer + 0x2C));
	
	/* load according to program header */
	u32 *p_header;
	for (i = 0; i < phnum; i++)
	{
		sd_dma_spin_read((u32)buffer, 2, (u32)(buffer + (phoff >> 9)));
		p_header = (u32 *)(buffer + (phoff & 511));
		u32 phe_offset = p_header[1];
		u32 phe_vaddr = p_header[2];
		u32 phe_filesz = p_header[4];
		u8 offset = phe_offset & 511;
		u32 start_id = phe_offset >> 9;
		u32 end_id = ((phe_offset + (u32)phe_filesz - 1) >> 9) + 1;
		sd_dma_spin_read(phe_vaddr - offset, end_id - start_id, start_id + elf_addr);
		phoff += phentsize >> 2;
	}
	
	/*now entering kernel*/
	kernel_entry();
	while (1);
}
