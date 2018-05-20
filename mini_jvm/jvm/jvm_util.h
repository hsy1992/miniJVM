//
// Created by gust on 2017/8/8.
//

#ifndef MINI_JVM_UTIL_H
#define MINI_JVM_UTIL_H


#include "jvm.h"
#include "../utils/hashset.h"
#include "jdwp.h"


#ifdef __cplusplus
extern "C" {
#endif

#define NANO_2_SEC_SCALE 1000000000
#define NANO_2_MILLS_SCALE 1000000
#define MILL_2_SEC_SCALE 1000
static s64 NANO_START = 0;
typedef unsigned short uni_char;

//======================= utils =============================
typedef struct _InstanceFieldInfo {
    FieldInfo *string_offset;
    FieldInfo *string_count;
    FieldInfo *string_value;
    //
    FieldInfo *thread_name;
    FieldInfo *thread_stackFrame;
    //
    FieldInfo *stacktrace_declaringClass;
    FieldInfo *stacktrace_methodName;
    FieldInfo *stacktrace_fileName;
    FieldInfo *stacktrace_lineNumber;
    FieldInfo *stacktrace_parent;

} InstanceFieldInfo;
extern InstanceFieldInfo ins_field_offset;

s32 utf8_2_unicode(Utf8String *ustr, u16 *arr);

int unicode_2_utf8(u16 *jchar_arr, Utf8String *ustr, s32 totalSize);

void swap_endian_little_big(u8 *ptr, s32 size);

s32 getDataTypeIndex(c8 ch);

s32 isDataReferByTag(c8 c);

s32 isData8ByteByTag(c8 c);

s32 isDataReferByIndex(s32 index);

u8 getDataTypeTag(s32 index);

JClass *array_class_get(Utf8String *desc);

s64 currentTimeMillis(void);

s64 nanoTime(void);

s64 threadSleep(s64 ms);

s32 sys_properties_load(ClassLoader *loader);

void sys_properties_dispose(void);

void instance_release_from_thread(Instance *ref, Runtime *runtime);

void instance_hold_to_thread(Instance *ref, Runtime *runtime);

int jvm_printf(const char *, ...);

void invoke_deepth(Runtime *runtime);

s32 parseMethodPara(Utf8String *methodType, Utf8String *out);

Instance *getInstanceInStack(JClass *clazz, ConstantMethodRef *cmr, RuntimeStack *stack);

void printDumpOfClasses(void);


Instance *exception_create(s32 exception_type, Runtime *runtime);

Instance *exception_create_str(s32 exception_type, Runtime *runtime, c8 *errmsg);


//c8 *getInstanceFieldPtr(Instance *ins, FieldInfo *fi);
//
//c8 *getStaticFieldPtr(FieldInfo *fi);
//
//void setFieldInt(c8 *ptr, s32 v);
//
//void setFieldRefer(c8 *ptr, __refer v);
//
//void setFieldLong(c8 *ptr, s64 v);
//
//void setFieldShort(c8 *ptr, s16 v);
//
//void setFieldByte(c8 *ptr, s8 v);
//
//void setFieldDouble(c8 *ptr, f64 v);
//
//void setFieldFloat(c8 *ptr, f32 v);
//
//s64 getFieldLong(c8 *ptr);
//
//s8 getFieldByte(c8 *ptr);
//
//s16 getFieldShort(c8 *ptr);
//
//s32 getFieldInt(c8 *ptr);
//
//__refer getFieldRefer(c8 *ptr);
//
//f64 getFieldDouble(c8 *ptr);
//
//f32 getFieldFloat(c8 *ptr);

/**
 * get instance field value address
 * @param ins ins
 * @param fi fi
 * @return addr
 */
static inline c8 *getInstanceFieldPtr(Instance *ins, FieldInfo *fi) {
    return &(ins->obj_fields[fi->offset_instance]);
}

static inline c8 *getStaticFieldPtr(FieldInfo *fi) {
    return &(fi->_this_class->field_static[fi->offset]);
}


static inline void setFieldInt(c8 *ptr, s32 v) {
    *((s32 *) ptr) = v;
}

static inline void setFieldRefer(c8 *ptr, __refer v) {
    *((__refer *) ptr) = v;
}

static inline void setFieldLong(c8 *ptr, s64 v) {
    *((s64 *) ptr) = v;
}

static inline void setFieldShort(c8 *ptr, s16 v) {
    *((s16 *) ptr) = v;
}

static inline void setFieldByte(c8 *ptr, s8 v) {
    *((s8 *) ptr) = v;
}

static inline void setFieldDouble(c8 *ptr, f64 v) {
    *((f64 *) ptr) = v;
}

static inline void setFieldFloat(c8 *ptr, f32 v) {
    *((f32 *) ptr) = v;
}

static inline s32 getFieldInt(c8 *ptr) {
    return *((s32 *) ptr);
}

static inline __refer getFieldRefer(c8 *ptr) {
    return *((__refer *) ptr);
}

static inline s16 getFieldShort(c8 *ptr) {
    return *((s16 *) ptr);
}

static inline s8 getFieldByte(c8 *ptr) {
    return *((s8 *) ptr);
}

static inline s64 getFieldLong(c8 *ptr) {
    return *((s64 *) ptr);
}

static inline f32 getFieldFloat(c8 *ptr) {
    return *((f32 *) ptr);
}


static inline f64 getFieldDouble(c8 *ptr) {
    return *((f64 *) ptr);
}

s32 getLineNumByIndex(CodeAttribute *ca, s32 offset);

s32 _loadFileContents(c8 *file, ByteBuf *buf);

ByteBuf *load_file_from_classpath(ClassLoader *loader, Utf8String *path);
////======================= jstring =============================

Instance *jstring_create(Utf8String *src, Runtime *runtime);

Instance *jstring_create_cstr(c8 *cstr, Runtime *runtime);

void jstring_set_count(Instance *jstr, s32 count);

s32 jstring_get_count(Instance *jstr);

s32 jstring_get_offset(Instance *jstr);

c8 *jstring_get_value_ptr(Instance *jstr);

Instance *jstring_get_value_array(Instance *jstr);

s16 jstring_char_at(Instance *jstr, s32 index);

s32 jstring_index_of(Instance *jstr, uni_char ch, s32 startAt);

s32 jstring_equals(Instance *jstr1, Instance *jstr2);

s32 jstring_2_utf8(Instance *jstr, Utf8String *utf8);

CStringArr *cstringarr_create(Instance *jstr_arr);

void cstringarr_destory(CStringArr *);

void referarr_destory(CStringArr *ref_arr);

ReferArr *referarr_create(Instance *jobj_arr);

void referarr_2_jlongarr(ReferArr *ref_arr, Instance *jlong_arr);

////======================= thread =============================

struct _JavaThreadInfo {
    Instance *jthread;
    Runtime *top_runtime;
    ArrayList *instance_holder;//for jni hold java object
    s32 volatile suspend_count;//for jdwp suspend ,>0 suspend, ==0 resume
    u8 volatile thread_status;
    u8 volatile is_suspend;
    u8 volatile is_blocking;
    u8 un_use;

    thrd_t pthread;
    //调试器相关字段
    JdwpStep jdwp_step;
};

struct _ThreadLock {
    cnd_t thread_cond;
    mtx_t mutex_lock; //互斥锁
};


s32 jthread_init(Instance *jthread, Runtime *runtime);

s32 jthread_dispose(Instance *jthread);

s32 jtherad_run(void *para);

thrd_t jthread_start(Instance *ins);

__refer jthread_get_stackframe_value(Instance *ins);

s32 jthread_get_daemon_value(Instance *ins);

void jthread_set_stackframe_value(Instance *ins, void *val);

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

Instance *jarray_create_by_class(s32 count, JClass *clazz);

Instance *jarray_create(s32 count, s32 typeIdx, Utf8String *type);

s32 jarray_destory(Instance *arr);

Instance *jarray_multi_create(ArrayList *dim, Utf8String *desc, s32 deep);

void jarray_set_field(Instance *arr, s32 index, s64 val);

s64 jarray_get_field(Instance *arr, s32 index);

JClass *jarray_get_class(Utf8String *type);

JClass *jarray_get_class_by_index(s32 typeIdx);

static inline s32 jarray_check_exception(Instance *arr, s32 index, Runtime *runtime) {
    if (arr == NULL) {
        Instance *exception = exception_create(JVM_EXCEPTION_NULLPOINTER, runtime);
        push_ref(runtime->stack, (__refer) exception);
        return RUNTIME_STATUS_EXCEPTION;
    } else if (index >= arr->arr_length || index < 0) {
        Instance *exception = exception_create(JVM_EXCEPTION_ARRAYINDEXOUTOFBOUNDS, runtime);
        push_ref(runtime->stack, (__refer) exception);
        return RUNTIME_STATUS_EXCEPTION;
    }
    return RUNTIME_STATUS_NORMAL;
}

c8 *getFieldPtr_byName_c(Instance *instance, c8 *pclassName, c8 *pfieldName, c8 *pfieldType);

c8 *getFieldPtr_byName(Instance *instance, Utf8String *clsName, Utf8String *fieldName, Utf8String *fieldType);

JClass *classes_get_c(c8 *clsName);

JClass *classes_get(Utf8String *clsName);

JClass *classes_load_get_c(c8 *pclassName, Runtime *runtime);

s32 classes_put(JClass *clazz);

JClass *classes_load_get(Utf8String *pclassName, Runtime *runtime);

void threadlist_add(Runtime *r);

void threadlist_remove(Runtime *r);

Runtime *threadlist_get(s32 i);

s32 threadlist_count_none_daemon();

void threadinfo_destory(JavaThreadInfo *threadInfo);

JavaThreadInfo *threadinfo_create(void);

#ifdef __cplusplus
}
#endif

#endif //MINI_JVM_UTIL_H
