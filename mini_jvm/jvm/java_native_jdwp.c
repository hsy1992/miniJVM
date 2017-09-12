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


s32 javax_mini_jdwp_reflect_JdwpNative_referenceTyepSize(Runtime *runtime, Class *clazz) {
    push_int(runtime->stack, sizeof(__refer));

#if _JVM_DEBUG
    printf("javax_mini_jdwp_reflect_JdwpNative_referenceTyepSize\n");
#endif
    return 0;
}

s32 com_egls_jvm_JvmThreads_getStatus(Runtime *runtime, Class *clazz) {
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

s32 com_egls_jvm_JvmThreads_suspendThread(Runtime *runtime, Class *clazz) {
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

s32 com_egls_jvm_JvmThreads_resumeThread(Runtime *runtime, Class *clazz) {
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


s32 com_egls_jvm_JvmThreads_getSuspendCount(Runtime *runtime, Class *clazz) {
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

s32 com_egls_jvm_JvmThreads_getFrameCount(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Runtime *trun = (Runtime *) jthread_get_threadq_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    int i = 0;
    while (trun) {
        i++;
        trun = trun->son;
    }
    push_int(runtime->stack, i);
#if _JVM_DEBUG
    printf("com_egls_jvm_JvmThreads_getFrameCount\n");
#endif
    return 0;
}

s32 com_egls_jvm_JvmThreads_stopThread(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Long2Double l2d;
    l2d.i2l.i1=(runtime->localVariables + 1)->integer;
    l2d.i2l.i0=(runtime->localVariables + 2)->integer;
    Instance *ins=(__refer)(long)l2d.l;
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


static java_native_method method_jdwp_table[] = {
        {"javax/mini/jdwp/reflect/JdwpNative", "referenceTyepSize", "()I",                   javax_mini_jdwp_reflect_JdwpNative_referenceTyepSize},
        {"com/egls/jvm/JvmThreads",            "getStatus",         "(Ljava/lang/Thread;)I", com_egls_jvm_JvmThreads_getStatus},
        {"com/egls/jvm/JvmThreads",            "suspendThread",     "(Ljava/lang/Thread;)I", com_egls_jvm_JvmThreads_suspendThread},
        {"com/egls/jvm/JvmThreads",            "resumeThread",      "(Ljava/lang/Thread;)I", com_egls_jvm_JvmThreads_resumeThread},
        {"com/egls/jvm/JvmThreads",            "getSuspendCount",   "(Ljava/lang/Thread;)I", com_egls_jvm_JvmThreads_getSuspendCount},
        {"com/egls/jvm/JvmThreads",            "getFrameCount",     "(Ljava/lang/Thread;)I", com_egls_jvm_JvmThreads_getFrameCount},
        {"com/egls/jvm/JvmThreads",            "stopThread",     "(Ljava/lang/Thread;J)I", com_egls_jvm_JvmThreads_stopThread},

};


void reg_jdwp_native_lib() {
    native_reg_lib(&(method_jdwp_table[0]), sizeof(method_jdwp_table) / sizeof(java_native_method));
}


#ifdef __cplusplus
}
#endif