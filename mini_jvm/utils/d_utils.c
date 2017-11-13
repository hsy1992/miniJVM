//
// Created by Gust on 2017/10/5.
//

#include <memory.h>
#include <stdio.h>
#include "sched.h"
#include "d_type.h"
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

//========================     spinlock     =========================
#if __JVM_OS_MAC__

int pthread_spin_init(pthread_spinlock_t *lock, int pshared) {
    __asm__ __volatile__ ("":: : "memory");
    *lock = 0;
    return 0;
}

int pthread_spin_destroy(pthread_spinlock_t *lock) {
    return 0;
}

int pthread_spin_lock(pthread_spinlock_t *lock) {
    while (1) {
        int i;
        for (i = 0; i < 10000; i++) {
            if (__sync_bool_compare_and_swap(lock, 0, 1)) {
                return 0;
            }
        }
        //printf(" %d\n",i);
        sched_yield();
    }
}

int pthread_spin_trylock(pthread_spinlock_t *lock) {
    if (__sync_bool_compare_and_swap(lock, 0, 1)) {
        return 0;
    }
    return 1;
}

int pthread_spin_unlock(pthread_spinlock_t *lock) {
    __asm__ __volatile__ ("":: : "memory");
    *lock = 0;
    return 0;
}

#endif
//========================     mem alloc     =========================

#ifndef __MEM_LEAK_DETECT

/**
 * 在分配的内存块前面加4个字节用于存放此块内存的长度
 *
 */
void *jvm_calloc(u32 size) {
    return calloc(size, 1);
}

void *jvm_malloc(u32 size) {
    return malloc(size);
}

s32 jvm_free(void *ptr) {
    free(ptr);
    return 0;
}

void *jvm_realloc(void *pPtr, u32 size) {
    return realloc(pPtr, size);

}

#endif //__MEM_LEAK_DETECT
