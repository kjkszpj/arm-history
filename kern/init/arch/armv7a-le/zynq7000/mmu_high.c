//  2G              RAM
//  2G+512M         page table
//  2G+512M+16k     program
//  4G              stack
//  TODO, reallocate memory for device;

#include <mmu_high.h>

u32 mmu_high_main()
{
//    page mapping to clear:
//    1.    PA = VA         for (VA < 2G, below kernel base)
//    2.    PA = VA - 2G    for (VA >= 2G, above kernel base)

//    page rest:
//    3.    PA = VA         for IO related
//    4.    PA = 1M-2M      for 0xa0100000, rest program(mmu_high) here
//    5.    PA = 511M-512M  for 0xfff00000, "stack" here

    u32 i;

    for (i = 0; i < (KERNEL_BASE >> 20); i++) page_table[i] = 0;
    // for (i = 0xA00; i < 0xE00; i++) page_table[i] = 0;

    asm volatile
    (
        "mov r0, #0\n"
        "mcr p15, 0, r0, c8, c7, 0\n"
        "dsb\n"
        "isb\n"
        :
        :
        :"r0"
    );
    return 0;
}