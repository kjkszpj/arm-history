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

void unmmap(u32 *pt_vaddr, u32 start, u32 finish)
{
    int i, j, temp;
    ptel1_page_table_t* l1pte;
    ptel2_page_t* l2page;

    for (i = finish / PAGE_L1SIZE; i >= start / PAGE_L1SIZE; i--)
    {
        l1pte = (ptel1_page_table_t*)(pt_vaddr + i);
        if (l1pte->type == 0b00) continue;
        l2page = (ptel2_page_t*)P2V(l1pte->base);
        for (j = 0; j < PTE_L2SIZE; j++)
        {
            temp = i * PAGE_L1SIZE + j * PAGE_L2SIZE;
            if (temp < start || temp >= finish) continue;
            if (l2page[j].type == 0b10 || l2page[j].type == 0b11)
                pages_free(l2page[j].base * PAGE_L2SIZE, (l2page[j].base + 1) * PAGE_L2SIZE);
        }
        //  free l2 page table structure.
        slb_free_align(l2page, PTE_L2SIZE, PTE_L2ALIGN);
    }
}

void mmap(u32 *pt_vaddr, u32 start, u32 finish, u32 pattern1, u32 pattern2)
{
    int i, j, temp;
    u32* k;
    ptel1_page_table_t* l1pte;
    ptel2_page_t* l2page;

    for (i = start / PAGE_L1SIZE; i <= (finish - 1) / PAGE_L1SIZE; i++)
    {
        l1pte = (ptel1_page_table_t*)(pt_vaddr + i);
        if (l1pte->type == 0b00)
        {
            l2page = (ptel2_page_t*)slb_alloc_align(PTE_L2SIZE, PTE_L2ALIGN);
            //  memset for user
            memset(l2page, 0, PTE_L2SIZE);
            pt_vaddr[i] = V2P((u32)l2page) | pattern1;
        }
        l2page = (ptel2_page_t*)P2V(l1pte->base);
        for (j = 0; j < PTE_L2SIZE; j++)
        {
            temp = i * PTE_L1SIZE + j * PTE_L2SIZE;
            if (temp < start || temp >= finish) continue;
            if (l2page[j].type == 0)
            {
                temp = pages_alloc(PAGE_L2SIZE);
                k = (u32*)(l2page + j);
                *k = temp | pattern2;
            }
        }
    }
}