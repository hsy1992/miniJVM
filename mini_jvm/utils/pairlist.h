//
// Created by gust on 2017/8/25.
//

#ifndef MINI_JVM_PAIRLIST_H
#define MINI_JVM_PAIRLIST_H


#include "../jvm/global.h"

typedef struct _Pairlist {
    __refer *ptr;
    s32 count;
    s32 _alloced;
} Pairlist;


Pairlist *pairlist_create(s32 len);

void pairlist_destory(Pairlist *list);

s32 pairlist_put(Pairlist *list, __refer left, __refer right);

__refer pairlist_get(Pairlist *list, __refer left);

#endif //MINI_JVM_PAIRLIST_H
