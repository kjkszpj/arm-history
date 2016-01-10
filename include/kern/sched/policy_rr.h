//
// Created by Peijie You on 16/1/10.
//

#ifndef AIMV6_SCHED_RR_H
#define AIMV6_SCHED_RR_H

#include <kern/sched/pcb.h>

//  todo, account info, should also update here.

/*
 * ------ other sched policy algorithm should implement the SAME interface, then ok (like FCFS, always choose the one)
 * life cycle for pcb
 * 1.   create in NEW
 * 2.   from NEW to READY
 * 3.   from READY to RUNNING
 * 4.   from RUNNING to WAIT
 * 5.   from RUNNING to READY
 * 6.   from WAIT to READY
 * 7.   from RUNNING to DONE
 * 8.   from ?? to ABORT
 */

/*
 * rr like, but no priority, rr every where.
 * fuck! why priority should take all resource
 */

//  interface below
void sched_new(pcb_t* task);         //  1
void sched_mature(pcb_t* task);      //  2
void sched_allow(pcb_t* task);       //  3
void sched_block(pcb_t* task);       //  4
void sched_preempt(pcb_t* task);     //  5
void sched_wake(pcb_t* task);        //  6
void sched_finish(pcb_t* task);      //  7
void sched_kill(pcb_t* task);        //  8

pcb_t* sched_get_running();
pcb_t* sched_get_bypid(int pid);

//  todo, data structure, just for sched_RR

#endif //AIMV6_SCHED_RR_H
