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

    Class *cl = classes_load_get_c(STR_CLASS_JAVA_LANG_CLASS, runtime);
    Instance *jarr = jarray_create(size, DATATYPE_REFERENCE, cl);
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
    Utf8String* ustr = utf8_create();
    jstring_2_utf8(jstr, ustr);
    Class *cl = classes_get(ustr);
    push_ref(runtime->stack, cl);
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
    Class *cl = classes_load_get_c(STR_CLASS_JAVA_LANG_THREAD, runtime);
    Instance *jarr = jarray_create(thread_list->length, DATATYPE_REFERENCE, cl);
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

static c8 *JDWP_REFERENCE = "javax/mini/jdwp/reflect/Reference";
static c8 *JDWP_FIELD = "javax/mini/jdwp/reflect/Field";
static c8 *JDWP_METHOD = "javax/mini/jdwp/reflect/Method";
static c8 *JDWP_RUNTIME = "javax/mini/jdwp/reflect/Runtime";

s32 javax_mini_jdwp_reflect_Reference_mapReference(Runtime *runtime, Class *clazz) {
    int pos = 0;
    Instance *ins = (Instance *) (runtime->localVariables + pos++)->refer;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    Class *target = (__refer) (long) l2d.l;
    if (target) {
        u8 *ptr;
        ptr = getFieldPtr_byName(ins, JDWP_REFERENCE, "className", "Ljava/lang/String;");
        Instance *clsName = jstring_create(target->name, runtime);
        if (ptr)setFieldRefer(ptr, clsName);
        garbage_refer(clsName, ins);
        //
        ptr = getFieldPtr_byName(ins, JDWP_REFERENCE, "accessFlags", "S");
        if (ptr)setFieldShort(ptr, target->cff.access_flags);
        //
        ptr = getFieldPtr_byName(ins, JDWP_REFERENCE, "status", "I");
        //VERIFIED = 1;PREPARED = 2;INITIALIZED = 4;ERROR = 8;
        if (ptr)
            setFieldInt(ptr, target->status >= CLASS_STATUS_CLINITED ?
                             4 : (target->status >= CLASS_STATUS_PREPARED ? 2 : 1));
        //
        ptr = getFieldPtr_byName(ins, JDWP_REFERENCE, "source", "Ljava/lang/String;");
        Instance *source = jstring_create(target->source, runtime);
        if (ptr)setFieldRefer(ptr, source);
        garbage_refer(source, ins);
        //
        s32 i;
        {
            ptr = getFieldPtr_byName(ins, JDWP_REFERENCE, "fieldIds", "[J");
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
            ptr = getFieldPtr_byName(ins, JDWP_REFERENCE, "methodIds", "[J");
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
        ptr = getFieldPtr_byName(ins, JDWP_FIELD, "fieldName", "Ljava/lang/String;");
        Instance *fieldName = jstring_create(fieldInfo->name, runtime);
        if (ptr)setFieldRefer(ptr, fieldName);
        garbage_refer(fieldName, ins);
        //
        ptr = getFieldPtr_byName(ins, JDWP_FIELD, "accessFlags", "S");
        if (ptr)setFieldShort(ptr, fieldInfo->access_flags);
        //
        ptr = getFieldPtr_byName(ins, JDWP_FIELD, "signature", "Ljava/lang/String;");
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
        ptr = getFieldPtr_byName(ins, JDWP_METHOD, "methodName", "Ljava/lang/String;");
        Instance *methodName = jstring_create(methodInfo->name, runtime);
        if (ptr)setFieldRefer(ptr, methodName);
        garbage_refer(methodName, ins);
        //
        ptr = getFieldPtr_byName(ins, JDWP_METHOD, "signature", "Ljava/lang/String;");
        Instance *signature = jstring_create(methodInfo->descriptor, runtime);
        if (ptr)setFieldRefer(ptr, signature);
        garbage_refer(signature, ins);
        //
        ptr = getFieldPtr_byName(ins, JDWP_METHOD, "accessFlags", "S");
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
        ptr = getFieldPtr_byName(ins, JDWP_METHOD, "codeStart", "J");
        if (ptr)if (att)setFieldLong(ptr, 0); else setFieldShort(ptr, -1);
        //
        ptr = getFieldPtr_byName(ins, JDWP_METHOD, "codeEnd", "J");
        if (ptr)if (att)setFieldLong(ptr, att->converted_code->attribute_length); else setFieldLong(ptr, -1);
        //
        ptr = getFieldPtr_byName(ins, JDWP_METHOD, "lines", "I");
        if (ptr)if (att)setFieldInt(ptr, att->converted_code->line_number_table_length); else setFieldInt(ptr, 0);
        //
        if (att) {
            ptr = getFieldPtr_byName(ins, JDWP_METHOD, "lineNum", "[S");
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
        ptr = getFieldPtr_byName(ins, JDWP_RUNTIME, "classId", "J");
        if (ptr)setFieldLong(ptr, (u64) (long) target->clazz);
        //
        ptr = getFieldPtr_byName(ins, JDWP_RUNTIME, "sonId", "J");
        if (ptr)setFieldLong(ptr, (u64) (long) target->son);
        //
        ptr = getFieldPtr_byName(ins, JDWP_RUNTIME, "pc", "J");
        if (ptr)setFieldLong(ptr, (u64) (long) target->pc);
        //
        ptr = getFieldPtr_byName(ins, JDWP_RUNTIME, "byteCode", "J");
        if (ptr)setFieldLong(ptr, (u64) (long) target->bytecode);
        //
        ptr = getFieldPtr_byName(ins, JDWP_RUNTIME, "methodId", "J");
        if (ptr)setFieldLong(ptr, (u64) (long) target->methodInfo);
        //

    }
    return 0;
}

static java_native_method method_jdwp_table[] = {
        {"javax/mini/jdwp/vm/JdwpNative",     "referenceTyepSize", "()I",                                    javax_mini_jdwp_vm_JdwpNative_referenceTyepSize},
        {"javax/mini/jdwp/vm/JdwpNative",     "referenceId",       "(Ljava/lang/Object;)J",                  javax_mini_jdwp_vm_JdwpNative_referenceId},
        {"javax/mini/jdwp/vm/JdwpNative",     "referenceObj",      "(J)Ljava/lang/Object;",                  javax_mini_jdwp_vm_JdwpNative_referenceObj},
        {"javax/mini/jdwp/vm/JdwpNative",     "getClasses",        "()[Ljava/lang/Class;",                   javax_mini_jdwp_vm_JdwpNative_getClasses},
        {"javax/mini/jdwp/vm/JdwpNative",     "getClassByName",    "(Ljava/lang/String;)Ljava/lang/Class;", javax_mini_jdwp_vm_JdwpNative_getClassByName},
        {"javax/mini/jdwp/vm/MemObject",      "readByte0",         "(JI)B",                                  javax_mini_jdwp_vm_MemObject_readByte0},
        {"javax/mini/jdwp/vm/MemObject",      "readShort0",        "(JI)S",                                  javax_mini_jdwp_vm_MemObject_readShort0},
        {"javax/mini/jdwp/vm/MemObject",      "readInt0",          "(JI)I",                                  javax_mini_jdwp_vm_MemObject_readInt0},
        {"javax/mini/jdwp/vm/MemObject",      "readLong0",         "(JI)J",                                  javax_mini_jdwp_vm_MemObject_readLong0},
        {"javax/mini/jdwp/vm/MemObject",      "readRefer0",        "(JI)J",                                  javax_mini_jdwp_vm_MemObject_readRefer0},
        {"javax/mini/jdwp/vm/JdwpThreads",    "getThreads",        "()[Ljava/lang/Thread;",                  javax_mini_jdwp_vm_JdwpThreads_getThreads},
        {"javax/mini/jdwp/vm/JdwpThreads",    "getStatus",         "(Ljava/lang/Thread;)I",                  javax_mini_jdwp_vm_JdwpThreads_getStatus},
        {"javax/mini/jdwp/vm/JdwpThreads",    "suspendThread",     "(Ljava/lang/Thread;)I",                  javax_mini_jdwp_vm_JdwpThreads_suspendThread},
        {"javax/mini/jdwp/vm/JdwpThreads",    "resumeThread",      "(Ljava/lang/Thread;)I",                  javax_mini_jdwp_vm_JdwpThreads_resumeThread},
        {"javax/mini/jdwp/vm/JdwpThreads",    "getSuspendCount",   "(Ljava/lang/Thread;)I",                  javax_mini_jdwp_vm_JdwpThreads_getSuspendCount},
        {"javax/mini/jdwp/vm/JdwpThreads",    "getFrameCount",     "(Ljava/lang/Thread;)I",                  javax_mini_jdwp_vm_JdwpThreads_getFrameCount},
        {"javax/mini/jdwp/vm/JdwpThreads",    "stopThread",        "(Ljava/lang/Thread;J)I",                 javax_mini_jdwp_vm_JdwpThreads_stopThread},
        {"javax/mini/jdwp/vm/JdwpThreads",    "getTopRuntime",     "(Ljava/lang/Thread;)J",                  javax_mini_jdwp_vm_JdwpThreads_getTopRuntime},
        {"javax/mini/jdwp/reflect/Reference", "mapReference",      "(J)V",                                   javax_mini_jdwp_reflect_Reference_mapReference},
        {"javax/mini/jdwp/reflect/Field",     "mapField",          "(J)V",                                   javax_mini_jdwp_reflect_Field_mapField},
        {"javax/mini/jdwp/reflect/Method",    "mapMethod",         "(J)V",                                   javax_mini_jdwp_reflect_Method_mapMethod},
        {"javax/mini/jdwp/reflect/Runtime",   "mapRuntime",        "(J)V",                                   javax_mini_jdwp_reflect_Runtime_mapRuntime},

};


void reg_jdwp_native_lib() {
    native_reg_lib(&(method_jdwp_table[0]), sizeof(method_jdwp_table) / sizeof(java_native_method));
}

//=========================================================================
//                      Event
//=========================================================================

static c8 *JDWP_EVENT = "javax/mini/jdwp/events/Event";
static c8 *JDWP_EVENT = "javax/mini/jdwp/Location";
void event_class_prepar(Runtime *runtime, Class *clazz) {
    Class* cl=classes_load_get_c(JDWP_EVENT);
    Instance * ins=instance_create(cl);
    instance_init(ins,runtime);
    //
    ptr = getFieldPtr_byName(ins, JDWP_EVENT, "classId", "J");
    if (ptr)setFieldLong(ptr, (u64) (long) target->clazz);
}

#ifdef __cplusplus
}
#endif