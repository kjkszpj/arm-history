// ROUTINE:
// 1.	prepare page table mapping(low & high)
// 2.	invalidate TLB & update pt_base_address
// 3.	jump to kernel space(high)
// 4.	clean low address
// 5.	prepare free list

// page table mapping scheme
// 1.	execution flow, now at 0x200000, need low and high
// 2.	stack, now at 511M, need low and high
// 3.	IO related, now at xxxx, need high
// 4.  page table, high

//  TODO vaddr vs paddr

#include "mmu_low.h"

void prepare_page_table();
void asm_mmu(int pt_base);
void test();
void jump_high();

int mmu_low_main()
{
    uart_spin_puts("Hi from kernel.\r\n");
    uart_spin_puts("Now prepareing MMU\r\n");
    prepare_page_table();
    asm_mmu(PT_BASE);
    test();
    jump_high();
    uart_spin_puts("MMU DONE.");
    return 0;
}

void prepare_page_table()
{
    u32 i;
    u32 mask;
    u32 *page_table = (u32*)PT_BASE;

    //  prepare treasure for debug
    char *treasure = (char*) 0x123456;
    treasure[0] = 'G';
    treasure[1] = 'E';
    treasure[2] = '\r';
    treasure[3] = '\n';
    treasure[4] = 0;

    //  base address(31:20)|00||nG|S|AP[2]|TEX[2:0]|AP[1:0]|0|domain(8765)|00010
    for (i = 0; i < 0x800; i++)
    {
        mask = (i << 20) | (0 << 17) | (1 << 16) | (0b10 << 10) | 0b00010;
        page_table[i] = mask;
        page_table[i + 0x800] = mask;
    }
    //  IO mapping, TODO should reallocate it latter
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
    page_table[0xFFF] = (0xFFF << 20) | (0 << 17) | (1 << 16) | (0b10 << 10) | 0b00010;
    uart_spin_puts("DO");
}

//extern void asm_mmu(int pt_base);

void asm_mmu(int pt_base)
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
//        activate MMU via SCTLR.M
        "mrc p15, 0, r0, c1, c0, 0\n"
        "orr r0, r0, #0b1\n"
        "mcr p15, 0, r0, c1, c0, 0\n"
//        domain all client
        "ldr r0, =0x55555555\n"
        "mcr p15, 0, r0, c3, c0, 0\n"
//        TODO anything else? cache? domain, should be 0b01
        :
        :"r"(pt_base)
        :"r0"
    );
}

void test()
{
    uart_spin_puts((char *)0x80123456);
}

void jump_high()
{
    //  TODO jump by asm
    //  TODO clear stack
}