// 
// debug program by ypjie
//

#include <drivers/serial/uart.h>
#include <drivers/sd/sd-zynq7000.h>
#include <drivers/misc/dtb-zynq7000.h>

#include <stdio.h>
#include <string.h>
#include <config.h>

u8* kernel_base;

void uart_spin_puts(const char *str)
{
	printf("%s", str);
}

void puthex(u32 num)
{
	printf("%d\n", num);
}

//------done
//------to debug
// u32 init_pspace();
// u32 pages_alloc(u32 size);
// u32 pages_free(u32 st, u32 fi);
// u32 pages_split(free_area_t* p, u32 first_size);

// void* slb_alloc(u32 size);
// void* slb_alloc_align(u32 size, u32 align);
// int slb_free(void *p, u32 size);
// int slb_free_align(void* p, u32 size, u32 align);
// int slb_init();

// slb_pool_t* pool_alloc(u32 osize, u32 align);
// void pool_free(slb_pool_t* p);
// slb_pool_t* pool_segm(slb_pool_t* pool, u32 psize, u32 osize, u32 align);



// void *kmalloc(u32 size, u32 align);
// void kmfree(void *p);
// void kminit();



int main()
{
	u32 a;
	u32 b;
	u32 c;
	u32 a1;
	u32 a2;

	kernel_base = (u8*)malloc(sizeof(u8) * 0x2000000);
	printf("kernel_base: %x\n", (u32)kernel_base);
	if (init_pspace() == 0) puts("init GOOD");
	printf("%x\n", a = pages_alloc(0x100000*2));
	printf("%x\n", b = pages_alloc(0x100000*2));
	printf("%x\n", c = pages_alloc(0x100000*2));
	pages_free(a, a + 0x200000);
	printf("%x\n", a1 = pages_alloc(0x100000*3));

	printf("%x\n", a2 = pages_alloc(0x100000*1));
	printf("%x\n", a = pages_alloc(0xfff00000));
	printf("%x\n", a = pages_alloc(0xfff00000));
	pages_free(a1, a1 + 0x100000 * 3);
	pages_free(a2, a2 + 0x100000);
	pages_free(c, c + 0x200000);
	pages_free(b, b + 0x200000);
	printf("%x\n", a = pages_alloc(0xfff00000));
}