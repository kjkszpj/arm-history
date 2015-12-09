//
// Created by Peijie You on 15/11/14.
//

#include <kern/init/init.h>
#include <kern/mm/pages_manage.h>
#include <kern/mm/slb.h>
#include <interrupt_init.h>
#include <stdio.h>

u32 *page_table = (u32*)(KERNEL_BASE + PT_OFFSET);

// TODO, in fact, it is more like arch_init()
// DEBUG
static int print_cpu();

struct context_cpu
{
    u32 r0;
    u32 r1;
    u32 r2;
    u32 r3;
    u32 r4;
    u32 r5;
    u32 r6;
    u32 r7;
    u32 r8;
    u32 r9;
    u32 r10;
    u32 r11;
    u32 r12;
    u32 lr;
    u32 cpsr;
    u32 spsr;
}stack_svc;

void kinit()
{
    uart_spin_puts("GE\r\n\0");
    if (mmu_high_main() == 0) uart_spin_puts("MMU done.\r\n\0");
    if (l1cache_init() == 0) uart_spin_puts("L1 cache done.\r\n\0");
    if (scu_init() == 0) uart_spin_puts("SCU done.\r\n\0");

    if (init_pspace() == 0) uart_spin_puts("pages manage done.\r\n\0");
    if (slb_init() == 0) uart_spin_puts("slb manage done.\r\n\0");

    if (interrupt_init() == 0) uart_spin_puts("int done.\r\n\0");

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
//    align, data abort.
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
    uart_spin_puts("now trying snprintf\r\n\0");
    char temp[100];
    snprintf(temp, 90, "decimal:\t%d %d\r\n\0", 123, 0xFFFD0FA0);
    uart_spin_puts(temp);
    
    volatile u32 i;
    volatile u32 a;
    for (i = 0; i < 10; i++) a = i;
    uart_spin_puts("------Live to tell the story------\r\n\0");
    while (1);
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

//    uart_spin_puts("pc:\t\0");
//    asm volatile("MOV %0, pc\n" :"=r"(a) : :);
//    puthex(a);
//
//    uart_spin_puts("lr:\t\0");
//    asm volatile("MOV %0, lr\n" :"=r"(a) : :);
//    puthex(a);

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