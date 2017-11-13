//
// Created by gust on 2017/8/25.
//

#include <memory.h>
#include "pairlist.h"

Pairlist *pairlist_create(s32 len) {
    if (len <= 0) {
        len = 4;
    }
    Pairlist *list = (Pairlist *) jvm_calloc(sizeof(Pairlist));//每个位置放两个指针
    if (list) {
        list->ptr = jvm_calloc(sizeof(__refer) * 2 * len);//每个位置放两个指针
        list->_alloced = len;
        list->count = 0;
        return list;
    }
    return NULL;
};

s32 pairlist_putl(Pairlist *list, long left, long right) {
    return pairlist_put(list, (__refer) left, (__refer) right);
}

long pairlist_getl(Pairlist *list, long left) {
    return (long) pairlist_get(list, (__refer) left);
}

long pairlist_removel(Pairlist *list, long left) {
    return (long) pairlist_remove(list, (__refer) left);
}

s32 pairlist_put(Pairlist *list, __refer left, __refer right) {
    if (list->count >= list->_alloced) {//空间不足
        s32 newSize = list->_alloced << 1;
        void *p = jvm_realloc(list->ptr, (newSize << 1) * (sizeof(__refer) << 1));
        list->_alloced = newSize;
        list->ptr = p;
    }
    s32 i;
    for (i = 0; i < list->count; i++) {
        if ((list->ptr)[(i << 1)] == left) {
            (list->ptr)[(i << 1) + 1] = right;
            return 0;
        }
    }
    (list->ptr)[(list->count << 1)] = left;
    (list->ptr)[(list->count << 1) + 1] = right;
    list->count++;
    return 0;
};

__refer pairlist_get(Pairlist *list, __refer left) {
    s32 i;
    for (i = 0; i < list->count; i++) {
        if ((list->ptr)[(i << 1)] == left) {
            return (list->ptr)[(i << 1) + 1];
        }
    }
    return NULL;
};

Pair pairlist_get_pair(Pairlist *list, s32 index) {
    Pair p;
    p.left = list->ptr[index << 1];
    p.right = list->ptr[(index << 1) + 1];
    return p;
};

__refer pairlist_remove(Pairlist *list, __refer left) {
    s32 i;
    __refer right = NULL;
    for (i = 0; i < list->count; i++) {
        if ((list->ptr)[(i << 1)] == left) {
            right = (list->ptr)[(i << 1) + 1];
            memmove(&((list->ptr)[(i << 1)]), &((list->ptr)[((i + 1) << 1)]), (list->count - 1 - i) * sizeof(__refer));
            list->count--;
            break;
        }
    }
    return right;
};

void pairlist_destory(Pairlist *list) {
    if (list) {
        jvm_free(list->ptr);
        jvm_free(list);
    }
};
