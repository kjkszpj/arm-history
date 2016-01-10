#include <kern/init/init.h>
#include <kern/mm/slb.h>
#include <kern/mm/pte.h>
#include <kern/sched/pcb.h>
#include <kern/mm/pages_manage.h>
#include <interrupt.h>

#include <drivers/serial/uart.h>
#include <drivers/sd/sd-zynq7000.h>
#include <drivers/misc/dtb-zynq7000.h>

typedef struct pcb_t
{
//    small object first, for locality?
    taskd_t         td;
    proc_status_t   status;
    u32             ret_val;

    pgd_t*          page_table;
    context_cpu_t   cpu;
    sched_info_t    sched_info;
    account_t       account;
} pcb_t;

void init_sched()
{
    init_pcb();
    uart_spin_printf("Init sched.\r\n\0");

    /*
     * what we set for pcb created in init_sched()
     * set:
     * 1.   pid, ppid
     * 2.   status
     * 3.   ret_val? no use
     * 4.   page_table
     *
     * no set:
     * 1.   cpu context
     *
     * not yet set
     * 1.   sched_info
     * 2.   account
     */

    pcb_t* pcb_now = new_pcb();
    pcb_now->td.ppid = pcb_now->td.pid;
    pcb_now->status = RUNNING;
    pcb_now->ret_val = 0;
    pcb_now->page_table = (pgd_t*)(KERNEL_BASE + PT_OFFSET);
    sched_mature(pcb_now);
    sched_allow(pcb_now);

    pcb_t* pcb_mirror = new_pcb();
    //todo some fo(a)ck thing here, then exec?
}