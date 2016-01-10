//
// Created by Peijie You on 15/12/17.
//

#include <kern/init/init.h>
#include <kern/mm/slb.h>
#include <interrupt.h>

context_cpu_t* context_no;
context_cpu_t* context_ndef;
context_cpu_t* context_svc;
context_cpu_t* context_abort;
context_cpu_t* context_irq;
context_cpu_t* context_fiq;

static int check_ICDICFR();
static void prepare_ICCICR();
static void prepare_ICCPMR();
static void prepare_ICDDCR();
static void prepare_ICDIPR();
static void prepare_ICDIPTR();
static void prepare_ICDISER();
static int asm_sctlr();
static int init_context_container();


//  todo, context_cpu_t only or full pcb
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
//    test_all_interrupt();
    return 0;
}

//  init procedure
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

static void prepare_ICDDCR()
{
    u32* dcr = (u32*)(PERIPHBASE + ICDDCR_OFFSET);
    dcr[0] |= 0b11;
    uart_spin_printf("icddcr\t%d\r\n\0", dcr[0]);
}

static void prepare_ICDIPTR()
{
    u8* iptr = (u8*)(PERIPHBASE + ICDIPTR_OFFSET);

    bool used_irq[100];
    int i;

    /*
     * TODO, for now, all irq go to cpu0
     * other encodings:
     * 1.   for cpu1, use 0b10
     * 2.   for both, use 0b11
     *
     * irq[0-15], sgi(which is, in fact, an irq)
     * irq[16-31], ppi
     * irq[32-95], spi, cnt=95-32+1=64
     */
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
    /*
     * context container, for temperate storage
     * here the "context" means that context of program BEFORE entering the exception.
     */
    context_no =    (context_cpu_t*)slb_alloc(sizeof(context_cpu_t));
    context_ndef =  (context_cpu_t*)slb_alloc(sizeof(context_cpu_t));
    context_svc =   (context_cpu_t*)slb_alloc(sizeof(context_cpu_t));
    context_abort = (context_cpu_t*)slb_alloc(sizeof(context_cpu_t));
    context_irq =   (context_cpu_t*)slb_alloc(sizeof(context_cpu_t));
    context_fiq =   (context_cpu_t*)slb_alloc(sizeof(context_cpu_t));
    return 0;
}

static int asm_sctlr()
{
//    in case it is off
    asm volatile
    (
        "MRC p15, 0, r0, c1, c0, 0\n"
        // set VE[24] bit to 0
        "AND r0, r0, #0xFEFFFFFF\n"
        "ORR r0, r0, #0x2000\n"
        "MCR p15, 0, r0, c1, c0, 0\n"
        // enable irq, and fiq
        "MRS r0, cpsr\n"
        "AND r0, r0, #0xFFFFFE3F\n"
        "MSR cpsr, r0\n"
        :
        :
        :"r0"
    );
    return 0;
}