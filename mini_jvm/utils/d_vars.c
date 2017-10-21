//
// Created by Gust on 2017/10/5.
//

#include <memory.h>
#include "d_type.h"

s64 MAX_HEAP_SIZE = 20 * 1024 * 1024;
s64 heap_size = 0; //当前已经分配的内存总数

/**
 * get a autoprt value;
 * @param a
 * @return
 */
autoptr *autoptr_get(autoptr *a) {
    a->count++;
    //printf("__ins_r refer : %d\n",a->count);
    return a;
}

/**
 * create a autoptr
 * @param r
 * @return
 */
autoptr *autoptr_new(__refer r) {
    autoptr *a = jvm_alloc(sizeof(autoptr));
    a->ref = r;
    return autoptr_get(a);
}

/**
 * set autoptr to null
 * @param aref
 */
void autoptr_NULL(autoptr **aref) {
    if (!aref)
        return;
    autoptr *a = *aref;
    a->count--;
    //printf("__ins_r refer : %d\n",a->count);
    if (!a->count) {
        jvm_free(a->ref);
        jvm_free(a);
    }
    *aref = NULL;
}


#ifndef __MEM_LEAK_DETECT

/**
 * 在分配的内存块前面加4个字节用于存放此块内存的长度
 *
 */
void *jvm_alloc(u32 size) {

    if (!size)return NULL;
    size += 4;
    void *ptr = calloc(size, 1);
    if (ptr) {
        heap_size += size;
        memcpy(ptr, &size, sizeof(s32));
        return ptr + 4;
    }
    return NULL;
}

s32 jvm_free(void *ptr) {
    if (ptr) {
        s32 size;
        memcpy(&size, ptr - 4, sizeof(s32));
        heap_size -= size;
        free(ptr - 4);
        return size;
    }
    return 0;
}

void *jvm_realloc(void *pPtr, u32 size) {
    if (!pPtr)return NULL;
    if (!size)return NULL;
    size += 4;
    s32 v;
    memcpy(&v, pPtr - 4, sizeof(s32));
    heap_size -= v;
    void *ptr = realloc(pPtr - 4, size);
    if (ptr) {
        heap_size += size;
        memcpy(ptr, &size, sizeof(s32));
        return ptr + 4;
    }
    return NULL;
}

#endif //__MEM_LEAK_DETECT
