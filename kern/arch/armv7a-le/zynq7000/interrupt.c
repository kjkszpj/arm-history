//
// Created by Peijie You on 15/12/3.
//

//  TODO, not so sure about the hardware part of interrupt preparation
//  referring to zynq-7000, Chap 7, figure 7-4 and 7-5
//  in this .c file, we focus on dealing with the HARDWARE related issue when interrupt

/*
// system call design
// // process related syscall
// 1.  fork
// 2.  exec
// 3.  kill / exit?
// 4.  waitforpid?
// 5.  getpid
// -   getuid
// // file related syscall
// 6.  open
// 7.  close
// 8.  read
// 9.  write
// 10. relocate
// -   getdir
// -   chdir
// // time related syscall
// 11. sleep
// 12. time
// 13. clock
// // ipc related?
// -   ipc...
 */

#include <interrupt.h>
#include <kern/mm/slb.h>

//typedef struct
//{
////---order here is weird, because we want to use stmia when interrupt
//    u32 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12;
//    u32 pc;
//    u32 cpsr, spsr;
//    u32 sp, lr;
//} context_cpu_t;

//  inner function
//  for init
//  TODO, set ICDISER0-2
static int check_ICDICFR();
static void prepare_ICCICR();
static void prepare_ICCPMR();
static void prepare_ICDDCR();
static void prepare_ICDIPR();
static void prepare_ICDIPTR();
static void prepare_ICDISER();
static int asm_sctlr();
static int init_context_container();
//  for debug
static int print_cpu();
static int test_all_interrupt();

//  todo, context_cpu_t only or full pcb
context_cpu_t* context_no;
context_cpu_t* context_ndef;
context_cpu_t* context_svc;
context_cpu_t* context_abort;
context_cpu_t* context_irq;
context_cpu_t* context_fiq;


int interrupt_init()
{
    if (check_ICDICFR() != 0) return 1;
    prepare_ICDDCR();
    prepare_ICDIPTR();
    prepare_ICDIPR();
    prepare_ICDISER();
    prepare_ICCPMR();
    prepare_ICCICR();
    asm_sctlr();
    init_context_container();
    test_all_interrupt();
    return 0;
}


//  init procedure
static int asm_sctlr()
{
//    in case it is off
    asm volatile
    (
        "dsb\n"
        "isb\n"
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

static void prepare_ICDDCR()
{
    u32* dcr = (u32*)(PERIPHBASE + ICDDCR_OFFSET);
    uart_spin_puts("icddcr\t\0");
    puthex(dcr[0]);
    dcr[0] = dcr[0] | 0b11;
    puthex(dcr[0]);
}

static int check_ICDICFR()
{
    u32* icfr = (u32*)(PERIPHBASE + ICDICFR_OFFSET);
    int i;

    if (icfr[0] != 0xAAAAAAAA) return 1;
    if (icfr[1] != 0x7DC00000) return 1;

    uart_spin_puts("information ICDICFR:\r\n\0");
    for (i = 0; i < 6; i++) puthex(icfr[i]);
    return 0;
}

static void prepare_ICDIPTR()
{
    u8* iptr = (u8*)(PERIPHBASE + ICDIPTR_OFFSET);

    bool used_irq[100];
    int i;

//    TODO
//    for now, all irq go to cpu0
//    for cpu1, use 0b10
//    for both, use 0b11

//    irq[0-15], sgi-irq
//    irq[16-31], ppi-irq
//    irq[32-95], spi, cnt=95-32+1=64

    for (i = 0; i < 100; i++) used_irq[i] = false;

    for (i = 16; i < 32; i++) used_irq[i] = true;
    used_irq[36] = true;
    used_irq[93] = true;
    used_irq[94] = true;
    for (i = 0; i < 96; i++) if (!used_irq[i]) iptr[i] = (iptr[i] & 0xFC) | 0b01;
}

static void prepare_ICDIPR()
{
    u32* ipr = (u32*)(PERIPHBASE + ICDIPR_OFFSET);
    int i;

    uart_spin_puts("icdipr\r\n\0");
    for (i = 0; i < 24; i++) ipr[i] = 0x01020304;
}

static void prepare_ICDISER()
{
    u32* iser = (u32*)(PERIPHBASE + ICDISER_OFFSET);
    iser[0] = 0xFFFFFFFF;
    iser[1] = 0xFFFFFFFF;
    iser[2] = 0xFFFFFFFF;
    uart_spin_printf("icdiser\t%x\r\n\0", iser[0]);
}

static void prepare_ICCPMR()
{
    u32* pmr = (u32*)(PERIPHBASE+ICCPMR_OFFSET);
    pmr[0] = (pmr[0] & 0xFFFFFFF00) | 0xFF;
    uart_spin_printf("iccpmr\t%x\r\n\0", pmr[0]);
}

static void prepare_ICCICR()
{
    u32* icr = (u32*)(PERIPHBASE+ICCICR_OFFSET);
    icr[0] = icr[0] | 0b11;
    uart_spin_printf("iccicr\t%x\r\n\0", icr[0]);
}

static int init_context_container()
{
//    context container, for temperate storage
    context_no =    (context_cpu_t*)slb_alloc(sizeof(context_cpu_t));
    context_ndef =  (context_cpu_t*)slb_alloc(sizeof(context_cpu_t));
    context_svc =   (context_cpu_t*)slb_alloc(sizeof(context_cpu_t));
    context_abort = (context_cpu_t*)slb_alloc(sizeof(context_cpu_t));
    context_irq =   (context_cpu_t*)slb_alloc(sizeof(context_cpu_t));
    context_fiq =   (context_cpu_t*)slb_alloc(sizeof(context_cpu_t));

    return 0;
}


//  entry of interrupt
int int_ent_ndef()
{
    asm volatile("mov %0, r0\n" :"=r"(context_ndef->sp) : : );
    asm volatile("mov %0, lr\n" :"=r"(context_ndef->lr) : : );
    asm volatile("mrs %0, spsr\n" :"=r"(context_ndef->spsr) : : );

//    TODO, enable irq, fiq
    uart_spin_puts("It works!, now in ndef\r\n\0");
//    syscall();
//    TODO, disable irq, fiq

    asm volatile("msr spsr, %0\n" : :"r"(context_ndef->spsr) : );
    asm volatile("mov lr, %0\n" : :"r"(context_ndef->lr) : );
    asm volatile("mov r0, %0\n" : :"r"(context_ndef->sp) : );
    return context_ndef->sp;
}

//  mainly focus here
int int_ent_svc()
{
//    u32 temp;

    /*
     * save the bank register of mode before interrupt,
     * i.e. sp, lr, pc, cpsr, spsr
     * 1.   r0 now may hold the sp
     * 2.   can access tad lr now
     * 3.   pc is in the context stack, pc = lr thing
     * 4.   cpsr = saved spsr
     * 5.   spsr (if nessary)
     */

    asm volatile("mov %0, r0\n" :"=r"(context_svc->sp) : : );
//    i think we should not concern lr, it will push in stack?
    asm volatile("mov %0, lr\n" :"=r"(context_svc->lr) : : );
//    store (now) spsr, if interrupt from sys to sys(irq enabled), then we must be able to cascade return
    asm volatile("mrs %0, spsr\n" :"=r"(context_svc->spsr) : : );

//    TODO, enable irq, fiq
//    syscall();
    uart_spin_puts("It works!, now in svc\r\n\0");
    print_cpu();
    puthex((*context_svc).sp);
    puthex((*context_svc).lr);
    puthex((*context_svc).pc);
    puthex((*context_svc).cpsr);
    puthex((*context_svc).spsr);
//    TODO, disable irq, fiq

    asm volatile("msr spsr, %0\n" : :"r"(context_svc->spsr) : );
    asm volatile("mov lr, %0\n" : :"r"(context_svc->lr) : );
    asm volatile("mov r0, %0\n" : :"r"(context_svc->sp) : );
    uart_spin_puts("bye\r\n\0");
    return context_svc->sp;
}

int int_ent_prefetch_abort()
{
    asm volatile("mov %0, r0\n" :"=r"(context_abort->sp) : : );
    asm volatile("mov %0, lr\n" :"=r"(context_abort->lr) : : );
    asm volatile("mrs %0, spsr\n" :"=r"(context_abort->spsr) : : );

//    TODO, enable irq, fiq
    uart_spin_puts("It works!, now in prefetch abort\r\n\0");
//    prefetch_abort();
//    TODO, disable irq, fiq

    asm volatile("msr spsr, %0\n" : :"r"(context_abort->spsr) : );
    asm volatile("mov lr, %0\n" : :"r"(context_abort->lr) : );
    asm volatile("mov r0, %0\n" : :"r"(context_abort->sp) : );
    return context_abort->sp;
}

int int_ent_data_abort()
{
    asm volatile("mov %0, r0\n" :"=r"(context_abort->sp) : : );
    asm volatile("mov %0, lr\n" :"=r"(context_abort->lr) : : );
    asm volatile("mrs %0, spsr\n" :"=r"(context_abort->spsr) : : );
//    TODO, don't know why, in data abort, lr = pc + 8!
    (*context_abort).pc = (*context_abort).pc - 4;

//    TODO, enable irq, fiq
//    prefetch_data_abort();
//    TODO, disable irq, fiq
    uart_spin_puts("It works!, now in data abort\r\n\0");
    print_cpu();
    puthex((*context_abort).sp);
    puthex((*context_abort).lr);
    puthex((*context_abort).pc);
    puthex((*context_abort).cpsr);
    puthex((*context_abort).spsr);
    uart_spin_puts("bye\r\n\0");

    asm volatile("msr spsr, %0\n" : :"r"(context_abort->spsr) : );
    asm volatile("mov lr, %0\n" : :"r"(context_abort->lr) : );
    asm volatile("mov r0, %0\n" : :"r"(context_abort->sp) : );
    return context_abort->sp;
}

int int_ent_irq()
{
    asm volatile("mov %0, r0\n" :"=r"(context_irq->sp) : : );
    asm volatile("mov %0, lr\n" :"=r"(context_irq->lr) : : );
    asm volatile("mrs %0, spsr\n" :"=r"(context_irq->spsr) : : );

//    TODO, enable irq, fiq
//    irq();
//    TODO, disable irq, fiq
    u32 irq_id = *(u32*)(0xF8F0010C);
    uart_spin_puts("It works!, now in irq\r\n\0");
    puthex(irq_id);
    print_cpu();

    puthex((*context_irq).sp);
    puthex((*context_irq).lr);
    puthex((*context_irq).pc);
    puthex((*context_irq).cpsr);
    puthex((*context_irq).spsr);
    uart_spin_puts("bye\r\n\0");

    asm volatile("msr spsr, %0\n" : :"r"(context_irq->spsr) : );
    asm volatile("mov lr, %0\n" : :"r"(context_irq->lr) : );
    asm volatile("mov r0, %0\n" : :"r"(context_irq->sp) : );
    return context_irq->sp;
}

int int_ent_fiq()
{
//    TODO, fiq bank more register, save it
    asm volatile("mov %0, r0\n" :"=r"(context_fiq->sp) : : );
    asm volatile("mov %0, lr\n" :"=r"(context_fiq->lr) : : );
    asm volatile("mrs %0, spsr\n" :"=r"(context_fiq->spsr) : : );

//    TODO, enable irq, fiq
    uart_spin_puts("It works!, now in fiq\r\n\0");
//    fiq();
//    TODO, disable irq, fiq

    asm volatile("msr spsr, %0\n" : :"r"(context_fiq->spsr) : );
    asm volatile("mov lr, %0\n" : :"r"(context_fiq->lr) : );
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


//  debug
static int test_all_interrupt()
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
////    align, data abort-> invalid address, data abort->
//    uart_spin_puts("address data abort\r\n\0");
//    u32 *b;
////    todo, for now, data abort will return to pc which generate this interrupt, loop forever
//    b = 0x2;
//    *b = 123;
//    uart_spin_puts("align data abort\r\n\0");
//    b = 0x80000002;
//    *b = 123;
////  now try the private clock, irq
    uart_spin_puts("private clock\r\n\0");
     asm volatile
     (
         "mrs r0, cpsr\n"
         "and r0, r0, #0xFFFFFE3F\n"
         "msr cpsr, r0\n"
         "dsb\n"
         "isb\n"
         :
         :
         :"r0"
     );
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
    for (i = 0; i < 100000000; i++) i = a;
    uart_spin_puts("------enough!------\r\n\0");
    puthex(pcr[0]);
    puthex(pcr[1]);
    puthex(pcr[2]);
    puthex(pcr[3]);
    print_cpu();

//    uart_spin_puts("---sgi---\r\n\0");
//    pcr = (PERIPHBASE + ICDSGIR_OFFSET);
//    a = pcr[0];
//    puthex(a);
//    a = a | 0b11;
//    pcr[0] = a;
//    puthex(pcr[0]);
    return 0;
}
