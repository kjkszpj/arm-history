//
// Created by Peijie You on 15/11/11.
//

#ifndef AIMV6_START_MMU_H
#define AIMV6_START_MMU_H

#include <config.h>

#define KERN_BASE   0x80000000
#define PT0_BASE 0x100000
#define PT1_BASE 0x100000

void asm_mmu(u32 pt0_paddr, u32 pt1_paddr);
void jump_high(u32 high_exec_vaddr);
void mmu_low_main();
void prepare_page_table();

#endif //AIMV6_START_MMU_H