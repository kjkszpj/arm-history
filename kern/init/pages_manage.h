//
// Created by Peijie You on 15/11/14.
//

#ifndef AIMV6_PAGES_MANAGE_H
#define AIMV6_PAGES_MANAGE_H

#include <config.h>

#ifndef KERNEL_BASE
#define KERNEL_BASE 0X80000000
#endif

#define P2V(paddr)          (((u32)paddr) + KERNEL_BASE)
#define V2P(vaddr)          (((u32)vaddr) - KERNEL_BASE)

struct FREE_AREA
{
    FREE_AREA* next;
    u32 size;
};

FREE_AREA* farea_head;

//  unit in byte---(4096Byte), so as below, will return the PHYSICAL address of the first page.
u32 init_pspace();
u32 alloc_pages(u32 size);
u32 free_pages(u32 st, u32 fi);
u32 split_pages(FREE_AREA* p, u32 first_size);

extern void uart_spin_puts(const char *str);
extern void puthex(u32 num);

#endif //AIMV6_PAGES_MANAGE_H
