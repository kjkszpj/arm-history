//
// Created by Peijie You on 15/11/14.
//

#ifndef AIMV6_INIT_H
#define AIMV6_INIT_H

#include <config.h>
#include <mmu_high.h>
#include <scu.h>
#include <settings.h>

#include <drivers/serial/uart.h>
#include <drivers/sd/sd-zynq7000.h>
#include <drivers/misc/dtb-zynq7000.h>

extern u32 *page_table;

int l1cache_init();

#endif //AIMV6_INIT_H
