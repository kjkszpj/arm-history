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

struct free_area_t
{
    free_area_t* next;
    u32 size;
};

//  TODO in fact, other program should NOT access this variable, how to protect?
free_area_t* farea_head;

//  unit in byte---(4096Byte), so as below, will return the PHYSICAL address of the first page.
u32 init_pspace();
u32 pages_alloc(u32 size);
u32 pages_free(u32 st, u32 fi);
u32 pages_split(free_area_t* p, u32 first_size);

extern void uart_spin_puts(const char *str);
extern void puthex(u32 num);

#endif //AIMV6_PAGES_MANAGE_H
