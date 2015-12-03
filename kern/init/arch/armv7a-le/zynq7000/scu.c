//
// Created by Peijie You on 15/11/19.
//

#include <scu.h>

u32 get_PERIPHBASE()
{
    u32 periphbase = 0;

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

int enable_scu()
{
    u32 *scu_register = (u32*)PERIPHBASE;

//    TODO, should know physical address first, then vmalloc, mapping and access it in kernel space.
//    make sure SAC and then change SCU control register
    scu_register[SCU_SAC_OFFSET] |= 0b0011;
    scu_register[SCU_CONTROL_OFFSET] |= 0b1;
    uart_spin_puts("---SCU control register:\t\0");
    puthex(scu_register[SCU_CONTROL_OFFSET]);
    return 0;
}

int scu_init()
{
//    only [31:13] is useful, drop the rest.
//    on armv7a-le zynq-7000, this value should always give 0xF8F00000 or something
    PERIPHBASE = get_PERIPHBASE() & 0xFFFFE000;
    uart_spin_puts("---PERIPHBASE is at:\t\0");
    puthex(PERIPHBASE);
//    PERIPHBASE may be in PHYSICAL ADDRESS
    if (enable_scu() != 0) uart_spin_puts("enable SCU failed.\r\n\0");
    return 0;
}