//
// Created by Peijie You on 16/1/10.
//

//  todo, WARNING, no secure using pte struct
//  todo, not so sure about these free thing!!!
#include <config.h>
#include <settings.h>
#include <kern/mm/pte.h>
#include <kern/mm/slb.h>
#include <kern/mm/pages_manage.h>
#include <string.h>
//  todo for debug
#include <kern/init/init.h>

//  use l2 page(4K in size) in user space
void unmmap(u32 *pt_vaddr, u32 start, u32 finish)
{
    uart_spin_printf("In unmmap()\r\n\0");
    int i, j;

    u32 temp;
    ptel1_page_table_t* l1pte;
    ptel2_page_t* l2page;

    for (i = (finish - 1) / PAGE_L1SIZE; i >= (int)(start / PAGE_L1SIZE); i--)
    {
        l1pte = (ptel1_page_table_t*)(pt_vaddr + i);
        if (l1pte->type == 0b00) continue;
        l2page = (ptel2_page_t*)P2V(l1pte->base);
        for (j = 0; j < PTE_L2SIZE >> 2; j++)
        {
            temp = i * PAGE_L1SIZE + j * PAGE_L2SIZE;
            if (temp < start || temp >= finish) continue;
            if (l2page[j].type == 0b10 || l2page[j].type == 0b11)
                pages_free(l2page[j].base * PAGE_L2SIZE, (l2page[j].base + 1) * PAGE_L2SIZE);
        }
        //  free l2 page table structure.
        slb_free_align(l2page, PTE_L2SIZE, PTE_L2ALIGN);
    }
    invalidate_tlb();
    uart_spin_printf("bye.\r\n\0");
}

void mmap(u32 *pt_vaddr, u32 start, u32 finish, u32 pattern1, u32 pattern2)
{
    uart_spin_printf("In mmap()\r\n\0");
    int i, j;
    u32 temp;
    u32* k;
    ptel1_page_table_t* l1pte;
    ptel2_page_t* l2page;

    uart_spin_printf("start: %x, finish %x\r\n\0", start, finish);
    for (i = start / PAGE_L1SIZE; i <= (int)((finish - 1) / PAGE_L1SIZE); i++)
    {
        l1pte = (ptel1_page_table_t*)(pt_vaddr + i);
        if (l1pte->type == 0b00)
        {
//            uart_spin_printf("new pte for %x\r\n\0", i);
            temp = (u32)slb_alloc_align(PTE_L2SIZE, PTE_L2ALIGN);
//            temp = (u32)P2V(pages_alloc(PAGE_L2SIZE));
            //  memset for user
            memset((void*)temp, 0, PTE_L2SIZE);
            pt_vaddr[i] = V2P(temp) | pattern1;
//            uart_spin_printf("there %x\r\n\0", temp);
//            uart_spin_printf("here %x\r\n***\r\n\0", pt_vaddr[i]);
            invalidate_tlb();
        }
        l1pte = (ptel1_page_table_t*)(pt_vaddr + i);
        l2page = (ptel2_page_t*)P2V(l1pte->base * PTE_L2SIZE);
//        uart_spin_printf("---%x\r\n\0", *(u32*)l2page);
        for (j = 0; j < (int)PTE_L2SIZE >> 2; j++)
        {
            temp = i * PAGE_L1SIZE + j * PAGE_L2SIZE;
            if (temp < start || temp >= finish) continue;
            if (l2page[j].type == 0)
            {
//                uart_spin_printf("new page for %x\r\n\0", temp);
                temp = (u32)pages_alloc(PAGE_L2SIZE);
                memset((void*)(P2V(temp)), 0, PAGE_L2SIZE);
//                uart_spin_printf("at %x\r\n\0", temp);
                k = (u32*)(l2page + j);
                *k = temp | pattern2;
                invalidate_tlb();
            }
            else
            {
//                uart_spin_printf("!!!!!!!!!!!!WRONG\r\n\0");
            }
//            uart_spin_printf("----\t%x\r\n\0", *(u32*)(l2page +j));
        }
    }
    invalidate_tlb();
    uart_spin_printf("bye.\r\n\0");
}

void copy_mem_img(u32* pt_frm, u32* pt_to, u32 start, u32 finish, u32 pattern1, u32 pattern2)
{
    uart_spin_printf("In copy_mem_img().\r\n\0");
    int i, j;

    u32 temp;
    u32 *k;
    ptel1_page_table_t *l1pte_frm, *l1pte_to;
    ptel2_page_t *l2page_frm, *l2page_to;

    for (i = start / PAGE_L1SIZE; i <= (int)((finish - 1) / PAGE_L1SIZE); i++)
    {
        l1pte_frm = (ptel1_page_table_t*)(pt_frm + i);
        l1pte_to = (ptel1_page_table_t*)(pt_to + i);
//        uart_spin_printf(">>>%x\r\n\0", *(u32*)l1pte_to);
        if (l1pte_frm->type == 0b00 || l1pte_frm->type == 0b11) continue;
        if (l1pte_to->type == 0b00)
        {
//            uart_spin_printf("new pte for %x\r\n\0", i);
            temp = (u32)slb_alloc_align(PTE_L2SIZE, PTE_L2ALIGN);
            memset((void*)temp, 0, PTE_L2SIZE);
            pt_to[i] = V2P(temp) | pattern1;
//            uart_spin_printf("  there %x\r\n\0", temp);
//            uart_spin_printf("  here %x\r\n***\r\n\0", pt_to[i]);
        }
        l1pte_to = (ptel1_page_table_t*)(pt_to + i);
        l2page_frm = (ptel2_page_t*)P2V(l1pte_frm->base);
        l2page_to = (ptel2_page_t*)P2V(l1pte_to->base * PTE_L2SIZE);
//        uart_spin_printf("===%x\r\n\0", *(u32*)l1pte_frm);
        if (l1pte_frm->type == 0b01)
            for (j = 0; j < (int)PTE_L2SIZE >> 2; j++)
            {
                temp = i * PAGE_L1SIZE + j * PAGE_L2SIZE;
                if (temp < start || temp >= finish) continue;
                if (l2page_frm[j].type == 0b10 || l2page_frm[j].type == 0b11)
                {
                    if (l2page_to[j].type == 0)
                    {
                        temp = (u32) pages_alloc(PAGE_L2SIZE);
                        memset((void *) (P2V(temp)), 0, PAGE_L2SIZE);
                        k = (u32 * )(l2page_to + j);
                        *k = temp | pattern2;
                    }
                    memcpy((void*)(l2page_to[j].base * PAGE_L2SIZE), (void*)(l2page_frm[j].base * PAGE_L2SIZE), PAGE_L2SIZE);
                }
            }
        else
        {
            u32 section_base = pt_frm[i] & 0xFFF00000;
//            uart_spin_printf("section base: %d\r\n\0", section_base);
            for (j = 0; j < (int) PTE_L2SIZE >> 2; j++)
            {
                temp = i * PAGE_L1SIZE + j * PAGE_L2SIZE;
                if (temp < start || temp >= finish) continue;
//                uart_spin_printf("---------%x\r\n\0", l2page_to[j].type);
                if (l2page_to[j].type == 0)
                {
//                    uart_spin_printf("new l2 for %x\r\n\0", j);
                    temp = (u32)pages_alloc(PAGE_L2SIZE);
                    memset((void *) (P2V(temp)), 0, PAGE_L2SIZE);
                    k = (u32 * )(l2page_to + j);
                    *k = temp | pattern2;
                }
//                uart_spin_printf("---------%d\r\n\0", *(u32*)(l2page_to + j));
                memcpy((void*)P2V(l2page_to[j].base * PAGE_L2SIZE), (void*)P2V(section_base + j * PAGE_L2SIZE), PAGE_L2SIZE);
            }
        }
    }
    invalidate_tlb();
    uart_spin_printf("bye.\r\n\0");
}