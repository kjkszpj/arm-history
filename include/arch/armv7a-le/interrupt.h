//
// Created by Peijie You on 15/12/3.
//

#ifndef AIMV6_INTERRUPT_INIT_H
#define AIMV6_INTERRUPT_INIT_H

#include <config.h>
#include <kern/sched/pcb.h>

#include <drivers/serial/uart.h>
#include <drivers/sd/sd-zynq7000.h>
#include <drivers/misc/dtb-zynq7000.h>

#define ICDICFR_OFFSET 0x00001C00
#define ICDIPTR_OFFSET 0x00001800
#define	ICDSGIR_OFFSET 0x00001F00

extern context_cpu_t* context_no;
extern context_cpu_t* context_ndef;
extern context_cpu_t* context_svc;
extern context_cpu_t* context_abort;
extern context_cpu_t* context_irq;
extern context_cpu_t* context_fiq;

int interrupt_init();

int    int_ent_no();
int    int_ent_ndef();
int    int_ent_svc();
int    int_ent_prefetch_abort();
int    int_ent_data_abort();
int    int_ent_irq();
int    int_ent_fiq();

void    int_no();
void    int_ndef();
void    syscall();
void    prefetch_abort();
void    data_abort();
void    irq();
void    fiq();

extern u32 PERIPHBASE;
#endif //AIMV6_INTERRUPT_INIT_H
