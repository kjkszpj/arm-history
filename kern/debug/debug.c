// 
// debug program by ypjie
// 

#include <drivers/serial/uart.h>
#include <drivers/sd/sd-zynq7000.h>
#include <drivers/misc/dtb-zynq7000.h>

#include <stdio.h>
#include <string.h>
#include <config.h>

void uart_spin_puts(const char *str)
{
	printf("%s", str);
}

void puthex(u32 num)
{
	printf("%d\n", num);
}


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
	if (init_pspace() == 0) puts("GOOD");
}