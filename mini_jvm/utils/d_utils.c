//
// Created by Gust on 2017/10/5.
//

#include <memory.h>
#include "d_type.h"
#include "spinlock.h"


//========================     var     =========================


s64 heap_size = 0;


//========================     mem alloc     =========================


/**
 * 在分配的内存块前面加4个字节用于存放此块内存的长度
 *
 */
#ifndef __DEBUG
inline void *jvm_calloc(u32 size) {
    return calloc(size, 1);
}

inline void *jvm_malloc(u32 size) {
    return malloc(size);
}

inline void jvm_free(void *ptr) {
    free(ptr);
}

void *jvm_realloc(void *pPtr, u32 size) {
    return realloc(pPtr, size);

}
#else
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

#endif