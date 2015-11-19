//
// Created by Peijie You on 15/11/19.
//

#ifndef AIMV6_ARCH_SCU_H
#define AIMV6_ARCH_SCU_H

#include <config.h>

#define SCU_OFFSET          0x0000
#define SCU_CONTROL_OFFSET  0x00
#define SCU_CONFIG_OFFSET   0x04
#deinfe SCU_FILT_ST_OFFSET  0x40
#define SCU_FILT_ED_OFFSET  0x44
#define SCU_SAC_OFFSET      0x50

extern u32 PERIPHBASE;

u32 get_PERIPHBASE();
int scu_init();

extern void puthex(u32 num);

#endif //AIMV6_ARCH_SCU_H
