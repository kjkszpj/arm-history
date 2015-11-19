//
// Created by Peijie You on 15/11/14.
//

#ifndef AIMV6_INIT_H
#define AIMV6_INIT_H

#include <scu.h>
#include <config.h>
#include <mmu_high.h>

#include <drivers/serial/uart.h>
#include <drivers/sd/sd-zynq7000.h>
#include <drivers/misc/dtb-zynq7000.h>

#define KERNEL_BASE 0x80000000
#define PT_OFFSET   0x00100000

extern u32 *page_table;

#endif //AIMV6_INIT_H
