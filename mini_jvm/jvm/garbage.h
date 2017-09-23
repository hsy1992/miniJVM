

#ifndef _GARBAGE_H
#define _GARBAGE_H

#include "../utils/hashtable.h"
#include "../utils/hashset.h"
#include "jvm.h"
#include <pthread.h>

//回收线程
extern s64 GARBAGE_PERIOD_MS;//
pthread_t *_garbage_thread;//垃圾回收线程
pthread_mutexattr_t _garbage_attr;//
pthread_mutex_t _garbage_lock; //重入锁
extern s32 _garbage_thread_stop;
extern s32 _garbage_thread_pause;
pthread_cond_t _garbageCond;
extern ArrayList *_garbage_refer_set_pool;
extern s64 _garbage_count;

//每个线程一个回收站，线程多了就是灾难
typedef struct _RecycleBin {
    Hashtable *son_2_father; //key=mem_ptr, value=我被别人引用的列表
    Hashtable *father_2_son; //key=mem_ptr, value=别人被我引用的列表
} RecycleBin;

void recycle_bin_init(RecycleBin *bin);

void recycle_bin_free(RecycleBin *bin);

void *collect_thread_run(void *para);

void garbage_thread_lock();

void garbage_thread_unlock();

void garbage_thread_stop();

void garbage_thread_wait();

void garbage_thread_notify();

//其他函数

void *jvm_alloc(u32 size);

s32 jvm_free(void *ptr);

void *jvm_realloc(void *pPtr, u32 size);

s32 garbage_collector_create();

void garbage_collector_destory();

void *garbage_refer(void *sonPtr, void *parentPtr, Runtime *runtime);

void garbage_derefer(void *sonPtr, void *parentPtr, Runtime *runtime);

s32 garbage_collect();

#endif //_GARBAGE_H
