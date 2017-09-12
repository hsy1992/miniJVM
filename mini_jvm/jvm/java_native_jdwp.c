//
// Created by gust on 2017/9/1.
//

#include "jvm.h"
#include "java_native_std.h"
#include "garbage.h"
#include "jvm_util.h"


#ifdef __cplusplus
extern "C" {
#endif


s32 javax_mini_jdwp_vm_JdwpNative_referenceTyepSize(Runtime *runtime, Class *clazz) {
    push_int(runtime->stack, sizeof(__refer));

#if _JVM_DEBUG
    printf("javax_mini_jdwp_vm_JdwpNative_referenceTyepSize\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_JdwpNative_referenceId(Runtime *runtime, Class *clazz) {
    Instance *ins = (Instance *) (runtime->localVariables + 0)->refer;
    Long2Double l2d;
    l2d.l = (u64) (long) ins;
    push_long(runtime->stack, l2d.l);

#if _JVM_DEBUG
    printf("javax_mini_jdwp_vm_JdwpNative_referenceId\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_JdwpNative_referenceObj(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer r = (__refer) (long) l2d.l;//这里不能直接转化，可能在外部发生了数据精度丢失，只能从低位强转
    push_ref(runtime->stack, r);

#if _JVM_DEBUG
    printf("javax_mini_jdwp_JdwpNative_referenceObj\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_JdwpNative_getClasses(Runtime *runtime, Class *clazz) {
    s32 size = classes->entries;
    Instance *jarr = jarray_create(size, DATATYPE_REFERENCE);
    s32 i = 0;
    Long2Double l2d;
    HashtableIterator hti;
    hashtable_iterate(classes, &hti);
    s32 bytes = data_type_bytes[DATATYPE_REFERENCE];
    for (; hashtable_iter_has_more(&hti);) {
        Utf8String *k = hashtable_iter_next_key(&hti);
        l2d.r = classes_get(k);
        jarray_set_field(jarr, i, &l2d, bytes);
        i++;
    }
    push_ref(runtime->stack, jarr);//先放入栈，再关联回收器，防止多线程回收
    garbage_refer(jarr, NULL);

#if _JVM_DEBUG
    printf("javax_mini_jdwp_vm_JdwpNative_getClasses\n");
#endif
    return 0;
}


s32 javax_mini_jdwp_vm_MemObject_readByte0(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer r = (__refer) (long) l2d.l;
    s32 offset = (runtime->localVariables + 2)->integer;
    u8 val = getFieldByte(((c8 *) r) + offset);
    push_int(runtime->stack, val);
#if _JVM_DEBUG
    printf("javax_mini_jdwp_vm_MemObject_readByte0\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_MemObject_readShort0(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer r = (__refer) (long) l2d.l;
    s32 offset = (runtime->localVariables + 2)->integer;
    u16 val = getFieldShort(((c8 *) r) + offset);
    push_int(runtime->stack, val);
#if _JVM_DEBUG
    printf("javax_mini_jdwp_vm_MemObject_readShort0\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_MemObject_readInt0(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer r = (__refer) (long) l2d.l;
    s32 offset = (runtime->localVariables + 2)->integer;
    s32 val = getFieldInt(((c8 *) r) + offset);
    push_int(runtime->stack, val);
#if _JVM_DEBUG
    printf("javax_mini_jdwp_vm_MemObject_readInt0\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_MemObject_readLong0(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer r = (__refer) (long) l2d.l;
    s32 offset = (runtime->localVariables + 2)->integer;
    s64 val = getFieldLong(((c8 *) r) + offset);
    push_long(runtime->stack, val);
#if _JVM_DEBUG
    printf("javax_mini_jdwp_vm_MemObject_readLong0\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_MemObject_readRefer0(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer r = (__refer) (long) l2d.l;
    s32 offset = (runtime->localVariables + 2)->integer;
    __refer val = getFieldRefer(((c8 *) r) + offset);
    push_long(runtime->stack, (u64) (long) val);
#if _JVM_DEBUG
    printf("javax_mini_jdwp_vm_MemObject_readRefer0\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_JvmThreads_getThreads(Runtime *runtime, Class *clazz) {
    Instance *jarr = jarray_create(thread_list->length, DATATYPE_REFERENCE);
    s32 i = 0;
    Long2Double l2d;

    s32 bytes = data_type_bytes[DATATYPE_REFERENCE];
    for (i = 0; i < thread_list->length; i++) {
        Runtime *r = (Runtime *) arraylist_get_value(thread_list, i);
        l2d.r = r->threadInfo->jthread;
        jarray_set_field(jarr, i, &l2d, bytes);
    }
    push_ref(runtime->stack, jarr);//先放入栈，再关联回收器，防止多线程回收
    garbage_refer(jarr, NULL);

#if _JVM_DEBUG
    printf("javax_mini_jdwp_vm_JvmThreads_getThreads\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_JvmThreads_getStatus(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Runtime *trun = (Runtime *) jthread_get_threadq_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun)
        push_int(runtime->stack, trun->threadInfo->thread_status);
    else
        push_int(runtime->stack, THREAD_STATUS_ZOMBIE);
#if _JVM_DEBUG
    printf("com_egls_jvm_JvmThreads_getStatus\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_JvmThreads_suspendThread(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Runtime *trun = (Runtime *) jthread_get_threadq_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun) {
        trun->threadInfo->suspend_count++;
        push_int(runtime->stack, 0);
    } else
        push_int(runtime->stack, 1);
#if _JVM_DEBUG
    printf("com_egls_jvm_JvmThreads_suspendThread\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_JvmThreads_resumeThread(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Runtime *trun = (Runtime *) jthread_get_threadq_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun) {
        trun->threadInfo->suspend_count--;
        push_int(runtime->stack, 0);
    } else
        push_int(runtime->stack, 1);
#if _JVM_DEBUG
    printf("com_egls_jvm_JvmThreads_resumeThread\n");
#endif
    return 0;
}


s32 javax_mini_jdwp_vm_JvmThreads_getSuspendCount(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Runtime *trun = (Runtime *) jthread_get_threadq_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun) {
        push_int(runtime->stack, trun->threadInfo->suspend_count);
    } else
        push_int(runtime->stack, 0);
#if _JVM_DEBUG
    printf("com_egls_jvm_JvmThreads_getSuspendCount\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_JvmThreads_getFrameCount(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Runtime *trun = (Runtime *) jthread_get_threadq_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    int i = 0;
    while (trun) {
        i++;
        trun = trun->son;
    }
    push_int(runtime->stack, 200);
#if _JVM_DEBUG
    printf("com_egls_jvm_JvmThreads_getFrameCount\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_JvmThreads_stopThread(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 1)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 2)->integer;
    Instance *ins = (__refer) (long) l2d.l;
    Runtime *trun = (Runtime *) jthread_get_threadq_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun) {
        push_int(runtime->stack, 0);
    } else
        push_int(runtime->stack, 0);
#if _JVM_DEBUG
    printf("com_egls_jvm_JvmThreads_stopThread\n");
#endif
    return 0;
}


s32 javax_mini_jdwp_vm_JvmThreads_getTopRuntime(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Runtime *trun = (Runtime *) jthread_get_threadq_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun)
        push_long(runtime->stack, (u64) (long) trun);
    else
        push_int(runtime->stack, 0);
#if _JVM_DEBUG
    printf("javax_mini_jdwp_vm_JvmThreads_getTopRuntime\n");
#endif
    return 0;
}

static java_native_method method_jdwp_table[] = {
        {"javax/mini/jdwp/vm/JdwpNative", "referenceTyepSize", "()I",                    javax_mini_jdwp_vm_JdwpNative_referenceTyepSize},
        {"javax/mini/jdwp/vm/JdwpNative", "referenceId",       "(Ljava/lang/Object;)J",  javax_mini_jdwp_vm_JdwpNative_referenceId},
        {"javax/mini/jdwp/vm/JdwpNative", "referenceObj",      "(J)Ljava/lang/Object;",  javax_mini_jdwp_vm_JdwpNative_referenceObj},
        {"javax/mini/jdwp/vm/JdwpNative", "getClasses",        "()[Ljava/lang/Class;",   javax_mini_jdwp_vm_JdwpNative_getClasses},
        {"javax/mini/jdwp/vm/MemObject",  "readByte0",         "(JI)B",                  javax_mini_jdwp_vm_MemObject_readByte0},
        {"javax/mini/jdwp/vm/MemObject",  "readShort0",        "(JI)S",                  javax_mini_jdwp_vm_MemObject_readShort0},
        {"javax/mini/jdwp/vm/MemObject",  "readInt0",          "(JI)I",                  javax_mini_jdwp_vm_MemObject_readInt0},
        {"javax/mini/jdwp/vm/MemObject",  "readLong0",         "(JI)J",                  javax_mini_jdwp_vm_MemObject_readLong0},
        {"javax/mini/jdwp/vm/MemObject",  "readRefer0",        "(JI)J",                  javax_mini_jdwp_vm_MemObject_readRefer0},
        {"javax/mini/jdwp/vm/JvmThreads", "getThreads",        "()[Ljava/lang/Thread;",  javax_mini_jdwp_vm_JvmThreads_getThreads},
        {"javax/mini/jdwp/vm/JvmThreads", "getStatus",         "(Ljava/lang/Thread;)I",  javax_mini_jdwp_vm_JvmThreads_getStatus},
        {"javax/mini/jdwp/vm/JvmThreads", "suspendThread",     "(Ljava/lang/Thread;)I",  javax_mini_jdwp_vm_JvmThreads_suspendThread},
        {"javax/mini/jdwp/vm/JvmThreads", "resumeThread",      "(Ljava/lang/Thread;)I",  javax_mini_jdwp_vm_JvmThreads_resumeThread},
        {"javax/mini/jdwp/vm/JvmThreads", "getSuspendCount",   "(Ljava/lang/Thread;)I",  javax_mini_jdwp_vm_JvmThreads_getSuspendCount},
        {"javax/mini/jdwp/vm/JvmThreads", "getFrameCount",     "(Ljava/lang/Thread;)I",  javax_mini_jdwp_vm_JvmThreads_getFrameCount},
        {"javax/mini/jdwp/vm/JvmThreads", "stopThread",        "(Ljava/lang/Thread;J)I", javax_mini_jdwp_vm_JvmThreads_stopThread},
        {"javax/mini/jdwp/vm/JvmThreads", "getTopRuntime",     "(Ljava/lang/Thread;)J",  javax_mini_jdwp_vm_JvmThreads_getTopRuntime},

};


void reg_jdwp_native_lib() {
    native_reg_lib(&(method_jdwp_table[0]), sizeof(method_jdwp_table) / sizeof(java_native_method));
}


#ifdef __cplusplus
}
#endif