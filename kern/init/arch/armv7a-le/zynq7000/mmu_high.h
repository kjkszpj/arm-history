//
// Created by Peijie You on 15/11/13.
//

#ifndef AIMV6_MMU_HIGH_H
#define AIMV6_MMU_HIGH_H

#include <config.h>
//#include <drivers/serial/uart.h>
//#include <drivers/sd/sd-zynq7000.h>
//#include <drivers/misc/dtb-zynq7000.h>

//#define KERNEL_BASE 0x80000000
//#define PT_OFFSET   0x20000000
//
//#define P2V(paddr)  ((u32)paddr + KERNEL_BASE)
//#define V2P(vaddr)  ((u32)vaddr - KERNEL_BASE)
//#define V2P_PT(vaddr)   ((page_table[vaddr] >> 20) << 20)

extern u32 *page_table;

void mmu_high_main();
#endif //AIMV6_MMU_HIGH_H
