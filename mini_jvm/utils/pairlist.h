//
// Created by gust on 2017/8/25.
//

#ifndef MINI_JVM_PAIRLIST_H
#define MINI_JVM_PAIRLIST_H


#include "d_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Pair {
    union {
        __refer left;
        long leftl;
    };
    union {
        __refer right;
        long rightl;
    };
} Pair;

typedef struct _Pairlist {
    __refer *ptr;
    s32 count;
    s32 _alloced;
} Pairlist;


Pairlist *pairlist_create(s32 len);

void pairlist_destory(Pairlist *list);

s32 pairlist_put(Pairlist *list, __refer left, __refer right);

__refer pairlist_get(Pairlist *list, __refer left);

__refer pairlist_remove(Pairlist *list, __refer left);

Pair pairlist_get_pair(Pairlist *list, s32 index);

long pairlist_removel(Pairlist *list, long left);

long pairlist_getl(Pairlist *list, long left);

s32 pairlist_putl(Pairlist *list, long left, long right);


#ifdef __cplusplus
}
#endif

#endif //MINI_JVM_PAIRLIST_H
