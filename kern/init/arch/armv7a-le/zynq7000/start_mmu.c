// ROUTINE:
// 1.	prepare page table mapping(low & high)
// 2.	invalidate TLB & update pt_base_address
// 3.	jump to kernel space(high)
// 4.	clean low address
// 5.	prepare free list

// page table mapping scheme
// 1.	execution flow, now at xxxxx, need low and high
// 2.	stack, now at 511M, need low and high
// 3.	IO related, now at xxxx, need high

#include "start_mmu.h"

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

    //  TODO domain?
    //base address(31:20)|00||nG|S|AP[2]|TEX[2:0]|AP[1:0]|0|domain(8765)|00010
    for (i = 0; i < 0x1000; i++)
    {
        mask = (i << 20) | (0 << 17) | (1 << 16) | (0b10 << 10) | 0b00010;
        page_table[i] = mask;
    }
}

//extern void asm_mmu(int page_table_base_address);

void test()
{
    uart_spin_puts((char *)0x123456);
}

void clear_low()
{
    //TODO clear low memory
}

int start_mmu()
{
    prepare_page_table();
//    asm_mmu(PT_BASE);
    test();
    clear_low();
    return 0;
}