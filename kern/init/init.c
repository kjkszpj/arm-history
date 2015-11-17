//
// Created by Peijie You on 15/11/14.
//

#include <kern/init/init.h>

u32 *page_table = (u32*)(KERNEL_BASE + PT_OFFSET);

void kinit()
{
    uart_spin_puts("Hi from kernel space.\r\n\0");
    if (mmu_high_main() != 0) uart_spin_puts("MMU_down.\r\n\0");
//    if (init_pspace() != 0) uart_spin_puts("MM init down.\r\n\0");
    while (1);
}