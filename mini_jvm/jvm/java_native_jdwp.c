//
// Created by gust on 2017/9/1.
//

#include "jvm.h"
#include "java_native_std.h"
#include "garbage.h"
#include "jvm_util.h"
#include "jdwp.h"


#ifdef __cplusplus
extern "C" {
#endif

//=========================================================================
//          Event  VM产生事件后，生成实例放入eventmanager的列表中
//=========================================================================

//
static c8 *JDWP_CLASS_EVENTMANAGER = "javax/mini/jdwp/events/EventManager";
static c8 *JDWP_CLASS_EVENT = "javax/mini/jdwp/events/Event";
static c8 *JDWP_CLASS_LOCATION = "javax/mini/jdwp/type/Location";

//===============jdwp tools
void jdwp_add_event(Runtime *runtime, Instance *event) {
    Utf8String *clsName = utf8_create_c(JDWP_CLASS_EVENTMANAGER);
    Utf8String *methodName = utf8_create_c("putEvent");
    Utf8String *methodType = utf8_create_c("(Ljavax/mini/jdwp/events/Event;)V");
    Class *cl = classes_load_get(clsName, runtime);

    MethodInfo *method = find_methodInfo_by_name(clsName, methodName, methodType);
    if (method) {
        push_ref(runtime->stack, event);
        execute_method(method, runtime, cl);
    }
    utf8_destory(clsName);
    utf8_destory(methodName);
    utf8_destory(methodType);
}

Instance *jdwp_get_location(Runtime *location_runtime) {
    Class *e_cl = classes_load_get_c(JDWP_CLASS_LOCATION, location_runtime);
    Instance *ins = instance_create(e_cl);
    instance_init(ins, location_runtime);
    //
    __refer ptr;
    //
    ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_LOCATION, "typeTag", "B");
    //CLASS = 1;INTERFACE = 2;ARRAY = 3;
    if (ptr)setFieldByte(ptr, 1);
    //
    ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_LOCATION, "classID", "J");
    if (ptr)setFieldLong(ptr, (u64) (long) location_runtime->clazz);
    //
    ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_LOCATION, "methodID", "J");
    if (ptr)setFieldLong(ptr, (u64) (long) location_runtime->methodInfo);
    //
    ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_LOCATION, "execIndex", "J");
    if (ptr)setFieldLong(ptr, ((u64) (long) location_runtime->pc) - ((u64) (long) location_runtime->bytecode));
    return ins;
}

//==================== jdwp event
void jni_event_on_class_prepar(Runtime *runtime, Class *clazz) {
    Class *e_cl = classes_load_get_c(JDWP_CLASS_EVENT, runtime);
    Instance *ins = instance_create(e_cl);
    instance_init(ins, runtime);
    //
    __refer ptr;
    //
    ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_EVENT, "eventKind", "B");
    if (ptr)setFieldByte(ptr, 1);//CLASS=1;INTERFACE=2;ARRAY=3
    //
    ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_EVENT, "thread", "J");
    if (ptr)setFieldLong(ptr, (u64) (long) runtime->threadInfo->jthread);
    //
    ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_EVENT, "refTypeTag", "B");
    if (ptr)setFieldByte(ptr, 1);
    //
    ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_EVENT, "typeID", "J");
    if (ptr)setFieldLong(ptr, (u64) (long) clazz);
    //
    ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_EVENT, "signature", "Ljava/lang/String;");
    Utf8String *ustr = utf8_create_copy(clazz->name);
    utf8_insert(ustr, 0, 'L');
    utf8_append_c(ustr, ";");
    Instance *signature = jstring_create(ustr, runtime);
    utf8_destory(ustr);
    if (ptr)setFieldRefer(ptr, signature);
    garbage_refer(signature, ins);
    //
    ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_EVENT, "status", "I");
    //VERIFIED = 1;PREPARED = 2;INITIALIZED = 4;ERROR = 8;
    if (ptr)
        setFieldInt(ptr, clazz->status >= CLASS_STATUS_CLINITED ?
                         7 : (clazz->status >= CLASS_STATUS_PREPARED ? 3 : 1));

    //
    jdwp_add_event(runtime, ins);
}

void jni_event_on_class_unload(Runtime *runtime, Class *clazz) {
    Class *e_cl = classes_load_get_c(JDWP_CLASS_EVENT, runtime);
    Instance *ins = instance_create(e_cl);
    instance_init(ins, runtime);
    //
    __refer ptr;
    //
    ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_EVENT, "eventKind", "B");
    if (ptr)setFieldByte(ptr, JDWP_EVENT_CLASS_UNLOAD);
    //
    ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_EVENT, "signature", "Ljava/lang/String;");
    Utf8String *ustr = utf8_create_copy(clazz->name);
    utf8_insert(ustr, 0, 'L');
    utf8_append_c(ustr, ";");
    Instance *signature = jstring_create(ustr, runtime);
    utf8_destory(ustr);
    if (ptr)setFieldRefer(ptr, signature);
    garbage_refer(signature, ins);
    //
    jdwp_add_event(runtime, ins);
}

void jni_event_on_thread_start(Runtime *runtime, Instance *jthread) {
    Class *e_cl = classes_load_get_c(JDWP_CLASS_EVENT, runtime);
    Instance *ins = instance_create(e_cl);
    instance_init(ins, runtime);
    //
    __refer ptr;
    //
    ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_EVENT, "eventKind", "B");
    if (ptr)setFieldByte(ptr, JDWP_EVENT_THREAD_START);
    //
    ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_EVENT, "thread", "J");
    if (ptr)setFieldLong(ptr, (u64) (long) jthread);
    //
    jdwp_add_event(runtime, ins);
}

void jni_event_on_thread_death(Runtime *runtime, Instance *jthread) {
    Class *e_cl = classes_load_get_c(JDWP_CLASS_EVENT, runtime);
    Instance *ins = instance_create(e_cl);
    instance_init(ins, runtime);
    //
    __refer ptr;
    //
    ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_EVENT, "eventKind", "B");
    if (ptr)setFieldByte(ptr, JDWP_EVENT_THREAD_DEATH);
    //
    ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_EVENT, "thread", "J");
    if (ptr)setFieldLong(ptr, (u64) (long) jthread);
    //
    jdwp_add_event(runtime, ins);
}


void jni_event_on_breakpoint(Runtime *breakpoint_runtime) {
    Class *e_cl = classes_load_get_c(JDWP_CLASS_EVENT, breakpoint_runtime);
    Instance *ins = instance_create(e_cl);
    instance_init(ins, breakpoint_runtime);
    //
    __refer ptr;
    //
    ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_EVENT, "eventKind", "B");
    if (ptr)setFieldByte(ptr, JDWP_EVENT_BREAKPOINT);
    //
    ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_EVENT, "thread", "J");
    if (ptr)setFieldLong(ptr, (u64) (long) breakpoint_runtime->threadInfo->jthread);
    //
    ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_EVENT, "loc", "Ljavax/mini/jdwp/type/Location;");
    Instance *loc = jdwp_get_location(breakpoint_runtime);
    if (ptr)setFieldRefer(ptr, loc);
    garbage_refer(loc, ins);
    //
    jdwp_add_event(breakpoint_runtime, ins);
}

//========  native============================================================================

s32 javax_mini_jdwp_vm_JdwpNative_referenceTyepSize(Runtime *runtime, Class *clazz) {
    push_int(runtime->stack, sizeof(__refer));

#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_jdwp_vm_JdwpNative_referenceTyepSize\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_JdwpNative_referenceId(Runtime *runtime, Class *clazz) {
    Instance *ins = (Instance *) (runtime->localVariables + 0)->refer;
    Long2Double l2d;
    l2d.l = (u64) (long) ins;
    push_long(runtime->stack, l2d.l);

#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_jdwp_vm_JdwpNative_referenceId\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_JdwpNative_referenceObj(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer r = (__refer) (long) l2d.l;//这里不能直接转化，可能在外部发生了数据精度丢失，只能从低位强转
    push_ref(runtime->stack, r);

#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_jdwp_JdwpNative_referenceObj\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_JdwpNative_getClasses(Runtime *runtime, Class *clazz) {
    s32 size = classes->entries;

    Utf8String *ustr = utf8_create_c(STR_INS_JAVA_LANG_CLASS);
    Instance *jarr = jarray_create(size, DATATYPE_REFERENCE, ustr);
    utf8_destory(ustr);
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

#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_jdwp_vm_JdwpNative_getClasses\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_JdwpNative_getClassByName(Runtime *runtime, Class *clazz) {
    Instance *jstr = (Instance *) (runtime->localVariables + 0)->refer;
    Utf8String *ustr = utf8_create();
    jstring_2_utf8(jstr, ustr);
    Class *cl = classes_get(ustr);
    push_ref(runtime->stack, cl);
    return 0;
}

s32 javax_mini_jdwp_vm_JdwpNative_setBreakPoint(Runtime *runtime, Class *clazz) {
    int pos = 0;
    s32 setOrClear = (runtime->localVariables + pos++)->integer;
    u8 type_tag = (u8) (runtime->localVariables + pos++)->integer;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    Class *cl = (Class *) (__refer) (long) l2d.l;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    MethodInfo *method = (MethodInfo *) (long) l2d.l;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    u64 execIndex = l2d.l;
    jdwp_set_breakpoint(setOrClear, cl, method, execIndex);
    push_int(runtime->stack, 0);
    return 0;
}

s32 javax_mini_jdwp_vm_JdwpNative_setLocalVal(Runtime *runtime, Class *clazz) {
    int pos = 0;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    Runtime *r = (Runtime *) (__refer) (long) l2d.l;
    s32 slot = (runtime->localVariables + pos++)->integer;
    u8 type = (u8) (runtime->localVariables + pos++)->integer;

    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;

    s32 bytes = (runtime->localVariables + pos++)->integer;
    if (slot < r->localvar_count) {
        switch (bytes) {
            case 'R':
                r->localVariables[slot].refer = (__refer) (long) l2d.l;
                break;
            case '8':
                r->localVariables[slot].integer = l2d.i2l.i0;
                r->localVariables[slot + 1].integer = l2d.i2l.i1;
                break;
            case '4':
            case '2':
            case '1':
                r->localVariables[slot].integer = (s32) l2d.l;
                break;
        }
        push_int(runtime->stack, 0);
    } else {
        push_int(runtime->stack, 1);
    }
    return 0;
}

s32 javax_mini_jdwp_vm_JdwpNative_getLocalVal(Runtime *runtime, Class *clazz) {
    int pos = 0;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    Runtime *r = (Runtime *) (__refer) (long) l2d.l;
    s32 slot = (runtime->localVariables + pos++)->integer;
    Instance *valuetype = (runtime->localVariables + pos++)->refer;

    u8 *ptr = getFieldPtr_byName_c(valuetype, JDWP_CLASS_VALUETYPE, "bytes", "C");
    u16 bytes = (u16) getFieldShort(ptr);
    ptr = getFieldPtr_byName_c(valuetype, JDWP_CLASS_VALUETYPE, "value", "J");
    if (slot < r->localvar_count) {
        switch (bytes) {
            case 'R':
                l2d.l = (s64) (long) r->localVariables[slot].refer;
                break;
            case '8':
                l2d.i2l.i1 = r->localVariables[slot].integer;
                l2d.i2l.i0 = r->localVariables[slot + 1].integer;
                break;
            case '4':
            case '2':
            case '1':
                l2d.l = r->localVariables[slot].integer;
                break;
        }
        setFieldLong(ptr, l2d.l);
        push_int(runtime->stack, 0);
    } else {
        push_int(runtime->stack, 1);
    }
    return 0;
}

s32 javax_mini_jdwp_vm_JdwpNative_getFieldVal(Runtime *runtime, Class *clazz) {
    int pos = 0;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    Instance *ins = (Instance *) (__refer) (long) l2d.l;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    FieldInfo *fi = (FieldInfo *) (__refer) (long) l2d.l;
    Instance *valuetype = (runtime->localVariables + pos++)->refer;

    c8 *ptr = getFieldPtr_byName_c(valuetype, JDWP_CLASS_VALUETYPE, "bytes", "C");
    u16 bytes = (u16) getFieldShort(ptr);
    ptr = getFieldPtr_byName_c(valuetype, JDWP_CLASS_VALUETYPE, "value", "J");
    c8 *fptr;
    fptr = getFieldPtr_byName(ins, fi->_this_class->name, fi->name, fi->descriptor);
    s64 val;
    switch (bytes) {
        case 'R':
            val = (s64) (long) getFieldRefer(fptr);

            break;
        case '8':
            val = getFieldLong(fptr);
            break;
        case '4':
            val = getFieldInt(fptr);
            break;
        case '2':
            val = getFieldShort(fptr);
            break;
        case '1':
            val = getFieldByte(fptr);
            break;
    }
    setFieldLong(ptr, val);
    push_int(runtime->stack, 0);

    return 0;
}


s32 javax_mini_jdwp_reflect_MemAccess_readByte0(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer r = (__refer) (long) l2d.l;
    s32 offset = (runtime->localVariables + 2)->integer;
    u8 val = getFieldByte(((c8 *) r) + offset);
    push_int(runtime->stack, val);
#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_jdwp_vm_MemObject_readByte0\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_reflect_MemAccess_readShort0(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer r = (__refer) (long) l2d.l;
    s32 offset = (runtime->localVariables + 2)->integer;
    u16 val = getFieldShort(((c8 *) r) + offset);
    push_int(runtime->stack, val);
#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_jdwp_vm_MemObject_readShort0\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_reflect_MemAccess_readInt0(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer r = (__refer) (long) l2d.l;
    s32 offset = (runtime->localVariables + 2)->integer;
    s32 val = getFieldInt(((c8 *) r) + offset);
    push_int(runtime->stack, val);
#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_jdwp_vm_MemObject_readInt0\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_reflect_MemAccess_readLong0(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer r = (__refer) (long) l2d.l;
    s32 offset = (runtime->localVariables + 2)->integer;
    s64 val = getFieldLong(((c8 *) r) + offset);
    push_long(runtime->stack, val);
#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_jdwp_vm_MemObject_readLong0\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_reflect_MemAccess_readRefer0(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer r = (__refer) (long) l2d.l;
    s32 offset = (runtime->localVariables + 2)->integer;
    __refer val = getFieldRefer(((c8 *) r) + offset);
    push_long(runtime->stack, (u64) (long) val);
#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_jdwp_vm_MemObject_readRefer0\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_JdwpThreads_getThreads(Runtime *runtime, Class *clazz) {
    Utf8String *ustr = utf8_create_c(STR_INS_JAVA_LANG_THREAD);
    Instance *jarr = jarray_create(thread_list->length, DATATYPE_REFERENCE, ustr);
    utf8_destory(ustr);
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

#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_jdwp_vm_JdwpThreads_getThreads\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_JdwpThreads_getStatus(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Runtime *trun = (Runtime *) jthread_get_threadq_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun)
        push_int(runtime->stack, trun->threadInfo->thread_status);
    else
        push_int(runtime->stack, THREAD_STATUS_ZOMBIE);
#if _JVM_DEBUG > 5
    jvm_printf("com_egls_jvm_JdwpThreads_getStatus\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_JdwpThreads_suspendThread(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Runtime *trun = (Runtime *) jthread_get_threadq_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun) {
        trun->threadInfo->suspend_count++;
        push_int(runtime->stack, 0);
    } else
        push_int(runtime->stack, 1);
#if _JVM_DEBUG > 5
    jvm_printf("com_egls_jvm_JdwpThreads_suspendThread\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_JdwpThreads_resumeThread(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Runtime *trun = (Runtime *) jthread_get_threadq_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun) {
        if (trun->threadInfo->suspend_count > 0)trun->threadInfo->suspend_count--;
        push_int(runtime->stack, 0);
    } else
        push_int(runtime->stack, 1);
#if _JVM_DEBUG > 5
    jvm_printf("com_egls_jvm_JdwpThreads_resumeThread\n");
#endif
    return 0;
}


s32 javax_mini_jdwp_vm_JdwpThreads_getSuspendCount(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Runtime *trun = (Runtime *) jthread_get_threadq_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun) {
        push_int(runtime->stack, trun->threadInfo->suspend_count);
    } else
        push_int(runtime->stack, 0);
#if _JVM_DEBUG > 5
    jvm_printf("com_egls_jvm_JdwpThreads_getSuspendCount\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_vm_JdwpThreads_getFrameCount(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Runtime *trun = (Runtime *) jthread_get_threadq_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    int i = 0;
    while (trun) {
        i++;
        trun = trun->son;
    }
    i--;
    push_int(runtime->stack, i);
    return 0;
}

s32 javax_mini_jdwp_vm_JdwpThreads_stopThread(Runtime *runtime, Class *clazz) {
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
#if _JVM_DEBUG > 5
    jvm_printf("com_egls_jvm_JdwpThreads_stopThread\n");
#endif
    return 0;
}


s32 javax_mini_jdwp_vm_JdwpThreads_getTopRuntime(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Runtime *trun = (Runtime *) jthread_get_threadq_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun)
        push_long(runtime->stack, (u64) (long) trun);
    else
        push_int(runtime->stack, 0);
#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_jdwp_vm_JdwpThreads_getTopRuntime\n");
#endif
    return 0;
}

s32 javax_mini_jdwp_reflect_Reference_mapReference(Runtime *runtime, Class *clazz) {
    int pos = 0;
    Instance *ins = (Instance *) (runtime->localVariables + pos++)->refer;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    Class *target = (__refer) (long) l2d.l;
    if (target) {
        u8 *ptr;
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_REFERENCE, "className", "Ljava/lang/String;");
        Instance *clsName = jstring_create(target->name, runtime);
        if (ptr)setFieldRefer(ptr, clsName);
        garbage_refer(clsName, ins);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_REFERENCE, "superclass", "J");
        if (ptr)
            setFieldLong(ptr, (s64) (long) getSuperClass(target));
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_REFERENCE, "accessFlags", "S");
        if (ptr)setFieldShort(ptr, target->cff.access_flags);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_REFERENCE, "status", "I");
        //VERIFIED = 1;PREPARED = 2;INITIALIZED = 4;ERROR = 8;
        if (ptr)
            setFieldInt(ptr, target->status >= CLASS_STATUS_CLINITED ?
                             7 : (target->status >= CLASS_STATUS_PREPARED ? 3 : 1));
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_REFERENCE, "source", "Ljava/lang/String;");
        Instance *source = jstring_create(target->source, runtime);
        if (ptr)setFieldRefer(ptr, source);
        garbage_refer(source, ins);
        //
        s32 i;
        {
            ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_REFERENCE, "fieldIds", "[J");
            Instance *jarr = jarray_create(target->fieldPool.field_used, DATATYPE_LONG, NULL);
            for (i = 0; i < target->fieldPool.field_used; i++) {
                setFieldLong(&(jarr->arr_body[i * data_type_bytes[DATATYPE_LONG]]),
                             (u64) (long) &target->fieldPool.field[i]);
            }
            if (ptr)setFieldRefer(ptr, jarr);
            garbage_refer(jarr, ins);
        }
        //
        {
            ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_REFERENCE, "methodIds", "[J");
            Instance *jarr = jarray_create(target->methodPool.method_used, DATATYPE_LONG, NULL);
            for (i = 0; i < target->methodPool.method_used; i++) {
                setFieldLong(&(jarr->arr_body[i * data_type_bytes[DATATYPE_LONG]]),
                             (u64) (long) &target->methodPool.method[i]);
            }
            if (ptr)setFieldRefer(ptr, jarr);
            garbage_refer(jarr, ins);
        }
        //
        {
            ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_REFERENCE, "interfaces", "[J");
            Instance *jarr = jarray_create(target->interfacePool.clasz_used, DATATYPE_LONG, NULL);
            for (i = 0; i < target->interfacePool.clasz_used; i++) {
                Class *cl = classes_load_get(target->interfacePool.clasz[i].name, runtime);
                setFieldLong(&(jarr->arr_body[i * data_type_bytes[DATATYPE_LONG]]),
                             (u64) (long) cl);
            }
            if (ptr)setFieldRefer(ptr, jarr);
            garbage_refer(jarr, ins);
        }
    }
    return 0;
}

s32 javax_mini_jdwp_reflect_Field_mapField(Runtime *runtime, Class *clazz) {
    int pos = 0;
    Instance *ins = (Instance *) (runtime->localVariables + pos++)->refer;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    FieldInfo *fieldInfo = (__refer) (long) l2d.l;
    if (ins && fieldInfo) {
        u8 *ptr;
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_FIELD, "fieldName", "Ljava/lang/String;");
        Instance *fieldName = jstring_create(fieldInfo->name, runtime);
        if (ptr)setFieldRefer(ptr, fieldName);
        garbage_refer(fieldName, ins);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_FIELD, "accessFlags", "S");
        if (ptr)setFieldShort(ptr, fieldInfo->access_flags);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_FIELD, "signature", "Ljava/lang/String;");
        Instance *signature = jstring_create(fieldInfo->descriptor, runtime);
        if (ptr)setFieldRefer(ptr, signature);
        garbage_refer(signature, ins);
    }
    return 0;
}

Instance *localVarTable2java(Class *clazz, LocalVarTable *lvt, Runtime *runtime) {
    Class *cl = classes_load_get_c(JDWP_CLASS_LOCALVARTABLE, runtime);
    Instance *ins = instance_create(cl);
    instance_init(ins, runtime);
    if (ins && lvt) {
        u8 *ptr;
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_LOCALVARTABLE, "name", "Ljava/lang/String;");
        Instance *name = jstring_create(get_utf8_string(clazz, lvt->name_index), runtime);
        if (ptr)setFieldRefer(ptr, name);
        garbage_refer(name, ins);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_LOCALVARTABLE, "signature", "Ljava/lang/String;");
        Instance *signature = jstring_create(get_utf8_string(clazz, lvt->descriptor_index), runtime);
        if (ptr)setFieldRefer(ptr, signature);
        garbage_refer(signature, ins);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_LOCALVARTABLE, "codeIndex", "J");
        if (ptr)setFieldLong(ptr, lvt->start_pc);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_LOCALVARTABLE, "length", "I");
        if (ptr)setFieldInt(ptr, lvt->length);
    }
    return ins;
}

s32 javax_mini_jdwp_reflect_Method_mapMethod(Runtime *runtime, Class *clazz) {
    int pos = 0;
    Instance *ins = (Instance *) (runtime->localVariables + pos++)->refer;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    MethodInfo *methodInfo = (__refer) (long) l2d.l;
    if (ins && methodInfo) {
        u8 *ptr;
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "methodName", "Ljava/lang/String;");
        Instance *methodName = jstring_create(methodInfo->name, runtime);
        if (ptr)setFieldRefer(ptr, methodName);
        garbage_refer(methodName, ins);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "signature", "Ljava/lang/String;");
        Instance *signature = jstring_create(methodInfo->descriptor, runtime);
        if (ptr)setFieldRefer(ptr, signature);
        garbage_refer(signature, ins);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "accessFlags", "S");
        if (ptr)setFieldShort(ptr, methodInfo->access_flags);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "argCnt", "I");
        if (ptr)setFieldInt(ptr, methodInfo->para_count);
        //
        s32 i;
        AttributeInfo *att = NULL;
        for (i = 0; i < methodInfo->attributes_count; i++) {
            if (methodInfo->attributes[i].converted_code) {
                att = &methodInfo->attributes[i];
            }
        }
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "codeStart", "J");
        if (ptr)if (att)setFieldLong(ptr, 0); else setFieldShort(ptr, -1);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "codeEnd", "J");
        if (ptr)if (att)setFieldLong(ptr, att->converted_code->attribute_length); else setFieldLong(ptr, -1);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "lines", "I");
        if (ptr)if (att)setFieldInt(ptr, att->converted_code->line_number_table_length); else setFieldInt(ptr, 0);
        //
        if (att) {
            {
                ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "lineNum", "[S");
                Instance *jarr = jarray_create(att->converted_code->line_number_table_length * 2, DATATYPE_SHORT, NULL);
                memcpy(jarr->arr_body, att->converted_code->line_number_table,
                       att->converted_code->line_number_table_length * 4);
                if (ptr)setFieldRefer(ptr, jarr);
                garbage_refer(jarr, ins);
            }
            {
                //
                u8 *table_type = "[Ljavax/mini/jdwp/reflect/LocalVarTable;";
                ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "localVarTable", table_type);
                Utf8String *ustr = utf8_create_c(table_type);
                Instance *jarr = jarray_create(att->converted_code->local_var_table_length, DATATYPE_REFERENCE,
                                               ustr);
                utf8_destory(ustr);
                for (i = 0; i < att->converted_code->local_var_table_length; i++) {
                    LocalVarTable *lvt = &att->converted_code->local_var_table[i];
                    Instance *jlocaltable = localVarTable2java(methodInfo->_this_class, lvt, runtime);
                    setFieldRefer(&jarr->arr_body[i], jlocaltable);
                    garbage_refer(jlocaltable, jarr);
                }
                if (ptr)setFieldRefer(ptr, jarr);
                garbage_refer(jarr, ins);
            }
        }

    }
    return 0;
}

s32 javax_mini_jdwp_reflect_StackFrame_mapRuntime(Runtime *runtime, Class *clazz) {
    int pos = 0;
    Instance *ins = (Instance *) (runtime->localVariables + pos++)->refer;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    Runtime *target = (__refer) (long) l2d.l;
    if (ins && target) {
        u8 *ptr;
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_RUNTIME, "classId", "J");
        if (ptr)setFieldLong(ptr, (u64) (long) target->clazz);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_RUNTIME, "sonId", "J");
        if (ptr)setFieldLong(ptr, (u64) (long) target->son);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_RUNTIME, "pc", "J");
        if (ptr)setFieldLong(ptr, (u64) (long) target->pc);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_RUNTIME, "byteCode", "J");
        if (ptr)setFieldLong(ptr, (u64) (long) target->bytecode);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_RUNTIME, "methodId", "J");
        if (ptr)setFieldLong(ptr, (u64) (long) target->methodInfo);
        //
        if (target->methodInfo && !(target->methodInfo->access_flags & ACC_STATIC)) {//top runtime method is null
            ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_RUNTIME, "localThis", "J");
            if (ptr)setFieldLong(ptr, (s64) (long) (&target->localVariables[0])->refer);
        }
    }
    return 0;
}

s32 javax_mini_jdwp_reflect_Array_mapArray(Runtime *runtime, Class *clazz) {
    int pos = 0;
    Instance *ins = (Instance *) (runtime->localVariables + pos++)->refer;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    Instance *target = (__refer) (long) l2d.l;
    if (ins && target) {
        u8 *ptr;
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_RUNTIME, "length", "I");
        if (ptr)setFieldInt(ptr, target->arr_length);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_RUNTIME, "arr_addr", "J");
        if (ptr)setFieldLong(ptr, (u64) (long) target->arr_body);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_RUNTIME, "type", "B");
        if (ptr)setFieldByte(ptr, (c8) utf8_char_at(target->mb.clazz->name, 1));
        //

    }
    return 0;
}

static java_native_method method_jdwp_table[] = {
        {"javax/mini/jdwp/vm/JdwpNative",      "referenceTyepSize", "()I",                                   javax_mini_jdwp_vm_JdwpNative_referenceTyepSize},
        {"javax/mini/jdwp/vm/JdwpNative",      "referenceId",       "(Ljava/lang/Object;)J",                 javax_mini_jdwp_vm_JdwpNative_referenceId},
        {"javax/mini/jdwp/vm/JdwpNative",      "referenceObj",      "(J)Ljava/lang/Object;",                 javax_mini_jdwp_vm_JdwpNative_referenceObj},
        {"javax/mini/jdwp/vm/JdwpNative",      "getClasses",        "()[Ljava/lang/Class;",                  javax_mini_jdwp_vm_JdwpNative_getClasses},
        {"javax/mini/jdwp/vm/JdwpNative",      "getClassByName",    "(Ljava/lang/String;)Ljava/lang/Class;", javax_mini_jdwp_vm_JdwpNative_getClassByName},
        {"javax/mini/jdwp/vm/JdwpNative",      "setBreakPoint",     "(IBJJJ)I",                              javax_mini_jdwp_vm_JdwpNative_setBreakPoint},
        {"javax/mini/jdwp/vm/JdwpNative",      "setLocalVal",       "(JIBJI)I",                              javax_mini_jdwp_vm_JdwpNative_setLocalVal},
        {"javax/mini/jdwp/vm/JdwpNative",      "getLocalVal",       "(JILjavax/mini/jdwp/type/ValueType;)I", javax_mini_jdwp_vm_JdwpNative_getLocalVal},
        {"javax/mini/jdwp/vm/JdwpNative",      "getFieldVal",       "(JJLjavax/mini/jdwp/type/ValueType;)I", javax_mini_jdwp_vm_JdwpNative_getFieldVal},
        {"javax/mini/jdwp/vm/JdwpThreads",     "getThreads",        "()[Ljava/lang/Thread;",                 javax_mini_jdwp_vm_JdwpThreads_getThreads},
        {"javax/mini/jdwp/vm/JdwpThreads",     "getStatus",         "(Ljava/lang/Thread;)I",                 javax_mini_jdwp_vm_JdwpThreads_getStatus},
        {"javax/mini/jdwp/vm/JdwpThreads",     "suspendThread",     "(Ljava/lang/Thread;)I",                 javax_mini_jdwp_vm_JdwpThreads_suspendThread},
        {"javax/mini/jdwp/vm/JdwpThreads",     "resumeThread",      "(Ljava/lang/Thread;)I",                 javax_mini_jdwp_vm_JdwpThreads_resumeThread},
        {"javax/mini/jdwp/vm/JdwpThreads",     "getSuspendCount",   "(Ljava/lang/Thread;)I",                 javax_mini_jdwp_vm_JdwpThreads_getSuspendCount},
        {"javax/mini/jdwp/vm/JdwpThreads",     "getFrameCount",     "(Ljava/lang/Thread;)I",                 javax_mini_jdwp_vm_JdwpThreads_getFrameCount},
        {"javax/mini/jdwp/vm/JdwpThreads",     "stopThread",        "(Ljava/lang/Thread;J)I",                javax_mini_jdwp_vm_JdwpThreads_stopThread},
        {"javax/mini/jdwp/vm/JdwpThreads",     "getTopRuntime",     "(Ljava/lang/Thread;)J",                 javax_mini_jdwp_vm_JdwpThreads_getTopRuntime},
        {"javax/mini/jdwp/reflect/MemAccess",  "readByte0",         "(JI)B",                                 javax_mini_jdwp_reflect_MemAccess_readByte0},
        {"javax/mini/jdwp/reflect/MemAccess",  "readShort0",        "(JI)S",                                 javax_mini_jdwp_reflect_MemAccess_readShort0},
        {"javax/mini/jdwp/reflect/MemAccess",  "readInt0",          "(JI)I",                                 javax_mini_jdwp_reflect_MemAccess_readInt0},
        {"javax/mini/jdwp/reflect/MemAccess",  "readLong0",         "(JI)J",                                 javax_mini_jdwp_reflect_MemAccess_readLong0},
        {"javax/mini/jdwp/reflect/MemAccess",  "readRefer0",        "(JI)J",                                 javax_mini_jdwp_reflect_MemAccess_readRefer0},
        {"javax/mini/jdwp/reflect/Reference",  "mapReference",      "(J)V",                                  javax_mini_jdwp_reflect_Reference_mapReference},
        {"javax/mini/jdwp/reflect/Field",      "mapField",          "(J)V",                                  javax_mini_jdwp_reflect_Field_mapField},
        {"javax/mini/jdwp/reflect/Method",     "mapMethod",         "(J)V",                                  javax_mini_jdwp_reflect_Method_mapMethod},
        {"javax/mini/jdwp/reflect/StackFrame", "mapRuntime",        "(J)V",                                  javax_mini_jdwp_reflect_StackFrame_mapRuntime},
        {"javax/mini/jdwp/reflect/Array",      "mapArray",          "(J)V",                                  javax_mini_jdwp_reflect_Array_mapArray},

};


void reg_jdwp_native_lib() {
    native_reg_lib(&(method_jdwp_table[0]), sizeof(method_jdwp_table) / sizeof(java_native_method));
}

#ifdef __cplusplus
}
#endif