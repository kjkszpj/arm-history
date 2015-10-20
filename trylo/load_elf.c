/*	little endian to big endian */
#include "sys/types.h"
#define BS 512

/* TODO not test */

/*
 * u32 le2be(u32 x)
 * {
 *	x = ((x & 0xff00ff00) >> 8
 *	) | ((x & 0x00ff00ff) << 8);
 *	x = (x >> 16) | (x << 16);
 *	return x;
 * }
 */

int main(void)
{
	/*	sdb2, start from mbr[0x1d5] */
	void (*puts)(const char *) = (void *)0x1FF0000C;
	int (*sd_dma_spin_read)(u32 pa, u16 count, u32 offset) = (void *)0x1ff00014;
	//int (*sd_dma_spin_write)(u32 pa, u16 count, u32 offset) = (void *)0x1ff00014;
	
	/* sign the mbr */
	puts("MBR!\r\n");
	
	/* get second partition */
	/* TODO sector = page? */
	/* TODO buffer address, fixed or ...? */
	/* TODO volatile buffer? */
	u32 elf_addr = *((u32 *)0x1001D6);
	volatile u8 *buffer = (void *) 0x120000;
	sd_dma_spin_read((u32)buffer, 2, elf_addr);
	puts("MBR!\r\n");
	
	void (*kernel_entry)(void) = (void *)(*(u32 *)(buffer + 0x18)); /* offset 0x18 */
	u32 *phoff = *((u32 *)(buffer + 0x1C)); /* offset 0x1C */
	u16 phentsize = *((u16 *)(buffer + 0x2A)); /* offset 0x2A */
	u16 phnum = *((u16 *)(buffer + 0x2C)); /* offset 0x2E */
	puts("MBR!\r\n");
	
	/* address of current program header entry */
	phoff += (u32)buffer;
	/* TODO alignment issue */
	for (u8 i = 0; i < phnum; i++)
	{
		u32 phe_offset = phoff[1];
		u32 phe_vaddr = phoff[2];
		u32 phe_filesz = phoff[4];
		/* u32 phe_memsz = le2be((u32 *)(phoff + 0x5)); */
		u8 offset = phe_offset & 511;
		u32 start_id = phe_offset >> 9;
		u32 end_id = ((phe_offset + (u32)buffer - 1) >> 9) + 1;
		sd_dma_spin_read(phe_vaddr - offset, end_id - start_id, phe_offset + buffer);
		phoff += phentsize >> 2;
	}
	puts("MBR!\r\n");
	
	/*now entering kernel*/
	kernel_entry();
	puts("MBR!\r\n");
	
	while (1);
}
