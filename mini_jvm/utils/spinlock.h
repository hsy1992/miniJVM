//
// Created by Gust on 2017/11/15 0015.
//


#ifndef MINI_JVM_SPINLOCK_H
#define MINI_JVM_SPINLOCK_H

//======================= spinlock =============================



typedef int spinlock_t;

int spin_init(volatile spinlock_t *lock, int pshared);

int spin_destroy(spinlock_t *lock);

int spin_lock(volatile spinlock_t *lock);

int spin_lock_count(volatile spinlock_t *lock, int count);

int spin_trylock(volatile spinlock_t *lock);

int spin_unlock(volatile spinlock_t *lock);


#endif //MINI_JVM_SPINLOCK_H
