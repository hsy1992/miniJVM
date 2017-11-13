//
// Created by gust on 2017/9/25.
//

#ifndef MINI_JVM_JVM_TYPE_H
#define MINI_JVM_JVM_TYPE_H

#include "stdlib.h"
//#define __MEM_LEAK_DETECT


// x86   x64 ...
#define __JVM_LITTLE_ENDIAN__ 1
// arm
#define __JVM_BIG_ENDIAN__ 0


#ifdef __MINGW_H
#define __JVM_OS_MINGW__ 1
#endif
#ifdef _CYGWIN_CONFIG_H
#define __JVM_OS_CYGWIN__ 1
#endif
#ifdef __DARWIN_C_ANSI
#define __JVM_OS_MAC__ 1
#endif
#if defined(__GNU_LIBRARY__) || defined(__ANDROID__)
#define __JVM_OS_LINUX__ 1
#endif


//compile
#define __C99
//libary option : -lpthread -lws2_32

#ifdef __MEM_LEAK_DETECT
#include "../cmem/memleak.h"
#endif //__MEM_LEAK_DETECT
#ifdef __cplusplus
extern "C" {
#endif


typedef unsigned char u8;
typedef char c8;
typedef unsigned short int u16;
typedef signed short int s16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef double f64;
typedef unsigned long long u64;
typedef signed long long s64;
typedef void *__refer;

typedef struct _autoptr {
    __refer ref;
    s32 count;
} autoptr;

//======================= smart ptr =============================

autoptr *autoptr_get(autoptr *a);

autoptr *autoptr_new(__refer r);

void autoptr_NULL(autoptr **aref);

//======================= spinlock =============================

#if __JVM_OS_MAC__

typedef int pthread_spinlock_t;

int pthread_spin_init(pthread_spinlock_t *lock, int pshared);

int pthread_spin_destroy(pthread_spinlock_t *lock);

int pthread_spin_lock(pthread_spinlock_t *lock);

int pthread_spin_trylock(pthread_spinlock_t *lock);

int pthread_spin_unlock(pthread_spinlock_t *lock);

#endif
//======================= memory manage =============================

#ifndef __MEM_LEAK_DETECT

void *jvm_calloc(u32 size);

void *jvm_malloc(u32 size);

s32 jvm_free(void *ptr);

void *jvm_realloc(void *pPtr, u32 size);

#endif //__MEM_LEAK_DETECT

#ifdef __cplusplus
}
#endif


#endif //MINI_JVM_JVM_TYPE_H
