//
// Created by Gust on 2017/10/5.
//

#include <memory.h>
#include "d_type.h"
#include "spinlock.h"


//========================     var     =========================


s64 heap_size = 0;

//========================     autoprt     =========================

/**
 * get a autoprt value;
 * @param a var
 * @return var
 */
autoptr *autoptr_get(autoptr *a) {
    a->count++;
    //printf("__ins_r refer : %d\n",a->count);
    return a;
}

/**
 * create a autoptr
 * @param r var
 * @return var
 */
autoptr *autoptr_new(__refer r) {
    autoptr *a = jvm_calloc(sizeof(autoptr));
    a->ref = r;
    return autoptr_get(a);
}

/**
 * set autoptr to null
 * @param aref var
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


//========================     mem alloc     =========================

#ifndef __MEM_LEAK_DETECT

/**
 * 在分配的内存块前面加4个字节用于存放此块内存的长度
 *
 */
//inline void *jvm_calloc(u32 size) {
//    return calloc(size, 1);
//}
//
//inline void *jvm_malloc(u32 size) {
//    return malloc(size);
//}
//
//inline void jvm_free(void *ptr) {
//    free(ptr);
//}
//
//void *jvm_realloc(void *pPtr, u32 size) {
//    return realloc(pPtr, size);
//
//}
//
static spinlock_t mlock = 0;

void *jvm_malloc(u32 size) {
    if (!size)return NULL;
    size += 4;
    void *ptr = malloc(size);
    if (ptr) {
        spin_lock(&mlock);
        heap_size += size;
        spin_unlock(&mlock);
        *(u32 *) (ptr) = size;
        return ptr + 4;
    }
    return NULL;
}

void *jvm_calloc(u32 size) {
    if (!size)return NULL;
    size += 4;
    void *ptr = calloc(size, 1);
    if (ptr) {
        spin_lock(&mlock);
        heap_size += size;
        spin_unlock(&mlock);
        *(u32 *) (ptr) = size;
        return ptr + 4;
    }
    return NULL;
}

void jvm_free(void *ptr) {
    if (ptr) {
        spin_lock(&mlock);
        heap_size -= *(u32 *) (ptr - 4);
        spin_unlock(&mlock);
        free(ptr - 4);
    }
}

void *jvm_realloc(void *pPtr, u32 size) {
    if (!pPtr)return NULL;
    if (!size)return NULL;
    size += 4;
    u32 old_size = *(u32 *) (pPtr - 4);
    void *ptr = realloc(pPtr - 4, size);
    if (ptr) {
        spin_lock(&mlock);
        heap_size += size - old_size ;
        spin_unlock(&mlock);
        *(u32 *) (ptr) = size;
        return ptr + 4;
    }
    return NULL;
}

#endif //__MEM_LEAK_DETECT
