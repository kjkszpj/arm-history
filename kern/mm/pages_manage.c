//
// Created by Peijie You on 15/11/14.
//

//  here to manage PHYSICAL MEMORY.
//  TODO, I want buddy system here instead!(like LINUX?)
//  TODO, define DEBUG?

#include <kern/mm/pages_manage.h>

static free_area_t* farea_head;

u32 init_pspace()
{
    farea_head = NULL;
    return pages_free(0x800000, 0x1ff00000);
}

u32 pages_alloc(u32 need_size)
{
//    Content NOT CLEARED when allocate.
//    ADDITIONAL: align option
//    L2 page table is NOT concerned here.

    free_area_t* last = NULL;
    free_area_t* next;

    if ((need_size & 0xFFF) != 0)
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
        if (pages_split(next, need_size) != 0) return 1;
    }
    if (last != NULL) last->next = next->next;
    if (next == farea_head) farea_head = next->next;
    return V2P(next);
}

u32 pages_free(u32 st, u32 fi)
{
//    st, fi are in PHYSICAL ADDRESS
//    procedure:
//    scan for proper position
//    prepate new FPAGE
//    add to FPAGE list
//    merge

    free_area_t* last = NULL;
    free_area_t* next;
    free_area_t* temp = (free_area_t*)P2V(st);

//    error if necessary
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

u32 pages_split(free_area_t* p, u32 first_size)
{
    if (p == NULL || p->size < first_size || (first_size & 0xFFF) != 0)
    {
        uart_spin_puts("---WRONG:\tin split_pages()\r\n\0");
        return 1;
    }
    if (p->size == first_size) return 0;
    free_area_t* q = (free_area_t*)((u32)p + first_size);
    q->size = p->size - first_size;
    q->next = p->next;

    p->size = first_size;
    p->next = q;
    return 0;
}