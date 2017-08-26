

#ifndef _GARBAGE_H
#define _GARBAGE_H

#include "../utils/hashtable.h"
#include "jvm.h"
#include <pthread.h>

//回收线程
extern s64 GARBAGE_PERIOD_MS;//
pthread_t *_garbage_thread;//垃圾回收线程
pthread_mutexattr_t _garbage_attr;//
pthread_mutex_t _garbage_lock; //重入锁
static s32 _garbage_thread_stop = 0;

void *collect_thread_run(void *para);

void garbage_thread_lock();

void garbage_thread_unlock();

void garbage_thread_stop();

//其他函数

void *jvm_alloc(u32 size);

void jvm_free(void *ptr);

void *jvm_realloc(void *pPtr, u32 size);

s32 garbage_collector_create();

s32 garbage_collect();

void dump_refer();

void *garbage_refer(void *sonPtr, void *parentPtr);

void garbage_derefer(void *sonPtr, void *parentPtr);

void garbage_derefer_all(void *parentPtr);

#endif //_GARBAGE_H
