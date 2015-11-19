//
// Created by Peijie You on 15/11/16.
//

#ifndef AIMV6_KMEMORY_MANAGE_H
#define AIMV6_KMEMORY_MANAGE_H

#include <config.h>

void *kmalloc(u32 size, u32 aligned);
void kmfree(void *p);
void kminit();  // if necessary.

#endif //AIMV6_KMEMORY_MANAGE_H
