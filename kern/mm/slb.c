//
// Created by Peijie You on 15/11/25.
//

//  TODO LOCK issue
//  TODO link in free object, is this design secure?
//  for object only? use kmalloc for nurmal allocation

#include <kern/mm/pages_manage.h>
#include <kern/mm/slb.h>

static slb_t* slb_head;

inline static int min(int a, int b) {return a < b ? a : b;}
inline static int max(int a, int b) {return a > b ? a : b;}
static void pool_free(slb_pool_t* p);
static slb_pool_t* pool_alloc(u32 osize, u32 align);
static slb_pool_t* pool_segm(slb_pool_t* pool, u32 psize, u32 osize, u32 align);

void* slb_alloc(u32 size) {return slb_alloc_align(size, 0);}
int slb_free(void *p, u32 size) {return slb_free_align(p, size, 0);}

void* slb_alloc_align(u32 osize, u32 align)
{
    slb_t* iter_slb;

    if (align == 0) align = min(osize, 4);
    osize = max(osize, sizeof(slb_obj_t));
    for (iter_slb = slb_head; iter_slb != NULL; iter_slb = iter_slb->next_slb)
        if (iter_slb->obj_align == align && iter_slb->obj_size == osize)
        {
            slb_pool_t* iter_pool = iter_slb->pool_head;
            while (iter_pool != NULL && iter_pool->obj_head == NULL) iter_pool = iter_pool->next_pool;
            if (iter_pool == NULL)
            {
                iter_pool = pool_alloc(osize, align);
                if (iter_pool == NULL) return NULL;
                iter_pool->next_pool = iter_slb->pool_head;
                iter_slb->pool_head = iter_pool;
            }
            void* result = (void*)iter_pool->obj_head;
            iter_pool->obj_head = iter_pool->obj_head->next_obj;
            iter_pool->cnt_inuse++;
            return result;
        }

    if (iter_slb == NULL)
    {
        iter_slb = (slb_t*)slb_alloc(sizeof(slb_t));
        if (iter_slb == NULL)
        {
            // TODO, kernel panic, as this situation will never happen
            return NULL;
        }
        iter_slb->obj_align = align;
        iter_slb->obj_size = osize;
        iter_slb->pool_head = pool_alloc(osize, align);
        if (iter_slb->pool_head == NULL)
        {
            slb_free((void*)iter_slb, sizeof(slb_t));
            return NULL;
        }
        iter_slb->next_slb = slb_head;
        slb_head = iter_slb;
        return slb_alloc_align(osize, align);
        // TODO si xun huan?
    }
    // should never reach here
    return NULL;
}

int slb_free_align(void* p, u32 size, u32 align)
{
    slb_t* iter_slb;
    slb_pool_t* iter_pool;

    if (align == 0 && size <= 4) align = size;
    for (iter_slb = slb_head; iter_slb != NULL; iter_slb = iter_slb->next_slb)
        if (iter_slb->obj_align == align && iter_slb->obj_size == size) break;
    if (iter_slb == NULL)
    {
        // should never happen, panic?
        return 1;
    }
    for (iter_pool = iter_slb->pool_head; iter_pool != NULL; iter_pool = iter_pool->next_pool)
        if ((u32)iter_pool <= (u32)p && (u32)iter_pool + iter_pool->pool_size > (u32)p)
        {
            slb_obj_t* new_obj = (slb_obj_t*)p;
            new_obj->next_obj = iter_pool->obj_head;
            iter_pool->obj_head = new_obj;
            iter_pool->cnt_inuse--;
               return 0;
        }
    return 1;
}

int slb_init()
{
    slb_head = NULL;

    slb_pool_t* init_pool = pool_alloc(4 << 2, 0);
    if (init_pool == NULL) return 1;

    slb_head = (slb_t*)init_pool->obj_head;
    init_pool->obj_head = init_pool->obj_head->next_obj;
    // just in case we change struct slb_t and slb_obj_t latter
    slb_head->obj_align = min(4, sizeof(slb_t));
    slb_head->obj_size = max(sizeof(slb_t), sizeof(slb_obj_t));
    slb_head->pool_head = init_pool;
    slb_head->next_slb = NULL;
    return 0;
}

static slb_pool_t* pool_alloc(u32 osize, u32 align)
{
    // still need a container outside this function
    // TODO, what if pool is too small

    u32 psize = osize * 10 + sizeof(slb_pool_t) +10;
    slb_pool_t* pool;

    if ((psize & 0xFFF) != 0) psize = ((psize >> 12) + 1) << 12;
    while ((pool = (slb_pool_t*)P2V(pages_alloc(psize))) == NULL && psize > 0) psize -= 0x1000;
    if (pool_segm(pool, psize, osize, align) == NULL)
    {
        pages_free((u32)pool, (u32)pool + psize);
        pool = NULL;
    }
    return pool;
}

static void pool_free(slb_pool_t* p)
{
    pages_free(V2P(p), V2P(p) + (p->pool_size));
}

static slb_pool_t* pool_segm(slb_pool_t* pool, u32 psize, u32 osize, u32 align)
{
    if (pool == NULL) return NULL;

    pool->cnt_inuse = 0;
    pool->pool_size = psize;
    pool->obj_head = NULL;
    pool->next_pool = NULL;

    u32 i = (u32)pool + (4 << 2);
    if (align != 0 && (i & (align - 1)) != 0) i = i + align - ((align - 1) & i);
    if (i >= (u32)pool + psize) return NULL;
    pool->obj_head = NULL;

    for (; i + osize <= (u32)pool + psize; i += MAX(osize, align))
        if (align == 0 || (i & (align - 1)) == 0)
        {
            slb_obj_t* new_obj = (slb_obj_t*)i;
            new_obj->next_obj = pool->obj_head;
            pool->obj_head = new_obj;
        }
    return pool;
}