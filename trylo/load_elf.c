/*	little endian to big endian */
#include <sys/types.h>

u32 le2be(u32 long x)
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
	int (*sd_dma_spin_read)(u32 pa, u16 count, u32 offset) = (void *)0x1ff00014;
	
	/* sign the mbr */
	puts("MBR!\r\n");
	
	volatile u32 elf_addr = 0;
	u32 phoff = 0;
	u32 phentsize = 0;
	u32 phnum = 0;
	for (u8 i = 0; i < phnum; i++)
	{
		u32 phe_offset = 0;
		u32 phe_vaddr = 0;
		u32 phe_filesz = 0;
		u32 phe_memsz = 0;
		/* load file + phe_offset, into phe_vaddr; */
		phoff += phentsize;
	}
	while (1);
}
