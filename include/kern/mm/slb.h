//
// Created by Peijie You on 15/11/25.
//

#ifndef AIMV6_SLB_H
#define AIMV6_SLB_H

#include <config.h>

//  TODO slab / silly b / si loud b

struct slb_cache_t
{
    slb_cache_t* next;
};

struct slb_pool_t
{
    u32 cnt_used;

    u32 unit_align;
    u32 unit_size;

    u32 pool_base;
    u32 pool_size;

    slb_pool_t* next_pool;
    slb_cache_t* first_cache;
};

#endif //AIMV6_SLB_H
