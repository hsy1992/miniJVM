//
// Created by Gust on 2017/11/15 0015.
//
#include "pthread.h"



#ifndef MINI_JVM_SPINLOCK_H
#define MINI_JVM_SPINLOCK_H

//======================= spinlock =============================

#if  __JVM_OS_MAC__

typedef int pthread_spinlock_t;

int pthread_spin_init(pthread_spinlock_t *lock, int pshared);

int pthread_spin_destroy(pthread_spinlock_t *lock);

int pthread_spin_lock(pthread_spinlock_t *lock);

int pthread_spin_trylock(pthread_spinlock_t *lock);

int pthread_spin_unlock(pthread_spinlock_t *lock);

#endif


#endif //MINI_JVM_SPINLOCK_H
