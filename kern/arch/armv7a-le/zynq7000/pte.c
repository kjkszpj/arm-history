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
//      just in case, i think here dsb, isb is not needed
        "dsb\n"
        "isb\n"
        :
        :
        :"r0"
    );
}