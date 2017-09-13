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

static c8 *JDWP_REFERENCE = "javax/mini/jdwp/reflect/Reference";
static c8 *JDWP_FIELD = "javax/mini/jdwp/reflect/Field";
static c8 *JDWP_METHOD = "javax/mini/jdwp/reflect/Method";

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
        ptr = getFieldPtr_byName(ins, JDWP_REFERENCE, "accessFlags", "S");
        if (ptr)setFieldShort(ptr, target->cff.access_flags);
        ptr = getFieldPtr_byName(ins, JDWP_REFERENCE, "fieldCount", "I");
        if (ptr)setFieldInt(ptr, target->fieldPool.field_used);
        ptr = getFieldPtr_byName(ins, JDWP_REFERENCE, "methodCount", "I");
        if (ptr)setFieldInt(ptr, target->methodPool.method_used);
        //
        ptr = getFieldPtr_byName(ins, JDWP_REFERENCE, "source", "Ljava/lang/String;");
        Instance *source = jstring_create(target->source, runtime);
        if (ptr)setFieldRefer(ptr, source);
        garbage_refer(source, ins);
    }
    return 0;
}

s32 javax_mini_jdwp_reflect_Reference_mapField(Runtime *runtime, Class *clazz) {
    int pos = 0;
    Instance *ins = (Instance *) (runtime->localVariables + pos++)->refer;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    Class *target = (__refer) (long) l2d.l;
    s32 index = (runtime->localVariables + pos++)->integer;
    if (ins && target) {
        u8 *ptr;
        FieldInfo *fieldInfo = (FieldInfo *) (target->fieldPool.field + index);
        //
        ptr = getFieldPtr_byName(ins, JDWP_FIELD, "fieldId", "J");
        if (ptr)setFieldLong(ptr, (s64) (long) fieldInfo);
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

s32 javax_mini_jdwp_reflect_Reference_mapMethod(Runtime *runtime, Class *clazz) {
    int pos = 0;
    Instance *ins = (Instance *) (runtime->localVariables + pos++)->refer;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    Class *target = (__refer) (long) l2d.l;
    s32 index = (runtime->localVariables + pos++)->integer;
    if (ins && target) {
        u8 *ptr;
        MethodInfo *methodInfo = (MethodInfo *) (target->methodPool.method + index);
        //
        ptr = getFieldPtr_byName(ins, JDWP_METHOD, "methodId", "J");
        if (ptr)setFieldLong(ptr, (s64) (long) methodInfo);
        //
        ptr = getFieldPtr_byName(ins, JDWP_METHOD, "methodName", "Ljava/lang/String;");
        Instance *fieldName = jstring_create(methodInfo->name, runtime);
        if (ptr)setFieldRefer(ptr, fieldName);
        garbage_refer(fieldName, ins);
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
        if (ptr)if (att)setFieldShort(ptr, 0); else setFieldShort(ptr, -1);
        //
        ptr = getFieldPtr_byName(ins, JDWP_METHOD, "codeEnd", "J");
        if (ptr)if (att)setFieldLong(ptr, att->converted_code->attribute_length); else setFieldLong(ptr, -1);
        //
        ptr = getFieldPtr_byName(ins, JDWP_METHOD, "lines", "I");
        if (ptr)if (att)setFieldInt(ptr, att->converted_code->line_number_table_length); else setFieldInt(ptr, 0);
        //
        if (att) {
            ptr = getFieldPtr_byName(ins, JDWP_METHOD, "lineNum", "[S");
            Instance *jarr = jarray_create(att->converted_code->attribute_length * 2, DATATYPE_SHORT);
            memcpy(jarr->arr_body, att->converted_code->line_number_table, att->converted_code->attribute_length * 4);
            if (ptr)setFieldRefer(ptr, jarr);
            garbage_refer(jarr, ins);
        }
    }
    return 0;
}

static java_native_method method_jdwp_table[] = {
        {"javax/mini/jdwp/vm/JdwpNative",     "referenceTyepSize", "()I",                                   javax_mini_jdwp_vm_JdwpNative_referenceTyepSize},
        {"javax/mini/jdwp/vm/JdwpNative",     "referenceId",       "(Ljava/lang/Object;)J",                 javax_mini_jdwp_vm_JdwpNative_referenceId},
        {"javax/mini/jdwp/vm/JdwpNative",     "referenceObj",      "(J)Ljava/lang/Object;",                 javax_mini_jdwp_vm_JdwpNative_referenceObj},
        {"javax/mini/jdwp/vm/JdwpNative",     "getClasses",        "()[Ljava/lang/Class;",                  javax_mini_jdwp_vm_JdwpNative_getClasses},
        {"javax/mini/jdwp/vm/MemObject",      "readByte0",         "(JI)B",                                 javax_mini_jdwp_vm_MemObject_readByte0},
        {"javax/mini/jdwp/vm/MemObject",      "readShort0",        "(JI)S",                                 javax_mini_jdwp_vm_MemObject_readShort0},
        {"javax/mini/jdwp/vm/MemObject",      "readInt0",          "(JI)I",                                 javax_mini_jdwp_vm_MemObject_readInt0},
        {"javax/mini/jdwp/vm/MemObject",      "readLong0",         "(JI)J",                                 javax_mini_jdwp_vm_MemObject_readLong0},
        {"javax/mini/jdwp/vm/MemObject",      "readRefer0",        "(JI)J",                                 javax_mini_jdwp_vm_MemObject_readRefer0},
        {"javax/mini/jdwp/vm/JvmThreads",     "getThreads",        "()[Ljava/lang/Thread;",                 javax_mini_jdwp_vm_JvmThreads_getThreads},
        {"javax/mini/jdwp/vm/JvmThreads",     "getStatus",         "(Ljava/lang/Thread;)I",                 javax_mini_jdwp_vm_JvmThreads_getStatus},
        {"javax/mini/jdwp/vm/JvmThreads",     "suspendThread",     "(Ljava/lang/Thread;)I",                 javax_mini_jdwp_vm_JvmThreads_suspendThread},
        {"javax/mini/jdwp/vm/JvmThreads",     "resumeThread",      "(Ljava/lang/Thread;)I",                 javax_mini_jdwp_vm_JvmThreads_resumeThread},
        {"javax/mini/jdwp/vm/JvmThreads",     "getSuspendCount",   "(Ljava/lang/Thread;)I",                 javax_mini_jdwp_vm_JvmThreads_getSuspendCount},
        {"javax/mini/jdwp/vm/JvmThreads",     "getFrameCount",     "(Ljava/lang/Thread;)I",                 javax_mini_jdwp_vm_JvmThreads_getFrameCount},
        {"javax/mini/jdwp/vm/JvmThreads",     "stopThread",        "(Ljava/lang/Thread;J)I",                javax_mini_jdwp_vm_JvmThreads_stopThread},
        {"javax/mini/jdwp/vm/JvmThreads",     "getTopRuntime",     "(Ljava/lang/Thread;)J",                 javax_mini_jdwp_vm_JvmThreads_getTopRuntime},
        {"javax/mini/jdwp/reflect/Reference", "mapReference",      "(J)V",                                  javax_mini_jdwp_reflect_Reference_mapReference},
        {"javax/mini/jdwp/reflect/Reference", "mapField",          "(Ljavax/mini/jdwp/reflect/Field;JI)V",  javax_mini_jdwp_reflect_Reference_mapField},
        {"javax/mini/jdwp/reflect/Reference", "mapMethod",         "(Ljavax/mini/jdwp/reflect/Method;JI)V", javax_mini_jdwp_reflect_Reference_mapMethod},

};


void reg_jdwp_native_lib() {
    native_reg_lib(&(method_jdwp_table[0]), sizeof(method_jdwp_table) / sizeof(java_native_method));
}


#ifdef __cplusplus
}
#endif