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

s32 isData8ByteByTag(c8 c);

s32 isDataReferByIndex(s32 index);

u8 getDataTypeTag(s32 index);

Class *array_class_get(Utf8String *desc);

s64 currentTimeMillis(void);

s64 nanoTime(void);

s64 threadSleep(s64 ms);

s32 sys_properties_load(Utf8String *path);

void sys_properties_dispose(void);


int jvm_printf(const char *, ...);

void invoke_deepth(Runtime *runtime);


c8 *getInstanceFieldPtr(Instance *ins, FieldInfo *fi);

c8 *getStaticFieldPtr(FieldInfo *fi);

void setFieldInt(c8 *ptr, s32 v);

__refer setFieldRefer(c8 *ptr, __refer v);

void setFieldLong(c8 *ptr, s64 v);

void setFieldShort(c8 *ptr, s16 v);

void setFieldByte(c8 *ptr, c8 v);

void setFieldDouble(c8 *ptr, f64 v);

void setFieldFloat(c8 *ptr, f32 v);

s64 getFieldLong(c8 *ptr);

c8 getFieldByte(c8 *ptr);

s16 getFieldShort(c8 *ptr);

s32 getFieldInt(c8 *ptr);

__refer getFieldRefer(c8 *ptr);

f32 getFieldDouble(c8 *ptr);

f32 getFieldFloat(c8 *ptr);

s32 getLineNumByIndex(CodeAttribute *ca, s32 offset);

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

void printDumpOfClasses(void);



////======================= thread =============================

struct _JavaThreadInfo {
    Instance *jthread;
    Runtime *top_runtime;
    u8 volatile thread_status;
    s32 volatile suspend_count;//for jdwp suspend ,>0 suspend, ==0 resume
    u8 volatile is_suspend;
    u8 volatile is_blocking;

    pthread_t pthread;
    //调试器相关字段
    JdwpStep jdwp_step;
};

struct _ThreadLock {
    pthread_cond_t thread_cond;
    pthread_mutexattr_t lock_attr;
    pthread_mutex_t mutex_lock; //互斥锁
    Instance *jthread_holder;
};

s32 jthread_init(Instance *jthread);

s32 jthread_init_with_runtime(Instance *jthread, Runtime *runtime);

s32 jthread_dispose(Instance *jthread);

void *jtherad_run(void *para);

pthread_t jthread_start(Instance *ins);

__refer jthread_get_threadq_value(Instance *ins);

void jthread_set_threadq_value(Instance *ins, void *val);

__refer jthread_get_name_value(Instance *ins);

void jthreadlock_create(MemoryBlock *mb);

void jthreadlock_destory(MemoryBlock *mb);

s32 jthread_lock(MemoryBlock *mb, Runtime *runtime);

s32 jthread_unlock(MemoryBlock *mb, Runtime *runtime);

s32 jthread_notify(MemoryBlock *mb, Runtime *runtime);

s32 jthread_notifyAll(MemoryBlock *mb, Runtime *runtime);

s32 jthread_waitTime(MemoryBlock *mb, Runtime *runtime, s64 waitms);

s32 jthread_sleep(Runtime *runtime, s64 ms);

s32 jthread_yield(Runtime *runtime);

s32 jthread_resume(Runtime *runtime);

s32 jthread_suspend(Runtime *runtime);

void jthread_block_exit(Runtime *runtime);

void jthread_block_enter(Runtime *runtime);

s32 check_suspend_and_pause(Runtime *runtime);

void thread_lock_dispose(ThreadLock *lock);

void thread_lock_init(ThreadLock *lock);

Instance *jarray_create_des(s32 count, Utf8String *desc);

Instance *jarray_create(s32 count, s32 typeIdx, Utf8String *type);

s32 jarray_destory(Instance *arr);

Instance *jarray_multi_create(ArrayList *dim, Utf8String *desc, s32 deep);

void jarray_set_field(Instance *arr, s32 index, Long2Double *l2d);

void jarray_get_field(Instance *arr, s32 index, Long2Double *l2d);

Instance *jstring_create(Utf8String *src, Runtime *runtime);

Instance *exception_create(s32 exception_type, Runtime *runtime);

Instance *exception_create_str(s32 exception_type, Runtime *runtime, c8 *errmsg);

c8 *getFieldPtr_byName_c(Instance *instance, c8 *pclassName, c8 *pfieldName, c8 *pfieldType);

c8 *getFieldPtr_byName(Instance *instance, Utf8String *clsName, Utf8String *fieldName, Utf8String *fieldType);

Class *classes_get_c(c8 *clsName);

Class *classes_get(Utf8String *clsName);

Class *classes_load_get_c(c8 *pclassName, Runtime *runtime);

s32 classes_put(Class *clazz);

Class *classes_load_get(Utf8String *pclassName, Runtime *runtime);

void threadlist_add(Runtime *r);

void threadlist_remove(Runtime *r);

Runtime *threadlist_get(s32 i);


void threadinfo_destory(JavaThreadInfo *threadInfo);

JavaThreadInfo *threadinfo_create(void);

#ifdef __cplusplus
}
#endif

#endif //MINI_JVM_UTIL_H
