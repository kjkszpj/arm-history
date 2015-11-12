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

#include "start_mmu.h"

void prepare_page_table();
void asm_mmu(int pt_base);
void test();
void clear_low();

int main()
{
    uart_spin_puts("Hi from kernel.\r\n");
    uart_spin_puts("Now prepareing MMU\r\n");
    prepare_page_table();
    asm_mmu(PT_BASE);
    test();
    clear_low();
    uart_spin_puts("MMU DONE.");
    return 0;
}

void prepare_page_table()
{
//    1:1 mapping for now
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

    //  TODO domain?
    //  base address(31:20)|00||nG|S|AP[2]|TEX[2:0]|AP[1:0]|0|domain(8765)|00010
    for (i = 0; i < 0x1000; i++)
    {
        mask = (i << 20) | (0 << 17) | (1 << 16) | (0b10 << 10) | 0b00010;
        page_table[i] = mask;
    }
    uart_spin_puts("DO");
    page_table[0x801] = (1 << 20) | (0 << 17) | (1 << 16) | (0b10 << 10) | 0b00010;
    page_table[0x800] = (1 << 20) | (0 << 17) | (1 << 16) | (0b10 << 10) | 0b00010;
    page_table[0x802] = (1 << 20) | (0 << 17) | (1 << 16) | (0b10 << 10) | 0b00010;
}

//extern void asm_mmu(int pt_base);

void asm_mmu(int pt_base)
{
    asm
    (
//        load variable pt_base into r0
        "mov r0, %0\n"
//        pass r0 to xxxx
        "mcr p15, 0, r0, c2, c0, 0\n"
//        activate MMU via SCTLR.M
        "mrc p15, 0, r0, c1, c0, 0\n"
        "orr r0, r0, #0b1\n"
        "mcr p15, 0, r0, c1, c0, 0\n"
//        TODO invalidate TLB, anything else? cache?
        :
        :"r"(pt_base)
        :"r0"
    );
}

void test()
{
    uart_spin_puts((char *)0x80123456);
}

void clear_low()
{
    //TODO clear low memory
}