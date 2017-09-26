

#ifndef _GARBAGE_H
#define _GARBAGE_H

#include "../utils/hashtable.h"
#include "../utils/hashset.h"
#include "jvm.h"
#include <pthread.h>

//回收线程
extern s64 GARBAGE_PERIOD_MS;//
extern Collector* collector;


//每个线程一个回收站，线程多了就是灾难
typedef struct _Collector {
    //
    Hashtable *son_2_father; //key=mem_ptr, value=我被别人引用的列表
    Hashtable *father_2_son; //key=mem_ptr, value=别人被我引用的列表
    ArrayList *_garbage_refer_set_pool;
    //
    pthread_t _garbage_thread;//垃圾回收线程
    pthread_mutexattr_t _garbage_attr;//
    pthread_mutex_t _garbage_lock; //重入锁
    pthread_cond_t _garbageCond;

    s32 _garbage_thread_stop;
    s32 _garbage_thread_pause;
    s64 _garbage_count;
} Collector;

void *collect_thread_run(void *para);

void garbage_thread_lock();

void garbage_thread_unlock();

void garbage_thread_stop();

void garbage_thread_wait();

void garbage_thread_timedwait(s64 ms);

void garbage_thread_notify();

//其他函数

s32 garbage_collector_create();

void garbage_collector_destory();

s32 garbage_collect();

void dump_refer();

void *garbage_refer(void *sonPtr, void *parentPtr);

void garbage_derefer(void *sonPtr, void *parentPtr);

void garbage_derefer_all(void *parentPtr);

s32 garbage_is_refer_by(__refer sonPtr, __refer parentPtr);

void garbage_destory_memobj(__refer k);

s32 garbage_mark_by_threads();

s32 garbage_mark_refered_obj(Runtime *pruntime);

#endif //_GARBAGE_H
