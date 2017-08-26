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

int unicode_2_utf8(Utf8String *ustr, u16 *jchar_arr, s32 totalSize);

void swap_endian_little_big(u8 *ptr, s32 size);

s32 getDataTypeIndex(c8 ch);

s32 isReference(c8 c);

s64 currentTimeMillis();

s64 nanoTime();

s32 threadWait(s64 ms);

//s32 threadSleep(s64 ms);

void jstring_set_count(Instance *jstr, s32 count);

s32 jstring_get_count(Instance *jstr);

u8 *jstring_get_value_ptr(Instance *jstr);

Instance *jstring_get_value_array(Instance *jstr);

s16 jstring_char_at(Instance *jstr, s32 index);

s32 jstring_index_of(Instance *jstr, uni_char ch, s32 startAt);

s32 jstring_equals(Instance *jstr1, Instance *jstr2);

void parseMethodPara(Utf8String *methodType, Utf8String *out);

Instance *getInstanceInStack(Class *clazz, ConstantMethodRef *cmr, StackFrame *stack);

void printDumpOfClasses();

Class *getClass(c8 *pclassName, Runtime *runtime);

void runtime_create(Runtime *runtime);

////======================= thread =============================
typedef struct _JavaThreadLock {
    pthread_mutex_t f_lock; //互斥锁
    Instance *jthread_holder;
    s32 hold_count;
} JavaThreadLock;

pthread_t *thread_create_reg(Instance *ins, pthread_t *pthread);

__refer jthread_get_threadq_value(Instance *ins);

void jthread_set_threadq_value(Instance *ins, void *val);

JavaThreadLock *jthreadlock_create();

void jthreadlock_destory(JavaThreadLock *jtl);

s32 jthread_lock(Instance *ins, Runtime *runtime);

s32 jthread_unlock(Instance *ins, Runtime *runtime);

#endif //MINI_JVM_UTIL_H
