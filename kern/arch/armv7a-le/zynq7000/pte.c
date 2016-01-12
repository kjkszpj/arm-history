//
// Created by Peijie You on 15/12/11.
//

#include <kern/mm/pte.h>

#include <kern/init/init.h>

void invalidate_tlb()
{
    asm volatile
    (
        "dsb\n"
        "isb\n"
        "mov r0, #0\n"
        "mcr p15, 0, r0, c8, c7, 0\n"
//      just in case, i think here dsb & isb is not needed
        "dsb\n"
        "isb\n"
        :
        :
        :"r0"
    );
}

void move_TTBR0(u32 pt0_paddr)
{
    u32 tsp;
    uart_spin_printf("______open___\r\n\0");
    asm volatile
    (
        "mov r0, %0\n"
        "mcr p15, 0, r0, c2, c0, 0\n"
        "dsb\n"
        "isb\n"
        :
        :"r"(pt0_paddr)
        :"r0"
    );
    uart_spin_printf("______epen___\r\n\0");
    invalidate_tlb();
//    asm volatile
//    (
//    "mov %0, sp"
//    :"=r"(tsp)
//    :
//    :
//    );
//    uart_spin_printf("____sp: %x\r\n\0", tsp);
}

void move_TTBR1(u32 pt1_paddr)
{
    asm volatile
    (
        "mov r0, %0\n"
        "mcr p15, 0, r0, c2, c0, 1\n"
        "dsb\n"
        "isb\n"
        :
        :"r"(pt1_paddr)
        :"r0"
    );
    invalidate_tlb();
}