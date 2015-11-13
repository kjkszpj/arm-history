//  2G              RAM
//  2G+512M         page table
//  2G+512M+16k     program
//  4G              stack
#include "mmu_high.h"

u32 pt_offset = 0x20000000;
u32 *page_table = (u32*)(KERNEL_BASE + 0x20000000);

void mmu_high_main()
{
    uart_spin_puts("DONE?\r\n\0");
//    clear_low();
//    build_free_list();
    while (1);
}

void clear_low()
{
    u32 i;

    for (i = 0; i < 0x800; i++) page_table[i] = page_table[i] & 0xFFFFFFFC;
}

void build_free_list()
{
    u32 i;

    for (i = 8; i < 512; i++)
    {
//        free1M(P2V(i * 0x100000))
    }
}