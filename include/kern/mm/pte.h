//
// Created by Peijie You on 15/12/10.
//

/*
 * program that alter page table, please include this file and operate on these struct
 * ---refer to lulu2184
 */

#ifndef AIMV6_PTE_H
#define AIMV6_PTE_H

#include <config.h>
#include <settings.h>

#define P2V(paddr)          (((u32)paddr) + KERNEL_BASE)
#define V2P(vaddr)          (((u32)vaddr) - KERNEL_BASE)

typedef struct
{
    unsigned type       : 2;
    unsigned SBZ1       : 1;
    unsigned NS         : 1;
    unsigned SBZ2       : 1;
    unsigned domain     : 4;
    unsigned empty      : 1;
    unsigned base       : 22;
} ptel1_page_table_t;

typedef struct
{
    unsigned type       : 2;
    unsigned B          : 1;
    unsigned C          : 1;
    unsigned XN         : 1;
    unsigned domain     : 4;
    unsigned zero1      : 1;
    unsigned AP_L       : 2;
    unsigned TEX        : 3;
    unsigned AP_H       : 1;
    unsigned S          : 1;
    unsigned ng         : 1;
    unsigned zero2      : 1;
    unsigned NS         : 1;
    unsigned base       : 12;
} ptel1_section_t;

typedef struct
{
    unsigned type       : 2;
    unsigned memattr    : 2;
    unsigned AP         : 2;
    unsigned TEX        : 3;
    unsigned APX        : 1;
    unsigned S          : 1;
    unsigned ng         : 1;
    unsigned base       : 20;
} ptel2_page_t;

typedef struct
{
    unsigned type       : 2;
    unsigned info       : 30;
} ptel1_empty_t;

typedef struct
{
    unsigned type       : 2;
    unsigned info       : 30;
} ptel2_empty_t;

//  useful function
void invalidate_tlb();
void move_TTBR0(u32 pt0_paddr);
void move_TTBR1(u32 pt1_paddr);

void mmap(u32 *pt_vaddr, u32 start, u32 finish, u32 pattern1, u32 pattern2);
void unmmap(u32 *pt_vaddr, u32 start, u32 finish);
void copy_mem_img(u32* pt_frm, u32* pt_to, u32 start, u32 finish, u32 pattern1, u32 pattern2);

#endif //AIMV6_PTE_H
