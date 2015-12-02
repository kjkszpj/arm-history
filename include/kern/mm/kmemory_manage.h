//
// Created by Peijie You on 15/11/16.
//

#ifndef AIMV6_KMEMORY_MANAGE_H
#define AIMV6_KMEMORY_MANAGE_H

#include <config.h>
#include <kern/mm/slb.h>

void *kmalloc(u32 size);
int kmfree(void *p);

#endif //AIMV6_KMEMORY_MANAGE_H