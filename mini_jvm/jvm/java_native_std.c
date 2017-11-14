

#include <time.h>
#include "java_native_std.h"
#include "garbage.h"
#include "jvm_util.h"
#include <math.h>


s32 com_sun_cldc_io_ConsoleOutputStream_write(Runtime *runtime, Class *clazz) {
    s16 ch = localvar_getInt(runtime, 1);
    printf("%c", ch);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("com_sun_cldc_io_ConsoleOutputStream_write\n");
#endif
    return 0;
}

s32 java_lang_Class_forName(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *jstr = (Instance *) localvar_getRefer(runtime, 0);
    Class *cl = NULL;
    s32 ret = RUNTIME_STATUS_NORMAL;
    if (jstr) {
        Utf8String *ustr = utf8_create();
        jstring_2_utf8(jstr, ustr);
        utf8_replace_c(ustr, ".", "/");
        cl = classes_load_get(ustr, runtime);
        if (!cl) {
            Instance *exception = exception_create(JVM_EXCEPTION_CLASSNOTFOUND, runtime);
            push_ref(stack, (__refer) exception);
            ret = RUNTIME_STATUS_EXCEPTION;
        } else
            push_ref(stack, (__refer) cl);
        utf8_destory(ustr);
    } else {
        Instance *exception = exception_create(JVM_EXCEPTION_NULLPOINTER, runtime);
        push_ref(stack, (__refer) exception);
        ret = RUNTIME_STATUS_EXCEPTION;
    }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Class_forName [%llx]\n", (s64) (long) cl);
#endif
    return ret;
}

s32 java_lang_Class_newInstance(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Class *cl = (Class *) localvar_getRefer(runtime, 0);
    Instance *ins = NULL;
    s32 ret = 0;
    if (cl) {
        ins = instance_create(cl);
        instance_init(ins, runtime);
    }
    if (ins) {
        push_ref(stack, (__refer) ins);

    } else {
        Instance *exception = exception_create(JVM_EXCEPTION_INSTANTIATIONEXCEPTION, runtime);
        push_ref(stack, (__refer) exception);
        ret = RUNTIME_STATUS_EXCEPTION;
    }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Class_newInstance  class:[%llx] ins:[%llx]\n", (s64) (long) cl, (s64) (long) ins);
#endif
    return ret;
}

s32 java_lang_Class_isInstance(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Class *cl = (Class *) localvar_getRefer(runtime, 0);
    Instance *ins = (Instance *) localvar_getRefer(runtime, 1);
    if (instance_of(cl, ins)) {
        push_int(stack, 1);
    } else {
        push_int(stack, 0);
    }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Class_isInstance\n");
#endif
    return 0;
}

s32 java_lang_Class_isAssignableFrom(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Class *c0 = (Class *) localvar_getRefer(runtime, 0);
    Class *c1 = (Class *) localvar_getRefer(runtime, 1);

    if (assignable_from(c1, c0)) {
        push_int(stack, 1);
    } else {
        push_int(stack, 0);
    }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Class_isAssignableFrom\n");
#endif
    return 0;
}

s32 java_lang_Class_isInterface(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Class *c0 = (Class *) localvar_getRefer(runtime, 0);

    if (c0->cff.access_flags & ACC_INTERFACE) {//
        push_int(stack, 1);
    } else {
        push_int(stack, 0);
    }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Class_isAssignableFrom\n");
#endif
    return 0;
}

s32 java_lang_Class_isArray(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Class *cl = (Class *) localvar_getRefer(runtime, 0);
    if (cl->arr_type_index) {
        push_int(stack, 1);
    } else {
        push_int(stack, 0);
    }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Class_isArray\n");
#endif
    return 0;
}

s32 java_lang_Class_getName(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Class *cl = (Class *) localvar_getRefer(runtime, 0);
    if (cl) {
        Instance *ins = jstring_create(cl->name, runtime);
        push_ref(stack, ins);
    } else {
        push_ref(stack, NULL);
    }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Class_getName\n");
#endif
    return 0;
}

s32 java_lang_Class_getComponentType(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Class *cl = (Class *) localvar_getRefer(runtime, 0);
    if (cl->mb.type == MEM_TYPE_ARR) {
        //todo
    } else {
        push_ref(stack, NULL);
    }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Class_getComponentType\n");
#endif
    return 0;
}

s32 java_lang_Double_doubleToLongBits(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Long2Double l2d;
    l2d.i2l.i1 = localvar_getInt(runtime, 0);
    l2d.i2l.i0 = localvar_getInt(runtime, 1);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Double_doubleToLongBits %lf to %lld\n", l2d.d, l2d.l);
#endif
    push_long(stack, l2d.l);
    return 0;
}

s32 java_lang_Double_longBitsToDouble(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Long2Double l2d;
    l2d.i2l.i1 = localvar_getInt(runtime, 0);
    l2d.i2l.i0 = localvar_getInt(runtime, 1);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Double_longBitsToDouble\n");
#endif
    push_double(stack, l2d.d);
    return 0;
}


s32 java_lang_Float_intBitsToFloat(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Int2Float i2f;
    i2f.i = localvar_getInt(runtime, 0);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Float_intBitsToFloat r = %f\n", i2f.f);
#endif
    push_float(stack, i2f.f);
    return 0;
}

s32 java_lang_Float_floatToIntBits(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Int2Float i2f;
    i2f.i = localvar_getInt(runtime, 0);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Float_floatToIntBits r = %f\n", i2f.i);
#endif
    push_int(stack, i2f.i);
    return 0;
}

s32 java_lang_Math_exp(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Long2Double l2d;
    l2d.i2l.i1 = localvar_getInt(runtime, 0);
    l2d.i2l.i0 = localvar_getInt(runtime, 1);
    f64 r = exp(l2d.d);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Math_sin r = %f\n", r);
#endif
    push_double(stack, r);
    return 0;
}

s32 java_lang_Math_random(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    f64 r = 0.0f;
    s32 i;
    s32 times = 0;
    srand((u32) time(0));
    times = rand() % 100;
    for (i = 0; i < times; i++)
        r = ((f64) rand() / (f64) RAND_MAX);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Math_random r = %f\n", r);
#endif
    push_double(stack, r);
    return 0;
}

s32 java_lang_Math_sin(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Long2Double l2d;
    l2d.i2l.i1 = localvar_getInt(runtime, 0);
    l2d.i2l.i0 = localvar_getInt(runtime, 1);
    f64 r = sin(l2d.d);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Math_sin r = %f\n", r);
#endif
    push_double(stack, r);
    return 0;
}


s32 java_lang_Math_cos(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Long2Double l2d;
    l2d.i2l.i1 = localvar_getInt(runtime, 0);
    l2d.i2l.i0 = localvar_getInt(runtime, 1);
    f64 r = cos(l2d.d);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Math_cos r = %f\n", r);
#endif
    push_double(stack, r);
    return 0;
}

s32 java_lang_Math_tan(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Long2Double l2d;
    l2d.i2l.i1 = localvar_getInt(runtime, 0);
    l2d.i2l.i0 = localvar_getInt(runtime, 1);
    f64 r = tan(l2d.d);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Math_tan r = %f\n", r);
#endif
    push_double(stack, r);
    return 0;
}

s32 java_lang_Math_sqrt(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Long2Double l2d;
    l2d.i2l.i1 = localvar_getInt(runtime, 0);
    l2d.i2l.i0 = localvar_getInt(runtime, 1);
    f64 r = sqrt(l2d.d);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Math_sqrt r = %f\n", r);
#endif
    push_double(stack, r);
    return 0;
}

s32 java_lang_Math_ceil(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Long2Double l2d;
    l2d.i2l.i1 = localvar_getInt(runtime, 0);
    l2d.i2l.i0 = localvar_getInt(runtime, 1);
    f64 r = ceil(l2d.d);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Math_ceil r = %f\n", r);
#endif
    push_double(stack, r);
    return 0;
}


s32 java_lang_Math_floor(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Long2Double l2d;
    l2d.i2l.i1 = localvar_getInt(runtime, 0);
    l2d.i2l.i0 = localvar_getInt(runtime, 1);
    f64 r = floor(l2d.d);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Math_floor r = %f\n", r);
#endif
    push_double(stack, r);
    return 0;
}

s32 java_lang_Object_getClass(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *ins = (Instance *) localvar_getRefer(runtime, 0);
    push_ref(stack, (__refer) ins->mb.clazz);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Object_getClass %d\n", ins);
#endif
    return 0;
}

s32 java_lang_Object_hashCode(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *ins = (Instance *) localvar_getRefer(runtime, 0);
    push_int(stack, (s32) (long) ins);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Object_hashCode %llx\n", (s32) (long) ins);
#endif
    return 0;
}

s32 java_lang_Object_notify(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *ins = (Instance *) localvar_getRefer(runtime, 0);

    jthread_notify(&ins->mb, runtime);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Object_notify %d\n", ins);
#endif
    return 0;
}

s32 java_lang_Object_notifyAll(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *ins = (Instance *) localvar_getRefer(runtime, 0);
    jthread_notifyAll(&ins->mb, runtime);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Object_notifyAll %d\n", ins);
#endif
    return 0;
}

s32 java_lang_Object_wait(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *ins = (Instance *) localvar_getRefer(runtime, 0);
    Long2Double l2d;
    l2d.i2l.i1 = localvar_getInt(runtime, 1);
    l2d.i2l.i0 = localvar_getInt(runtime, 2);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Object_wait %llx  wait %lld\n", (s64) (long) ins, l2d.l);
#endif
    jthread_waitTime(&ins->mb, runtime, l2d.l);
    garbage_thread_unlock();
    return 0;
}

s32 java_lang_Runtime_exitInternal(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    s32 status = localvar_getInt(runtime, 1);
    exit(status);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Runtime_exitInternal %d\n", status);
#endif
    return 0;
}

s32 java_lang_Runtime_freeMemory(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;

    push_long(stack, MAX_HEAP_SIZE - collector->heap_size);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Runtime_freeMemory \n");
#endif
    return 0;
}

s32 java_lang_Runtime_totalMemory(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;

    push_long(stack, MAX_HEAP_SIZE);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Runtime_totalMemory \n");
#endif
    return 0;
}

s32 java_lang_Runtime_gc(Runtime *runtime, Class *clazz) {
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Runtime_gc \n");
#endif
    jthread_block_enter(runtime);
    garbage_collect();
    jthread_block_exit(runtime);
    return 0;
}

s32 java_lang_String_charAt0(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *jstr = (Instance *) localvar_getRefer(runtime, 0);
    s32 index = localvar_getInt(runtime, 1);
    s16 ch = jstring_char_at(jstr, index);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_String_charAt ch = %d\n", ch);
#endif
    push_int(stack, ch);
    return 0;
}

s32 java_lang_String_equals(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *jstr1 = (Instance *) localvar_getRefer(runtime, 0);
    Instance *jstr2 = (Instance *) localvar_getRefer(runtime, 1);
    s32 r = jstring_equals(jstr1, jstr2);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_String_equals r = %f\n", r);
#endif
    push_int(stack, r);
    return 0;
}

s32 java_lang_String_indexOf(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *jstr = (Instance *) localvar_getRefer(runtime, 0);
    u16 ch = localvar_getInt(runtime, 1);
    s32 r = jstring_index_of(jstr, ch, 0);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_String_indexOf r = %f\n", r);
#endif
    push_int(stack, r);
    return 0;
}

s32 java_lang_String_indexOfFrom(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *jstr = (Instance *) localvar_getRefer(runtime, 0);
    u16 ch = localvar_getInt(runtime, 1);
    s32 startAt = localvar_getInt(runtime, 2);
    s32 r = jstring_index_of(jstr, ch, startAt);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_String_indexOfFrom r = %f\n", r);
#endif
    push_int(stack, r);
    return 0;
}

s32 java_lang_String_intern(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *jstr = (Instance *) localvar_getRefer(runtime, 0);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_String_intern \n");
#endif
    push_ref(stack, (__refer) jstr);
    return 0;
}

s32 java_lang_StringBuffer_append(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *jstr = (Instance *) localvar_getRefer(runtime, 0);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_StringBuffer_append \n");
#endif
    push_ref(stack, (__refer) jstr);
    return 0;
}


s32 java_lang_System_arraycopy(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;

    s32 count = localvar_getInt(runtime, 4);
    s32 dest_start = localvar_getInt(runtime, 3);
    Instance *dest = (Instance *) localvar_getRefer(runtime, 2);
    s32 src_start = localvar_getInt(runtime, 1);
    Instance *src = (Instance *) localvar_getRefer(runtime, 0);
    if (src == NULL || dest == NULL) {
        Instance *exception = exception_create(JVM_EXCEPTION_NULLPOINTER, runtime);
        push_ref(stack, (__refer) exception);
    } else {
        s32 bytes = data_type_bytes[src->mb.clazz->arr_type_index];
        //根据元素宽
        src_start *= bytes;
        count *= bytes;
        dest_start *= bytes;
        if (src && dest && src->arr_body && dest->arr_body && count > 0)
            memcpy(&(dest->arr_body[dest_start]), &(src->arr_body[src_start]), count);
    }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_System_arraycopy\n");
#endif

    return 0;
}


s32 java_lang_System_doubleToString(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;

    Long2Double l2d;
    l2d.i2l.i1 = localvar_getInt(runtime, 0);
    l2d.i2l.i0 = localvar_getInt(runtime, 1);
    c8 buf[22];
    sprintf(buf, "%lf", l2d.d);
    Utf8String *str = utf8_create_c(buf);
    Instance *jstr = jstring_create(str, runtime);
    push_ref(stack, (__refer) jstr);
    utf8_destory(str);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_System_doubleToString\n");
#endif

    return 0;
}

s32 java_lang_System_currentTimeMillis(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;

    s64 curMs = currentTimeMillis();
    push_long(stack, curMs);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_System_currentTimeMillis\n");
#endif

    return 0;
}

s32 java_lang_System_nanotime(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;

    s64 nano = nanoTime();
    push_long(stack, nano);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_System_nanotime\n");
#endif

    return 0;
}

s32 java_lang_System_identityHashCode(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *tmps = (Instance *) localvar_getRefer(runtime, 0);
    push_ref(stack, (__refer) tmps);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_System_identityHashCode\n");
#endif

    return 0;
}

s32 java_lang_System_getProperty0(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;

    Instance *jstr1 = (Instance *) localvar_getRefer(runtime, 0);
    Utf8String *key = utf8_create();
    jstring_2_utf8(jstr1, key);
    Utf8String *val = (Utf8String *) hashtable_get(sys_prop, key);
    if (val) {
        Instance *jstr = jstring_create(val, runtime);
        push_ref(stack, jstr);
    } else {
        push_ref(stack, (__refer) (long) NULL);
    }
    utf8_destory(key);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_System_getProperty0 \n");
#endif
    return 0;
}

s32 java_lang_Thread_currentThread(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    push_ref(stack, (__refer) runtime->threadInfo->jthread);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Thread_currentThread \n");
#endif

    return 0;
}

s32 java_lang_Thread_yield(Runtime *runtime, Class *clazz) {
    jthread_yield(runtime);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Thread_yield \n");
#endif
    return 0;
}

s32 java_lang_Thread_sleep(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Long2Double l2d;
    l2d.i2l.i1 = localvar_getInt(runtime, 0);
    l2d.i2l.i0 = localvar_getInt(runtime, 1);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Thread_sleep %lld\n", l2d.l);
#endif
    jthread_sleep(runtime, l2d.l);
    return 0;
}

s32 java_lang_Thread_start(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *ins = (Instance *) localvar_getRefer(runtime, 0);
    jthread_start(ins);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Thread_start \n");
#endif

    return 0;
}

s32 java_lang_Thread_isAlive(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *ins = (Instance *) localvar_getRefer(runtime, 0);
    Runtime *rt = jthread_get_stackframe_value(ins);
    if (rt)
        push_int(stack, runtime->threadInfo->thread_status != THREAD_STATUS_ZOMBIE);
    else
        push_int(stack, 0);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Thread_isAlive \n");
#endif

    return 0;
}

s32 java_lang_Thread_activeCount(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;

    push_int(stack, thread_list->length);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Thread_activeCount \n");
#endif

    return 0;
}


s32 java_lang_Thread_setPriority0(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *ins = (Instance *) localvar_getRefer(runtime, 0);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Thread_setPriority0 \n");
#endif

    return 0;
}


s32 java_lang_Thread_interrupt0(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *ins = (Instance *) localvar_getRefer(runtime, 0);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_lang_Thread_interrupt0 \n");
#endif

    return 0;
}


s32 java_io_PrintStream_printImpl(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_io_PrintStream_printImpl \n");
#endif

    Instance *tmps = NULL;
    tmps = (Instance *) localvar_getRefer(runtime, 0);
    if (tmps) {

        c8 *fieldPtr = jstring_get_value_ptr(tmps);
        Instance *ptr = (Instance *) getFieldRefer(fieldPtr);
        //jvm_printf("printImpl [%x]\n", arr_body);
        if (ptr && ptr->arr_body) {
            u16 *jchar_arr = (u16 *) ptr->arr_body;
            s32 i = 0;
            for (; i < ptr->arr_length; i++) {
                u16 ch = jchar_arr[i];
                //swap_endian_little_big((u8*)&ch, sizeof(ch));
                printf("%c", ch);
            }
        }
    }
    return 0;
}

s32 java_io_Throwable_printStackTrace0(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *tmps = (Instance *) localvar_getRefer(runtime, 0);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_io_Throwable_printStackTrace0 %s \n", utf8_cstr(tmps->mb.clazz->name));
#endif
    return 0;
}

Instance *buildStackElement(Runtime *runtime, Runtime *target) {
    Class *clazz = classes_load_get_c(STR_CLASS_JAVA_LANG_STACKTRACE, target);
    if (clazz) {
        Instance *ins = instance_create(clazz);
        garbage_refer_hold(ins);
        instance_init(ins, runtime);
        c8 *ptr;
        //
        ptr = getFieldPtr_byName_c(ins, STR_CLASS_JAVA_LANG_STACKTRACE, "declaringClass", STR_INS_JAVA_LANG_STRING);
        if (ptr) {
            Instance *name = jstring_create(target->clazz->name, runtime);
            setFieldRefer(ptr, name);
        }
        //
        ptr = getFieldPtr_byName_c(ins, STR_CLASS_JAVA_LANG_STACKTRACE, "methodName", STR_INS_JAVA_LANG_STRING);
        if (ptr) {
            Instance *name = jstring_create(target->method->name, runtime);
            setFieldRefer(ptr, name);
        }
        //
        ptr = getFieldPtr_byName_c(ins, STR_CLASS_JAVA_LANG_STACKTRACE, "fileName", STR_INS_JAVA_LANG_STRING);
        if (ptr) {
            Instance *name = jstring_create(target->clazz->source, runtime);
            setFieldRefer(ptr, name);
        }
        //
        ptr = getFieldPtr_byName_c(ins, STR_CLASS_JAVA_LANG_STACKTRACE, "lineNumber", "I");
        if (ptr) {
            if (target->method->access_flags & ACC_NATIVE) {
                setFieldInt(ptr, -1);
            } else {
                setFieldInt(ptr, getLineNumByIndex(target->ca, (s32) (target->pc - target->ca->code)));
            }
        }
        if (target->parent && target->parent->parent) {
            ptr = getFieldPtr_byName_c(ins, STR_CLASS_JAVA_LANG_STACKTRACE, "parent", "Ljava/lang/StackTraceElement;");
            if (ptr) {
                Instance *parent = buildStackElement(runtime, target->parent);
                setFieldRefer(ptr, parent);
            }
        }
        garbage_refer_release(ins);
        return ins;
    }
    return NULL;
}

s32 java_io_Throwable_buildStackElement(Runtime *runtime, Class *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *tmps = (Instance *) localvar_getRefer(runtime, 0);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("java_io_Throwable_buildStackElement %s \n", utf8_cstr(tmps->mb.clazz->name));
#endif
    Instance *ins = buildStackElement(runtime, runtime->parent);
    push_ref(stack, ins);
    return 0;
}

static java_native_method method_table[] = {
        {"com/sun/cldc/io/ConsoleOutputStream", "write",             "(I)V",                                       com_sun_cldc_io_ConsoleOutputStream_write},
//        {"com/sun/cldc/io/ResourceInputStream", "open",       "",              com_sun_cldc_io_ResourceInputStream_open},
//        {"com/sun/cldc/io/ResourceInputStream", "close",       "",             com_sun_cldc_io_ResourceInputStream_close},
//        {"com/sun/cldc/io/ResourceInputStream", "size",       "",              com_sun_cldc_io_ResourceInputStream_size},
//        {"com/sun/cldc/io/ResourceInputStream", "read",       "",              com_sun_cldc_io_ResourceInputStream_read},
//        {"com/sun/cldc/io/ResourceInputStream", "readBytes",       "",         com_sun_cldc_io_ResourceInputStream_readBytes},
//        {"com/sun/cldc/io/Waiter",              "waitForIO",       "",         com_sun_cldc_io_ResourceInputStream_waitForIO},
        {"java/lang/Class",                     "forName",           "(Ljava/lang/String;)Ljava/lang/Class;",      java_lang_Class_forName},
        {"java/lang/Class",                     "newInstance",       "()Ljava/lang/Object;",                       java_lang_Class_newInstance},
        {"java/lang/Class",                     "isInstance",        "(Ljava/lang/Object;)Z",                      java_lang_Class_isInstance},
        {"java/lang/Class",                     "isAssignableFrom",  "(Ljava/lang/Class;)Z",                       java_lang_Class_isAssignableFrom},
        {"java/lang/Class",                     "isInterface",       "()Z",                                        java_lang_Class_isInterface},
        {"java/lang/Class",                     "isArray",           "()Z",                                        java_lang_Class_isArray},
        {"java/lang/Class",                     "getName",           "()Ljava/lang/String;",                       java_lang_Class_getName},
        {"java/lang/Class",                     "getComponentType",  "()Ljava/lang/Class;",                        java_lang_Class_getComponentType},
        {"java/lang/Double",                    "doubleToLongBits",  "(D)J",                                       java_lang_Double_doubleToLongBits},
        {"java/lang/Double",                    "longBitsToDouble",  "(J)D",                                       java_lang_Double_longBitsToDouble},
        {"java/lang/Float",                     "floatToIntBits",    "(F)I",                                       java_lang_Float_floatToIntBits},
        {"java/lang/Float",                     "intBitsToFloat",    "(I)F",                                       java_lang_Float_intBitsToFloat},
        {"java/lang/Math",                      "exp",               "(D)D",                                       java_lang_Math_exp},
        {"java/lang/Math",                      "random",            "",                                           java_lang_Math_random},
        {"java/lang/Math",                      "sin",               "(D)D",                                       java_lang_Math_sin},
        {"java/lang/Math",                      "cos",               "(D)D",                                       java_lang_Math_cos},
        {"java/lang/Math",                      "tan",               "(D)D",                                       java_lang_Math_tan},
        {"java/lang/Math",                      "sqrt",              "(D)D",                                       java_lang_Math_sqrt},
        {"java/lang/Math",                      "ceil",              "(D)D",                                       java_lang_Math_ceil},
        {"java/lang/Math",                      "floor",             "(D)D",                                       java_lang_Math_floor},
        {"java/lang/Object",                    "getClass",          "()Ljava/lang/Class;",                        java_lang_Object_getClass},
        {"java/lang/Object",                    "hashCode",          "()I",                                        java_lang_Object_hashCode},
        {"java/lang/Object",                    "notify",            "()V",                                        java_lang_Object_notify},
        {"java/lang/Object",                    "notifyAll",         "()V",                                        java_lang_Object_notifyAll},
        {"java/lang/Object",                    "wait",              "(J)V",                                       java_lang_Object_wait},
        {"java/lang/Runtime",                   "exitInternal",      "(I)V",                                       java_lang_Runtime_exitInternal},
        {"java/lang/Runtime",                   "freeMemory",        "()J",                                        java_lang_Runtime_freeMemory},
        {"java/lang/Runtime",                   "totalMemory",       "()J",                                        java_lang_Runtime_totalMemory},
        {"java/lang/Runtime",                   "gc",                "()V",                                        java_lang_Runtime_gc},
        {"java/lang/String",                    "charAt0",           "(I)C",                                       java_lang_String_charAt0},
        {"java/lang/String",                    "equals",            "(Ljava/lang/Object;)Z",                      java_lang_String_equals},
        {"java/lang/String",                    "indexOf",           "(I)I",                                       java_lang_String_indexOf},
        {"java/lang/String",                    "indexOf",           "(II)I",                                      java_lang_String_indexOfFrom},
        {"java/lang/String",                    "intern",            "()Ljava/lang/String;",                       java_lang_String_intern},
//        {"java/lang/StringBuffer",              "append",       "",            java_lang_StringBuffer_append},
//        {"java/lang/StringBuffer",              "appendi",       "",            java_lang_StringBuffer_appendi},
//        {"java/lang/StringBuffer",              "toString",       "",          java_lang_StringBuffer_toString},
        {"java/lang/System",                    "arraycopy",         "(Ljava/lang/Object;ILjava/lang/Object;II)V", java_lang_System_arraycopy},
        {"java/lang/System",                    "doubleToString",    "(D)Ljava/lang/String;",                      java_lang_System_doubleToString},
        {"java/lang/System",                    "currentTimeMillis", "()J",                                        java_lang_System_currentTimeMillis},
        {"java/lang/System",                    "nanoTime",          "()J",                                        java_lang_System_nanotime},
        {"java/lang/System",                    "identityHashCode",  "(Ljava/lang/Object;)I",                      java_lang_System_identityHashCode},
        {"java/lang/System",                    "getProperty0",      "(Ljava/lang/String;)Ljava/lang/String;",     java_lang_System_getProperty0},
        {"java/lang/Thread",                    "currentThread",     "()Ljava/lang/Thread;",                       java_lang_Thread_currentThread},
        {"java/lang/Thread",                    "yield",             "()V",                                        java_lang_Thread_yield},
        {"java/lang/Thread",                    "sleep",             "(J)V",                                       java_lang_Thread_sleep},
        {"java/lang/Thread",                    "start",             "()V",                                        java_lang_Thread_start},
        {"java/lang/Thread",                    "isAlive",           "()Z",                                        java_lang_Thread_isAlive},
        {"java/lang/Thread",                    "activeCount",       "()I",                                        java_lang_Thread_activeCount},
        {"java/lang/Thread",                    "setPriority0",      "(I)V",                                       java_lang_Thread_setPriority0},
        {"java/lang/Thread",                    "interrupt0",        "()V",                                        java_lang_Thread_interrupt0},
        {"java/lang/Throwable",                 "printStackTrace0",  "",                                           java_io_Throwable_printStackTrace0},
        {"java/lang/Throwable",                 "buildStackElement", "()Ljava/lang/StackTraceElement;",            java_io_Throwable_buildStackElement},
        {"java/io/PrintStream",                 "printImpl",         "(Ljava/lang/String;)V",                      java_io_PrintStream_printImpl},
};


java_native_method *find_native_method(c8 *cls_name, c8 *method_name, c8 *method_type) {
    s32 i, j;
    for (j = 0; j < native_libs->length; j++) {
        JavaNativeLib *lib = arraylist_get_value(native_libs, j);
        java_native_method *methods = lib->methods;
        for (i = 0; i < lib->methods_count; i++)
            if (strcmp(cls_name, (methods + i)->clzname) == 0 &&
                strcmp(method_name, (methods + i)->methodname) == 0 &&
                strcmp(method_type, (methods + i)->methodtype) == 0)
                return (methods + i);
    }
    return 0;
}

s32 invoke_native_method(Runtime *runtime, Class *p,
                         c8 *cls_name, c8 *method_name, c8 *method_type) {
    java_native_method *method = find_native_method(cls_name, method_name, method_type);
    if (method != 0) {
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
        invoke_deepth(runtime);
        jvm_printf("invoke native %s/%s %s\n", method->clzname, method->methodname, method_type);
#endif
        method->func_pointer(runtime, p);
        return 1;
    }
    return 0;
}

void reg_std_native_lib() {
    native_reg_lib(&(method_table[0]), sizeof(method_table) / sizeof(java_native_method));
}

s32 native_reg_lib(java_native_method *methods, s32 method_size) {
    JavaNativeLib *lib = jvm_calloc(sizeof(JavaNativeLib));
    lib->methods_count = method_size;
    lib->methods = methods;
    arraylist_push_end(native_libs, lib);
    return 0;
}

s32 native_remove_lib(JavaNativeLib *lib) {
    arraylist_remove(native_libs, lib);
    return 0;
}

s32 native_lib_destory() {
    s32 i;
    for (i = 0; i < native_libs->length; i++) {
        __refer lib = arraylist_get_value(native_libs, i);
        jvm_free(lib);
    }
    arraylist_destory(native_libs);
    return 0;
}
