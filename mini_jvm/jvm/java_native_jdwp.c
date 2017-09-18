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

static u16 JDWP_ERROR_INVALID_TAG = 500; //object type id or class tag
static u16 JDWP_ERROR_ALREADY_INVOKING = 502; //previous invoke not complete
static u16 JDWP_ERROR_INVALID_INDEX = 503;
static u16 JDWP_ERROR_INVALID_LENGTH = 504;
static u16 JDWP_ERROR_INVALID_STRING = 506;
static u16 JDWP_ERROR_INVALID_CLASS_LOADER = 507;
static u16 JDWP_ERROR_INVALID_ARRAY = 508;
static u16 JDWP_ERROR_TRANSPORT_LOAD = 509;
static u16 JDWP_ERROR_TRANSPORT_INIT = 510;
static u16 JDWP_ERROR_NATIVE_METHOD = 511;
static u16 JDWP_ERROR_INVALID_COUNT = 512;
static u16 JDWP_ERROR_NONE = 0;
static u16 JDWP_ERROR_INVALID_THREAD = 10;
static u16 JDWP_ERROR_INVALID_THREAD_GROUP = 11;
static u16 JDWP_ERROR_INVALID_PRIORITY = 12;
static u16 JDWP_ERROR_THREAD_NOT_SUSPENDED = 13;
static u16 JDWP_ERROR_THREAD_SUSPENDED = 14;
static u16 JDWP_ERROR_INVALID_OBJECT = 20;
static u16 JDWP_ERROR_INVALID_CLASS = 21;
static u16 JDWP_ERROR_CLASS_NOT_PREPARED = 22;
static u16 JDWP_ERROR_INVALID_METHODID = 23;
static u16 JDWP_ERROR_INVALID_LOCATION = 24;
static u16 JDWP_ERROR_INVALID_FIELDID = 25;
static u16 JDWP_ERROR_INVALID_FRAMEID = 30;
static u16 JDWP_ERROR_NO_MORE_FRAMES = 31;
static u16 JDWP_ERROR_OPAQUE_FRAME = 32;
static u16 JDWP_ERROR_NOT_CURRENT_FRAME = 33;
static u16 JDWP_ERROR_TYPE_MISMATCH = 34;
static u16 JDWP_ERROR_INVALID_SLOT = 35;
static u16 JDWP_ERROR_DUPLICATE = 40;
static u16 JDWP_ERROR_NOT_FOUND = 41;
static u16 JDWP_ERROR_INVALID_MONITOR = 50;
static u16 JDWP_ERROR_NOT_MONITOR_OWNER = 51;
static u16 JDWP_ERROR_INTERRUPT = 52;
static u16 JDWP_ERROR_INVALID_CLASS_FORMAT = 60;
static u16 JDWP_ERROR_CIRCULAR_CLASS_DEFINITION = 61;
static u16 JDWP_ERROR_FAILS_VERIFICATION = 62;
static u16 JDWP_ERROR_ADD_METHOD_NOT_IMPLEMENTED = 63;
static u16 JDWP_ERROR_SCHEMA_CHANGE_NOT_IMPLEMENTED = 64;
static u16 JDWP_ERROR_INVALID_TYPESTATE = 65;
static u16 JDWP_ERROR_NOT_IMPLEMENTED = 99;
static u16 JDWP_ERROR_NULL_POINTER = 100;
static u16 JDWP_ERROR_ABSENT_INFORMATION = 101;
static u16 JDWP_ERROR_INVALID_EVENT_TYPE = 102;
static u16 JDWP_ERROR_ILLEGAL_ARGUMENT = 103;
static u16 JDWP_ERROR_OUT_OF_MEMORY = 110;
static u16 JDWP_ERROR_ACCESS_DENIED = 111;
static u16 JDWP_ERROR_VM_DEAD = 112;
static u16 JDWP_ERROR_INTERNAL = 113;
static u16 JDWP_ERROR_UNATTACHED_THREAD = 115;

/////
static u8 JDWP_EVENT_SINGLE_STEP = 1;
static u8 JDWP_EVENT_BREAKPOINT = 2;
static u8 JDWP_EVENT_FRAME_POP = 3; //not used in JDWP
static u8 JDWP_EVENT_EXCEPTION = 4;
static u8 JDWP_EVENT_USER_DEFINED = 5; //not used in JDWP
static u8 JDWP_EVENT_THREAD_START = 6;
static u8 JDWP_EVENT_THREAD_DEATH = 7;
static u8 JDWP_EVENT_THREAD_END = 7;
static u8 JDWP_EVENT_CLASS_PREPARE = 8;
static u8 JDWP_EVENT_CLASS_UNLOAD = 9;
static u8 JDWP_EVENT_CLASS_LOAD = 10; //not used in JDWP
static u8 JDWP_EVENT_FIELD_ACCESS = 20;
static u8 JDWP_EVENT_FIELD_MODIFICATION = 21;
static u8 JDWP_EVENT_EXCEPTION_CATCH = 30; //not used in JDWP
static u8 JDWP_EVENT_METHOD_ENTRY = 40;
static u8 JDWP_EVENT_METHOD_EXIT = 41;
static u8 JDWP_EVENT_VM_START = 90;
static u8 JDWP_EVENT_VM_INIT = 90;
static u8 JDWP_EVENT_VM_DEATH = 99;
static u8 JDWP_EVENT_VM_DISCONNECTED = 100; //Never sent by across JDWP


static c8 *JDWP_CLASS_REFERENCE = "javax/mini/jdwp/reflect/Reference";
static c8 *JDWP_CLASS_FIELD = "javax/mini/jdwp/reflect/Field";
static c8 *JDWP_CLASS_METHOD = "javax/mini/jdwp/reflect/Method";
static c8 *JDWP_CLASS_RUNTIME = "javax/mini/jdwp/reflect/Runtime";



//==============   tool
s32 jdwp_set_breakpoint(s32 setOrClear, Class *clazz, MethodInfo *methodInfo, s64 execIndex) {
    if (!methodInfo->breakpoint) {
        methodInfo->breakpoint = pairlist_create(4);
    }
    int i;
    for (i = 0; i < methodInfo->attributes_count; i++) {
        if (methodInfo->attributes[i].converted_code) {
            if (setOrClear) {
                pairlist_putl(methodInfo->breakpoint, (long) execIndex, 1);
                return JDWP_ERROR_NONE;
            } else {
                pairlist_removel(methodInfo->breakpoint, (long) execIndex);
                if (methodInfo->breakpoint->count == 0) {
                    jvm_free(methodInfo->breakpoint);
                    methodInfo->breakpoint = NULL;
                }
                return JDWP_ERROR_NONE;
            }
        }
    }
    return JDWP_ERROR_INVALID_LOCATION;
}
//========  native

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


s32 javax_mini_jdwp_vm_MemObject_readByte0(Runtime *runtime, Class *clazz) {
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

s32 javax_mini_jdwp_vm_MemObject_readShort0(Runtime *runtime, Class *clazz) {
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

s32 javax_mini_jdwp_vm_MemObject_readInt0(Runtime *runtime, Class *clazz) {
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

s32 javax_mini_jdwp_vm_MemObject_readLong0(Runtime *runtime, Class *clazz) {
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

s32 javax_mini_jdwp_vm_MemObject_readRefer0(Runtime *runtime, Class *clazz) {
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
        ptr = getFieldPtr_byName(ins, JDWP_CLASS_REFERENCE, "className", "Ljava/lang/String;");
        Instance *clsName = jstring_create(target->name, runtime);
        if (ptr)setFieldRefer(ptr, clsName);
        garbage_refer(clsName, ins);
        //
        ptr = getFieldPtr_byName(ins, JDWP_CLASS_REFERENCE, "accessFlags", "S");
        if (ptr)setFieldShort(ptr, target->cff.access_flags);
        //
        ptr = getFieldPtr_byName(ins, JDWP_CLASS_REFERENCE, "status", "I");
        //VERIFIED = 1;PREPARED = 2;INITIALIZED = 4;ERROR = 8;
        if (ptr)
            setFieldInt(ptr, target->status >= CLASS_STATUS_CLINITED ?
                             7 : (target->status >= CLASS_STATUS_PREPARED ? 3 : 1));
        //
        ptr = getFieldPtr_byName(ins, JDWP_CLASS_REFERENCE, "source", "Ljava/lang/String;");
        Instance *source = jstring_create(target->source, runtime);
        if (ptr)setFieldRefer(ptr, source);
        garbage_refer(source, ins);
        //
        s32 i;
        {
            ptr = getFieldPtr_byName(ins, JDWP_CLASS_REFERENCE, "fieldIds", "[J");
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
            ptr = getFieldPtr_byName(ins, JDWP_CLASS_REFERENCE, "methodIds", "[J");
            Instance *jarr = jarray_create(target->methodPool.method_used, DATATYPE_LONG, NULL);
            for (i = 0; i < target->methodPool.method_used; i++) {
                setFieldLong(&(jarr->arr_body[i * data_type_bytes[DATATYPE_LONG]]),
                             (u64) (long) &target->methodPool.method[i]);
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
        ptr = getFieldPtr_byName(ins, JDWP_CLASS_FIELD, "fieldName", "Ljava/lang/String;");
        Instance *fieldName = jstring_create(fieldInfo->name, runtime);
        if (ptr)setFieldRefer(ptr, fieldName);
        garbage_refer(fieldName, ins);
        //
        ptr = getFieldPtr_byName(ins, JDWP_CLASS_FIELD, "accessFlags", "S");
        if (ptr)setFieldShort(ptr, fieldInfo->access_flags);
        //
        ptr = getFieldPtr_byName(ins, JDWP_CLASS_FIELD, "signature", "Ljava/lang/String;");
        Instance *signature = jstring_create(fieldInfo->descriptor, runtime);
        if (ptr)setFieldRefer(ptr, signature);
        garbage_refer(signature, ins);
    }
    return 0;
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
        ptr = getFieldPtr_byName(ins, JDWP_CLASS_METHOD, "methodName", "Ljava/lang/String;");
        Instance *methodName = jstring_create(methodInfo->name, runtime);
        if (ptr)setFieldRefer(ptr, methodName);
        garbage_refer(methodName, ins);
        //
        ptr = getFieldPtr_byName(ins, JDWP_CLASS_METHOD, "signature", "Ljava/lang/String;");
        Instance *signature = jstring_create(methodInfo->descriptor, runtime);
        if (ptr)setFieldRefer(ptr, signature);
        garbage_refer(signature, ins);
        //
        ptr = getFieldPtr_byName(ins, JDWP_CLASS_METHOD, "accessFlags", "S");
        if (ptr)setFieldShort(ptr, methodInfo->access_flags);
        //
        s32 i;
        AttributeInfo *att = NULL;
        for (i = 0; i < methodInfo->attributes_count; i++) {
            if (methodInfo->attributes[i].converted_code) {
                att = &methodInfo->attributes[i];
            }
        }
        //
        ptr = getFieldPtr_byName(ins, JDWP_CLASS_METHOD, "codeStart", "J");
        if (ptr)if (att)setFieldLong(ptr, 0); else setFieldShort(ptr, -1);
        //
        ptr = getFieldPtr_byName(ins, JDWP_CLASS_METHOD, "codeEnd", "J");
        if (ptr)if (att)setFieldLong(ptr, att->converted_code->attribute_length); else setFieldLong(ptr, -1);
        //
        ptr = getFieldPtr_byName(ins, JDWP_CLASS_METHOD, "lines", "I");
        if (ptr)if (att)setFieldInt(ptr, att->converted_code->line_number_table_length); else setFieldInt(ptr, 0);
        //
        if (att) {
            ptr = getFieldPtr_byName(ins, JDWP_CLASS_METHOD, "lineNum", "[S");
            Instance *jarr = jarray_create(att->converted_code->line_number_table_length * 2, DATATYPE_SHORT, NULL);
            memcpy(jarr->arr_body, att->converted_code->line_number_table,
                   att->converted_code->line_number_table_length * 4);
            if (ptr)setFieldRefer(ptr, jarr);
            garbage_refer(jarr, ins);
        }
    }
    return 0;
}

s32 javax_mini_jdwp_reflect_Runtime_mapRuntime(Runtime *runtime, Class *clazz) {
    int pos = 0;
    Instance *ins = (Instance *) (runtime->localVariables + pos++)->refer;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    Runtime *target = (__refer) (long) l2d.l;
    if (ins && target) {
        u8 *ptr;
        //
        ptr = getFieldPtr_byName(ins, JDWP_CLASS_RUNTIME, "classId", "J");
        if (ptr)setFieldLong(ptr, (u64) (long) target->clazz);
        //
        ptr = getFieldPtr_byName(ins, JDWP_CLASS_RUNTIME, "sonId", "J");
        if (ptr)setFieldLong(ptr, (u64) (long) target->son);
        //
        ptr = getFieldPtr_byName(ins, JDWP_CLASS_RUNTIME, "pc", "J");
        if (ptr)setFieldLong(ptr, (u64) (long) target->pc);
        //
        ptr = getFieldPtr_byName(ins, JDWP_CLASS_RUNTIME, "byteCode", "J");
        if (ptr)setFieldLong(ptr, (u64) (long) target->bytecode);
        //
        ptr = getFieldPtr_byName(ins, JDWP_CLASS_RUNTIME, "methodId", "J");
        if (ptr)setFieldLong(ptr, (u64) (long) target->methodInfo);
        //

    }
    return 0;
}

static java_native_method method_jdwp_table[] = {
        {"javax/mini/jdwp/vm/JdwpNative",     "referenceTyepSize", "()I",                                   javax_mini_jdwp_vm_JdwpNative_referenceTyepSize},
        {"javax/mini/jdwp/vm/JdwpNative",     "referenceId",       "(Ljava/lang/Object;)J",                 javax_mini_jdwp_vm_JdwpNative_referenceId},
        {"javax/mini/jdwp/vm/JdwpNative",     "referenceObj",      "(J)Ljava/lang/Object;",                 javax_mini_jdwp_vm_JdwpNative_referenceObj},
        {"javax/mini/jdwp/vm/JdwpNative",     "getClasses",        "()[Ljava/lang/Class;",                  javax_mini_jdwp_vm_JdwpNative_getClasses},
        {"javax/mini/jdwp/vm/JdwpNative",     "getClassByName",    "(Ljava/lang/String;)Ljava/lang/Class;", javax_mini_jdwp_vm_JdwpNative_getClassByName},
        {"javax/mini/jdwp/vm/JdwpNative",     "setBreakPoint",     "(IBJJJ)I",                              javax_mini_jdwp_vm_JdwpNative_setBreakPoint},
        {"javax/mini/jdwp/vm/MemObject",      "readByte0",         "(JI)B",                                 javax_mini_jdwp_vm_MemObject_readByte0},
        {"javax/mini/jdwp/vm/MemObject",      "readShort0",        "(JI)S",                                 javax_mini_jdwp_vm_MemObject_readShort0},
        {"javax/mini/jdwp/vm/MemObject",      "readInt0",          "(JI)I",                                 javax_mini_jdwp_vm_MemObject_readInt0},
        {"javax/mini/jdwp/vm/MemObject",      "readLong0",         "(JI)J",                                 javax_mini_jdwp_vm_MemObject_readLong0},
        {"javax/mini/jdwp/vm/MemObject",      "readRefer0",        "(JI)J",                                 javax_mini_jdwp_vm_MemObject_readRefer0},
        {"javax/mini/jdwp/vm/JdwpThreads",    "getThreads",        "()[Ljava/lang/Thread;",                 javax_mini_jdwp_vm_JdwpThreads_getThreads},
        {"javax/mini/jdwp/vm/JdwpThreads",    "getStatus",         "(Ljava/lang/Thread;)I",                 javax_mini_jdwp_vm_JdwpThreads_getStatus},
        {"javax/mini/jdwp/vm/JdwpThreads",    "suspendThread",     "(Ljava/lang/Thread;)I",                 javax_mini_jdwp_vm_JdwpThreads_suspendThread},
        {"javax/mini/jdwp/vm/JdwpThreads",    "resumeThread",      "(Ljava/lang/Thread;)I",                 javax_mini_jdwp_vm_JdwpThreads_resumeThread},
        {"javax/mini/jdwp/vm/JdwpThreads",    "getSuspendCount",   "(Ljava/lang/Thread;)I",                 javax_mini_jdwp_vm_JdwpThreads_getSuspendCount},
        {"javax/mini/jdwp/vm/JdwpThreads",    "getFrameCount",     "(Ljava/lang/Thread;)I",                 javax_mini_jdwp_vm_JdwpThreads_getFrameCount},
        {"javax/mini/jdwp/vm/JdwpThreads",    "stopThread",        "(Ljava/lang/Thread;J)I",                javax_mini_jdwp_vm_JdwpThreads_stopThread},
        {"javax/mini/jdwp/vm/JdwpThreads",    "getTopRuntime",     "(Ljava/lang/Thread;)J",                 javax_mini_jdwp_vm_JdwpThreads_getTopRuntime},
        {"javax/mini/jdwp/reflect/Reference", "mapReference",      "(J)V",                                  javax_mini_jdwp_reflect_Reference_mapReference},
        {"javax/mini/jdwp/reflect/Field",     "mapField",          "(J)V",                                  javax_mini_jdwp_reflect_Field_mapField},
        {"javax/mini/jdwp/reflect/Method",    "mapMethod",         "(J)V",                                  javax_mini_jdwp_reflect_Method_mapMethod},
        {"javax/mini/jdwp/reflect/Runtime",   "mapRuntime",        "(J)V",                                  javax_mini_jdwp_reflect_Runtime_mapRuntime},

};


void reg_jdwp_native_lib() {
    native_reg_lib(&(method_jdwp_table[0]), sizeof(method_jdwp_table) / sizeof(java_native_method));
}

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
    ptr = getFieldPtr_byName(ins, JDWP_CLASS_LOCATION, "typeTag", "B");
    //CLASS = 1;INTERFACE = 2;ARRAY = 3;
    if (ptr)setFieldByte(ptr, 1);
    //
    ptr = getFieldPtr_byName(ins, JDWP_CLASS_LOCATION, "classID", "J");
    if (ptr)setFieldLong(ptr, (u64) (long) location_runtime->clazz);
    //
    ptr = getFieldPtr_byName(ins, JDWP_CLASS_LOCATION, "methodID", "J");
    if (ptr)setFieldLong(ptr, (u64) (long) location_runtime->methodInfo);
    //
    ptr = getFieldPtr_byName(ins, JDWP_CLASS_LOCATION, "execIndex", "J");
    if (ptr)setFieldLong(ptr, ((u64) (long) location_runtime->pc) - ((u64) (long) location_runtime->bytecode));
    return ins;
}

//==================== jdwp event
void event_on_class_prepar(Runtime *runtime, Class *clazz) {
    Class *e_cl = classes_load_get_c(JDWP_CLASS_EVENT, runtime);
    Instance *ins = instance_create(e_cl);
    instance_init(ins, runtime);
    //
    __refer ptr;
    //
    ptr = getFieldPtr_byName(ins, JDWP_CLASS_EVENT, "eventKind", "B");
    if (ptr)setFieldByte(ptr, 1);//CLASS=1;INTERFACE=2;ARRAY=3
    //
    ptr = getFieldPtr_byName(ins, JDWP_CLASS_EVENT, "thread", "J");
    if (ptr)setFieldLong(ptr, (u64) (long) runtime->threadInfo->jthread);
    //
    ptr = getFieldPtr_byName(ins, JDWP_CLASS_EVENT, "refTypeTag", "B");
    if (ptr)setFieldByte(ptr, 1);
    //
    ptr = getFieldPtr_byName(ins, JDWP_CLASS_EVENT, "typeID", "J");
    if (ptr)setFieldLong(ptr, (u64) (long) clazz);
    //
    ptr = getFieldPtr_byName(ins, JDWP_CLASS_EVENT, "signature", "Ljava/lang/String;");
    Utf8String *ustr = utf8_create_copy(clazz->name);
    utf8_insert(ustr, 0, 'L');
    utf8_append_c(ustr, ";");
    Instance *signature = jstring_create(ustr, runtime);
    utf8_destory(ustr);
    if (ptr)setFieldRefer(ptr, signature);
    garbage_refer(signature, ins);
    //
    ptr = getFieldPtr_byName(ins, JDWP_CLASS_EVENT, "status", "I");
    //VERIFIED = 1;PREPARED = 2;INITIALIZED = 4;ERROR = 8;
    if (ptr)
        setFieldInt(ptr, clazz->status >= CLASS_STATUS_CLINITED ?
                         7 : (clazz->status >= CLASS_STATUS_PREPARED ? 3 : 1));

    //
    jdwp_add_event(runtime, ins);
}

void event_on_class_unload(Runtime *runtime, Class *clazz) {
    Class *e_cl = classes_load_get_c(JDWP_CLASS_EVENT, runtime);
    Instance *ins = instance_create(e_cl);
    instance_init(ins, runtime);
    //
    __refer ptr;
    //
    ptr = getFieldPtr_byName(ins, JDWP_CLASS_EVENT, "eventKind", "B");
    if (ptr)setFieldByte(ptr, JDWP_EVENT_CLASS_UNLOAD);
    //
    ptr = getFieldPtr_byName(ins, JDWP_CLASS_EVENT, "signature", "Ljava/lang/String;");
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

void event_on_thread_start(Runtime *runtime, Instance *jthread) {
    Class *e_cl = classes_load_get_c(JDWP_CLASS_EVENT, runtime);
    Instance *ins = instance_create(e_cl);
    instance_init(ins, runtime);
    //
    __refer ptr;
    //
    ptr = getFieldPtr_byName(ins, JDWP_CLASS_EVENT, "eventKind", "B");
    if (ptr)setFieldByte(ptr, JDWP_EVENT_THREAD_START);
    //
    ptr = getFieldPtr_byName(ins, JDWP_CLASS_EVENT, "thread", "J");
    if (ptr)setFieldLong(ptr, (u64) (long) jthread);
    //
    jdwp_add_event(runtime, ins);
}

void event_on_thread_death(Runtime *runtime, Instance *jthread) {
    Class *e_cl = classes_load_get_c(JDWP_CLASS_EVENT, runtime);
    Instance *ins = instance_create(e_cl);
    instance_init(ins, runtime);
    //
    __refer ptr;
    //
    ptr = getFieldPtr_byName(ins, JDWP_CLASS_EVENT, "eventKind", "B");
    if (ptr)setFieldByte(ptr, JDWP_EVENT_THREAD_DEATH);
    //
    ptr = getFieldPtr_byName(ins, JDWP_CLASS_EVENT, "thread", "J");
    if (ptr)setFieldLong(ptr, (u64) (long) jthread);
    //
    jdwp_add_event(runtime, ins);
}


void event_on_breakpoint(Runtime *breakpoint_runtime) {
    Class *e_cl = classes_load_get_c(JDWP_CLASS_EVENT, breakpoint_runtime);
    Instance *ins = instance_create(e_cl);
    instance_init(ins, breakpoint_runtime);
    //
    __refer ptr;
    //
    ptr = getFieldPtr_byName(ins, JDWP_CLASS_EVENT, "eventKind", "B");
    if (ptr)setFieldByte(ptr, JDWP_EVENT_BREAKPOINT);
    //
    ptr = getFieldPtr_byName(ins, JDWP_CLASS_EVENT, "thread", "J");
    if (ptr)setFieldLong(ptr, (u64) (long) breakpoint_runtime->threadInfo->jthread);
    //
    ptr = getFieldPtr_byName(ins, JDWP_CLASS_EVENT, "loc", "Ljavax/mini/jdwp/type/Location;");
    Instance *loc = jdwp_get_location(breakpoint_runtime);
    if (ptr)setFieldRefer(ptr, loc);
    garbage_refer(loc, ins);
    //
    jdwp_add_event(breakpoint_runtime, ins);
}

#ifdef __cplusplus
}
#endif