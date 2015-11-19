//
// Created by Peijie You on 15/11/19.
//

#include <scu.h>

u32 get_PERIPHBASE()
{
    u32 periphbase;

    asm volatile
    (
    "MRC p15, 4, r0, c15, c0, 0\n"
            "MOV %0, r0\n"
    :"=r"(periphbase)
    :
    :"r0"
    );
    return periphbase;
}

u32 PERIPHBASE = 0;

int scu_init()
{
//    only [31:13] is useful, drop the rest.
    PERIPHBASE = get_PERIPHBASE() & 0xFFFFF000;
    uart_spin_puts("PERIPHBASE is at:\r\n\0");
    puthex(PERIPHBASE);
    if (enable_SCU() != 0) uart_spin_puts("enable SCU failed.\r\n\0");
}