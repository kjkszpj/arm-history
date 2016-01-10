//
// Created by Peijie You on 15/12/11.
//

#include <kern/mm/pte.h>

void invalidate_tlb()
{
    asm volatile
    (
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
    invalidate_tlb();
}

void move_TTBR1(u32 pt1_paddr)
{
    asm volatile
    (
        "mov r0, %0\n"
        "mcr p15, 0, r0, c2, c0, 0\n"
        "dsb\n"
        "isb\n"
        :
        :"r"(pt1_paddr)
        :"r0"
    );
    invalidate_tlb();
}