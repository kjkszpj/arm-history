//
// Created by Peijie You on 15/12/3.
//

#ifndef AIMV6_INTERRUPT_INIT_H
#define AIMV6_INTERRUPT_INIT_H

#include <config.h>


#include <drivers/serial/uart.h>
#include <drivers/sd/sd-zynq7000.h>
#include <drivers/misc/dtb-zynq7000.h>

#define ICDICFR_OFFSET 0x00001C00
#define ICDIPTR_OFFSET 0x00001800
#define	ICDSGIR_OFFSET 0x00001F00

int interrupt_init();

extern u32 PERIPHBASE;
#endif //AIMV6_INTERRUPT_INIT_H
