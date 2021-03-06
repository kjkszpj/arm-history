//
// Created by Peijie You on 16/1/10.
//

#ifndef AIMV6_SETTINGS_H
#define AIMV6_SETTINGS_H

#define KERNEL_BASE 0x80000000
#define PT_OFFSET   0x00100000

#define PTE_L1SIZE  0x00004000
#define PTE_L2SIZE  0x00000400
#define PTE_L1ALIGN PTE_L1SIZE
#define PTE_L2ALIGN PTE_L2SIZE

#define PAGE_L1SIZE 0x00100000
#define PAGE_L2SIZE 0x00001000

#define FRAME_SIZE  PAGE_L2SIZE

#define BLOCK_SIZE  512
#define ELF32

#define USER_STACK  0x00800000
#define USER_ENTRY  0x00300000

#define SYS_STACK   0xD0FFFFF0
extern u32 V_SYS_STACK;

#endif //AIMV6_SETTINGS_H
