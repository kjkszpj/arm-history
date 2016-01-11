//
// Created by Peijie You on 16/1/10.
//

#include <kern/init/init.h>
#include <kern/mm/slb.h>
#include <kern/mm/pte.h>
#include <kern/sched/pcb.h>
#include <kern/mm/pages_manage.h>
#include <kern/sched/sched.h>
#include <interrupt.h>

int init_sched()
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
     */

    pcb_t* pcb_now = new_pcb();
    pcb_now->td.ppid = pcb_now->td.pid;
    pcb_now->status = RUNNING;
    pcb_now->ret_val = 0;
    pcb_now->page_table = (pgd_t*)(KERNEL_BASE + PT_OFFSET);
    sched_mature(pcb_now);
    sched_allow(pcb_now);

    pcb_t* pcb_mirror = new_pcb();

    //todo some fo(a)ck thing here, then exec idle?

    uart_spin_printf("------DEBUG------\r\n\0");
    u32 cid;
    asm volatile
    (
        "SVC 1\n"
        "mov %0, r0\n"
        :"=r"(cid)
        :
        :"r0"
    );
    if (cid == 0)
    {
        u32 user_base = 0x003b9ad4;
        asm volatile
        (
            "mov r0, %0\n"
            "SVC 2\n"
            :
            :"r"(user_base)
            :"r0"
        );
    }
    else
    {
        uart_spin_printf("fork succeed, cid= %d\r\n\0", cid);
        uart_spin_printf("I am your father.\r\n\0");
        test_all_interrupt();
    }
    return 0;
}