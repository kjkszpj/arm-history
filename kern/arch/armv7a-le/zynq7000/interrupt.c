//
// Created by Peijie You on 15/12/3.
//

/*
 * TODO:
 * 1.   not so sure about the hardware part of interrupt preparation, check GIC carefully
 * 2.   check prefered return PC
 * 3.   new stack for sys mode
 *
 * design notice:
 * 1.   no nested interrupt
 * 2.   I mainly referring to zynq-7000, Chap 7, figure 7-4 and 7-5
 * 3.   in this .c file, we focus on dealing with the CONTEXT related issue when interrupt
 * 4.   for functionality of interrupt, should implement in another file, because it is arch (somehow) NOT related
 *
 *******system call design******
 *
 * process related syscall
 * 1.   fork
 * 2.   exec
 * 3.   kill / exit?
 * 4.   waitforpid?
 * 5.   getpid
 * -    getuid
 *
 * file related syscall
 * 6.   open
 * 7.   close
 * 8.   read
 * 9.   write
 * 10.  relocate
 * 14.  create
 * 15.  delete
 * -    getdir
 * -    chdir
 *
 * time related syscall
 * 11.  sleep
 * 12.  time
 * 13.  clock
 *
 * -    ipc related?
 */

#include <interrupt.h>
#include <kern/syscall.h>
#include <kern/sched/pcb.h>
#include <kern/sched/sched.h>
#include <string.h>

static int print_cpu();

//  entry of interrupt
int int_ent_ndef()
{
    asm volatile("mov %0, r0\n" :"=r"(context_ndef->sp) : : );
    asm volatile("mov %0, r1\n" :"=r"(context_ndef->lr) : : );
    asm volatile("mrs %0, spsr\n" :"=r"(context_ndef->spsr) : : );

    pcb_t* n_pcb = sched_get_running();
    memcpy(&n_pcb->cpu, context_ndef, sizeof(context_cpu_t));
    uart_spin_puts("Undefined instruction exception.\r\n\0");
//    syscall();
    asm volatile("msr spsr, %0\n" : :"r"(context_ndef->spsr) : );
    asm volatile("mov r1, %0\n" : :"r"(context_ndef->lr) : );
    asm volatile("mov r0, %0\n" : :"r"(context_ndef->sp) : );
    return context_ndef->sp;
}

//  mainly focus here
int int_ent_svc()
{
    u32 svc_id;

    /*
     * todo, other entry programs refer(copy) to this one, make sure they all work well.
     *
     * save the bank register of mode before interrupt,
     * i.e. sp, lr, pc, cpsr, spsr
     * 1.   r0 now may hold the sp
     * 2.   r1 hold lr, as it will change at "bl int_ent_svc()"
     * 2.   can access tad lr now
     * 3.   pc is in the context stack, pc = lr thing
     * 4.   cpsr = saved spsr
     * 5.   spsr (if nessary)
     */

    asm volatile("mov %0, r0\n" :"=r"(context_svc->sp) : : );
//    i think we should not concern lr, it will push in stack?
    asm volatile("mov %0, r1\n" :"=r"(context_svc->lr) : : );
    asm volatile("mrs %0, spsr\n" :"=r"(context_svc->spsr) : : );

    pcb_t* n_pcb = sched_get_running();
    memcpy(&n_pcb->cpu, context_svc, sizeof(context_cpu_t));
    uart_spin_puts("It works!, now in svc\r\n\0");
    print_cpu();

    uart_spin_printf("%x\r\n\0", context_svc->pc - 4);
    svc_id = (*(u32*)(context_svc->pc - 4)) & 0x00FFFFFF;
    syscall(svc_id);
    puthex((*context_svc).sp);
    puthex((*context_svc).lr);
    puthex((*context_svc).pc);
    puthex((*context_svc).cpsr);
    puthex((*context_svc).spsr);
    uart_spin_puts("bye\r\n\0");

    asm volatile("msr spsr, %0\n" : :"r"(context_svc->spsr) : );
    asm volatile("mov r1, %0\n" : :"r"(context_svc->lr) : );
    asm volatile("mov r0, %0\n" : :"r"(context_svc->sp) : );
    return context_svc->sp;
}

int int_ent_prefetch_abort()
{
    asm volatile("mov %0, r0\n" :"=r"(context_abort->sp) : : );
    asm volatile("mov %0, r1\n" :"=r"(context_abort->lr) : : );
    asm volatile("mrs %0, spsr\n" :"=r"(context_abort->spsr) : : );

    pcb_t* n_pcb = sched_get_running();
    memcpy(&n_pcb->cpu, context_abort, sizeof(context_cpu_t));
    uart_spin_puts("It works!, now in prefetch abort\r\n\0");
//    prefetch_abort();

    asm volatile("msr spsr, %0\n" : :"r"(context_abort->spsr) : );
    asm volatile("mov r1, %0\n" : :"r"(context_abort->lr) : );
    asm volatile("mov r0, %0\n" : :"r"(context_abort->sp) : );
    return context_abort->sp;
}

int int_ent_data_abort()
{
    asm volatile("mov %0, r0\n" :"=r"(context_abort->sp) : : );
    asm volatile("mov %0, r1\n" :"=r"(context_abort->lr) : : );
    asm volatile("mrs %0, spsr\n" :"=r"(context_abort->spsr) : : );
//    TODO, don't know why, in data abort, lr = pc + 8!
    (*context_abort).pc = (*context_abort).pc - 4;

//    prefetch_data_abort();
    pcb_t* n_pcb = sched_get_running();
    memcpy(&n_pcb->cpu, context_abort, sizeof(context_cpu_t));
    uart_spin_puts("It works!, now in data abort\r\n\0");
    print_cpu();
    puthex((*context_abort).sp);
    puthex((*context_abort).lr);
    puthex((*context_abort).pc);
    puthex((*context_abort).cpsr);
    puthex((*context_abort).spsr);
    uart_spin_puts("system down\r\n\0");
//    todo, for debug, now should exist no data abort
    while (1);

    asm volatile("msr spsr, %0\n" : :"r"(context_abort->spsr) : );
    asm volatile("mov r1, %0\n" : :"r"(context_abort->lr) : );
    asm volatile("mov r0, %0\n" : :"r"(context_abort->sp) : );
    return context_abort->sp;
}

int int_ent_irq()
{
    asm volatile("mov %0, r0\n" :"=r"(context_irq->sp) : : );
    asm volatile("mov %0, r1\n" :"=r"(context_irq->lr) : : );
    asm volatile("mrs %0, spsr\n" :"=r"(context_irq->spsr) : : );

    u32 irq_id = *(u32*)(PERIPHBASE + ICCIAR_OFFSET);
    pcb_t* n_pcb = sched_get_running();
    memcpy(&n_pcb->cpu, context_irq, sizeof(context_cpu_t));
    uart_spin_puts("It works!, now in fiq\r\n\0");
    puthex(irq_id);
    print_cpu();
    puthex((*context_irq).sp);
    puthex((*context_irq).lr);
    puthex((*context_irq).pc);
    puthex((*context_irq).cpsr);
    puthex((*context_irq).spsr);
    uart_spin_puts("bye\r\n\0");
    (*context_irq).pc -= 4;

    u32* temp = (u32*)(PERIPHBASE + ICCEOIR_OFFSET);
    puthex(temp[0]);
    temp[0] = irq_id;
//    u32* pcr = (u32*)(PERIPHBASE+0x0600);
//    pcr[3] = 0;
//    puthex(pcr[0]);
//    puthex(pcr[1]);
//    puthex(pcr[2]);
//    puthex(pcr[3]);

    asm volatile("msr spsr, %0\n" : :"r"(context_irq->spsr) : );
    asm volatile("mov r1, %0\n" : :"r"(context_irq->lr) : );
    asm volatile("mov r0, %0\n" : :"r"(context_irq->sp) : );
    return context_irq->sp;
}

int int_ent_fiq()
{
//    TODO, fiq bank more register, save it
    asm volatile("mov %0, r0\n" :"=r"(context_fiq->sp) : : );
    asm volatile("mov %0, r1\n" :"=r"(context_fiq->lr) : : );
    asm volatile("mrs %0, spsr\n" :"=r"(context_fiq->spsr) : : );

    pcb_t* n_pcb = sched_get_running();
    memcpy(&n_pcb->cpu, context_fiq, sizeof(context_cpu_t));
    uart_spin_puts("It works!, now in fiq\r\n\0");
//    fiq();

    asm volatile("msr spsr, %0\n" : :"r"(context_fiq->spsr) : );
    asm volatile("mov r1, %0\n" : :"r"(context_fiq->lr) : );
    asm volatile("mov r0, %0\n" : :"r"(context_fiq->sp) : );
    return context_fiq->sp;
}

// for debug, and visualization
static int print_cpu()
{
    u32 a;

    asm volatile("MRS r0, cpsr\nMOV %0, r0\n" :"=r"(a) : :"r0");
    uart_spin_printf("cpu\r\ncpsr:\t%x\r\n\0", a);

    asm volatile("MRS r0, spsr\nMOV %0, r0\n" :"=r"(a) : :"r0");
    uart_spin_printf("spsr:\t%x\r\n\0", a);

    asm volatile("MOV %0, sp\n" :"=r"(a) : :);
    uart_spin_printf("sp:\t%x\r\n\0", a);

    asm volatile("MOV %0, r0\n" :"=r"(a) : :);
    uart_spin_printf("r0:\t%x\r\n\0", a);

    asm volatile("MOV %0, r1\n" :"=r"(a) : :);
    uart_spin_printf("r1:\t%x\r\n\0", a);

    asm volatile("MOV %0, r2\n" :"=r"(a) : :);
    uart_spin_printf("r2:\t%x\r\n\0", a);
    return 0;
}


int test_all_interrupt()
{
//    test? fuck, do not test here!
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
    uart_spin_puts("svc2\r\n\0");
    asm volatile
    (
        "SVC #3"
        :
        :
        :
    );
////    align, data abort-> invalid address
//    uart_spin_puts("address data abort\r\n\0");
//    u32 *b;
////    todo, for now, data abort will return to pc which generate this interrupt, loop forever
//    b = 0x2;
//    *b = 123;
//    uart_spin_puts("align data abort\r\n\0");
//    b = 0x80000002;
//    *b = 123;

//  now try the private clock, irq
    uart_spin_puts("private clock\r\n\0");
    // print_cpu();
    u32* pcr = (u32*)(PERIPHBASE+0x0600);
    puthex(pcr[0]);
    puthex(pcr[1]);
    puthex(pcr[2]);
    puthex(pcr[3]);
    pcr[0] = 0x123400;
    pcr[1] = 0x123400;
    pcr[2] = (pcr[2] & 0xFFFFFFF8) | 0b111;
    u32 i = 0;
    volatile u32 a = 0;
    for (i = 0; i < 30000000; i++) i += a;
    uart_spin_puts("------enough!------\r\n\0");
    puthex(pcr[0]);
    puthex(pcr[1]);
    puthex(pcr[2]);
    puthex(pcr[3]);
    print_cpu();

//    TODO, for sgi
//    uart_spin_puts("---sgi---\r\n\0");
//    pcr = (PERIPHBASE + ICDSGIR_OFFSET);
//    a = pcr[0];
//    puthex(a);
//    a = a | 0b11;
//    pcr[0] = a;
//    puthex(pcr[0]);
    return 0;
}
