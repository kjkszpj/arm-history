//
// Created by Peijie You on 15/11/14.
//

//  TODO, define DEBUG?

#include "pages_manage.h"

u32 init_pspace()
{
    farea_head = NULL;
    pages_free(0x300000, 0x1FF00000);
}

u32 alloc_pages(u32 need_size)
{
//    TODO when to use L2 page table?
    FREE_AREA* last;
    FREE_AREA* next;

    u32 i;
    u8* content;

    if ((size & 0xFFF) != 0)
    {
        uart_spin_puts("---WRONG:\tin alloc_pages(), size not aligned.\r\n\0");
//        TODO why return 0?
        return 0;
    }
    for (next = farea_head; next != NULL && need_size > next->size; last = next, next = next->next);
//    not founded, TODO maybe solve by swapping
    if (next == NULL || next->size < need_size)
    {
        uart_spin_puts("---WRONG:\tin alloc_pages(), no more memory space.\r\n\0");
//        TODO why return 0?
        return 0;
    }

    if (need_size < next->size)
    {
        if (split_pages(next, need_size)) return 1;
    }
    if (last != NULL) last->next = next->next;
    if (next == farea_head) farea_head = next->next;

//    use memset instead?
    content = (u8*)next;
    for (i = 0; i < need_size; i++) content[i] = 0;
    return V2P(next);
//    TODO clear content;
}

u32 free_pages(u32 st, u32 fi)
{
//    procedure:
//    scan for proper position
//    prepate new FPAGE
//    add to FPAGE list
//    merge

    FREE_AREA *last = NULL;
    FREE_AREA *next;
    FREE_AREA *temp = (FREE_AREA*)P2V(st);

    if ((st & 0xFFF) != 0 || (fi & 0xFFF) != 0)
    {
        uart_spin_puts("---WRONG:\tin free_pages(), st & fi not aligned\r\n\0");
        return 1;
    }
    for (next = farea_head; next != NULL && V2P(next) < st; last = next, next = next->next);

    temp->size = fi - st;
    temp->next = next;

    if (last == NULL) farea_head = temp; else last->next = temp;

    if (last != NULL && P2V(last) + last->size == P2V(temp))
    {
        last->next = temp->next;
        last->size += temp->size;
        temp = last;
    }
    if (next != NULL && P2V(temp) + temp->size == P2V(next))
    {
        temp->next = next->next;
        temp->size += next->size;
    }
    return 0;
}

u32 split_pages(FREE_AREA* p, u32 first_size)
{
    if (p == NULL || p->size <= first_size || (first_size & 0xFFF) != 0)
    {
        uart_spin_puts("---WRONG:\tin split_pages()\r\n\0");
        return 1;
    }
    FREE_AREA* q = (u32)p + first_size;
    q->size = p->size - first_size;
    q->next = p->next;

    p->size = first_size;
    p->next = q;
    return 0;
}