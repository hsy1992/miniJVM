//
// Created by gust on 2017/8/8.
//

#ifndef MINI_JVM_UTIL_H
#define MINI_JVM_UTIL_H

#include "jvm.h"
#include <pthread.h>

static s64 NANO_START = 0;
typedef unsigned short uni_char;

//======================= utils =============================

s32 utf8_2_unicode(Utf8String *ustr, u16 *arr);

int unicode_2_utf8(u16 *jchar_arr, Utf8String *ustr, s32 totalSize);

void swap_endian_little_big(u8 *ptr, s32 size);

s32 getDataTypeIndex(c8 ch);

s32 isReference(c8 c);

u8 getDataTypeFlag(s32 index);

s64 currentTimeMillis();

s64 nanoTime();

s32 threadSleep(s64 ms);

s32 loadSysProperties(Utf8String* path);

//s32 threadSleep(s64 ms);

void jstring_set_count(Instance *jstr, s32 count);

s32 jstring_get_count(Instance *jstr);

s32 jstring_get_offset(Instance *jstr);

u8 *jstring_get_value_ptr(Instance *jstr);

Instance *jstring_get_value_array(Instance *jstr);

s16 jstring_char_at(Instance *jstr, s32 index);

s32 jstring_index_of(Instance *jstr, uni_char ch, s32 startAt);

s32 jstring_equals(Instance *jstr1, Instance *jstr2);

void parseMethodPara(Utf8String *methodType, Utf8String *out);

Instance *getInstanceInStack(Class *clazz, ConstantMethodRef *cmr, StackFrame *stack);

void printDumpOfClasses();


void runtime_create(Runtime *runtime);

void runtime_destory(Runtime *runtime);

////======================= thread =============================
typedef struct _JavaThreadInfo {
    Instance *jthread;
    Runtime *top_runtime;
    volatile u8 garbage_collect_mark_task;
    volatile u8 thread_running;
} JavaThreadInfo;

typedef struct _JavaThreadLock {
    pthread_cond_t thread_cond;
    pthread_mutexattr_t lock_attr;
    pthread_mutex_t mutex_lock; //互斥锁
    Instance *jthread_holder;
    s32 hold_count;
} JavaThreadLock;

pthread_t jthread_create_and_start(Instance *ins);

__refer jthread_get_threadq_value(Instance *ins);

void jthread_set_threadq_value(Instance *ins, void *val);

JavaThreadLock *jthreadlock_create();

void jthreadlock_destory(JavaThreadLock *jtl);

s32 jthread_lock(MemoryBlock *ins, Runtime *runtime);

s32 jthread_unlock(MemoryBlock *ins, Runtime *runtime);

s32 jthread_notify(MemoryBlock *ins, Runtime *runtime);

s32 jthread_notifyAll(MemoryBlock *ins, Runtime *runtime);

s32 jthread_wait(MemoryBlock *ins, Runtime *runtime);

s32 jthread_waitTime(MemoryBlock *ins, Runtime *runtime, long waitms);

#endif //MINI_JVM_UTIL_H
