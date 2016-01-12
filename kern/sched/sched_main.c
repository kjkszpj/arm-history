    //
// Created by Peijie You on 15/12/17.
//

/*
 * todo, lock?
 */

// for init
#include <kern/init/init.h>
// for construct pcb wrapper
#include <kern/mm/slb.h>
// for switch mm
#include <kern/mm/pte.h>
// for sched
#include <kern/sched/pcb.h>
#include <kern/sched/sched.h>
// for P2V V2P thing
#include <kern/mm/pages_manage.h>
// for init_entry
#include <interrupt.h>

#include <string.h>

// for debug
#include <drivers/serial/uart.h>
#include <drivers/sd/sd-zynq7000.h>
#include <drivers/misc/dtb-zynq7000.h>

//------ you are welcome to chang this desgin
static u32 cnt_pcb = 0;
static pcb_t* pcb_list[MAX_PCB];

//  implement context switch
void switch_mm(u32 pt_from, u32 pd_to)
{
    /*
     * I think here previous pgd location is fixed and thus should not be save again.
     *
     * args are pointer to the pt_base in KERNEL VIRTUAL SPACE
     * should convert to physical address
     */
//    uart_spin_printf("aaaaaaaa%x\r\n\0", V2P(pd_to));
//    uart_spin_printf("bbbbbbbb%x\r\n\0", *(u32*)P2V(V2P(pd_to) + 0));
//    uart_spin_printf("cccccccc%x\r\n\0", *(u32*)P2V(V2P(pd_to) + 4 * 0x800));

    move_TTBR0(V2P(pd_to));
//    uart_spin_printf("aaaaaaaa%x\r\n\0", V2P(pd_to));
//    uart_spin_printf("bbbbbbbb%x\r\n\0", *(u32*)P2V(V2P(pd_to) + 0));
//    uart_spin_printf("cccccccc%x\r\n\0", *(u32*)P2V(V2P(pd_to) + 4 * 0x800));
}

void switch_to(context_cpu_t* cpu_from, context_cpu_t* cpu_to, context_cpu_t* on_return)
{
    /*
     * Now, we are in SYS mode.
     * Here both task_from and task_to are USER program, so no spsr, we need r0-r12, lr, sp, pc, cpsr
     * We construct a stack like thing to jump to task_to finally.
     * jump from here
     *
     */

    // todo, fine tune the exception return address.

    memcpy(cpu_from, on_return, sizeof(context_cpu_t));
    memcpy(on_return, cpu_to, sizeof(context_cpu_t));
}

void context_switch(pcb_t* task_from, pcb_t* task_to, context_cpu_t* on_return)
{
//    uart_spin_printf("It works! Now going to context switch.\r\n\0");
//    sched_preempt(task_from);
//    sched_allow(task_to);
    uart_spin_printf("@@@@@@@\r\n\0");
    switch_mm((u32)task_from->page_table, (u32)task_to->page_table);
    uart_spin_printf("@@@@@@@\r\n\0");
    switch_to(&(task_from->cpu), &(task_to->cpu), on_return);
    uart_spin_printf("@@@@@@@\r\n\0");
}

//  new/delete
pcb_t* new_pcb()
{
    if (cnt_pcb == MAX_PCB)
    {
        uart_spin_printf("**ERROR**, no more pcb.\r\n\0");
    }
    pcb_list[cnt_pcb] = slb_alloc(sizeof(pcb_t));
    memset(pcb_list[cnt_pcb], 0, sizeof(pcb_t));
    pcb_list[cnt_pcb]->status = NEW;
    pcb_list[cnt_pcb]->td.pid = cnt_pcb;
    //  here we also add to sched_rr
    sched_new(pcb_list[cnt_pcb]);
    return pcb_list[cnt_pcb++];
}

void delete_pcb(pcb_t task)
{
//    pass, should never delete
}

void init_pcb()
{
    memset(pcb_list, 0, sizeof(pcb_list));
}

//for debug
void print_pcb(pcb_t* task)
{
    uart_spin_printf("---LOOK PCB---:\r\n\0");
    uart_spin_printf("  pid:\t%d\r\n\0", task->td.pid);
    uart_spin_printf("  page table:\t%x\r\n\0", (u32)task->page_table);
    uart_spin_printf("    cpu, sp:\t%x\r\n\0", (u32)task->cpu.sp);
    uart_spin_printf("    cpu, lr:\t%x\r\n\0", (u32)task->cpu.lr);
    uart_spin_printf("    cpu, pc:\t%x\r\n\0", (u32)task->cpu.pc);
    uart_spin_printf("    cpu, r0:\t%x\r\n\0", (u32)task->cpu.r0);
    uart_spin_printf("    cpu, cpsr:\t%x\r\n\0", (u32)task->cpu.cpsr);
    uart_spin_printf("  status:\t%d\r\n\0", (u32)task->status);
}

void sched_main()
{
//  rr implement here.

    sched_debug();
    uart_spin_printf("---sched main() start------\r\n\0");
    pcb_t* now_pcb = sched_get_running();
    sched_preempt(now_pcb);
    print_pcb(now_pcb);
//    memcpy(&now_pcb->cpu, context_irq, sizeof(context_cpu_t));

    uart_spin_printf("---sched main() 1------\r\n\0");
    // chances are that new_pcb == now_pcb
    pcb_t* new_pcb = sched_pick();
    sched_allow(new_pcb);
    print_pcb(new_pcb);

    uart_spin_printf("---sched main() 2------\r\n\0");
    context_switch(now_pcb, new_pcb, context_irq);
    uart_spin_printf("---sched main() byre------\r\n\0");
    sched_debug();
}

//void cs_debug()
//{
//    memcpy(&(pcb_running->cpu), context_svc, sizeof(context_cpu_t));
//    print_pcb(pcb_running);
//    pcb_t* temp = new_pcb();
//    u32 pid = temp->td.pid;
//    memcpy(temp, pcb_running, sizeof(pcb_t));
//    temp->td.pid = pid;
//    temp->cpu.pc -= 4;
//    context_switch(pcb_running, temp, context_svc);
//    pcb_running = temp;
//}