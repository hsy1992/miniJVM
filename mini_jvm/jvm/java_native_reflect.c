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

s32 org_mini_reflect_vm_RefNative_refIdSize(Runtime *runtime, JClass *clazz) {
    push_int(runtime->stack, sizeof(__refer));

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("org_mini_reflect_vm_RefNative_refIdSize\n");
#endif
    return 0;
}

s32 org_mini_reflect_vm_RefNative_obj2id(Runtime *runtime, JClass *clazz) {
    Instance *ins = (Instance *) localvar_getRefer(runtime->localvar, 0);
    Long2Double l2d;
    l2d.l = (u64) (intptr_t) ins;
    push_long(runtime->stack, l2d.l);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("org_mini_reflect_vm_RefNative_obj2id\n");
#endif
    return 0;
}

s32 org_mini_reflect_vm_RefNative_id2obj(Runtime *runtime, JClass *clazz) {
    Long2Double l2d;
    l2d.l = localvar_getLong(runtime->localvar, 0);
    __refer r = (__refer) (intptr_t) l2d.l;//这里不能直接转化，可能在外部发生了数据精度丢失，只能从低位强转
    push_ref(runtime->stack, r);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("org_mini_jdwp_RefNative_id2obj\n");
#endif
    return 0;
}

s32 org_mini_reflect_vm_RefNative_getClasses(Runtime *runtime, JClass *clazz) {
    s32 size = (s32) sys_classloader->classes->entries;

    Utf8String *ustr = utf8_create_c(STR_CLASS_JAVA_LANG_CLASS);
    Instance *jarr = jarray_create_by_type_name(runtime, size, ustr);
    utf8_destory(ustr);
    s32 i = 0;
    HashtableIterator hti;
    hashtable_iterate(sys_classloader->classes, &hti);

    for (; hashtable_iter_has_more(&hti);) {
        Utf8String *k = hashtable_iter_next_key(&hti);
        JClass *r = classes_get(k);
        jarray_set_field(jarr, i, (intptr_t) insOfJavaLangClass_create_get(runtime, r));
        i++;
    }
    push_ref(runtime->stack, jarr);//先放入栈，再关联回收器，防止多线程回收

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("org_mini_reflect_vm_RefNative_getClasses\n");
#endif
    return 0;
}

s32 org_mini_reflect_vm_RefNative_getClassByName(Runtime *runtime, JClass *clazz) {
    Instance *jstr = (Instance *) localvar_getRefer(runtime->localvar, 0);
    Utf8String *ustr = utf8_create();
    jstring_2_utf8(jstr, ustr);
    utf8_replace_c(ustr, ".", "/");
    JClass *cl = classes_load_get(ustr, runtime);
    utf8_destory(ustr);
    push_ref(runtime->stack, insOfJavaLangClass_create_get(runtime, cl));
    return 0;
}


s32 org_mini_reflect_vm_RefNative_newWithoutInit(Runtime *runtime, JClass *clazz) {
    RuntimeStack *stack = runtime->stack;
    JClass *cl = insOfJavaLangClass_get_classHandle((Instance *) localvar_getRefer(runtime->localvar, 0));
    Instance *ins = NULL;
    s32 ret = 0;
    if (cl && !cl->mb.arr_type_index) {//class exists and not array class
        ins = instance_create(runtime, cl);
    }
    if (ins) {
        push_ref(stack, (__refer) ins);

    } else {
        Instance *exception = exception_create(JVM_EXCEPTION_INSTANTIATION, runtime);
        push_ref(stack, (__refer) exception);
        ret = RUNTIME_STATUS_EXCEPTION;
    }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_reflect_vm_RefNative_newWithoutInit  class:[%llx] ins:[%llx]\n", (s64) (intptr_t) cl, (s64) (intptr_t) ins);
#endif
    return ret;
}


s32 org_mini_reflect_vm_RefNative_setLocalVal(Runtime *runtime, JClass *clazz) {
    int pos = 0;
    Long2Double l2d;
    l2d.l = l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    Runtime *r = (Runtime *) (__refer) (intptr_t) l2d.l;
    s32 slot = localvar_getInt(runtime->localvar, pos++);
    u8 type = (u8) localvar_getInt(runtime->localvar, pos++);

    l2d.l = l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;

    s32 bytes = localvar_getInt(runtime->localvar, pos++);
    if (slot < r->localvar_count) {
        switch (bytes) {
            case 'R':
                localvar_setRefer(r->localvar, slot, (__refer) (intptr_t) l2d.l);
                break;
            case '8':
                localvar_setLong(r->localvar, slot, l2d.l);
                break;
            case '4':
            case '2':
            case '1':
                localvar_setInt(r->localvar, slot, (s32) l2d.l);
                break;
        }
        push_int(runtime->stack, 0);
    } else {
        push_int(runtime->stack, 1);
    }
    return 0;
}

s32 org_mini_reflect_vm_RefNative_getLocalVal(Runtime *runtime, JClass *clazz) {
    int pos = 0;
    Long2Double l2d;
    l2d.l = l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    Runtime *r = (Runtime *) (__refer) (intptr_t) l2d.l;
    s32 slot = localvar_getInt(runtime->localvar, pos++);
    Instance *valuetype = localvar_getRefer(runtime->localvar, pos++);

    c8 *ptr = getFieldPtr_byName_c(valuetype, JDWP_CLASS_VALUETYPE, "bytes", "C");
    u16 bytes = (u16) getFieldShort(ptr);
    ptr = getFieldPtr_byName_c(valuetype, JDWP_CLASS_VALUETYPE, "value", "J");
    if (slot < r->localvar_count) {
        switch (bytes) {
            case 'R':
                l2d.l = (s64) (intptr_t) localvar_getRefer(r->localvar, slot);
                break;
            case '8':
                l2d.l = localvar_getLong(runtime->localvar, slot);
//                l2d.i2l.i1 = localvar_getInt(r->localvar, slot);
//                l2d.i2l.i0 = localvar_getInt(r->localvar, slot + 1);
                break;
            case '4':
            case '2':
            case '1':
                l2d.l = localvar_getInt(r->localvar, slot);
                break;
        }
        setFieldLong(ptr, l2d.l);
        push_int(runtime->stack, 0);
    } else {
        push_int(runtime->stack, 1);
    }
    return 0;
}

s32 org_mini_reflect_ReflectField_getFieldVal(Runtime *runtime, JClass *clazz) {
    int pos = 0;
    Long2Double l2d;
    l2d.l = l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    Instance *ins = (Instance *) (__refer) (intptr_t) l2d.l;
    l2d.l = l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    FieldInfo *fi = (FieldInfo *) (__refer) (intptr_t) l2d.l;

    c8 *fptr;
    if (fi->access_flags & ACC_STATIC) {
        fptr = getStaticFieldPtr(fi);
    } else {
        fptr = getInstanceFieldPtr(ins, fi);
    }
    s64 val = 0;
    switch (fi->datatype_bytes) {
        case 'R':
            val = (s64) (intptr_t) getFieldRefer(fptr);

            break;
        case '8':
            val = getFieldLong(fptr);
            break;
        case '4':
            val = getFieldInt(fptr);
            break;
        case '2':
            if (fi->datatype_idx == DATATYPE_JCHAR) {
                val = getFieldChar(fptr);
            } else {
                val = getFieldShort(fptr);
            }
            break;
        case '1':
            val = getFieldByte(fptr);
            break;
    }

    push_long(runtime->stack, val);

    return 0;
}

s32 org_mini_reflect_ReflectField_setFieldVal(Runtime *runtime, JClass *clazz) {
    int pos = 0;
    Long2Double l2d;
    l2d.l = l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    Instance *ins = (Instance *) (__refer) (intptr_t) l2d.l;
    l2d.l = l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    FieldInfo *fi = (FieldInfo *) (__refer) (intptr_t) l2d.l;
    s64 val = localvar_getLong(runtime->localvar, pos);

    c8 *fptr;
    if (fi->access_flags & ACC_STATIC) {
        fptr = getStaticFieldPtr(fi);
    } else {
        fptr = getInstanceFieldPtr(ins, fi);
    }
    switch (fi->datatype_bytes) {
        case 'R':
            setFieldRefer(fptr, (__refer) (intptr_t) val);
            break;
        case '8':
            setFieldLong(fptr, (s64) val);
            break;
        case '4':
            setFieldInt(fptr, (s32) val);
            break;
        case '2':
            setFieldShort(fptr, (s16) val);
            break;
        case '1':
            setFieldByte(fptr, (s8) val);
            break;
    }

    return 0;
}


s32 org_mini_reflect_ReflectArray_getVal(Runtime *runtime, JClass *clazz) {
    s32 pos = 0;
    Long2Double l2d;
    l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    Instance *jarr = (__refer) (intptr_t) l2d.l;
    s32 index = localvar_getInt(runtime->localvar, pos++);
    s32 ret = jarray_check_exception(jarr, index, runtime);
    if (!ret) {
        s64 val = jarray_get_field(jarr, index);
        push_long(runtime->stack, val);
    } else {
        push_long(runtime->stack, 0);
    }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("org_mini_reflect_ReflectArray_getVal\n");
#endif
    return 0;
}


s32 org_mini_reflect_ReflectArray_newArray(Runtime *runtime, JClass *clazz) {
    s32 pos = 0;
    JClass *cl = insOfJavaLangClass_get_classHandle((Instance *) localvar_getRefer(runtime->localvar, pos++));
    s32 count = localvar_getInt(runtime->localvar, pos++);
    Instance *arr = jarray_create_by_class(runtime, count, cl);

    push_ref(runtime->stack, arr);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("org_mini_reflect_ReflectArray_newArray\n");
#endif
    return 0;
}

s32 org_mini_reflect_ReflectArray_multiNewArray(Runtime *runtime, JClass *clazz) {
    s32 pos = 0;
    JClass *cl = insOfJavaLangClass_get_classHandle((Instance *) localvar_getRefer(runtime->localvar, pos++));
    Instance *dimarr = localvar_getRefer(runtime->localvar, pos++);
    Utf8String *desc = utf8_create();
    if (cl->primitive) {
        utf8_insert(desc, 0, getDataTypeTagByName(cl->name));
    } else if (cl->mb.arr_type_index) {
        utf8_append(desc, cl->name);
    } else {
        utf8_append_c(desc, "L");
        utf8_append(desc, cl->name);
        utf8_append_c(desc, ";");
    }
    s32 i;
    for (i = 0; i < dimarr->arr_length; i++) {
        utf8_insert(desc, 0, '[');
    }

    Instance *arr = jarray_multi_create(runtime, (s32 *) dimarr->arr_body, dimarr->arr_length, desc, 0);

    push_ref(runtime->stack, arr);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("org_mini_reflect_ReflectArray_multiNewArray\n");
#endif
    return 0;
}


s32 org_mini_reflect_ReflectArray_setVal(Runtime *runtime, JClass *clazz) {
    s32 pos = 0;
    Long2Double l2d;
    l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    Instance *jarr = (__refer) (intptr_t) l2d.l;
    s32 index = localvar_getInt(runtime->localvar, pos++);
    s64 val = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    s32 ret = jarray_check_exception(jarr, index, runtime);
    if (!ret) jarray_set_field(jarr, index, val);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("org_mini_reflect_ReflectArray_setVal\n");
#endif
    return 0;
}

struct _list_getthread_para {
    s32 i;
    Instance *jarr;
    s64 val;
};

void _list_iter_getthread(ArrayListValue value, void *para) {
    if (value) {
        Runtime *r = value;
        struct _list_getthread_para *p = para;
        p->val = (intptr_t) r->threadInfo->jthread;
        jarray_set_field(p->jarr, p->i, p->val);
    }
}

s32 org_mini_reflect_vm_RefNative_getThreads(Runtime *runtime, JClass *clazz) {
//    garbage_thread_lock();
    Utf8String *ustr = utf8_create_c(STR_CLASS_JAVA_LANG_THREAD);
    Instance *jarr = jarray_create_by_type_name(runtime, thread_list->length, ustr);
    utf8_destory(ustr);

    struct _list_getthread_para para;
    para.i = 0;
    para.jarr = jarr;
    arraylist_iter_safe(thread_list, _list_iter_getthread, &para);
//    s32 i = 0;
//    for (i = 0; i < thread_list->length; i++) {
//        Runtime *r = threadlist_get(i);
//        if(r) {
//            l2d.r = r->threadInfo->jthread;
//            jarray_set_field(jarr, i, &l2d);
//        }
//    }
    push_ref(runtime->stack, jarr);//先放入栈，再关联回收器，防止多线程回收
//    garbage_thread_unlock();
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("org_mini_reflect_vm_RefNative_getThreads\n");
#endif
    return 0;
}

s32 org_mini_reflect_vm_RefNative_getStatus(Runtime *runtime, JClass *clazz) {
    Instance *thread = (Instance *) localvar_getRefer(runtime->localvar, 0);
    Runtime *trun = (Runtime *) jthread_get_stackframe_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun)
        push_int(runtime->stack, trun->threadInfo->thread_status);
    else
        push_int(runtime->stack, THREAD_STATUS_ZOMBIE);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("com_egls_jvm_RefNative_getStatus\n");
#endif
    return 0;
}

s32 org_mini_reflect_vm_RefNative_suspendThread(Runtime *runtime, JClass *clazz) {
    Instance *thread = (Instance *) localvar_getRefer(runtime->localvar, 0);
    Runtime *trun = (Runtime *) jthread_get_stackframe_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun) {
        jthread_suspend(trun);
        push_int(runtime->stack, 0);
    } else
        push_int(runtime->stack, 1);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("com_egls_jvm_RefNative_suspendThread\n");
#endif
    return 0;
}

s32 org_mini_reflect_vm_RefNative_resumeThread(Runtime *runtime, JClass *clazz) {
    Instance *thread = (Instance *) localvar_getRefer(runtime->localvar, 0);
    Runtime *trun = (Runtime *) jthread_get_stackframe_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun) {
        if (trun->threadInfo->suspend_count > 0)trun->threadInfo->suspend_count--;
        push_int(runtime->stack, 0);
    } else
        push_int(runtime->stack, 1);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("com_egls_jvm_RefNative_resumeThread\n");
#endif
    return 0;
}


s32 org_mini_reflect_vm_RefNative_getSuspendCount(Runtime *runtime, JClass *clazz) {
    Instance *thread = (Instance *) localvar_getRefer(runtime->localvar, 0);
    Runtime *trun = (Runtime *) jthread_get_stackframe_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun) {
        push_int(runtime->stack, trun->threadInfo->suspend_count);
    } else
        push_int(runtime->stack, 0);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("com_egls_jvm_RefNative_getSuspendCount\n");
#endif
    return 0;
}

s32 org_mini_reflect_vm_RefNative_getFrameCount(Runtime *runtime, JClass *clazz) {
    Instance *thread = (Instance *) localvar_getRefer(runtime->localvar, 0);
    Runtime *trun = (Runtime *) jthread_get_stackframe_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    int i = 0;
    while (trun) {
        i++;
        trun = trun->son;
    }
    i--;
    push_int(runtime->stack, i);
    return 0;
}

s32 org_mini_reflect_vm_RefNative_stopThread(Runtime *runtime, JClass *clazz) {
    Instance *thread = (Instance *) localvar_getRefer(runtime->localvar, 0);
    Long2Double l2d;
    l2d.l = localvar_getLong(runtime->localvar, 1);
    Instance *ins = (__refer) (intptr_t) l2d.l;
    Runtime *trun = (Runtime *) jthread_get_stackframe_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun) {
        push_int(runtime->stack, 0);
    } else
        push_int(runtime->stack, 0);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("com_egls_jvm_RefNative_stopThread\n");
#endif
    return 0;
}


s32 org_mini_reflect_vm_RefNative_getStackFrame(Runtime *runtime, JClass *clazz) {
    Instance *thread = (Instance *) localvar_getRefer(runtime->localvar, 0);
    Runtime *trun = (Runtime *) jthread_get_stackframe_value(thread);//线程结束之后会清除掉runtime,因为其是一个栈变量，不可再用
    if (trun) {
        while (trun) {
            if (!trun->son)break;
            trun = trun->son;
        }
        push_long(runtime->stack, (u64) (intptr_t) trun->parent);
    } else
        push_long(runtime->stack, 0);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("org_mini_reflect_vm_RefNative_getStackFrame %llx\n", (u64) (intptr_t) trun);
#endif
    return 0;
}

s32 org_mini_reflect_vm_RefNative_getGarbageReferedObjs(Runtime *runtime, JClass *clazz) {
    s32 size = (s32) collector->runtime_refer_copy->length;

    Utf8String *ustr = utf8_create_c(STR_CLASS_JAVA_LANG_OBJECT);
    Instance *jarr = jarray_create_by_type_name(runtime, size, ustr);
    utf8_destory(ustr);
    s32 i = 0;

    for (i = 0; i < collector->runtime_refer_copy->length; i++) {
        __refer r = arraylist_get_value(collector->runtime_refer_copy, i);
        jarray_set_field(jarr, i, (intptr_t) r);
    }
    push_ref(runtime->stack, jarr);//先放入栈，再关联回收器，防止多线程回收

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("org_mini_reflect_vm_RefNative_getGarbageReferedObjs %llx\n", (u64) (intptr_t) jarr);
#endif
    return 0;
}

s32 org_mini_reflect_vm_RefNative_getGarbageStatus(Runtime *runtime, JClass *clazz) {

    push_int(runtime->stack, collector->_garbage_thread_status);//先放入栈，再关联回收器，防止多线程回收

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("org_mini_reflect_vm_RefNative_getGarbageStatus %d\n", collector->_garbage_thread_status);
#endif
    return 0;
}

s32 org_mini_reflect_vm_RefNative_defineClass(Runtime *runtime, JClass *clazz) {
    s32 pos = 0;
    Instance *cloader = localvar_getRefer(runtime->localvar, pos);
    pos += 2;
    Instance *namejstr = localvar_getRefer(runtime->localvar, pos);
    pos += 2;
    Instance *bytesarr = localvar_getRefer(runtime->localvar, pos);
    pos += 2;
    s32 offset = localvar_getInt(runtime->localvar, pos++);
    s32 len = localvar_getInt(runtime->localvar, pos++);

    ByteBuf *bytebuf = bytebuf_create(len);
    bytebuf_read_batch(bytebuf, bytesarr->arr_body + offset, len);
    JClass *cl = resole_class(bytebuf, runtime);
    cl->jClassLoader = cloader;
    bytebuf_destory(bytebuf);

    Instance *clIns = insOfJavaLangClass_create_get(runtime, cl);

    push_ref(runtime->stack, clIns);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("org_mini_reflect_vm_RefNative_defineClass %d\n", collector->_garbage_thread_status);
#endif
    return 0;
}

s32 org_mini_reflect_ReflectClass_mapReference(Runtime *runtime, JClass *clazz) {
    int pos = 0;
    Instance *ins = (Instance *) localvar_getRefer(runtime->localvar, pos++);
    Long2Double l2d;
    l2d.l = l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    JClass *target = (__refer) (intptr_t) l2d.l;
    if (target) {
        c8 *ptr;
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_REFERENCE, "className", STR_INS_JAVA_LANG_STRING);
        if (ptr) {
            Instance *clsName = jstring_create(target->name, runtime);
            setFieldRefer(ptr, clsName);
        }
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_REFERENCE, "superclass", "J");
        if (ptr)
            setFieldLong(ptr, (s64) (intptr_t) getSuperClass(target));
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
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_REFERENCE, "source", STR_INS_JAVA_LANG_STRING);
        if (ptr) {
            Instance *source = jstring_create(target->source, runtime);
            setFieldRefer(ptr, source);
        }
        //
        s32 i;
        {
            ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_REFERENCE, "fieldIds", "[J");
            if (ptr) {
                Instance *jarr = jarray_create_by_type_index(runtime, target->fieldPool.field_used, DATATYPE_LONG);
                setFieldRefer(ptr, jarr);
                for (i = 0; i < target->fieldPool.field_used; i++) {
                    s64 val = (u64) (intptr_t) &target->fieldPool.field[i];
                    jarray_set_field(jarr, i, val);
                }
            }
        }
        //
        {
            ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_REFERENCE, "methodIds", "[J");
            if (ptr) {
                Instance *jarr = jarray_create_by_type_index(runtime, target->methodPool.method_used, DATATYPE_LONG);
                setFieldRefer(ptr, jarr);
                for (i = 0; i < target->methodPool.method_used; i++) {
                    s64 val = (u64) (intptr_t) &target->methodPool.method[i];
                    jarray_set_field(jarr, i, val);
                }
            }
        }
        //
        {
            ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_REFERENCE, "interfaces", "[J");
            if (ptr) {
                Instance *jarr = jarray_create_by_type_index(runtime, target->interfacePool.clasz_used, DATATYPE_LONG);
                setFieldRefer(ptr, jarr);
                for (i = 0; i < target->interfacePool.clasz_used; i++) {
                    JClass *cl = classes_load_get(target->interfacePool.clasz[i].name, runtime);
                    s64 val = (u64) (intptr_t) cl;
                    jarray_set_field(jarr, i, val);
                }
            }
        }
    }
    return 0;
}

s32 org_mini_reflect_ReflectField_mapField(Runtime *runtime, JClass *clazz) {
    int pos = 0;
    Instance *ins = (Instance *) localvar_getRefer(runtime->localvar, pos++);
    Long2Double l2d;
    l2d.l = l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    FieldInfo *fieldInfo = (__refer) (intptr_t) l2d.l;
    if (ins && fieldInfo) {
        c8 *ptr;
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_FIELD, "fieldName", STR_INS_JAVA_LANG_STRING);
        if (ptr) {
            Instance *fieldName = jstring_create(fieldInfo->name, runtime);
            setFieldRefer(ptr, fieldName);
        }
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_FIELD, "accessFlags", "S");
        if (ptr)setFieldShort(ptr, fieldInfo->access_flags);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_FIELD, "signature", STR_INS_JAVA_LANG_STRING);
        if (ptr) {
            Instance *signature = jstring_create(fieldInfo->descriptor, runtime);
            setFieldRefer(ptr, signature);
        }
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_ARRAY, "type", "B");
        if (ptr)setFieldByte(ptr, (s8) utf8_char_at(fieldInfo->descriptor, 1));
    }
    return 0;
}

Instance *localVarTable2java(JClass *clazz, LocalVarTable *lvt, Runtime *runtime) {
    JClass *cl = classes_load_get_c(JDWP_CLASS_LOCALVARTABLE, runtime);
    Instance *ins = instance_create(runtime, cl);
    gc_refer_hold(ins);// hold by manual
    instance_init(ins, runtime);

    if (ins && lvt) {
        c8 *ptr;
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_LOCALVARTABLE, "name", STR_INS_JAVA_LANG_STRING);
        if (ptr) {
            Instance *name = jstring_create(class_get_utf8_string(clazz, lvt->name_index), runtime);
            setFieldRefer(ptr, name);
        }
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_LOCALVARTABLE, "signature", STR_INS_JAVA_LANG_STRING);
        if (ptr) {
            Instance *signature = jstring_create(class_get_utf8_string(clazz, lvt->descriptor_index), runtime);
            setFieldRefer(ptr, signature);
        }
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_LOCALVARTABLE, "codeIndex", "J");
        if (ptr)setFieldLong(ptr, lvt->start_pc);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_LOCALVARTABLE, "length", "I");
        if (ptr)setFieldInt(ptr, lvt->length);
    }
    gc_refer_release(ins);//release by manual
    return ins;
}

s32 org_mini_reflect_ReflectMethod_mapMethod(Runtime *runtime, JClass *clazz) {
    int pos = 0;
    Instance *ins = (Instance *) localvar_getRefer(runtime->localvar, pos++);
    Long2Double l2d;
    l2d.l = l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    MethodInfo *methodInfo = (__refer) (intptr_t) l2d.l;
    if (ins && methodInfo) {
        c8 *ptr;

        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "methodName", STR_INS_JAVA_LANG_STRING);
        if (ptr) {
            Instance *methodName = jstring_create(methodInfo->name, runtime);
            setFieldRefer(ptr, methodName);
        }
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "signature", STR_INS_JAVA_LANG_STRING);
        if (ptr) {
            Instance *signature = jstring_create(methodInfo->descriptor, runtime);
            setFieldRefer(ptr, signature);
        }
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "accessFlags", "S");
        if (ptr)setFieldShort(ptr, methodInfo->access_flags);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "argCnt", "I");
        if (ptr)setFieldInt(ptr, methodInfo->para_slots);
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
                    Instance *jarr = jarray_create_by_type_index(runtime, att->converted_code->line_number_table_length * 2, DATATYPE_SHORT);
                    setFieldRefer(ptr, jarr);
                    memcpy(jarr->arr_body, att->converted_code->line_number_table,
                           att->converted_code->line_number_table_length * 4);
                }
            }
            {
                //
                c8 *table_type = "org/mini/reflect/LocalVarTable";
                ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_METHOD, "localVarTable", table_type);
                if (ptr) {
                    Utf8String *ustr = utf8_create_c(table_type);
                    utf8_substring(ustr, 1, ustr->length);
                    Instance *jarr = jarray_create_by_type_name(runtime, att->converted_code->local_var_table_length, ustr);
                    setFieldRefer(ptr, jarr);
                    utf8_destory(ustr);
                    for (i = 0; i < att->converted_code->local_var_table_length; i++) {
                        LocalVarTable *lvt = &att->converted_code->local_var_table[i];
                        s64 val = (intptr_t) localVarTable2java(methodInfo->_this_class, lvt, runtime);
                        jarray_set_field(jarr, i, val);

                    }
                }
            }
        }

    }
    return 0;
}


s32 org_mini_reflect_ReflectMethod_invokeMethod(Runtime *runtime, JClass *clazz) {
    s32 pos = 0;
    Instance *method_ins = (Instance *) localvar_getRefer(runtime->localvar, pos++);
    Long2Double l2d;
    l2d.l = l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    MethodInfo *methodInfo = (__refer) (intptr_t) l2d.l;
    Instance *ins = (Instance *) localvar_getRefer(runtime->localvar, pos++);
    Instance *argsArr = (Instance *) localvar_getRefer(runtime->localvar, pos++);
    s32 ret = 0;
    if (methodInfo) {
        if (!(methodInfo->access_flags & ACC_STATIC)) {
            push_ref(runtime->stack, ins);
        }
        s32 i;
        for (i = 0; i < argsArr->arr_length; i++) {
            s64 val = jarray_get_field(argsArr, i);
            switch (utf8_char_at(methodInfo->paraType, i)) {
                case '4': {
                    push_int(runtime->stack, (s32) val);
                    break;
                }
                case '8': {
                    push_long(runtime->stack, val);
                    break;
                }
                case 'R': {
                    push_ref(runtime->stack, (__refer) (intptr_t) val);
                    break;
                }
            }
        }
        ret = execute_method_impl(methodInfo, runtime, methodInfo->_this_class);
        if (ret == RUNTIME_STATUS_NORMAL) {
            utf8_char ch = utf8_char_at(methodInfo->returnType, 0);
            s64 v = 0;
            if (ch == 'V') {
            } else if (isDataReferByTag(ch)) {
                v = (s64) (intptr_t) pop_ref(runtime->stack);
            } else if (isData8ByteByTag(ch)) {
                v = pop_long(runtime->stack);
            } else {
                v = pop_int(runtime->stack);
            }
            push_long(runtime->stack, v);
        } else {
            print_exception(runtime);
        }
    } else
        push_long(runtime->stack, 0);
    return ret;
}

s32 org_mini_reflect_StackFrame_mapRuntime(Runtime *runtime, JClass *clazz) {
    int pos = 0;
    Instance *ins = (Instance *) localvar_getRefer(runtime->localvar, pos++);
    Long2Double l2d;
    l2d.l = l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    Runtime *target = (__refer) (intptr_t) l2d.l;
    if (ins && target) {
        c8 *ptr;
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_RUNTIME, "classId", "J");
        if (ptr)setFieldLong(ptr, (u64) (intptr_t) target->clazz);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_RUNTIME, "parentId", "J");
        if (ptr)
            if ((target->parent) && (target->parent->parent))
                setFieldLong(ptr, (u64) (intptr_t) target->parent);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_RUNTIME, "pc", "J");
        if (ptr)setFieldLong(ptr, (u64) (intptr_t) target->pc);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_RUNTIME, "byteCode", "J");
        if (ptr)setFieldLong(ptr, target->ca ? (u64) (intptr_t) target->ca->code : 0);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_RUNTIME, "methodId", "J");
        if (ptr)setFieldLong(ptr, (u64) (intptr_t) target->method);
        //
        if (target->method && !(target->method->access_flags & ACC_STATIC)) {//top runtime method is null
            ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_RUNTIME, "localThis", "J");
            if (ptr)setFieldLong(ptr, (s64) (intptr_t) localvar_getRefer(target->localvar, 0));
        }
    }
    return 0;
}

s32 org_mini_reflect_ReflectArray_mapArray(Runtime *runtime, JClass *clazz) {
    int pos = 0;
    Instance *ins = (Instance *) localvar_getRefer(runtime->localvar, pos++);
    Long2Double l2d;
    l2d.l = l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    Instance *target = (__refer) (intptr_t) l2d.l;
    if (ins && target) {
        c8 *ptr;
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_ARRAY, "length", "I");
        if (ptr)setFieldInt(ptr, target->arr_length);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_ARRAY, "arr_addr", "J");
        if (ptr)setFieldLong(ptr, (u64) (intptr_t) target->arr_body);
        //
        ptr = getFieldPtr_byName_c(ins, JDWP_CLASS_ARRAY, "type", "B");
        if (ptr)setFieldByte(ptr, (s8) utf8_char_at(target->mb.clazz->name, 1));
        //

    }
    return 0;
}

static java_native_method method_jdwp_table[] = {
        {"org/mini/reflect/vm/RefNative",  "refIdSize",             "()I",                                                              org_mini_reflect_vm_RefNative_refIdSize},
        {"org/mini/reflect/vm/RefNative",  "obj2id",                "(Ljava/lang/Object;)J",                                            org_mini_reflect_vm_RefNative_obj2id},
        {"org/mini/reflect/vm/RefNative",  "id2obj",                "(J)Ljava/lang/Object;",                                            org_mini_reflect_vm_RefNative_id2obj},
        {"org/mini/reflect/vm/RefNative",  "getClasses",            "()[Ljava/lang/Class;",                                             org_mini_reflect_vm_RefNative_getClasses},
        {"org/mini/reflect/vm/RefNative",  "getClassByName",        "(Ljava/lang/String;)Ljava/lang/Class;",                            org_mini_reflect_vm_RefNative_getClassByName},
        {"org/mini/reflect/vm/RefNative",  "newWithoutInit",        "(Ljava/lang/Class;)Ljava/lang/Object;",                            org_mini_reflect_vm_RefNative_newWithoutInit},
        {"org/mini/reflect/vm/RefNative",  "setLocalVal",           "(JIBJI)I",                                                         org_mini_reflect_vm_RefNative_setLocalVal},
        {"org/mini/reflect/vm/RefNative",  "getLocalVal",           "(JILorg/mini/jdwp/type/ValueType;)I",                              org_mini_reflect_vm_RefNative_getLocalVal},
        {"org/mini/reflect/vm/RefNative",  "getThreads",            "()[Ljava/lang/Thread;",                                            org_mini_reflect_vm_RefNative_getThreads},
        {"org/mini/reflect/vm/RefNative",  "getStatus",             "(Ljava/lang/Thread;)I",                                            org_mini_reflect_vm_RefNative_getStatus},
        {"org/mini/reflect/vm/RefNative",  "suspendThread",         "(Ljava/lang/Thread;)I",                                            org_mini_reflect_vm_RefNative_suspendThread},
        {"org/mini/reflect/vm/RefNative",  "resumeThread",          "(Ljava/lang/Thread;)I",                                            org_mini_reflect_vm_RefNative_resumeThread},
        {"org/mini/reflect/vm/RefNative",  "getSuspendCount",       "(Ljava/lang/Thread;)I",                                            org_mini_reflect_vm_RefNative_getSuspendCount},
        {"org/mini/reflect/vm/RefNative",  "getFrameCount",         "(Ljava/lang/Thread;)I",                                            org_mini_reflect_vm_RefNative_getFrameCount},
        {"org/mini/reflect/vm/RefNative",  "stopThread",            "(Ljava/lang/Thread;J)I",                                           org_mini_reflect_vm_RefNative_stopThread},
        {"org/mini/reflect/vm/RefNative",  "getStackFrame",         "(Ljava/lang/Thread;)J",                                            org_mini_reflect_vm_RefNative_getStackFrame},
        {"org/mini/reflect/vm/RefNative",  "getGarbageReferedObjs", "()[Ljava/lang/Object;",                                            org_mini_reflect_vm_RefNative_getGarbageReferedObjs},
        {"org/mini/reflect/vm/RefNative",  "getGarbageStatus",      "()I",                                                              org_mini_reflect_vm_RefNative_getGarbageStatus},
        {"org/mini/reflect/vm/RefNative",  "defineClass",           "(Ljava/lang/ClassLoader;Ljava/lang/String;[BII)Ljava/lang/Class;", org_mini_reflect_vm_RefNative_defineClass},
        {"org/mini/reflect/ReflectClass",  "mapReference",          "(J)V",                                                             org_mini_reflect_ReflectClass_mapReference},
        {"org/mini/reflect/ReflectField",  "mapField",              "(J)V",                                                             org_mini_reflect_ReflectField_mapField},
        {"org/mini/reflect/ReflectField",  "getFieldVal",           "(JJ)J",                                                            org_mini_reflect_ReflectField_getFieldVal},
        {"org/mini/reflect/ReflectField",  "setFieldVal",           "(JJJ)V",                                                           org_mini_reflect_ReflectField_setFieldVal},
        {"org/mini/reflect/ReflectMethod", "mapMethod",             "(J)V",                                                             org_mini_reflect_ReflectMethod_mapMethod},
        {"org/mini/reflect/ReflectMethod", "invokeMethod",          "(JLjava/lang/Object;[J)J",                                         org_mini_reflect_ReflectMethod_invokeMethod},
        {"org/mini/reflect/StackFrame",    "mapRuntime",            "(J)V",                                                             org_mini_reflect_StackFrame_mapRuntime},
        {"org/mini/reflect/ReflectArray",  "mapArray",              "(J)V",                                                             org_mini_reflect_ReflectArray_mapArray},
        {"org/mini/reflect/ReflectArray",  "setVal",                "(JIJ)V",                                                           org_mini_reflect_ReflectArray_setVal},
        {"org/mini/reflect/ReflectArray",  "getVal",                "(JI)J",                                                            org_mini_reflect_ReflectArray_getVal},
        {"org/mini/reflect/ReflectArray",  "newArray",              "(Ljava/lang/Class;I)Ljava/lang/Object;",                           org_mini_reflect_ReflectArray_newArray},
        {"org/mini/reflect/ReflectArray",  "multiNewArray",         "(Ljava/lang/Class;[I)Ljava/lang/Object;",                          org_mini_reflect_ReflectArray_multiNewArray},

};


void reg_jdwp_native_lib() {
    native_reg_lib(&(method_jdwp_table[0]), sizeof(method_jdwp_table) / sizeof(java_native_method));
}

#ifdef __cplusplus
}
#endif
