//
// Created by Peijie You on 15/12/17.
//

#include <kern/sched/pcb.h>
#include <kern/init/init.h>

void switch_mm(pgd_t pt_from, pgd_t pd_to)
{
    /*
     * args are pointer to the pt_base in KERNEL VIRTUAL SPACE
     * should convert to physical address
     */

}

void switch_to()
{

}

void context_switch(pcb_t task_from, pcb_t task_to)
{
    switch_mm(task_from.page_table, task_to.page_table);
    switch_to();
}