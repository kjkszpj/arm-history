//
// Created by Peijie You on 15/11/14.
//

#include <kern/init/init.h>

u32 *page_table = (u32*)(KERNEL_BASE + PT_OFFSET);

// in fact, it is more like arch_init()
void kinit()
{
//    TODO, why this?

    uart_spin_puts("GE\r\n\0");
    if (mmu_high_main() != 0) uart_spin_puts("mmu_high fail.\r\n\0"); else uart_spin_puts("MMU done.\r\n\0");
    if (l1cache_init() != 0) uart_spin_puts("L1 cache init fail\r\n\0"); else uart_spin_puts("L1 cache done.\r\n\0");
    if (scu_init() != 0) uart_spin_puts("SCU init fail.\r\n\0"); else uart_spin_puts("SCU done.\r\n\0");


    volatile u32 i;
    volatile u32 a;
    for (i = 0; i < 10; i++) a = i;
    uart_spin_puts("------Live to tell the story------\r\n\0");
    while (1);
}