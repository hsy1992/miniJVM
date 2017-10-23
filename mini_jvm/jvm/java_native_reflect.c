//
// Created by gust on 2017/9/1.
//

#include "jvm.h"
#include "java_native_std.h"
#include "garbage.h"
#include "jvm_util.h"
#include "java_native_reflect.h"


#ifdef __cplusplus
extern "C" {
#endif


//========  native============================================================================

s32 javax_mini_reflect_vm_RefNative_refIdSize(Runtime *runtime, Class *clazz) {
    push_int(runtime->stack, sizeof(__refer));

#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_reflect_vm_RefNative_refIdSize\n");
#endif
    return 0;
}

s32 javax_mini_reflect_vm_RefNative_obj2id(Runtime *runtime, Class *clazz) {
    Instance *ins = (Instance *) (runtime->localVariables + 0)->refer;
    Long2Double l2d;
    l2d.l = (u64) (long) ins;
    push_long(runtime->stack, l2d.l);

#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_reflect_vm_RefNative_obj2id\n");
#endif
    return 0;
}

s32 javax_mini_reflect_vm_RefNative_id2obj(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer r = (__refer) (long) l2d.l;//这里不能直接转化，可能在外部发生了数据精度丢失，只能从低位强转
    push_ref(runtime->stack, r);

#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_jdwp_RefNative_id2obj\n");
#endif
    return 0;
}

s32 javax_mini_reflect_vm_RefNative_getClasses(Runtime *runtime, Class *clazz) {
    s32 size = (s32)sys_classloader->classes->entries;

    Utf8String *ustr = utf8_create_c(STR_INS_JAVA_LANG_CLASS);
    Instance *jarr = jarray_create(size, DATATYPE_REFERENCE, ustr);
    utf8_destory(ustr);
    s32 i = 0;
    Long2Double l2d;
    HashtableIterator hti;
    hashtable_iterate(sys_classloader->classes, &hti);
    s32 bytes = data_type_bytes[DATATYPE_REFERENCE];
    for (; hashtable_iter_has_more(&hti);) {
        Utf8String *k = hashtable_iter_next_key(&hti);
        l2d.r = classes_get(k);
        jarray_set_field(jarr, i, &l2d);
        i++;
    }
    push_ref(runtime->stack, jarr);//先放入栈，再关联回收器，防止多线程回收
    garbage_refer(jarr, NULL);

#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_reflect_vm_RefNative_getClasses\n");
#endif
    return 0;
}

s32 javax_mini_reflect_vm_RefNative_getClassByName(Runtime *runtime, Class *clazz) {
    Instance *jstr = (Instance *) (runtime->localVariables + 0)->refer;
    Utf8String *ustr = utf8_create();
    jstring_2_utf8(jstr, ustr);
    Class *cl = classes_get(ustr);
    if (!cl) {
        cl = array_class_get(ustr);
    }
    utf8_destory(ustr);
    push_ref(runtime->stack, cl);
    return 0;
}


s32 javax_mini_reflect_vm_RefNative_setLocalVal(Runtime *runtime, Class *clazz) {
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

s32 javax_mini_reflect_vm_RefNative_getLocalVal(Runtime *runtime, Class *clazz) {
    int pos = 0;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    Runtime *r = (Runtime *) (__refer) (long) l2d.l;
    s32 slot = (runtime->localVariables + pos++)->integer;
    Instance *valuetype = (runtime->localVariables + pos++)->refer;

    c8 *ptr = getFieldPtr_byName_c(valuetype, JDWP_CLASS_VALUETYPE, "bytes", "C");
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

s32 javax_mini_reflect_vm_RefNative_getFieldVal(Runtime *runtime, Class *clazz) {
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
    s64 val = 0;
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


s32 javax_mini_reflect_MemAccess_readByte0(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer r = (__refer) (long) l2d.l;
    s32 offset = (runtime->localVariables + 2)->integer;
    u8 val = getFieldByte(((c8 *) r) + offset);
    push_int(runtime->stack, val);
#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_reflect_vm_MemObject_readByte0\n");
#endif
    return 0;
}

s32 javax_mini_reflect_MemAccess_readShort0(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer r = (__refer) (long) l2d.l;
    s32 offset = (runtime->localVariables + 2)->integer;
    u16 val = getFieldShort(((c8 *) r) + offset);
    push_int(runtime->stack, val);
#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_reflect_vm_MemObject_readShort0\n");
#endif
    return 0;
}

s32 javax_mini_reflect_MemAccess_readInt0(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer r = (__refer) (long) l2d.l;
    s32 offset = (runtime->localVariables + 2)->integer;
    s32 val = getFieldInt(((c8 *) r) + offset);
    push_int(runtime->stack, val);
#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_reflect_vm_MemObject_readInt0\n");
#endif
    return 0;
}

s32 javax_mini_reflect_MemAccess_readLong0(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer r = (__refer) (long) l2d.l;
    s32 offset = (runtime->localVariables + 2)->integer;
    s64 val = getFieldLong(((c8 *) r) + offset);
    push_long(runtime->stack, val);
#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_reflect_vm_MemObject_readLong0\n");
#endif
    return 0;
}

s32 javax_mini_reflect_MemAccess_readRefer0(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer r = (__refer) (long) l2d.l;
    s32 offset = (runtime->localVariables + 2)->integer;
    __refer val = getFieldRefer(((c8 *) r) + offset);
    push_long(runtime->stack, (u64) (long) val);
#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_reflect_vm_MemObject_readRefer0\n");
#endif
    return 0;
}

s32 javax_mini_reflect_vm_RefNative_getThreads(Runtime *runtime, Class *clazz) {
    thread_lock(&threadlist_lock);
    Utf8String *ustr = utf8_create_c(STR_INS_JAVA_LANG_THREAD);
    Instance *jarr = jarray_create(thread_list->length, DATATYPE_REFERENCE, ustr);
    utf8_destory(ustr);
    s32 i = 0;
    Long2Double l2d;

    for (i = 0; i < thread_list->length; i++) {
        Runtime *r = threadlist_get(i);
        l2d.r = r->threadInfo->jthread;
        jarray_set_field(jarr, i, &l2d);
    }
    push_ref(runtime->stack, jarr);//先放入栈，再关联回收器，防止多线程回收
    garbage_refer(jarr, NULL);
    thread_unlock(&threadlist_lock);
#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_reflect_vm_RefNative_getThreads\n");
#endif
    return 0;
}

s32 javax_mini_reflect_vm_RefNative_getStatus(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Runtime *trun = (Runtime *) jthread_get_threadq_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun)
        push_int(runtime->stack, trun->threadInfo->thread_status);
    else
        push_int(runtime->stack, THREAD_STATUS_ZOMBIE);
#if _JVM_DEBUG > 5
    jvm_printf("com_egls_jvm_RefNative_getStatus\n");
#endif
    return 0;
}

s32 javax_mini_reflect_vm_RefNative_suspendThread(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Runtime *trun = (Runtime *) jthread_get_threadq_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun) {
        jthread_suspend(trun);
        push_int(runtime->stack, 0);
    } else
        push_int(runtime->stack, 1);
#if _JVM_DEBUG > 5
    jvm_printf("com_egls_jvm_RefNative_suspendThread\n");
#endif
    return 0;
}

s32 javax_mini_reflect_vm_RefNative_resumeThread(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Runtime *trun = (Runtime *) jthread_get_threadq_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun) {
        if (trun->threadInfo->suspend_count > 0)trun->threadInfo->suspend_count--;
        push_int(runtime->stack, 0);
    } else
        push_int(runtime->stack, 1);
#if _JVM_DEBUG > 5
    jvm_printf("com_egls_jvm_RefNative_resumeThread\n");
#endif
    return 0;
}


s32 javax_mini_reflect_vm_RefNative_getSuspendCount(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Runtime *trun = (Runtime *) jthread_get_threadq_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun) {
        push_int(runtime->stack, trun->threadInfo->suspend_count);
    } else
        push_int(runtime->stack, 0);
#if _JVM_DEBUG > 5
    jvm_printf("com_egls_jvm_RefNative_getSuspendCount\n");
#endif
    return 0;
}

s32 javax_mini_reflect_vm_RefNative_getFrameCount(Runtime *runtime, Class *clazz) {
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

s32 javax_mini_reflect_vm_RefNative_stopThread(Runtime *runtime, Class *clazz) {
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
    jvm_printf("com_egls_jvm_RefNative_stopThread\n");
#endif
    return 0;
}


s32 javax_mini_reflect_vm_RefNative_getStackFrame(Runtime *runtime, Class *clazz) {
    Instance *thread = (Instance *) (runtime->localVariables + 0)->refer;
    Runtime *trun = (Runtime *) jthread_get_threadq_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun) {
        while (trun) {
            if (!trun->son)break;
            trun = trun->son;
        }
        push_long(runtime->stack, (u64) (long) trun);
    } else
        push_long(runtime->stack, 0);
#if _JVM_DEBUG > 5
    jvm_printf("javax_mini_reflect_vm_RefNative_getStackFrame %llx\n",(u64) (long) trun);
#endif
    return 0;
}

s32 javax_mini_reflect_Reference_mapReference(Runtime *runtime, Class *clazz) {
    int pos = 0;
    Instance *ins = (Instance *) (runtime->localVariables + pos++)->refer;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    Class *target = (__refer) (long) l2d.l;
    if (target) {
        c8 *ptr;
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_REFERENCE, "className", "Ljava/lang/String;");
        if (ptr) {
            Instance *clsName = jstring_create(target->name, runtime);
            setFieldRefer(ptr, clsName);
            garbage_refer(clsName, ins);
        }
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
        if (ptr) {
            Instance *source = jstring_create(target->source, runtime);
            setFieldRefer(ptr, source);
            garbage_refer(source, ins);
        }
        //
        s32 i;
        {
            ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_REFERENCE, "fieldIds", "[J");
            if (ptr) {
                Instance *jarr = jarray_create(target->fieldPool.field_used, DATATYPE_LONG, NULL);
                for (i = 0; i < target->fieldPool.field_used; i++) {
                    l2d.l = (u64) (long) &target->fieldPool.field[i];
                    jarray_set_field(jarr, i, &l2d);
                }
                setFieldRefer(ptr, jarr);
                garbage_refer(jarr, ins);
            }
        }
        //
        {
            ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_REFERENCE, "methodIds", "[J");
            if (ptr) {
                Instance *jarr = jarray_create(target->methodPool.method_used, DATATYPE_LONG, NULL);
                for (i = 0; i < target->methodPool.method_used; i++) {
                    l2d.l = (u64) (long) &target->methodPool.method[i];
                    jarray_set_field(jarr, i, &l2d);
                }
                setFieldRefer(ptr, jarr);
                garbage_refer(jarr, ins);
            }
        }
        //
        {
            ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_REFERENCE, "interfaces", "[J");
            if (ptr) {
                Instance *jarr = jarray_create(target->interfacePool.clasz_used, DATATYPE_LONG, NULL);
                for (i = 0; i < target->interfacePool.clasz_used; i++) {
                    Class *cl = classes_load_get(target->interfacePool.clasz[i].name, runtime);
                    l2d.l = (u64) (long) cl;
                    jarray_set_field(jarr, i, &l2d);
                }
                setFieldRefer(ptr, jarr);
                garbage_refer(jarr, ins);
            }
        }
    }
    return 0;
}

s32 javax_mini_reflect_Field_mapField(Runtime *runtime, Class *clazz) {
    int pos = 0;
    Instance *ins = (Instance *) (runtime->localVariables + pos++)->refer;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    FieldInfo *fieldInfo = (__refer) (long) l2d.l;
    if (ins && fieldInfo) {
        c8 *ptr;
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_FIELD, "fieldName", "Ljava/lang/String;");
        if (ptr) {
            Instance *fieldName = jstring_create(fieldInfo->name, runtime);
            setFieldRefer(ptr, fieldName);
            garbage_refer(fieldName, ins);
        }
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_FIELD, "accessFlags", "S");
        if (ptr)setFieldShort(ptr, fieldInfo->access_flags);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_FIELD, "signature", "Ljava/lang/String;");
        if (ptr) {
            Instance *signature = jstring_create(fieldInfo->descriptor, runtime);
            setFieldRefer(ptr, signature);
            garbage_refer(signature, ins);
        }
    }
    return 0;
}

Instance *localVarTable2java(Class *clazz, LocalVarTable *lvt, Runtime *runtime) {
    Class *cl = classes_load_get_c(JDWP_CLASS_LOCALVARTABLE, runtime);
    Instance *ins = instance_create(cl);
    instance_init(ins, runtime);
    if (ins && lvt) {
        c8 *ptr;
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_LOCALVARTABLE, "name", "Ljava/lang/String;");
        if (ptr) {
            Instance *name = jstring_create(get_utf8_string(clazz, lvt->name_index), runtime);
            setFieldRefer(ptr, name);
            garbage_refer(name, ins);
        }
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_LOCALVARTABLE, "signature", "Ljava/lang/String;");
        if (ptr) {
            Instance *signature = jstring_create(get_utf8_string(clazz, lvt->descriptor_index), runtime);
            setFieldRefer(ptr, signature);
            garbage_refer(signature, ins);
        }
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_LOCALVARTABLE, "codeIndex", "J");
        if (ptr)setFieldLong(ptr, lvt->start_pc);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_LOCALVARTABLE, "length", "I");
        if (ptr)setFieldInt(ptr, lvt->length);
    }
    return ins;
}

s32 javax_mini_reflect_Method_mapMethod(Runtime *runtime, Class *clazz) {
    int pos = 0;
    Instance *ins = (Instance *) (runtime->localVariables + pos++)->refer;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    MethodInfo *methodInfo = (__refer) (long) l2d.l;
    if (ins && methodInfo) {
        c8 *ptr;
        Long2Double l2d;
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "methodName", "Ljava/lang/String;");
        if (ptr) {
            Instance *methodName = jstring_create(methodInfo->name, runtime);
            setFieldRefer(ptr, methodName);
            garbage_refer(methodName, ins);
        }
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "signature", "Ljava/lang/String;");
        if (ptr) {
            Instance *signature = jstring_create(methodInfo->descriptor, runtime);
            setFieldRefer(ptr, signature);
            garbage_refer(signature, ins);
        }
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
        if (ptr)setFieldLong(ptr, att ? 0 : -1);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "codeEnd", "J");
        if (ptr)setFieldLong(ptr, att ? att->converted_code->attribute_length : -1);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "lines", "I");
        if (ptr)setFieldInt(ptr, att ? att->converted_code->line_number_table_length : 0);
        //
        if (att) {
            {
                ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "lineNum", "[S");
                if (ptr) {
                    Instance *jarr = jarray_create(att->converted_code->line_number_table_length * 2, DATATYPE_SHORT,
                                                   NULL);
                    memcpy(jarr->arr_body, att->converted_code->line_number_table,
                           att->converted_code->line_number_table_length * 4);
                    setFieldRefer(ptr, jarr);
                    garbage_refer(jarr, ins);
                }
            }
            {
                //
                c8 *table_type = "[Ljavax/mini/reflect/LocalVarTable;";
                ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "localVarTable", table_type);
                if (ptr) {
                    Utf8String *ustr = utf8_create_c(table_type);
                    utf8_substring(ustr, 1, ustr->length);
                    Instance *jarr = jarray_create(att->converted_code->local_var_table_length, DATATYPE_REFERENCE,
                                                   ustr);
                    utf8_destory(ustr);
                    for (i = 0; i < att->converted_code->local_var_table_length; i++) {
                        LocalVarTable *lvt = &att->converted_code->local_var_table[i];
                        l2d.r = localVarTable2java(methodInfo->_this_class, lvt, runtime);
                        jarray_set_field(jarr, i, &l2d);
                    }
                    setFieldRefer(ptr, jarr);
                    garbage_refer(jarr, ins);
                }
            }
        }

    }
    return 0;
}


s32 javax_mini_reflect_Method_invokeMethod(Runtime *runtime, Class *clazz) {
    s32 pos = 0;
    Instance *method_ins = (Instance *) (runtime->localVariables + pos++)->refer;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    MethodInfo *methodInfo = (__refer) (long) l2d.l;
    Instance *ins = (Instance *) (runtime->localVariables + pos++)->refer;
    Instance *argsArr = (Instance *) (runtime->localVariables + pos++)->refer;
    s32 ret = 0;
    if (methodInfo) {
        if (!(methodInfo->access_flags & ACC_STATIC)) {
            push_ref(runtime->stack, ins);
        }
        s32 i;
        for (i = 0; i < argsArr->arr_length; i++) {
            Long2Double l2d;
            jarray_get_field(argsArr, i, &l2d);
            switch (utf8_char_at(methodInfo->paraType, i)) {
                case '4': {
                    push_int(runtime->stack, (s32) l2d.l);
                    break;
                }
                case '8': {
                    push_long(runtime->stack, l2d.l);
                    break;
                }
                case 'R': {
                    push_ref(runtime->stack, l2d.r);
                    break;
                }
            }
        }
        ret = execute_method(methodInfo, runtime, methodInfo->_this_class);
        if (ret == RUNTIME_STATUS_NORMAL) {
            utf8_char ch = utf8_char_at(methodInfo->returnType, 0);
            s64 v = 0;
            if (ch == 'V') {
            } else if (isDataReferByTag(ch)) {
                v = (s64) (long) pop_ref(runtime->stack);
            } else if (isData8ByteByTag(ch)) {
                v = pop_long(runtime->stack);
            } else {
                v = pop_int(runtime->stack);
            }
            push_long(runtime->stack, v);
        }
    } else
        push_long(runtime->stack, 0);
    return ret;
}

s32 javax_mini_reflect_StackFrame_mapRuntime(Runtime *runtime, Class *clazz) {
    int pos = 0;
    Instance *ins = (Instance *) (runtime->localVariables + pos++)->refer;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    Runtime *target = (__refer) (long) l2d.l;
    if (ins && target) {
        c8 *ptr;
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_RUNTIME, "classId", "J");
        if (ptr)setFieldLong(ptr, (u64) (long) target->clazz);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_RUNTIME, "parentId", "J");
        if (ptr)
            if ((target->parent) && (target->parent->parent))
                setFieldLong(ptr, (u64) (long) target->parent);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_RUNTIME, "pc", "J");
        if (ptr)setFieldLong(ptr, (u64) (long) target->pc);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_RUNTIME, "byteCode", "J");
        if (ptr)setFieldLong(ptr, target->ca ? (u64) (long) target->ca->code : 0);
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

s32 javax_mini_reflect_Array_mapArray(Runtime *runtime, Class *clazz) {
    int pos = 0;
    Instance *ins = (Instance *) (runtime->localVariables + pos++)->refer;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    Instance *target = (__refer) (long) l2d.l;
    if (ins && target) {
        c8 *ptr;
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
        {"javax/mini/reflect/vm/RefNative", "refIdSize",       "()I",                                   javax_mini_reflect_vm_RefNative_refIdSize},
        {"javax/mini/reflect/vm/RefNative", "obj2id",          "(Ljava/lang/Object;)J",                 javax_mini_reflect_vm_RefNative_obj2id},
        {"javax/mini/reflect/vm/RefNative", "id2obj",          "(J)Ljava/lang/Object;",                 javax_mini_reflect_vm_RefNative_id2obj},
        {"javax/mini/reflect/vm/RefNative", "getClasses",      "()[Ljava/lang/Class;",                  javax_mini_reflect_vm_RefNative_getClasses},
        {"javax/mini/reflect/vm/RefNative", "getClassByName",  "(Ljava/lang/String;)Ljava/lang/Class;", javax_mini_reflect_vm_RefNative_getClassByName},
        {"javax/mini/reflect/vm/RefNative", "setLocalVal",     "(JIBJI)I",                              javax_mini_reflect_vm_RefNative_setLocalVal},
        {"javax/mini/reflect/vm/RefNative", "getLocalVal",     "(JILjavax/mini/jdwp/type/ValueType;)I", javax_mini_reflect_vm_RefNative_getLocalVal},
        {"javax/mini/reflect/vm/RefNative", "getFieldVal",     "(JJLjavax/mini/jdwp/type/ValueType;)I", javax_mini_reflect_vm_RefNative_getFieldVal},
        {"javax/mini/reflect/vm/RefNative", "getThreads",      "()[Ljava/lang/Thread;",                 javax_mini_reflect_vm_RefNative_getThreads},
        {"javax/mini/reflect/vm/RefNative", "getStatus",       "(Ljava/lang/Thread;)I",                 javax_mini_reflect_vm_RefNative_getStatus},
        {"javax/mini/reflect/vm/RefNative", "suspendThread",   "(Ljava/lang/Thread;)I",                 javax_mini_reflect_vm_RefNative_suspendThread},
        {"javax/mini/reflect/vm/RefNative", "resumeThread",    "(Ljava/lang/Thread;)I",                 javax_mini_reflect_vm_RefNative_resumeThread},
        {"javax/mini/reflect/vm/RefNative", "getSuspendCount", "(Ljava/lang/Thread;)I",                 javax_mini_reflect_vm_RefNative_getSuspendCount},
        {"javax/mini/reflect/vm/RefNative", "getFrameCount",   "(Ljava/lang/Thread;)I",                 javax_mini_reflect_vm_RefNative_getFrameCount},
        {"javax/mini/reflect/vm/RefNative", "stopThread",      "(Ljava/lang/Thread;J)I",                javax_mini_reflect_vm_RefNative_stopThread},
        {"javax/mini/reflect/vm/RefNative", "getStackFrame",   "(Ljava/lang/Thread;)J",                 javax_mini_reflect_vm_RefNative_getStackFrame},
        {"javax/mini/reflect/MemAccess",    "readByte0",       "(JI)B",                                 javax_mini_reflect_MemAccess_readByte0},
        {"javax/mini/reflect/MemAccess",    "readShort0",      "(JI)S",                                 javax_mini_reflect_MemAccess_readShort0},
        {"javax/mini/reflect/MemAccess",    "readInt0",        "(JI)I",                                 javax_mini_reflect_MemAccess_readInt0},
        {"javax/mini/reflect/MemAccess",    "readLong0",       "(JI)J",                                 javax_mini_reflect_MemAccess_readLong0},
        {"javax/mini/reflect/MemAccess",    "readRefer0",      "(JI)J",                                 javax_mini_reflect_MemAccess_readRefer0},
        {"javax/mini/reflect/Reference",    "mapReference",    "(J)V",                                  javax_mini_reflect_Reference_mapReference},
        {"javax/mini/reflect/Field",        "mapField",        "(J)V",                                  javax_mini_reflect_Field_mapField},
        {"javax/mini/reflect/Method",       "mapMethod",       "(J)V",                                  javax_mini_reflect_Method_mapMethod},
        {"javax/mini/reflect/Method",       "invokeMethod",    "(JLjava/lang/Object;[J)J",              javax_mini_reflect_Method_invokeMethod},
        {"javax/mini/reflect/StackFrame",   "mapRuntime",      "(J)V",                                  javax_mini_reflect_StackFrame_mapRuntime},
        {"javax/mini/reflect/Array",        "mapArray",        "(J)V",                                  javax_mini_reflect_Array_mapArray},

};


void reg_jdwp_native_lib() {
    native_reg_lib(&(method_jdwp_table[0]), sizeof(method_jdwp_table) / sizeof(java_native_method));
}

#ifdef __cplusplus
}
#endif
