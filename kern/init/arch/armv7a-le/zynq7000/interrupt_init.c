//
// Created by Peijie You on 15/12/3.
//

//  TODO, not so sure about the hardware part of interrupt preparation
//  referring to zynq-7000, Chap 7, figure 7-4 and 7-5

#include <interrupt_init.h>

static int prepare_ICDICFR();
static int prepare_ICDIPTR();

int interrupt_init()
{
    if (prepare_ICDICFR() != 0) return 1;
    if (prepare_ICDIPTR() != 0) return 1;
//    TODO, what else to init
    return 0;
}

static int prepare_ICDICFR()
{
    u32* icfr = (u32*)(PERIPHBASE + ICDICFR_OFFSET);
    if (icfr[0] != 0xAAAAAAAA) return 1;
    if (icfr[1] != 0x7DC00000) return 1;

    int i;
    uart_spin_puts("information ICDICFR:\r\n\0");
    for (i = 0; i < 6; i++) puthex(icfr[i]);
//    TODO, update?
    return 0;
}

static int prepare_ICDIPTR()
{
    u8* iptr = (u8*)(PERIPHBASE + ICDIPTR_OFFSET);

    bool used_irq[100];

//    TODO
//    for now, all go to cpu0
//    for cpu1, use 0b10
//    for both, use 0b11

//    irq[0-15], sgi
//    irq[16-31], ppi
//    irq[32-95], spi, cnt=95-32+1=64

    for (i = 0; i < 100; i++) used_irq[i] = false;

    for (i = 16; i < 32; i++) used_irq[i] = true;
    used_irq[36] = true;
    used_irq[93] = true;
    used_irq[94] = true;
    for (i = 0; i < 96; i++)
        if (!used_irq[i])
            iptr[i] = (iptr[i] & 0xFC) | 0b01;
}
