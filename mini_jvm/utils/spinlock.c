//
// Created by Gust on 2017/11/15 0015.
//
#include <sched.h>
#include "d_type.h"
#include "spinlock.h"
//========================     spinlock     =========================


int spin_init(spinlock_t *lock, int pshared) {
    __asm__ __volatile__ ("":: : "memory");
    *lock = 0;
    return 0;
}

int spin_destroy(spinlock_t *lock) {
    return 0;
}

int spin_lock(spinlock_t *lock) {
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

int spin_trylock(spinlock_t *lock) {
    if (__sync_bool_compare_and_swap(lock, 0, 1)) {
        return 0;
    }
    return 1;
}

int spin_unlock(spinlock_t *lock) {
    __asm__ __volatile__ ("":: : "memory");
    *lock = 0;
    return 0;
}

