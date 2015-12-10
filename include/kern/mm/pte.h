//
// Created by Peijie You on 15/12/10.
//

#ifndef AIMV6_PTE_H
#define AIMV6_PTE_H

// program that alter page table, please include this file and operate on these struct
// refer to lulu2184
typedef struct
{
    unsigned type       : 2;
    unsigned SBZ1       : 1;
    unsigned NS         : 1;
    unsigned SBZ2       : 1;
    unsigned domain     : 4;
    unsigned empty      : 1;
    unsigned pbase      : 22;
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

void invalidate_tlb()
{
    asm volatile
    (
        "mov r0, #0\n"
        "mcr p15, 0, r0, c8, c7, 0\n"
//      just in case, i think here dsb, isb is not needed
        "dsb\n"
        "isb\n"
        :
        :
        :"r0"
    );
}

#endif //AIMV6_PTE_H
