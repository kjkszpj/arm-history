//
// Created by Peijie You on 15/11/25.
//

#ifndef AIMV6_SLB_H
#define AIMV6_SLB_H

#include <config.h>

//  TODO slab / silly b / si loud b

typedef struct slb_t
{
//    todo cache, no cache now, put it back to pool (slow, rather than cache) when slb_free
    u32 obj_align;
    u32 obj_size;

    struct slb_pool_t* pool_head;
    struct slb_t* next_slb;
}slb_t;

typedef struct slb_obj_t
{
    struct slb_obj_t* next_obj;
}slb_obj_t;

typedef struct slb_pool_t
{
    u32 cnt_inuse;
    u32 pool_size;

    struct slb_obj_t* obj_head;
    struct slb_pool_t* next_pool;
}slb_pool_t;

extern struct slb_t* slb_head;

void* slb_alloc(u32 size);
void* slb_alloc_align(u32 size, u32 align);
int slb_free(void *p, u32 size);
int slb_free_align(void* p, u32 size, u32 align);
int slb_init();

slb_pool_t* pool_alloc(u32 osize, u32 align);
void pool_free(slb_pool_t* p);
slb_pool_t* pool_segm(slb_pool_t* pool, u32 psize, u32 osize, u32 align);

#endif //AIMV6_SLB_H