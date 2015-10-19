/*	little endian to big endian */
#include <sys/types.h>
#define BS 512


/* TODO not test */
u32 le2be(u32 x)
{
	x = ((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8);
	x = (x >> 16) | (x << 16);
	return x;
}

int main()
{
	/*	sdb2, start from mbr[0x1d5] */
	void (*puts)(const char *) = (void *)0x1FF0000C;
	int (*sd_dma_spin_read)(u32 pa, u16 count, u32 offset) = (void *)0x1ff00014;
	//int (*sd_dma_spin_write)(u32 pa, u16 count, u32 offset) = (void *)0x1ff00014;
	
	/* sign the mbr */
	//puts("MBR!\r\n");
	
	/* get second partition  */
	/* TODO sector = page? */
	u32 *elf_addr = le2be((u32*)(0x100000 + 0x1D6));
	volatile u8 *buffer = (void *) 0x120000;
	sd_dma_spin_read((u32)buffer, 1, *elf_addr);
	
	/* further optimize! le2be only once */
	void (*kernel_entry)(void) = (void *)le2be(buffer[6]); /* offset 0x18 */
	u32 *phoff = (u32 *)le2be(buffer[7]); /* offset 0x1C */
	u16 phentsize = (u16)le2be(buffer[10]) >> 16; /* offset 0x2A */
	u16 phnum = (u16)le2be(buffer[11]) & 0xFF; /* offset 0x2E */
	
	phoff += (u32)buffer;
	/* TODO alignment issue */
	for (u8 i = 0; i < phnum; i++)
	{
		u32 phe_offset = le2be(phoff[1]);
		u32 phe_vaddr = le2be(phoff[2]);
		u32 phe_filesz = le2be(phoff[4]);
		/* u32 phe_memsz = le2be((u32 *)(phoff + 0x5)); */
		sd_dma_spin_read(phe_vaddr, /*count*/1, /*offset*/phe_offset + buffer);
		phoff += phentsize;
	}
	/*now entering kernel*/
	kernel_entry();
	while (1);
}
