//  action in "real mode"
//  1.  prepare basic page table
//  2.  start mmu
//  3.  prepare stack & jump
//  TODO serious jump

#include <mmu_low.h>

void mmu_low_main()
{
    prepare_page_table();
    asm_mmu(PT_BASE);
    jump_high(0x80000000 + 0x200000);
}

void prepare_page_table()
{
    u32 i;
    u32 mask;
    u32 *page_table = (u32*)PT_BASE;

//    page mapping for now:
//    1.    PA = VA         for (VA < 2G, below kernel base), should be clear after entering kernel space
//    2.    PA = VA - 2G    for (VA >= 2G, above kernel base)
//    3.    PA = VA         for IO related
//    5.    PA = 511M-512M  for 0xfff00000, new "stack" here

    //  base address(31:20)|00||nG|S|AP[2]|TEX[2:0]|AP[1:0]|0|domain(8765)|00010
    for (i = 0; i < 0x800; i++)
    {
        mask = (i << 20) | (0 << 17) | (1 << 16) | (0b10 << 10) | 0b00010;
        page_table[i] = mask;
        page_table[i + 0x800] = mask;
    }
    //  temp IO mapping, TODO should reallocate it latter
    page_table[0xE00] = (0xE00 << 20) | (0 << 17) | (1 << 16) | (0b10 << 10) | 0b00010;
    page_table[0xE01] = (0xE01 << 20) | (0 << 17) | (1 << 16) | (0b10 << 10) | 0b00010;
    page_table[0xE10] = (0xE10 << 20) | (0 << 17) | (1 << 16) | (0b10 << 10) | 0b00010;
    page_table[0xE20] = (0xE20 << 20) | (0 << 17) | (1 << 16) | (0b10 << 10) | 0b00010;
    page_table[0xE40] = (0xE40 << 20) | (0 << 17) | (1 << 16) | (0b10 << 10) | 0b00010;
    page_table[0xF80] = (0xF80 << 20) | (0 << 17) | (1 << 16) | (0b10 << 10) | 0b00010;
    page_table[0xF88] = (0xF88 << 20) | (0 << 17) | (1 << 16) | (0b10 << 10) | 0b00010;
    page_table[0xF89] = (0xF89 << 20) | (0 << 17) | (1 << 16) | (0b10 << 10) | 0b00010;
    page_table[0xF8F] = (0xF8F << 20) | (0 << 17) | (1 << 16) | (0b10 << 10) | 0b00010;
    page_table[0xFC0] = (0xFC0 << 20) | (0 << 17) | (1 << 16) | (0b10 << 10) | 0b00010;
//    stack here
    page_table[0xFFF] = (0x1FF << 20) | (0 << 17) | (1 << 16) | (0b10 << 10) | 0b00010;
}

void asm_mmu(u32 pt_paddr)
{
    asm volatile
    (
//        load variable pt_base into r0
        "mov r0, %0\n"
//        pass r0 to xxxx
        "mcr p15, 0, r0, c2, c0, 0\n"
//        invalidate TLB
        "mov r0, #0\n"
        "mcr p15, 0, r0, c8, c7, 0\n"
//        domain all client
        "ldr r0, =0x55555555\n"
        "mcr p15, 0, r0, c3, c0, 0\n"
        "dsb\n"
        "isb\n"
//        activate MMU via SCTLR.M
        "mrc p15, 0, r0, c1, c0, 0\n"
        "orr r0, r0, #0b1\n"
        "mcr p15, 0, r0, c1, c0, 0\n"
        :
        :"r"(pt_paddr)
        :"r0"
    );
}

void jump_high(u32 high_exec_vaddr)
{
    asm volatile
    (
//      TODO, decide what serious virtual address should sp be?
        "mov r0, %0\n"
        "blx r0\n"
        :
        :"r"(high_exec_vaddr)
        :"r0"
    );
}