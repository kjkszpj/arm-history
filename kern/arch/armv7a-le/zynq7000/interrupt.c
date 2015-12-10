//
// Created by Peijie You on 15/12/3.
//

//  TODO, not so sure about the hardware part of interrupt preparation
//  referring to zynq-7000, Chap 7, figure 7-4 and 7-5

#include <interrupt.h>

static int prepare_ICDICFR();
static int prepare_ICDIPTR();
static int asm_sctlr();

int interrupt_init()
{
    if (prepare_ICDICFR() != 0) return 1;
    if (prepare_ICDIPTR() != 0) return 1;
    asm_sctlr();
    return 0;
}

static int asm_sctlr()
{
    asm volatile
    (
        "MRC p15, 0, r0, c1, c0, 0\n"
        // set VE[24] bit to 0
        "AND r0, r0, #0xFEFFFFFF\n"
        "ORR r0, r0, #0x2000\n"
        "MCR p15, 0, r0, c1, c0, 0\n"
        :
        :
        :"r0"
    );
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
    int i;

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
    for (i = 0; i < 96; i++) if (!used_irq[i]) iptr[i] = (iptr[i] & 0xFC) | 0b01;
    return 0;
}


static int print_cpu()
{
    u32 a;

    uart_spin_puts("cpu\r\ncpsr:\t\0");
    asm volatile("MRS r0, cpsr\nMOV %0, r0\n" :"=r"(a) : :"r0");
    puthex(a);

    uart_spin_puts("spsr:\t\0");
    asm volatile("MRS r0, spsr\nMOV %0, r0\n" :"=r"(a) : :"r0");
    puthex(a);

    uart_spin_puts("sp:\t\0");
    asm volatile("MOV %0, sp\n" :"=r"(a) : :);
    puthex(a);

    uart_spin_puts("r0:\t\0");
    asm volatile("MOV %0, r0\n" :"=r"(a) : :);
    puthex(a);
    uart_spin_puts("r1:\t\0");
    asm volatile("MOV %0, r1\n" :"=r"(a) : :);
    puthex(a);
    uart_spin_puts("r2:\t\0");
    asm volatile("MOV %0, r2\n" :"=r"(a) : :);
    puthex(a);
    return 0;
}

// DEBUG

int int_ent_svc()
{
    uart_spin_puts("----It works! falling into interrupt svc\r\n\0");
    print_cpu();
    uart_spin_puts("struct context_cpu\r\n\0");
    puthex(&stack_svc);
    puthex(stack_svc.r0);
    puthex(stack_svc.lr);
    puthex(stack_svc.cpsr);
    puthex(stack_svc.spsr);
    uart_spin_puts("bye.\r\n\0");
    return 0;
}

int test_all_interrupt()
{
//    test? fuck, do not test here!
//    TODO, no stack for now.
    stack_svc = *(struct context_cpu*)slb_alloc(sizeof(struct context_cpu));
    puthex(&stack_svc);
    uart_spin_puts("------DEBUG------\r\norigin status:\r\n\0");
    print_cpu();
    uart_spin_puts("now trying interrupt\r\n\0");
    uart_spin_puts("svc\r\n\0");
    asm volatile
    (
        "SVC #2"
        :
        :
        :
    );
//    align, data abort. invalid address, data abort.
//    uart_spin_puts("address data abort\r\n\0");
//    u32 *b;
//    TODO, for now, data abort will return to pc which generate this interrupt, loop forever
//    b = 0x0;
//    *b = 123;
//    uart_spin_puts("align data abort\r\n\0");
//    b = 0x80000002;
//    *b = 123;

    uart_spin_puts("enough!\r\n\0");
    print_cpu();

}
