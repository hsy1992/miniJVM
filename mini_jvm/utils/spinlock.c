//
// Created by Gust on 2017/11/15 0015.
//
//#include <sched.h>
#include "d_type.h"
#include "tinycthread.h"
#include "spinlock.h"
//========================     spinlock     =========================


#if defined( __JVM_OS_VS__ )
int __sync_bool_compare_and_swap(int *lock,int desired, int expected) {
    return InterlockedCompareExchange(lock, expected, desired);
}
#endif

int spin_init(spinlock_t volatile *lock, int pshared) {
//    __asm__ __volatile__ ("":: : "memory");
    *lock = 0;
    return 0;
}

int spin_destroy(spinlock_t *lock) {
    return 0;
}


inline int spin_lock_count(spinlock_t *lock, int count) {
    while (1) {
        int i;
        for (i = 0; i < count; i++) {
            if (__sync_bool_compare_and_swap(lock, 0, 1)) {
                return 0;
            }
        }
        thrd_yield();
    }
}

int spin_lock(spinlock_t *lock) {
    return spin_lock_count(lock, 100);
}

int spin_trylock(spinlock_t *lock) {
    if (__sync_bool_compare_and_swap(lock, 0, 1)) {
        return 0;
    }
    return 1;
}

int spin_unlock(spinlock_t volatile *lock) {
    //__asm__ __volatile__ ("":: : "memory");
    *lock = 0;
    return 0;
}

