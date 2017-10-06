//
// Created by gust on 2017/8/8.
//

#ifndef MINI_JVM_UTIL_H
#define MINI_JVM_UTIL_H

#include "jvm.h"
#include "../utils/hashset.h"
#include "jdwp.h"
#include <pthread.h>


#ifdef __cplusplus
extern "C" {
#endif


static s64 NANO_START = 0;
typedef unsigned short uni_char;

//======================= utils =============================

s32 utf8_2_unicode(Utf8String *ustr, u16 *arr);

int unicode_2_utf8(u16 *jchar_arr, Utf8String *ustr, s32 totalSize);

void swap_endian_little_big(u8 *ptr, s32 size);

s32 getDataTypeIndex(c8 ch);

s32 isDataReferByTag(c8 c);

s32 isDataReferByIndex(s32 index);

u8 getDataTypeTag(s32 index);

Class *array_class_get(Utf8String *descript);

s64 currentTimeMillis();

s64 nanoTime();

s32 threadSleep(s64 ms);

s32 sys_properties_load(Utf8String *path);

void sys_properties_dispose();

void jstring_set_count(Instance *jstr, s32 count);

s32 jstring_get_count(Instance *jstr);

s32 jstring_get_offset(Instance *jstr);

c8 *jstring_get_value_ptr(Instance *jstr);

Instance *jstring_get_value_array(Instance *jstr);

s16 jstring_char_at(Instance *jstr, s32 index);

s32 jstring_index_of(Instance *jstr, uni_char ch, s32 startAt);

s32 jstring_equals(Instance *jstr1, Instance *jstr2);

s32 jstring_2_utf8(Instance *jstr, Utf8String *utf8);

s32 parseMethodPara(Utf8String *methodType, Utf8String *out);

Instance *getInstanceInStack(Class *clazz, ConstantMethodRef *cmr, RuntimeStack *stack);

void printDumpOfClasses();


void runtime_init(Runtime *runtime);

void runtime_dispose(Runtime *runtime);

////======================= thread =============================

 struct _JavaThreadInfo {
    Instance *jthread;
    Runtime *top_runtime;
    u8 volatile thread_status;
    s32 volatile suspend_count;//for jdwp suspend ,>0 suspend, ==0 resume
    u8 volatile is_suspend;

    pthread_t pthread;
    //调试器相关字段
    JdwpStep jdwp_step;
} ;

 struct _ThreadLock {
    pthread_cond_t thread_cond;
    pthread_mutexattr_t lock_attr;
    pthread_mutex_t mutex_lock; //互斥锁
    Instance *jthread_holder;
} ;

pthread_t jthread_create_and_start(Instance *ins);

__refer jthread_get_threadq_value(Instance *ins);

void jthread_set_threadq_value(Instance *ins, void *val);

__refer jthread_get_name_value(Instance *ins);

void jthreadlock_create(MemoryBlock *mb);

void jthreadlock_destory(MemoryBlock *mb);

s32 jthread_lock(MemoryBlock *mb, Runtime *runtime);

s32 jthread_unlock(MemoryBlock *mb, Runtime *runtime);

s32 jthread_notify(MemoryBlock *mb, Runtime *runtime);

s32 jthread_notifyAll(MemoryBlock *mb, Runtime *runtime);

s32 jthread_waitTime(MemoryBlock *mb, Runtime *runtime, long waitms);

s32 jthread_yield(Runtime *runtime);

s32 jthread_resume(Runtime *runtime);

s32 jthread_suspend(Runtime *runtime);

s32 check_suspend_and_pause(Runtime *runtime);

void thread_unlock(ThreadLock *lock);

void thread_lock(ThreadLock *lock);

void thread_lock_dispose(ThreadLock *lock);

void thread_lock_init(ThreadLock *lock);

#ifdef __cplusplus
}
#endif

#endif //MINI_JVM_UTIL_H
