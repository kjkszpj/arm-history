//
// Created by Peijie You on 15/11/16.
//

#ifndef AIMV6_KMEMORY_MANAGE_H
#define AIMV6_KMEMORY_MANAGE_H

void*   kmalloc(u32 size, u32 aligned);
int     kminit();
void*   kmfree(void *p);

#endif //AIMV6_KMEMORY_MANAGE_H
