

#include "jvm.h"
#include "java_native_std.h"
#include "garbage.h"
#include "jvm_util.h"
#include "../utils/utf8_string.h"
#include "../utils/arraylist.h"
#include "java_native_reflect.h"
#include "jdwp.h"


/* ==================================opcode implementation =============================*/


s32 op_nop(u8 **opCode, Runtime *runtime) {

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("nop\n");
#endif
    *opCode = *opCode + 1;
    return 0;
}


static inline s32 op_aload_n(u8 **opCode, Runtime *runtime, s32 i) {
    RuntimeStack *stack = runtime->stack;
    __refer value = (runtime->localVariables + i)->refer;
    push_ref(stack, value);
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("aload_%d push localvar [%llx] into stack\n", i, (s64) (long) value);
#endif
    *opCode = *opCode + 1;
    return 0;
}

s32 op_aload_0(u8 **opCode, Runtime *runtime) {
    return op_aload_n(opCode, runtime, 0);
}

s32 op_aload_1(u8 **opCode, Runtime *runtime) {
    return op_aload_n(opCode, runtime, 1);
}

s32 op_aload_2(u8 **opCode, Runtime *runtime) {
    return op_aload_n(opCode, runtime, 2);
}

s32 op_aload_3(u8 **opCode, Runtime *runtime) {
    return op_aload_n(opCode, runtime, 3);
}

static inline s32 op_xaload(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;

    s32 index = pop_int(stack);
    Instance *arr = (Instance *) pop_ref(stack);
    s32 bytes = data_type_bytes[arr->mb.clazz->arr_data_type];

    Long2Double l2d;
    jarray_get_field(arr, index, &l2d, bytes);

    if (bytes <= 4) {
        push_int(stack, l2d.i2l.i1);
    } else {
        push_long(stack, l2d.l);
    }

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("(icbdlfs)aload push arr[%llx]{%d bytes}.(%d)=%x:%d:%lld:%lf into stack\n", (u64) (long) arr, bytes,
               index,
               l2d.i2l.i1,
               l2d.i2l.i1, l2d.l, l2d.d);
#endif
    *opCode = *opCode + 1;
    return 0;
}

s32 op_iaload(u8 **opCode, Runtime *runtime) {
    return op_xaload(opCode, runtime);
}

s32 op_faload(u8 **opCode, Runtime *runtime) {
    return op_xaload(opCode, runtime);
}

s32 op_laload(u8 **opCode, Runtime *runtime) {
    return op_xaload(opCode, runtime);
}

s32 op_daload(u8 **opCode, Runtime *runtime) {
    return op_xaload(opCode, runtime);
}

s32 op_caload(u8 **opCode, Runtime *runtime) {
    return op_xaload(opCode, runtime);
}

s32 op_saload(u8 **opCode, Runtime *runtime) {
    return op_xaload(opCode, runtime);
}

s32 op_baload(u8 **opCode, Runtime *runtime) {
    return op_xaload(opCode, runtime);
}

s32 op_aaload(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;

    s32 index = pop_int(stack);
    Instance *arr = (Instance *) pop_ref(stack);
    s32 bytes = data_type_bytes[(arr->mb.clazz->arr_data_type)];

    Long2Double l2d;
    jarray_get_field(arr, index, &l2d, bytes);

    push_ref(stack, (__refer) l2d.r);


#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("aaload push arr[%llx]{%d bytes}.(%d)=%x:%d:%llx:%lf into stack\n", (s64) (long) arr, bytes, index,
               l2d.i2l.i1,
               l2d.i2l.i1, l2d.l, l2d.d);
#endif
    *opCode = *opCode + 1;
    return 0;
}


/* bipush */
s32 op_bipush(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;

    s32 value = (c8) opCode[0][1];
    push_int(stack, value);
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("bipush a byte %d onto the stack \n", value);
#endif
    *opCode = *opCode + 2;
    return 0;
}

/* dup */
s32 op_dup(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;

    StackEntry entry;
    pop_entry(stack, &entry);

    push_entry(stack, &entry);
    push_entry(stack, &entry);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("dup\n");
#endif
    *opCode = *opCode + 1;
    return 0;
}

s32 op_dup_x1(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    StackEntry entry1;
    pop_entry(stack, &entry1);
    StackEntry entry2;
    pop_entry(stack, &entry2);

    push_entry(stack, &entry1);
    push_entry(stack, &entry2);
    push_entry(stack, &entry1);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("dup_x1\n");
#endif
    *opCode = *opCode + 1;
    return 0;
}

s32 op_dup_x2(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    StackEntry entry;
    peek_entry(stack, &entry, stack->size - 2);
    if (is_cat2(&entry)) {
        StackEntry entry1;
        pop_entry(stack, &entry1);
        StackEntry entry2;
        pop_entry(stack, &entry2);

        push_entry(stack, &entry1);
        push_entry(stack, &entry2);
        push_entry(stack, &entry1);
    } else {
        StackEntry entry1;
        pop_entry(stack, &entry1);
        StackEntry entry2;
        pop_entry(stack, &entry2);
        StackEntry entry3;
        pop_entry(stack, &entry3);

        push_entry(stack, &entry1);
        push_entry(stack, &entry3);
        push_entry(stack, &entry2);
        push_entry(stack, &entry1);
    }
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("dup_x2 \n");
#endif
    *opCode = *opCode + 1;
    return 0;
}

s32 op_dup2(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    StackEntry entry;
    peek_entry(stack, &entry, stack->size - 1);
    if (is_cat2(&entry)) {
        StackEntry entry1;
        push_entry(stack, &entry1);
    } else {
        StackEntry entry1;
        peek_entry(stack, &entry1, stack->size - 1);
        StackEntry entry2;
        peek_entry(stack, &entry2, stack->size - 2);

        push_entry(stack, &entry2);
        push_entry(stack, &entry1);
    }
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("op_dup2\n");
#endif
    *opCode = *opCode + 1;
    return 0;
}


s32 op_dup2_x1(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    StackEntry entry;
    peek_entry(stack, &entry, stack->size - 1);
    if (is_cat2(&entry)) {
        StackEntry entry1;
        pop_entry(stack, &entry1);
        StackEntry entry2;
        pop_entry(stack, &entry2);

        push_entry(stack, &entry1);
        push_entry(stack, &entry2);
        push_entry(stack, &entry1);
    } else {
        StackEntry entry1;
        pop_entry(stack, &entry1);
        StackEntry entry2;
        pop_entry(stack, &entry2);
        StackEntry entry3;
        pop_entry(stack, &entry3);

        push_entry(stack, &entry2);
        push_entry(stack, &entry1);
        push_entry(stack, &entry3);
        push_entry(stack, &entry2);
        push_entry(stack, &entry1);
    }
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("dup2_x1\n");
#endif
    *opCode = *opCode + 1;
    return 0;
}

s32 op_dup2_x2(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;

    StackEntry entry1;
    peek_entry(stack, &entry1, stack->size - 1);
    StackEntry entry2;
    peek_entry(stack, &entry1, stack->size - 2);

    //都是64位，不包括64位指针
    if (is_cat2(&entry1) && is_cat2(&entry2)) {
        pop_int(stack);
        pop_int(stack);
        push_entry(stack, &entry1);
        push_entry(stack, &entry2);
        push_entry(stack, &entry1);
    } else {
        StackEntry entry3;
        peek_entry(stack, &entry1, stack->size - 3);
        if (is_cat1(&entry1) && is_cat1(&entry2) && is_cat2(&entry3)) {
            pop_int(stack);
            pop_int(stack);
            pop_int(stack);
            push_entry(stack, &entry2);
            push_entry(stack, &entry1);
            push_entry(stack, &entry3);
            push_entry(stack, &entry2);
            push_entry(stack, &entry1);
        } else if (is_cat2(&entry1) && is_cat1(&entry2) && is_cat1(&entry3)) {
            pop_int(stack);
            pop_int(stack);
            pop_int(stack);
            push_entry(stack, &entry1);
            push_entry(stack, &entry3);
            push_entry(stack, &entry2);
            push_entry(stack, &entry1);
        } else {
            StackEntry entry4;
            peek_entry(stack, &entry1, stack->size - 4);
            pop_int(stack);
            pop_int(stack);
            pop_int(stack);
            pop_int(stack);

            push_entry(stack, &entry2);
            push_entry(stack, &entry1);
            push_entry(stack, &entry4);
            push_entry(stack, &entry3);
            push_entry(stack, &entry2);
            push_entry(stack, &entry1);
        }
    }

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("dup2_x2\n");
#endif
    *opCode = *opCode + 1;
    return 0;
}

s32 op_swap(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;

    StackEntry entry1;
    pop_entry(stack, &entry1);
    StackEntry entry2;
    pop_entry(stack, &entry2);

    push_entry(stack, &entry1);
    push_entry(stack, &entry2);


#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("swap\n");
#endif
    *opCode = *opCode + 1;
    return 0;
}


s32 op_pop(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    pop_int(stack);
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("pop\n");
#endif
    *opCode = *opCode + 1;
    return 0;
}


s32 op_pop2(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;

    StackEntry entry;
    peek_entry(stack, &entry, stack->size - 1);
    if (is_cat2(&entry)) {
        pop_entry(stack, &entry);
    } else {
        pop_entry(stack, &entry);
        pop_entry(stack, &entry);
    }
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("pop2\n");
#endif
    *opCode = *opCode + 1;
    return 0;
}


/* iadd */
s32 op_iadd(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;

    s32 value1 = pop_int(stack);
    s32 value2 = pop_int(stack);
    s32 result = value1 + value2;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("iadd: %d + %d = %d\n", value1, value2, result);
#endif
    push_int(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_iconst_n(u8 **opCode, Runtime *runtime, s32 i) {
    RuntimeStack *stack = runtime->stack;

    push_int(stack, i);
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("iconst_%d: push %d into stack\n", i, i);
#endif
    *opCode = *opCode + 1;
    return 0;
}

s32 op_iconst_m1(u8 **opCode, Runtime *runtime) {
    return op_iconst_n(opCode, runtime, -1);
}

/* iconst_0 */
s32 op_iconst_0(u8 **opCode, Runtime *runtime) {
    return op_iconst_n(opCode, runtime, 0);
}

/* iconst_1 */
s32 op_iconst_1(u8 **opCode, Runtime *runtime) {
    return op_iconst_n(opCode, runtime, 1);
}

/* iconst_2 */
s32 op_iconst_2(u8 **opCode, Runtime *runtime) {
    return op_iconst_n(opCode, runtime, 2);
}

/* iconst_3 */
s32 op_iconst_3(u8 **opCode, Runtime *runtime) {
    return op_iconst_n(opCode, runtime, 3);
}

/* iconst_4 */
s32 op_iconst_4(u8 **opCode, Runtime *runtime) {
    return op_iconst_n(opCode, runtime, 4);
}

/* iconst_5 */
s32 op_iconst_5(u8 **opCode, Runtime *runtime) {
    return op_iconst_n(opCode, runtime, 5);
}

s32 op_aconst_null(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;

    push_ref(stack, NULL);
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("aconst_null: push %d into stack\n", 0);
#endif
    *opCode = *opCode + 1;
    return 0;
}

s32 op_lconst_n(u8 **opCode, Runtime *runtime, s64 i) {
    RuntimeStack *stack = runtime->stack;

    push_long(stack, i);
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("lconst_%lld: push %lld into stack\n", i, i);
#endif
    *opCode = *opCode + 1;
    return 0;
}

s32 op_lconst_0(u8 **opCode, Runtime *runtime) {
    return op_lconst_n(opCode, runtime, 0);
}

s32 op_lconst_1(u8 **opCode, Runtime *runtime) {
    return op_lconst_n(opCode, runtime, 1);
}

s32 op_fconst_n(u8 **opCode, Runtime *runtime, f32 f) {
    RuntimeStack *stack = runtime->stack;
    push_float(stack, f);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("fconst_%f: push %f into stack\n", (s32) f, f);
#endif
    *opCode = *opCode + 1;
    return 0;
}

s32 op_fconst_0(u8 **opCode, Runtime *runtime) {
    return op_fconst_n(opCode, runtime, 0.0f);
}

s32 op_fconst_1(u8 **opCode, Runtime *runtime) {
    return op_fconst_n(opCode, runtime, 1.0f);
}

s32 op_fconst_2(u8 **opCode, Runtime *runtime) {
    return op_fconst_n(opCode, runtime, 2.0f);
}


s32 op_dconst_n(u8 **opCode, Runtime *runtime, f64 d) {
    RuntimeStack *stack = runtime->stack;
    push_double(stack, d);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("dconst_%d: push %lf into stack\n", (s32) (d), d);
#endif
    *opCode = *opCode + 1;
    return 0;
}

s32 op_dconst_0(u8 **opCode, Runtime *runtime) {
    return op_dconst_n(opCode, runtime, 0.0f);
}

/* 0x0F dconst_1 */
s32 op_dconst_1(u8 **opCode, Runtime *runtime) {
    return op_dconst_n(opCode, runtime, 1.0f);
}

s32 op_idiv(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;

    s32 value2 = pop_int(stack);
    s32 value1 = pop_int(stack);
    s32 result = 0;
    result = value1 / value2;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("idiv: %d / %d = %d\n", value1, value2, result);
#endif
    push_int(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_ldiv(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s64 value1 = pop_long(stack);
    s64 value2 = pop_long(stack);
    s64 result = value2 / value1;

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("ldiv: %lld / %lld = %lld\n", value2, value1, result);
#endif
    push_long(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_ddiv(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f64 value1 = pop_double(stack);
    f64 value2 = pop_double(stack);
    f64 result = value2 / value1;

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("ddiv: %f / %f = %f\n", value2, value1, result);
#endif
    push_double(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_fdiv(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f32 value1 = pop_float(stack);
    f32 value2 = pop_float(stack);
    f32 result = value2 / value1;

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("fdiv: %f / %f = %f\n", value2, value1, result);
#endif
    push_float(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_fadd(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f32 value1 = pop_float(stack);
    f32 value2 = pop_float(stack);
    f32 result = value2 + value1;

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("ddiv: %lf + %lf = %lf\n", value2, value1, result);
#endif
    push_float(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_ladd(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s64 value1 = pop_long(stack);
    s64 value2 = pop_long(stack);
    s64 result = value2 + value1;

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("ladd: %lld + %lld = %lld\n", value2, value1, result);
#endif
    push_long(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_lsub(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s64 value1 = pop_long(stack);
    s64 value2 = pop_long(stack);
    s64 result = value2 - value1;

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("lsub: %lld - %lld = %lld\n", value2, value1, result);
#endif
    push_long(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_fsub(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f32 value1 = pop_float(stack);
    f32 value2 = pop_float(stack);
    f32 result = value2 - value1;

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("fsub: %f - %f = %f\n", value2, value1, result);
#endif
    push_float(stack, result);
    *opCode = *opCode + 1;
    return 0;
}


s32 op_dsub(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f64 value1 = pop_double(stack);
    f64 value2 = pop_double(stack);
    f64 result = value2 - value1;

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("dsub: %lf - %lf = %lf\n", value2, value1, result);
#endif
    push_double(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_lmul(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s64 value1 = pop_long(stack);
    s64 value2 = pop_long(stack);
    s64 result = value2 * value1;

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("lmul: %lld * %lld = %lld\n", value2, value1, result);
#endif
    push_long(stack, result);
    *opCode = *opCode + 1;
    return 0;
}


s32 op_iload(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    Short2Char s2c;
    if (runtime->wideMode) {
        s2c.c1 = opCode[0][1];
        s2c.c0 = opCode[0][2];
        *opCode = *opCode + 3;
    } else {
        s2c.s = (c8) opCode[0][1];
        *opCode = *opCode + 2;
    }
    runtime->wideMode = 0;

    s32 value = (runtime->localVariables + s2c.s)->integer;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("i(fa)load: push localvar(%d)= [%x]/%d  \n", s2c.s, value, value);
#endif
    push_int(stack, value);
    return 0;
}


s32 op_fload(u8 **opCode, Runtime *runtime) {
    return op_iload(opCode, runtime);
}

s32 op_aload(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    Short2Char s2c;
    if (runtime->wideMode) {
        s2c.c1 = opCode[0][1];
        s2c.c0 = opCode[0][2];
        *opCode = *opCode + 3;
    } else {
        s2c.s = (c8) opCode[0][1];
        *opCode = *opCode + 2;
    }
    runtime->wideMode = 0;

    __refer value = (runtime->localVariables + s2c.s)->refer;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("i(fa)load: push localvar(%d)= [%llx]  \n", s2c.s, (s64) (long) value);
#endif
    push_ref(stack, value);
    return 0;
}

s32 op_lload(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    Short2Char s2c;
    if (runtime->wideMode) {
        s2c.c1 = opCode[0][1];
        s2c.c0 = opCode[0][2];
        *opCode = *opCode + 3;
    } else {
        s2c.s = (c8) opCode[0][1];
        *opCode = *opCode + 2;
    }
    runtime->wideMode = 0;

    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + s2c.s)->integer;
    l2d.i2l.i0 = (runtime->localVariables + s2c.s + 1)->integer;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("l(d)load: push localvar(%d) [%llx]/%lf into stack \n", s2c.s, l2d.l, l2d.d);
#endif
    push_long(stack, l2d.l);
    return 0;
}

s32 op_dload(u8 **opCode, Runtime *runtime) {
    return op_lload(opCode, runtime);
}


static inline s32 op_ifload_n(u8 **opCode, Runtime *runtime, s32 i) {
    RuntimeStack *stack = runtime->stack;
    Int2Float i2f;
    i2f.i = (runtime->localVariables + i)->integer;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("i(f)load_%d: push localvar(%d)= [%x]/%d/%f  \n", i, i, i2f.i, i2f.i, i2f.f);
#endif
    push_int(stack, i2f.i);
    *opCode = *opCode + 1;
    return 0;
}

/* iload_0 */
s32 op_iload_0(u8 **opCode, Runtime *runtime) {
    return op_ifload_n(opCode, runtime, 0);
}

/* iload_1 */
s32 op_iload_1(u8 **opCode, Runtime *runtime) {
    return op_ifload_n(opCode, runtime, 1);
}

/* iload_2 */
s32 op_iload_2(u8 **opCode, Runtime *runtime) {
    return op_ifload_n(opCode, runtime, 2);
}

/* iload_3 */
s32 op_iload_3(u8 **opCode, Runtime *runtime) {
    return op_ifload_n(opCode, runtime, 3);
}

s32 op_fload_0(u8 **opCode, Runtime *runtime) {
    return op_ifload_n(opCode, runtime, 0);
}

s32 op_fload_1(u8 **opCode, Runtime *runtime) {
    return op_ifload_n(opCode, runtime, 1);
}

s32 op_fload_2(u8 **opCode, Runtime *runtime) {
    return op_ifload_n(opCode, runtime, 2);
}

s32 op_fload_3(u8 **opCode, Runtime *runtime) {
    return op_ifload_n(opCode, runtime, 3);
}

s32 op_lload_n(u8 **opCode, Runtime *runtime, s32 index) {
    RuntimeStack *stack = runtime->stack;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + index)->integer;
    l2d.i2l.i0 = (runtime->localVariables + index + 1)->integer;
    s64 value = l2d.l;

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("l(d)load_%d: push localvar(%d)= [%llx]/%lld/%lf  \n", index, index, value, value, value);
#endif
    push_long(stack, value);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_lload_0(u8 **opCode, Runtime *runtime) {
    return op_lload_n(opCode, runtime, 0);
}

s32 op_lload_1(u8 **opCode, Runtime *runtime) {
    return op_lload_n(opCode, runtime, 1);
}

s32 op_lload_2(u8 **opCode, Runtime *runtime) {
    return op_lload_n(opCode, runtime, 2);
}

s32 op_lload_3(u8 **opCode, Runtime *runtime) {
    return op_lload_n(opCode, runtime, 3);
}

s32 op_dload_0(u8 **opCode, Runtime *runtime) {
    return op_lload_n(opCode, runtime, 0);
}

s32 op_dload_1(u8 **opCode, Runtime *runtime) {
    return op_lload_n(opCode, runtime, 1);
}

s32 op_dload_2(u8 **opCode, Runtime *runtime) {
    return op_lload_n(opCode, runtime, 2);
}

s32 op_dload_3(u8 **opCode, Runtime *runtime) {
    return op_lload_n(opCode, runtime, 3);
}

/* imul */
s32 op_imul(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;

    s32 value1 = pop_int(stack);
    s32 value2 = pop_int(stack);
    s32 result = 0;
    result = value1 * value2;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("imul: %d * %d = %d\n", value1, value2, result);
#endif
    push_int(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

/* 0x63 dadd */
s32 op_dadd(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;

    f64 value1 = pop_double(stack);
    f64 value2 = pop_double(stack);
    f64 result = 0;
    result = value1 + value2;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("dadd: %lf + %lf = %lf\n", value1, value2, result);
#endif
    push_double(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

/* 0x6B dmul */
s32 op_dmul(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f64 value1 = pop_double(stack);
    f64 value2 = pop_double(stack);
    f64 result = 0;
    result = value1 * value2;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("dmul: %lf * %lf = %lf\n", value1, value2, result);
#endif
    push_double(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_fmul(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f32 value1 = pop_float(stack);
    f32 value2 = pop_float(stack);
    f32 result = 0;
    result = value1 * value2;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("fmul: %f * %f = %f\n", value1, value2, result);
#endif
    push_float(stack, result);
    *opCode = *opCode + 1;
    return 0;
}


s32 op_l2i(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s64 value = pop_long(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("l2i: %d <-- %lld\n", (s32) value, value);
#endif
    push_int(stack, (s32) value);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_l2f(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s64 value = pop_long(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("l2f: %f <-- %lld\n", (f32) value, value);
#endif
    push_float(stack, (f32) value);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_l2d(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s64 value = pop_long(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("l2d: %lf <-- %lld\n", (f64) value, value);
#endif
    push_double(stack, (f64) value);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_i2l(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s32 value = pop_int(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("i2l: %d --> %lld\n", (s32) value, (s64) value);
#endif
    push_long(stack, (s64) value);
    *opCode = *opCode + 1;
    return 0;
}


s32 op_i2c(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s32 value = pop_int(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("i2s(c): %d --> %d\n", (s16) value, value);
#endif
    push_int(stack, (u16) value);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_i2s(u8 **opCode, Runtime *runtime) {
    return op_i2c(opCode, runtime);
}

s32 op_i2b(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s32 value = pop_int(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("i2b: %d --> %d\n", (c8) value, value);
#endif
    push_int(stack, (c8) value);
    *opCode = *opCode + 1;
    return 0;
}


s32 op_i2f(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s32 value = pop_int(stack);
    f32 result = value;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("i2f: %d --> %f\n", (s32) value, result);
#endif
    push_float(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_i2d(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s32 value = pop_int(stack);
    f64 result = value;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("i2d: %d --> %lf\n", (s32) value, result);
#endif
    push_double(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_d2i(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f64 value1 = pop_double(stack);
    s32 result = 0;
    result = (s32) value1;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("d2i: %d <-- %lf\n", result, value1);
#endif
    push_int(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_d2l(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f64 value1 = pop_double(stack);
    s64 result = 0;
    result = (s64) value1;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("d2l: %lld <-- %lf\n", result, value1);
#endif
    push_long(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_d2f(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f64 value1 = pop_double(stack);
    f32 result = 0;
    result = (f32) value1;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("d2f: %f <-- %lf\n", result, value1);
#endif
    push_float(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_f2i(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f32 value1 = pop_float(stack);
    s32 result = 0;
    result = (s64) value1;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("f2i: %d <-- %f\n", result, value1);
#endif
    push_int(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_f2l(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f32 value1 = pop_float(stack);
    s64 result = 0;
    result = (s64) value1;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("f2l: %lld <-- %f\n", result, value1);
#endif
    push_long(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_f2d(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f32 value1 = pop_float(stack);
    f64 result = 0;
    result = value1;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("f2d: %f <-- %f\n", result, value1);
#endif
    push_double(stack, result);
    *opCode = *opCode + 1;
    return 0;
}


s32 op_irem(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s32 value1 = pop_int(stack);
    s32 value2 = pop_int(stack);
    s32 result = 0;
    result = value2 % value1;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("irem: %d % %d = %d\n", value2, value1, result);
#endif
    push_int(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_frem(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f32 value1 = pop_float(stack);
    f32 value2 = pop_float(stack);
    f32 result = 0;
    result = value2 - ((int) (value2 / value1) * value1);
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("frem: %f % %f = %f\n", value2, value1, result);
#endif
    push_float(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_lrem(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s64 value1 = pop_long(stack);
    s64 value2 = pop_long(stack);
    s64 result = value2 % value1;

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("lrem: %lld mod %lld = %lld\n", value2, value1, result);
#endif
    push_long(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_drem(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f64 value1 = pop_double(stack);
    f64 value2 = pop_double(stack);
    f64 result = value2 - ((s64) (value2 / value1) * value1);;

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("drem: %lf mod %lf = %lf\n", value2, value1, result);
#endif
    push_double(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_ineg(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s32 value1 = pop_int(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("ineg: -(%d) = %d\n", value1, -value1);
#endif
    push_int(stack, -value1);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_lneg(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s64 value1 = pop_long(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("lneg: -(%lld) = %lld\n", value1, -value1);
#endif
    push_long(stack, -value1);
    *opCode = *opCode + 1;
    return 0;
}


s32 op_fneg(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f32 value1 = pop_float(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("fneg: -(%f) = %f\n", value1, -value1);
#endif
    push_float(stack, -value1);
    *opCode = *opCode + 1;
    return 0;
}


s32 op_dneg(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f64 value1 = pop_double(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("dneg: -(%lf) = %lf\n", value1, -value1);
#endif
    push_double(stack, -value1);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_ishl(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s32 value1 = pop_int(stack);
    s32 value2 = pop_int(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("ishl: %x << %x =%x \n", value2, value1, value2 << value1);
#endif
    push_int(stack, value2 << value1);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_lshl(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s32 value1 = pop_int(stack);
    s64 value2 = pop_long(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("lshl: %llx << %x =%llx \n", value2, value1, (value2 << value1));
#endif
    push_long(stack, value2 << value1);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_ishr(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s32 value1 = pop_int(stack);
    s32 value2 = pop_int(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("ishr: %x >> %x =%x \n", value2, value1, value2 >> value1);
#endif
    push_int(stack, value2 >> value1);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_lshr(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s32 value1 = pop_int(stack);
    s64 value2 = pop_long(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("lshr: %llx >> %x =%llx \n", value2, value1, value2 >> value1);
#endif
    push_long(stack, value2 >> value1);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_iushr(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s32 value1 = pop_int(stack);
    u32 value2 = pop_int(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("iushr: %x >>> %x =%x \n", value2, value1, value2 >> value1);
#endif
    push_int(stack, value2 >> value1);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_lushr(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s32 value1 = pop_int(stack);
    u64 value2 = pop_long(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("lushr: %llx >>> %x =%llx \n", value2, value1, value2 >> value1);
#endif
    push_long(stack, value2 >> value1);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_iand(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    u32 value1 = pop_int(stack);
    u32 value2 = pop_int(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("iand: %x & %x =%x \n", value2, value1, value2 & value1);
#endif
    push_int(stack, value2 & value1);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_land(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    u64 value1 = pop_long(stack);
    u64 value2 = pop_long(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("land: %llx  &  %llx =%llx \n", value2, value1, value2 & value1);
#endif
    push_long(stack, value2 & value1);
    *opCode = *opCode + 1;
    return 0;
}


s32 op_ior(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    u32 value1 = pop_int(stack);
    u32 value2 = pop_int(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("ior: %x & %x =%x \n", value2, value1, value2 | value1);
#endif
    push_int(stack, value2 | value1);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_lor(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    u64 value1 = pop_long(stack);
    u64 value2 = pop_long(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("lor: %llx  |  %llx =%llx \n", value2, value1, value2 | value1);
#endif
    push_long(stack, value2 | value1);
    *opCode = *opCode + 1;
    return 0;
}


s32 op_ixor(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    u32 value1 = pop_int(stack);
    u32 value2 = pop_int(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("ixor: %x ^ %x =%x \n", value2, value1, value2 ^ value1);
#endif
    push_int(stack, value2 ^ value1);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_lxor(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    u64 value1 = pop_long(stack);
    u64 value2 = pop_long(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("lxor: %llx  ^  %llx =%llx \n", value2, value1, value2 ^ value1);
#endif
    push_long(stack, value2 ^ value1);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_iinc(u8 **opCode, Runtime *runtime) {
    Short2Char s2c1, s2c2;

    if (runtime->wideMode) {
        s2c1.c1 = opCode[0][1];
        s2c1.c0 = opCode[0][2];
        s2c2.c1 = opCode[0][3];
        s2c2.c0 = opCode[0][4];
        *opCode = *opCode + 5;
    } else {
        s2c1.s = (c8) opCode[0][1];
        s2c2.s = (c8) opCode[0][2];
        *opCode = *opCode + 3;
    }
    runtime->wideMode = 0;

    s32 oldv = (runtime->localVariables + s2c1.s)->integer;
    (runtime->localVariables + s2c1.s)->integer = oldv + s2c2.s;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("iinc: localvar(%d) = %d + %d\n", s2c1.s, oldv, s2c2.s);
#endif
    return 0;
}

/* istore */
s32 op_istore(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    Short2Char s2c;
    if (runtime->wideMode) {
        s2c.c1 = opCode[0][1];
        s2c.c0 = opCode[0][2];
        *opCode = *opCode + 3;
    } else {
        s2c.s = (c8) opCode[0][1];
        *opCode = *opCode + 2;
    }
    runtime->wideMode = 0;

    s32 value = pop_int(stack);
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("i(fa)store: save  localvar(%d) [%x]/%d \n", s2c.s, value, value);
#endif
    (runtime->localVariables + s2c.s)->integer = value;

    return 0;
}

s32 op_fstore(u8 **opCode, Runtime *runtime) {
    return op_istore(opCode, runtime);
}

s32 op_astore(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    Short2Char s2c;
    if (runtime->wideMode) {
        s2c.c1 = opCode[0][1];
        s2c.c0 = opCode[0][2];
        *opCode = *opCode + 3;
    } else {
        s2c.s = (c8) opCode[0][1];
        *opCode = *opCode + 2;
    }
    runtime->wideMode = 0;

    __refer value = pop_ref(stack);
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("i(fa)store: save  localvar(%d) [%llx] \n", s2c.s, (s64) (long) value);
#endif
    (runtime->localVariables + s2c.s)->refer = value;

    return 0;
}

s32 op_lstore(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;

    Short2Char s2c;
    if (runtime->wideMode) {
        s2c.c1 = opCode[0][1];
        s2c.c0 = opCode[0][2];
        *opCode = *opCode + 3;
    } else {
        s2c.s = (c8) opCode[0][1];
        *opCode = *opCode + 2;
    }
    runtime->wideMode = 0;

    Long2Double l2d;
    l2d.l = pop_long(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("l(d)store: save localvar(%d) %llx/%lld/%lf  \n", s2c.s, l2d.l, l2d.l, l2d.d);
#endif
    (runtime->localVariables + s2c.s)->integer = l2d.i2l.i1;
    (runtime->localVariables + s2c.s + 1)->integer = l2d.i2l.i0;
    return 0;
}

s32 op_dstore(u8 **opCode, Runtime *runtime) {
    return op_lstore(opCode, runtime);
}

s32 op_istore_n(u8 **opCode, Runtime *runtime, s32 i) {
    RuntimeStack *stack = runtime->stack;
    s32 value = pop_int(stack);
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("istore_%d: save %x/%d into localvar(%d)\n", i, value, value, i);
#endif
    (runtime->localVariables + i)->integer = value;
    *opCode = *opCode + 1;
    return 0;
}

s32 op_istore_0(u8 **opCode, Runtime *runtime) {
    return op_istore_n(opCode, runtime, 0);
}

/* istore_1 */
s32 op_istore_1(u8 **opCode, Runtime *runtime) {
    return op_istore_n(opCode, runtime, 1);
}

/* istore_2 */
s32 op_istore_2(u8 **opCode, Runtime *runtime) {
    return op_istore_n(opCode, runtime, 2);
}

/* istore_3 */
s32 op_istore_3(u8 **opCode, Runtime *runtime) {
    return op_istore_n(opCode, runtime, 3);
}


s32 op_fstore_0(u8 **opCode, Runtime *runtime) {
    return op_istore_n(opCode, runtime, 0);
}

s32 op_fstore_1(u8 **opCode, Runtime *runtime) {
    return op_istore_n(opCode, runtime, 1);
}

s32 op_fstore_2(u8 **opCode, Runtime *runtime) {
    return op_istore_n(opCode, runtime, 2);
}

s32 op_fstore_3(u8 **opCode, Runtime *runtime) {
    return op_istore_n(opCode, runtime, 3);
}

s32 op_lstore_n(u8 **opCode, Runtime *runtime, s32 i) {
    RuntimeStack *stack = runtime->stack;
    Long2Double l2d;
    l2d.l = pop_long(stack);

#if _JVM_DEBUG > 5

    invoke_deepth(runtime);
    jvm_printf("l(d)store_%d: save localvar(%d) [%llx]/%lld/%lf  \n", i, i, l2d.l, l2d.l, l2d.d);
#endif
    (runtime->localVariables + i)->integer = l2d.i2l.i1;
    (runtime->localVariables + i + 1)->integer = l2d.i2l.i0;
    *opCode = *opCode + 1;
    return 0;
}

s32 op_lstore_0(u8 **opCode, Runtime *runtime) {
    return op_lstore_n(opCode, runtime, 0);
}

s32 op_lstore_1(u8 **opCode, Runtime *runtime) {
    return op_lstore_n(opCode, runtime, 1);
}

s32 op_lstore_2(u8 **opCode, Runtime *runtime) {
    return op_lstore_n(opCode, runtime, 2);
}

s32 op_lstore_3(u8 **opCode, Runtime *runtime) {
    return op_lstore_n(opCode, runtime, 3);
}


s32 op_dstore_0(u8 **opCode, Runtime *runtime) {
    return op_lstore_n(opCode, runtime, 0);
}

s32 op_dstore_1(u8 **opCode, Runtime *runtime) {
    return op_lstore_n(opCode, runtime, 1);
}

s32 op_dstore_2(u8 **opCode, Runtime *runtime) {
    return op_lstore_n(opCode, runtime, 2);
}

s32 op_dstore_3(u8 **opCode, Runtime *runtime) {
    return op_lstore_n(opCode, runtime, 3);
}

s32 op_astore_n(u8 **opCode, Runtime *runtime, s32 i) {
    RuntimeStack *stack = runtime->stack;
    __refer value = pop_ref(stack);
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("astore_%d:  [%llx]\n", i, (s64) (long) value);
#endif
    (runtime->localVariables + i)->refer = value;
    *opCode = *opCode + 1;
    return 0;
}

s32 op_astore_0(u8 **opCode, Runtime *runtime) {
    return op_astore_n(opCode, runtime, 0);
}

s32 op_astore_1(u8 **opCode, Runtime *runtime) {
    return op_astore_n(opCode, runtime, 1);
}

s32 op_astore_2(u8 **opCode, Runtime *runtime) {
    return op_astore_n(opCode, runtime, 2);
}

s32 op_astore_3(u8 **opCode, Runtime *runtime) {
    return op_astore_n(opCode, runtime, 3);
}


static inline s32 op_xastore_impl(u8 **opCode, Runtime *runtime, u8 isReference) {
    RuntimeStack *stack = runtime->stack;
    StackEntry entry;
    pop_entry(stack, &entry);
    s32 index = pop_int(stack);
    Instance *ins = (Instance *) pop_ref(stack);
    s32 tidx = (ins->mb.clazz->arr_data_type);
    s32 bytes = data_type_bytes[tidx];
    Long2Double l2d;
    l2d.l = 0;
    if (tidx == DATATYPE_REFERENCE) {
        l2d.r = entry_2_refer(&entry);
    } else {
        if (bytes > 4) {
            l2d.l = entry_2_long(&entry);
        } else {
            l2d.i2l.i1 = entry_2_int(&entry);
        }
    }
    jarray_set_field(ins, index, &l2d, bytes);
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("(icbfald)astore: save array[%llx]{%d bytes}.(%d)=%d:%llx:%lf)\n",
               (s64) (long) ins, bytes, index,
               l2d.i2l.i1, (s64) (long) l2d.r,
               l2d.d);
#endif

    *opCode = *opCode + 1;
    return 0;
}

s32 op_iastore(u8 **opCode, Runtime *runtime) {
    return op_xastore_impl(opCode, runtime, 0);
}

s32 op_fastore(u8 **opCode, Runtime *runtime) {
    return op_xastore_impl(opCode, runtime, 0);
}

s32 op_castore(u8 **opCode, Runtime *runtime) {
    return op_xastore_impl(opCode, runtime, 0);
}

s32 op_sastore(u8 **opCode, Runtime *runtime) {
    return op_xastore_impl(opCode, runtime, 0);
}

s32 op_aastore(u8 **opCode, Runtime *runtime) {
    return op_xastore_impl(opCode, runtime, 1);
}

s32 op_bastore(u8 **opCode, Runtime *runtime) {
    return op_xastore_impl(opCode, runtime, 0);
}


s32 op_lastore(u8 **opCode, Runtime *runtime) {
    return op_xastore_impl(opCode, runtime, 0);
}

s32 op_dastore(u8 **opCode, Runtime *runtime) {
    return op_xastore_impl(opCode, runtime, 0);
}


/* isub */
s32 op_isub(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s32 value2 = pop_int(stack);
    s32 value1 = pop_int(stack);
    s32 result = 0;
    result = value1 - value2;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("isub : %d - %d = %d\n", value1, value2, result);
#endif
    push_int(stack, result);
    *opCode = *opCode + 1;
    return 0;
}

/* invokevirtual */
s32 op_invokevirtual(u8 **opCode, Runtime *runtime) {
    Class *clazz = runtime->clazz;
    Short2Char s2c;
    s2c.c1 = opCode[0][1];
    s2c.c0 = opCode[0][2];
    u16 object_ref = s2c.s;
    if (runtime->clazz->status < CLASS_STATUS_CLINITED) {
        class_clinit(runtime->clazz, runtime);
    }
    s32 ret = 0;
    ConstantMethodRef *cmr = find_constant_method_ref(clazz, object_ref);//此cmr所描述的方法，对于不同的实例，有不同的method

    Instance *ins = getInstanceInStack(clazz, cmr, runtime->stack);
//    if (ins == 7)
//        ins = getInstanceInStack(clazz, cmr, runtime->stack);
    if (ins == NULL) {
        Instance *exception = exception_create(JVM_EXCEPTION_NULLPOINTER, runtime);
        push_ref(runtime->stack, (__refer) exception);
        ret = RUNTIME_STATUS_EXCEPTION;
    } else {
        MethodInfo *method = NULL;

        if (ins->mb.type == MEM_TYPE_CLASS || ins->mb.type == MEM_TYPE_ARR) {
            method = cmr->methodInfo;
        } else {
            method = (MethodInfo *) pairlist_get(cmr->virtual_methods, ins->mb.clazz);
            if (method == NULL) {
                method = find_instance_methodInfo_by_name(ins, cmr->name, cmr->descriptor);
                pairlist_put(cmr->virtual_methods, ins->mb.clazz, method);//放入缓存，以便下次直接调用
            }
        }

//        if (
//            utf8_equals_c(method->name, "java/util/Calendar") &&
//                utf8_equals_c(method->name, "t20_1")) {
//            s32 count = pairlist_getl(cmr->virtual_methods, -1);
//            count++;
//            pairlist_putl(cmr->virtual_methods, -1, count);
//            if (count == 1000 || count == 1001) {
//                mem_mgr_print();
//                int debug = 1;
//            }
//        }

#if _JVM_DEBUG > 5
        invoke_deepth(runtime);
        jvm_printf("invokevirtual    %s.%s%s  \n", utf8_cstr(method->_this_class->name),
                   utf8_cstr(method->name), utf8_cstr(method->descriptor));
#endif

        if (method) {
            ret = execute_method(method, runtime, method->_this_class);
        }
    }
    *opCode = *opCode + 3;
    return ret;
}


/* invokespecial */
s32 op_invokespecial(u8 **opCode, Runtime *runtime) {
    Short2Char s2c;
    s2c.c1 = opCode[0][1];
    s2c.c0 = opCode[0][2];
    u16 object_ref = s2c.s;
    if (runtime->clazz->status < CLASS_STATUS_CLINITED) {
        class_clinit(runtime->clazz, runtime);
    }
    s32 ret = 0;
    ConstantMethodRef *cmr = find_constant_method_ref(runtime->clazz, object_ref);
    MethodInfo *method = cmr->methodInfo;
    if (!method) {
        method = find_methodInfo_by_methodref(runtime->clazz, object_ref);
        cmr->methodInfo = method;
    }
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("invokespecial    %s.%s%s \n", utf8_cstr(method->_this_class->name),
               utf8_cstr(method->name), utf8_cstr(method->descriptor));
#endif
    if (method) {
        ret = execute_method(method, runtime, method->_this_class);
    }

    *opCode = *opCode + 3;
    return ret;
}


/* 0xb8 invokestatic */
s32 op_invokestatic(u8 **opCode, Runtime *runtime) {

    Short2Char s2c;
    s2c.c1 = opCode[0][1];
    s2c.c0 = opCode[0][2];
    u16 object_ref = s2c.s;
    ConstantMethodRef *cmr = find_constant_method_ref(runtime->clazz, object_ref);
    classes_load_get(cmr->clsName, runtime);
    if (runtime->clazz->status < CLASS_STATUS_CLINITED) {
        class_clinit(runtime->clazz, runtime);
    }

    s32 ret = 0;
    MethodInfo *method = cmr->methodInfo;
    if (!method) {
        method = find_methodInfo_by_methodref(runtime->clazz, object_ref);
        cmr->methodInfo = method;
    }
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("invokestatic   | %s.%s%s \n", utf8_cstr(method->_this_class->name),
               utf8_cstr(method->name), utf8_cstr(method->descriptor));
#endif
    if (method) {
        ret = execute_method(method, runtime, method->_this_class);
    }

    *opCode = *opCode + 3;
    return ret;
}


/* 0xb8 invokestatic */
s32 op_invokeinterface(u8 **opCode, Runtime *runtime) {
    Class *clazz = runtime->clazz;
    Short2Char s2c;
    s2c.c1 = opCode[0][1];
    s2c.c0 = opCode[0][2];
    u16 object_ref = s2c.s;

    s32 paraCount = (c8) opCode[0][3];

    s32 ret = 0;
    ConstantMethodRef *cmr = find_constant_method_ref(clazz, object_ref);
    Instance *ins = getInstanceInStack(clazz, cmr, runtime->stack);
    if (ins == NULL) {
        Instance *exception = exception_create(JVM_EXCEPTION_NULLPOINTER, runtime);
        push_ref(runtime->stack, (__refer) exception);
        ret = RUNTIME_STATUS_EXCEPTION;
    } else {
        MethodInfo *method = NULL;
        if (ins->mb.type == MEM_TYPE_CLASS) {
            method = cmr->methodInfo;
        } else {
            method = (MethodInfo *) pairlist_get(cmr->virtual_methods, ins->mb.clazz);
            if (method == NULL) {
                method = find_instance_methodInfo_by_name(ins, cmr->name, cmr->descriptor);
                pairlist_put(cmr->virtual_methods, ins->mb.clazz, method);//放入缓存，以便下次直接调用
            }
        }

#if _JVM_DEBUG > 5
        invoke_deepth(runtime);
        jvm_printf("invokeinterface   | %s.%s%s \n", utf8_cstr(method->_this_class->name),
                   utf8_cstr(method->name), utf8_cstr(method->descriptor));
#endif
        if (method) {
            ret = execute_method(method, runtime, method->_this_class);
        }
    }
    *opCode = *opCode + 5;
    return ret;
}

/* 0xb8 invokestatic */
s32 op_invokedynamic(u8 **opCode, Runtime *runtime) {
    Short2Char s2c;
    s2c.c1 = opCode[0][1];
    s2c.c0 = opCode[0][2];
    u16 object_ref = s2c.s;

    s32 ret = 0;
    MethodInfo *method = find_constant_method_ref(runtime->clazz, object_ref)->methodInfo;
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("invokedynamic   | %s.%s%s \n", utf8_cstr(method->_this_class->name),
               utf8_cstr(method->name), utf8_cstr(method->descriptor));
#endif
    if (method) {
        ret = execute_method(method, runtime, method->_this_class);
    }

    *opCode = *opCode + 3;
    return ret;
}


/* ldc */
static inline s32 op_ldc_impl(u8 **opCode, Runtime *runtime, s32 index) {
    RuntimeStack *stack = runtime->stack;
    Class *clazz = runtime->clazz;

    ConstantItem *item = find_constant_item(clazz, index);
    switch (item->tag) {
        case CONSTANT_INTEGER:
        case CONSTANT_FLOAT: {
            s32 v = get_constant_integer(clazz, index);
            push_int(stack, v);
#if _JVM_DEBUG > 5
            invoke_deepth(runtime);
            jvm_printf("ldc: [%x] \n", v);
#endif
            break;
        }
        case CONSTANT_STRING_REF: {
            ConstantUTF8 *cutf = find_constant_utf8(clazz, find_constant_stringref(clazz, index)->stringIndex);
            if (!cutf->jstr) {//缓存字符串
                Instance *jstr = jstring_create(cutf->utfstr, runtime);
                garbage_refer(jstr, clazz);
                cutf->jstr = jstr;
            }
            push_ref(stack, (__refer) cutf->jstr);
            //garbage_refer(cutf->jstr, NULL);

#if _JVM_DEBUG > 5
            invoke_deepth(runtime);
            jvm_printf("ldc: [%llx] =\"%s\"\n", (s64) (long) cutf->jstr, utf8_cstr(cutf->utfstr));
#endif
            break;
        }
    }

    return 0;
}

s32 op_ldc(u8 **opCode, Runtime *runtime) {
    s32 index = opCode[0][1];
    *opCode = *opCode + 2;
    return op_ldc_impl(opCode, runtime, index);
}

s32 op_ldc_w(u8 **opCode, Runtime *runtime) {
    Short2Char s2c;
    s2c.c1 = opCode[0][1];
    s2c.c0 = opCode[0][2];
    s32 index = s2c.s;
    op_ldc_impl(opCode, runtime, index);
    *opCode = *opCode + 3;
    return 0;
}

s32 op_ldc2_w(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    Short2Char s2c;
    s2c.c1 = opCode[0][1];
    s2c.c0 = opCode[0][2];

    s32 index = s2c.s;
    s64 value = get_constant_long(runtime->clazz, index);

    push_long(stack, value);
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("ldc2_w: push a constant(%d) [%llx] onto the stack \n", index, value);
#endif
    *opCode = *opCode + 3;
    return 0;
}

/* 0x11 op_sipush */
s32 op_sipush(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    Short2Char s2c;
    s2c.c1 = opCode[0][1];
    s2c.c0 = opCode[0][2];
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("sipush value %d\n", s2c.s);
#endif
    push_int(stack, s2c.s);
    *opCode = *opCode + 3;
    return 0;
}


static inline s32 op_putfield_impl(u8 **opCode, Runtime *runtime, s32 isStatic) {
    RuntimeStack *stack = runtime->stack;
    Class *clazz = runtime->clazz;
    Short2Char s2c;
    s2c.c1 = opCode[0][1];
    s2c.c0 = opCode[0][2];

    u16 field_ref = s2c.s;
//    if (utf8_equals_c(clazz->name, "javax/mini/jdwp/analyzer/AnalyzerManager")) {
//        int debug = 1;
//    }
    if (clazz->status < CLASS_STATUS_CLINITED) {
        class_clinit(clazz, runtime);
    }
    // check variable type to determain long/s32/f64/f32
    FieldInfo *fi = find_constant_fieldref(clazz, field_ref)->fieldInfo;
    if (fi == NULL) {
        fi = find_fieldInfo_by_fieldref(clazz, field_ref);
        find_constant_fieldref(clazz, field_ref)->fieldInfo = fi;
    }
    c8 ch = utf8_char_at(fi->descriptor, 0);
    s32 data_bytes = data_type_bytes[getDataTypeIndex(ch)];
    StackEntry entry;
    pop_entry(stack, &entry);


    Instance *ins = NULL;
    c8 *ptr;
    if (isStatic) {
        ptr = getStaticFieldPtr(fi);
    } else {
        ins = (Instance *) pop_ref(stack);
        if (!ins) {
            Instance *exception = exception_create(JVM_EXCEPTION_NULLPOINTER, runtime);
            push_ref(stack, (__refer) exception);
            return RUNTIME_STATUS_EXCEPTION;
        }
        ptr = getInstanceFieldPtr(ins, fi);
    }
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("%s  save:%s[%llx].%s[%llx]=[%llx]  \n",
               isStatic ? "putstatic" : "putfield", utf8_cstr(clazz->name),
               isStatic ? (s64) (long) clazz : (s64) (long) ins, utf8_cstr(fi->name),
               (s64) (long) ptr, entry_2_long(&entry));
#endif

    if (isDataReferByTag(ch)) {//垃圾回收标识
        __refer ref = getFieldRefer(ptr);
        if (ref) { //把老索引关闭
            if (isStatic) {
                garbage_derefer(ref, fi->_this_class);
            } else {
                garbage_derefer(ref, ins);
            }
        }
        __refer newins = entry_2_refer(&entry);
        if (isStatic) { //建立新索引
            garbage_refer(newins, fi->_this_class);
        } else {
            garbage_refer(newins, ins);
        }
        setFieldRefer(ptr, newins);
    } else {
        s32 value = 0;
        s64 valuelong = 0;
        if (data_bytes > 4)valuelong = entry_2_long(&entry);
        else value = entry_2_int(&entry);
        //非引用类型
        switch (data_bytes) {
            case 8: {
                setFieldLong(ptr, valuelong);
                break;
            }
            case 4: {
                setFieldInt(ptr, value);
                break;
            }
            case 2: {
                setFieldShort(ptr, value);
                break;
            }
            case 1: {
                setFieldByte(ptr, value);
                break;
            }
        }
    }
    *opCode = *opCode + 3;
    return 0;
}

s32 op_putfield(u8 **opCode, Runtime *runtime) {
    return op_putfield_impl(opCode, runtime, 0);
}

s32 op_putstatic(u8 **opCode, Runtime *runtime) {
    return op_putfield_impl(opCode, runtime, 1);
}

static inline s32 op_getfield_impl(u8 **opCode, Runtime *runtime, s32 isStatic) {
    RuntimeStack *stack = runtime->stack;
    Class *clazz = runtime->clazz;
    Short2Char s2c;
    s2c.c1 = opCode[0][1];
    s2c.c0 = opCode[0][2];

    u16 field_ref = s2c.s;
    if (clazz->status <= CLASS_STATUS_CLINITED) {
        class_clinit(clazz, runtime);
    }

    // check variable type to determain long/s32/f64/f32
    FieldInfo *fi = find_constant_fieldref(clazz, field_ref)->fieldInfo;
    if (fi == NULL) {
        fi = find_fieldInfo_by_fieldref(clazz, field_ref);
        find_constant_fieldref(clazz, field_ref)->fieldInfo = fi;
    }
    c8 ch = utf8_char_at(fi->descriptor, 0);
    s32 data_bytes = data_type_bytes[getDataTypeIndex(ch)];

    Instance *ins = NULL;
    c8 *ptr;
    if (isStatic) {
        ptr = getStaticFieldPtr(fi);
    } else {
        ins = (Instance *) pop_ref(stack);
        if (!ins) {
            Instance *exception = exception_create(JVM_EXCEPTION_NULLPOINTER, runtime);
            push_ref(stack, (__refer) exception);
            return RUNTIME_STATUS_EXCEPTION;
        }
        ptr = getInstanceFieldPtr(ins, fi);
    }
    Long2Double l2d;
    if (isDataReferByTag(ch)) {
        l2d.r = getFieldRefer(ptr);
        push_ref(stack, l2d.r);
    } else {
        switch (data_bytes) {
            case 8: {
                l2d.l = getFieldLong(ptr);
                push_long(stack, l2d.l);
                break;
            }
            case 4: {
                l2d.i2l.i1 = getFieldInt(ptr);
                push_int(stack, l2d.i2l.i1);
                break;
            }
            case 2: {
                l2d.i2l.i1 = getFieldShort(ptr);
                push_int(stack, l2d.i2l.i1);
                break;
            }
            case 1: {
                l2d.i2l.i1 = getFieldByte(ptr);
                push_int(stack, l2d.i2l.i1);
                break;
            }
        }
    }
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("%s: push %s[%llx].%s[%llx]=[%llx]\n",
               isStatic ? "getstatic" : "getfield", utf8_cstr(clazz->name),
               isStatic ? (s64) (long) clazz : (s64) (long) ins, utf8_cstr(fi->name),
               (s64) (long) ptr, l2d.l);
#endif
    *opCode = *opCode + 3;
    return 0;

}

s32 op_getfield(u8 **opCode, Runtime *runtime) {
    return op_getfield_impl(opCode, runtime, 0);
}

s32 op_getstatic(u8 **opCode, Runtime *runtime) {
    return op_getfield_impl(opCode, runtime, 1);
}


/* op_new */
s32 op_new(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    Class *clazz = runtime->clazz;
    Short2Char s2c;
    s2c.c1 = opCode[0][1];
    s2c.c0 = opCode[0][2];

    u16 object_ref = s2c.s;

    ConstantClassRef *ccf = find_constant_classref(clazz, object_ref);
    Utf8String *clsName = get_utf8_string(clazz, ccf->stringIndex);
    Class *other = classes_load_get(clsName, runtime);
    Instance *ins = NULL;
    if (other) {
        ins = instance_create(other);
        garbage_refer(ins, NULL);
    }
    push_ref(stack, (__refer) ins);
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("new %s [%llx]\n", utf8_cstr(clsName), (s64) (long) ins);
#endif
    *opCode = *opCode + 3;
    return 0;
}

static inline s32 op_newarray_impl(Runtime *runtime, s32 count, s32 typeIdx, Utf8String *type) {
    RuntimeStack *stack = runtime->stack;
    Instance *arr = jarray_create(count, typeIdx, type);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("(a)newarray  [%llx] type:%c , count:%d  \n", (s64) (long) arr, getDataTypeTag(typeIdx), count);
#endif
    if (arr) {
        push_ref(stack, (__refer) arr);
        garbage_refer(arr, NULL);
    } else {
        Instance *exception = exception_create(JVM_EXCEPTION_NULLPOINTER, runtime);
        push_ref(stack, (__refer) exception);
        return RUNTIME_STATUS_EXCEPTION;
    }

    return 0;
}

/* op_newarray */
s32 op_newarray(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s32 typeIdx = opCode[0][1];

    s32 count = pop_int(stack);
    *opCode = *opCode + 2;
    return op_newarray_impl(runtime, count, typeIdx, NULL);
}

s32 op_anewarray(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    Short2Char s2c;
    s2c.c1 = opCode[0][1];
    s2c.c0 = opCode[0][2];

    s32 count = pop_int(stack);
    *opCode = *opCode + 3;
    return op_newarray_impl(runtime, count, 0, get_utf8_string(runtime->clazz, s2c.s));
}

s32 op_multianewarray(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    //data type index
    Short2Char s2c;
    s2c.c1 = opCode[0][1];
    s2c.c0 = opCode[0][2];

    Utf8String *desc = get_utf8_string(runtime->clazz, s2c.s);
    //array dim
    s32 count = (c8) opCode[0][3];
    ArrayList *dim = arraylist_create(count);
    int i;
    for (i = 0; i < count; i++)
        arraylist_append(dim, (ArrayListValue) (long) pop_int(stack));

    Instance *arr = jarray_multi_create(dim, desc, 0);
    arraylist_destory(dim);
    garbage_refer(arr, NULL);
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("multianewarray  [%llx] type:%s , count:%d  \n", (s64) (long) arr, utf8_cstr(desc), count);
#endif
    if (arr) {
        push_ref(stack, (__refer) arr);
    } else {
        Instance *exception = exception_create(JVM_EXCEPTION_NULLPOINTER, runtime);
        push_ref(stack, (__refer) exception);
        return RUNTIME_STATUS_EXCEPTION;
    }
    *opCode = *opCode + 4;
    return 0;
}

s32 op_arraylength(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    Instance *arr_ref = (Instance *) pop_ref(stack);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("arraylength  [%llx].arr_body[%llx] len:%d  \n",
               (s64) (long) arr_ref, (s64) (long) arr_ref->arr_body, arr_ref->arr_length);
#endif
    push_int(stack, arr_ref->arr_length);
    *opCode = *opCode + 1;
    return 0;
}


s32 op_athrow(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    Instance *ins = (Instance *) pop_ref(stack);
    push_ref(stack, (__refer) ins);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("athrow  [%llx].exception throws  \n", (s64) (long) ins);
#endif
    //*opCode = *opCode + 1;
    return RUNTIME_STATUS_EXCEPTION;
}

s32 op_checkcast(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    Instance *ins = (Instance *) pop_ref(stack);
    Short2Char s2c;
    s2c.c1 = opCode[0][1];
    s2c.c0 = opCode[0][2];

    s32 typeIdx = s2c.s;
    s32 checkok = 0;
    if (ins != NULL) {
        if (ins->mb.type == MEM_TYPE_INS) {
            Class *cl = getClassByConstantClassRef(runtime->clazz, typeIdx);
            if (instance_of(cl, ins)) {
                checkok = 1;
            }
        } else if (ins->mb.type == MEM_TYPE_ARR) {
            Utf8String *utf = find_constant_classref(runtime->clazz, typeIdx)->name;
            u8 ch = utf8_char_at(utf, 1);
            if (getDataTypeIndex(ch) == ins->mb.clazz->arr_data_type) {
                checkok = 1;
            }
        }
    } else {
        checkok = 1;
    }
    if (!checkok) {
        Instance *exception = exception_create(JVM_EXCEPTION_CLASSCASTEXCEPTION, runtime);
        push_ref(stack, (__refer) exception);
        return RUNTIME_STATUS_EXCEPTION;
    } else
        push_ref(stack, (__refer) ins);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("checkcast  [%llx] instancof %s is:%d \n", (s64) (long) ins,
               utf8_cstr(find_constant_classref(runtime->clazz, typeIdx)->name),
               checkok);
#endif
    *opCode = *opCode + 3;
    return RUNTIME_STATUS_NORMAL;
}


s32 op_instanceof(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    Instance *ins = (Instance *) pop_ref(stack);
    Short2Char s2c;
    s2c.c1 = opCode[0][1];
    s2c.c0 = opCode[0][2];

    s32 typeIdx = s2c.s;

    s32 checkok = 0;
    if (ins->mb.type == MEM_TYPE_INS) {
        if (instance_of(getClassByConstantClassRef(runtime->clazz, typeIdx), ins)) {
            checkok = 1;
        }
    } else {
        if (utf8_equals(ins->mb.clazz->name, getClassByConstantClassRef(runtime->clazz, typeIdx)->name)) {//
            checkok = 1;
        }
    }
    push_int(stack, checkok);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("instanceof  [%llx] instancof %s  \n", (s64) (long) ins,
               utf8_cstr(find_constant_classref(runtime->clazz, typeIdx)->name));
#endif
    *opCode = *opCode + 3;
    return 0;
}

s32 op_monitorenter(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    Instance *ins = (Instance *) pop_ref(stack);
    jthread_lock(&ins->mb, runtime);
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("monitorenter  [%llx] %s  \n", (s64) (long) ins, ins ? utf8_cstr(ins->mb.clazz->name) : "null");
#endif
    *opCode = *opCode + 1;
    return 0;
}

s32 op_monitorexit(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    Instance *ins = (Instance *) pop_ref(stack);
    jthread_unlock(&ins->mb, runtime);
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("monitorexit  [%llx] %s  \n", (s64) (long) ins, ins ? utf8_cstr(ins->mb.clazz->name) : "null");
#endif
    *opCode = *opCode + 1;
    return 0;
}

s32 op_wide(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("wide  \n");
#endif
    runtime->wideMode = 1;
    //op_notsupport(opCode, runtime);
    *opCode = *opCode + 1;
    return 0;
}

s32 op_breakpoint(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;


#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("breakpoint \n");
#endif
    //*opCode = *opCode + 1;
    return 0;
}

/* return */
s32 op_return(u8 **opCode, Runtime *runtime) {
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("return: \n");
#endif
    *opCode = *opCode + 1;
    return RUNTIME_STATUS_RETURN;
}


s32 op_ireturn(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;

#if _JVM_DEBUG > 5
    StackEntry entry;
    peek_entry(stack, &entry, stack->size - 1);
    invoke_deepth(runtime);
    jvm_printf("i(lfda)return [%x]/%d/[%llx]\n", entry_2_int(&entry), entry_2_int(&entry), entry_2_long(&entry));
#endif
    *opCode = *opCode + 1;
    return RUNTIME_STATUS_RETURN;
}

s32 op_if_cmp_ia(u8 **opCode, Runtime *runtime, s32 type) {
    RuntimeStack *stack = runtime->stack;

    Short2Char s2c;
    s2c.c1 = opCode[0][1];
    s2c.c0 = opCode[0][2];

    s32 branchoffset = s2c.s;


    s32 v2 = pop_int(stack);
    s32 v1 = pop_int(stack);
    c8 *syb;
    s32 con = 0;
    switch (type) {
        case TYPE_IF_ACMPEQ:
            con = v1 == v2;
            syb = "==";
            break;
        case TYPE_IF_ACMPNE:
            con = v1 != v2;
            syb = "!=";
            break;
        case TYPE_IF_ICMPEQ:
            con = v1 == v2;
            syb = "==";
            break;
        case TYPE_IF_ICMPGE:
            con = v1 >= v2;
            syb = ">=";
            break;
        case TYPE_IF_ICMPGT:
            con = v1 > v2;
            syb = ">";
            break;
        case TYPE_IF_ICMPLE:
            con = v1 <= v2;
            syb = "<=";
            break;
        case TYPE_IF_ICMPLT:
            con = v1 < v2;
            syb = "<";
            break;
        case TYPE_IF_ICMPNE:
            con = v1 != v2;
            syb = "!=";
            break;
    }
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("ifcmp: %d %s %d =%d then %d\n", v1, syb, v2, con, branchoffset);
#endif
    if (con) {
        *opCode = *opCode + branchoffset;
    } else {
        *opCode = *opCode + 3;
    }
    return 0;
}

s32 op_if_acmpeq(u8 **opCode, Runtime *runtime) {
    return op_if_cmp_ia(opCode, runtime, TYPE_IF_ACMPEQ);
}

s32 op_if_acmpne(u8 **opCode, Runtime *runtime) {
    return op_if_cmp_ia(opCode, runtime, TYPE_IF_ACMPNE);
}

s32 op_if_icmpeq(u8 **opCode, Runtime *runtime) {
    return op_if_cmp_ia(opCode, runtime, TYPE_IF_ICMPEQ);
}

s32 op_if_icmpne(u8 **opCode, Runtime *runtime) {
    return op_if_cmp_ia(opCode, runtime, TYPE_IF_ICMPNE);
}

s32 op_if_icmple(u8 **opCode, Runtime *runtime) {
    return op_if_cmp_ia(opCode, runtime, TYPE_IF_ICMPLE);
}

s32 op_if_icmpge(u8 **opCode, Runtime *runtime) {
    return op_if_cmp_ia(opCode, runtime, TYPE_IF_ICMPGE);
}

s32 op_if_icmplt(u8 **opCode, Runtime *runtime) {
    return op_if_cmp_ia(opCode, runtime, TYPE_IF_ICMPLT);
}

s32 op_if_icmpgt(u8 **opCode, Runtime *runtime) {
    return op_if_cmp_ia(opCode, runtime, TYPE_IF_ICMPGT);
}


s32 op_lcmp(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    s64 value1 = pop_long(stack);
    s64 value2 = pop_long(stack);
    s32 result = value2 == value1 ? 0 : (value2 > value1 ? 1 : -1);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("lcmp: %llx cmp %llx = %d\n", value2, value1, result);
#endif
    push_int(stack, result);

    *opCode = *opCode + 1;
    return 0;
}


s32 op_fcmpl(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f32 value1 = pop_float(stack);
    f32 value2 = pop_float(stack);
    s32 result = value2 == value1 ? 0 : (value2 > value1 ? 1 : -1);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("fcmpl: %f < %f = %d\n", value2, value1, result);
#endif
    push_int(stack, result);

    *opCode = *opCode + 1;
    return 0;
}

s32 op_fcmpg(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f32 value1 = pop_float(stack);
    f32 value2 = pop_float(stack);
    s32 result = value2 == value1 ? 0 : (value2 > value1 ? 1 : -1);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("fcmpg: %f > %f = %d\n", value2, value1, result);
#endif
    push_int(stack, result);

    *opCode = *opCode + 1;
    return 0;
}

s32 op_dcmpl(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f64 value1 = pop_double(stack);
    f64 value2 = pop_double(stack);
    s32 result = value2 == value1 ? 0 : (value2 > value1 ? 1 : -1);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("dcmpl: %f < %f = %d\n", value2, value1, result);
#endif
    push_int(stack, result);

    *opCode = *opCode + 1;
    return 0;
}

s32 op_dcmpg(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    f64 value1 = pop_double(stack);
    f64 value2 = pop_double(stack);
    s32 result = value2 == value1 ? 0 : (value2 > value1 ? 1 : -1);

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("dcmpg: %f > %f = %d\n", value2, value1, result);
#endif
    push_int(stack, result);

    *opCode = *opCode + 1;
    return 0;
}


s32 op_if_0(u8 **opCode, Runtime *runtime, s32 type) {
    RuntimeStack *stack = runtime->stack;
    Short2Char s2c;
    s2c.c1 = opCode[0][1];
    s2c.c0 = opCode[0][2];

    s32 branchoffset = s2c.s;

    c8 *syb;
    StackEntry entry;
    pop_entry(stack, &entry);
    s32 con = 0;
    Long2Double l2d;
    if (is_ref(&entry)) {
        l2d.r = entry_2_refer(&entry);
    } else {
        l2d.i2l.i1 = entry_2_int(&entry);
    }
    switch (type) {
        case TYPE_IFEQ:
            con = l2d.i2l.i1 == 0;
            syb = "==";
            break;
        case TYPE_IFNE:
            con = l2d.i2l.i1 != 0;
            syb = "!=";
            break;
        case TYPE_IFLT:
            con = l2d.i2l.i1 < 0;
            syb = "<";
            break;
        case TYPE_IFGT:
            con = l2d.i2l.i1 > 0;
            syb = ">";
            break;
        case TYPE_IFLE:
            con = l2d.i2l.i1 <= 0;
            syb = "<";
            break;
        case TYPE_IFGE:
            con = l2d.i2l.i1 >= 0;
            syb = ">=";
            break;
        case TYPE_IFNONNULL:
            con = l2d.r != 0;
            syb = "!= NULL";
            break;
        case TYPE_IFNULL:
            con = l2d.r == 0;
            syb = "== NULL";
            break;
    }

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("if_0: %d/%llx %s 0  then %d \n", l2d.i2l.i1, (s64) (long) l2d.r, syb, branchoffset);
#endif
    if (con) {
        *opCode = *opCode + branchoffset;
    } else {
        *opCode = *opCode + 3;
    }
    return 0;
}


s32 op_ifnonnull(u8 **opCode, Runtime *runtime) {
    return op_if_0(opCode, runtime, TYPE_IFNONNULL);
}

s32 op_ifnull(u8 **opCode, Runtime *runtime) {
    return op_if_0(opCode, runtime, TYPE_IFNULL);
}

s32 op_iflt(u8 **opCode, Runtime *runtime) {
    return op_if_0(opCode, runtime, TYPE_IFLT);
}

s32 op_ifgt(u8 **opCode, Runtime *runtime) {
    return op_if_0(opCode, runtime, TYPE_IFGT);
}

s32 op_ifle(u8 **opCode, Runtime *runtime) {
    return op_if_0(opCode, runtime, TYPE_IFLE);
}

s32 op_ifge(u8 **opCode, Runtime *runtime) {
    return op_if_0(opCode, runtime, TYPE_IFGE);
}

s32 op_ifeq(u8 **opCode, Runtime *runtime) {
    return op_if_0(opCode, runtime, TYPE_IFEQ);
}

s32 op_ifne(u8 **opCode, Runtime *runtime) {
    return op_if_0(opCode, runtime, TYPE_IFNE);
}


s32 op_goto(u8 **opCode, Runtime *runtime) {
    Short2Char s2c;
    s2c.c1 = opCode[0][1];
    s2c.c0 = opCode[0][2];

    s32 branchoffset = s2c.s;

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("goto: %d\n", branchoffset);
#endif
    *opCode = *opCode + branchoffset;

    return 0;
}

s32 op_jsr(u8 **opCode, Runtime *runtime) {
    RuntimeStack *stack = runtime->stack;
    Short2Char s2c;
    s2c.c1 = opCode[0][1];
    s2c.c0 = opCode[0][2];

    s32 branchoffset = s2c.s;
    push_ref(stack, (__refer) (*opCode + 3));
#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("jsr: %d\n", branchoffset);
#endif
    *opCode = *opCode + branchoffset;

    return 0;
}

s32 op_ret(u8 **opCode, Runtime *runtime) {
    Short2Char s2c;
    if (runtime->wideMode) {
        s2c.c1 = opCode[0][1];
        s2c.c0 = opCode[0][2];
    } else {
        s2c.s = (c8) opCode[0][1];
    }
    runtime->wideMode = 0;

    __refer addr = (runtime->localVariables + s2c.s)->refer;

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("ret: %x\n", (s64) (long) addr);
#endif
    *opCode = (u8 *) addr;

    return 0;
}


s32 op_tableswitch(u8 **opCode, Runtime *runtime) {
    s32 pos = 0;
    pos = 4 - ((((u64) (long) *opCode) - (u64) (long) (runtime->ca->code)) % 4);//4 byte对齐

    Int2Float i2c;
    i2c.c3 = opCode[0][pos++];
    i2c.c2 = opCode[0][pos++];
    i2c.c1 = opCode[0][pos++];
    i2c.c0 = opCode[0][pos++];
    s32 default_offset = i2c.i;
    i2c.c3 = opCode[0][pos++];
    i2c.c2 = opCode[0][pos++];
    i2c.c1 = opCode[0][pos++];
    i2c.c0 = opCode[0][pos++];
    s32 low = i2c.i;
    i2c.c3 = opCode[0][pos++];
    i2c.c2 = opCode[0][pos++];
    i2c.c1 = opCode[0][pos++];
    i2c.c0 = opCode[0][pos++];
    s32 high = i2c.i;

    int val = pop_int(runtime->stack);// pop an int from the stack
    int offset = 0;
    if (val < low || val > high) {  // if its less than <low> or greater than <high>,
        offset = default_offset;              // branch to default
    } else {                        // otherwise
        pos += (val - low) * 4;
        i2c.c3 = opCode[0][pos++];
        i2c.c2 = opCode[0][pos++];
        i2c.c1 = opCode[0][pos++];
        i2c.c0 = opCode[0][pos++];
        offset = i2c.i;     // branch to entry in table
    }

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("tableswitch: val=%d, offset=%d\n", val, offset);
#endif
    *opCode = *opCode + offset;

    return 0;
}

s32 op_lookupswitch(u8 **opCode, Runtime *runtime) {
    s32 pos = 0;
    pos = 4 - ((((u64) (long) *opCode) - (u64) (long) (runtime->ca->code)) % 4);//4 byte对齐
    Int2Float i2c;
    i2c.c3 = opCode[0][pos++];
    i2c.c2 = opCode[0][pos++];
    i2c.c1 = opCode[0][pos++];
    i2c.c0 = opCode[0][pos++];
    s32 default_offset = i2c.i;
    i2c.c3 = opCode[0][pos++];
    i2c.c2 = opCode[0][pos++];
    i2c.c1 = opCode[0][pos++];
    i2c.c0 = opCode[0][pos++];
    s32 n = i2c.i;
    s32 i, key;

    int val = pop_int(runtime->stack);// pop an int from the stack
    int offset = default_offset;
    for (i = 0; i < n; i++) {
        i2c.c3 = opCode[0][pos++];
        i2c.c2 = opCode[0][pos++];
        i2c.c1 = opCode[0][pos++];
        i2c.c0 = opCode[0][pos++];
        key = i2c.i;
        if (key == val) {
            i2c.c3 = opCode[0][pos++];
            i2c.c2 = opCode[0][pos++];
            i2c.c1 = opCode[0][pos++];
            i2c.c0 = opCode[0][pos++];
            offset = i2c.i;
            break;
        } else {
            pos += 4;
        }
    }

#if _JVM_DEBUG > 5
    invoke_deepth(runtime);
    jvm_printf("tableswitch: val=%d, offset=%d\n", val, offset);
#endif
    *opCode = *opCode + offset;

}

s32 op_notsupport(u8 **opCode, Runtime *runtime) {
    invoke_deepth(runtime);
    jvm_printf("not support instruct [%x]\n", opCode[0][0]);
    exit(-3);
    return 0;
}

static Instruction instructionSet[] = {
        {"nop",             0x00, 1, op_nop},
        {"aconst_null",     0x01, 1, op_aconst_null},
        {"iconst_m1",       0x02, 1, op_iconst_m1},
        {"iconst_0",        0x03, 1, op_iconst_0},
        {"iconst_1",        0x04, 1, op_iconst_1},
        {"iconst_2",        0x05, 1, op_iconst_2},
        {"iconst_3",        0x06, 1, op_iconst_3},
        {"iconst_4",        0x07, 1, op_iconst_4},
        {"iconst_5",        0x08, 1, op_iconst_5},
        {"lconst_0",        0x09, 1, op_lconst_0},
        {"lconst_1",        0x0A, 1, op_lconst_1},
        {"fconst_0",        0x0B, 1, op_fconst_0},
        {"fconst_1",        0x0C, 1, op_fconst_1},
        {"fconst_2",        0x0D, 1, op_fconst_2},
        {"dconst_0",        0x0E, 1, op_dconst_0},
        {"dconst_1",        0x0F, 1, op_dconst_1},
        {"bipush",          0x10, 2, op_bipush},
        {"sipush",          0x11, 3, op_sipush},
        {"ldc",             0x12, 2, op_ldc},
        {"ldc_w",           0x13, 3, op_ldc_w},
        {"ldc2_w",          0x14, 3, op_ldc2_w},
        {"iload",           0x15, 2, op_iload},
        {"lload",           0x16, 2, op_lload},
        {"fload",           0x17, 2, op_fload},
        {"dload",           0x18, 2, op_dload},
        {"aload",           0x19, 2, op_aload},
        {"iload_0",         0x1A, 1, op_iload_0},
        {"iload_1",         0x1B, 1, op_iload_1},
        {"iload_2",         0x1C, 1, op_iload_2},
        {"iload_3",         0x1D, 1, op_iload_3},
        {"lload_0",         0x1E, 1, op_lload_0},
        {"lload_1",         0x1F, 1, op_lload_1},
        {"lload_2",         0x20, 1, op_lload_2},
        {"lload_3",         0x21, 1, op_lload_3},
        {"fload_0",         0x22, 1, op_fload_0},
        {"fload_1",         0x23, 1, op_fload_1},
        {"fload_2",         0x24, 1, op_fload_2},
        {"fload_3",         0x25, 1, op_fload_3},
        {"dload_0",         0x26, 1, op_dload_0},
        {"dload_1",         0x27, 1, op_dload_1},
        {"dload_2",         0x28, 1, op_dload_2},
        {"dload_3",         0x29, 1, op_dload_3},
        {"aload_0",         0x2A, 1, op_aload_0},
        {"aload_1",         0x2B, 1, op_aload_1},
        {"aload_2",         0x2C, 1, op_aload_2},
        {"aload_3",         0x2D, 1, op_aload_3},
        {"iaload",          0x2E, 1, op_iaload},
        {"laload",          0x2F, 1, op_laload},
        {"faload",          0x30, 1, op_faload},
        {"daload",          0x31, 1, op_daload},
        {"aaload",          0x32, 1, op_aaload},
        {"baload",          0x33, 1, op_baload},
        {"caload",          0x34, 1, op_caload},
        {"saload",          0x35, 1, op_saload},
        {"istore",          0x36, 2, op_istore},
        {"lstore",          0x37, 2, op_lstore},
        {"fstore",          0x38, 2, op_fstore},
        {"dstore",          0x39, 2, op_dstore},
        {"astore",          0x3A, 2, op_astore},
        {"istore_0",        0x3B, 1, op_istore_0},
        {"istore_1",        0x3C, 1, op_istore_1},
        {"istore_2",        0x3D, 1, op_istore_2},
        {"istore_3",        0x3E, 1, op_istore_3},
        {"lstore_0",        0x3F, 1, op_lstore_0},
        {"lstore_1",        0x40, 1, op_lstore_1},
        {"lstore_2",        0x41, 1, op_lstore_2},
        {"lstore_3",        0x42, 1, op_lstore_3},
        {"fstore_0",        0x43, 1, op_fstore_0},
        {"fstore_1",        0x44, 1, op_fstore_1},
        {"fstore_2",        0x45, 1, op_fstore_2},
        {"fstore_3",        0x46, 1, op_fstore_3},
        {"dstore_0",        0x47, 1, op_dstore_0},
        {"dstore_1",        0x48, 1, op_dstore_1},
        {"dstore_2",        0x49, 1, op_dstore_2},
        {"dstore_3",        0x4A, 1, op_dstore_3},
        {"astore_0",        0x4B, 1, op_astore_0},
        {"astore_1",        0x4C, 1, op_astore_1},
        {"astore_2",        0x4D, 1, op_astore_2},
        {"astore_3",        0x4E, 1, op_astore_3},
        {"iastore",         0x4F, 1, op_iastore},
        {"lastore",         0x50, 1, op_lastore},
        {"fastore",         0x51, 1, op_fastore},
        {"dastore",         0x52, 1, op_dastore},
        {"aastore",         0x53, 1, op_aastore},
        {"bastore",         0x54, 1, op_bastore},
        {"castore",         0x55, 1, op_castore},
        {"sastore",         0x56, 1, op_sastore},
        {"pop",             0x57, 1, op_pop},
        {"pop2",            0x58, 1, op_pop2},
        {"dup",             0x59, 1, op_dup},
        {"dup_x1",          0x5A, 1, op_dup_x1},
        {"dup_x2",          0x5B, 1, op_dup_x2},
        {"dup2",            0x5C, 1, op_dup2},
        {"dup2_x1",         0x5D, 1, op_dup2_x1},
        {"dup2_x2",         0x5E, 1, op_dup2_x2},
        {"swap",            0x5F, 1, op_swap},
        {"iadd",            0x60, 1, op_iadd},
        {"ladd",            0x61, 1, op_ladd},
        {"fadd",            0x62, 1, op_fadd},
        {"dadd",            0x63, 1, op_dadd},
        {"isub",            0x64, 1, op_isub},
        {"lsub",            0x65, 1, op_lsub},
        {"fsub",            0x66, 1, op_fsub},
        {"dsub",            0x67, 1, op_dsub},
        {"imul",            0x68, 1, op_imul},
        {"lmul",            0x69, 1, op_lmul},
        {"fmul",            0x6A, 1, op_fmul},
        {"dmul",            0x6B, 1, op_dmul},
        {"idiv",            0x6C, 1, op_idiv},
        {"ldiv",            0x6D, 1, op_ldiv},
        {"fdiv",            0x6E, 1, op_fdiv},
        {"ddiv",            0x6F, 1, op_ddiv},
        {"irem",            0x70, 1, op_irem},
        {"lrem",            0x71, 1, op_lrem},
        {"frem",            0x72, 1, op_frem},
        {"drem",            0x73, 1, op_drem},
        {"ineg",            0x74, 1, op_ineg},
        {"lneg",            0x75, 1, op_lneg},
        {"fneg",            0x76, 1, op_fneg},
        {"dneg",            0x77, 1, op_dneg},
        {"ishl",            0x78, 1, op_ishl},
        {"lshl",            0x79, 1, op_lshl},
        {"ishr",            0x7A, 1, op_ishr},
        {"lshr",            0x7B, 1, op_lshr},
        {"iushr",           0x7C, 1, op_iushr},
        {"lushr",           0x7D, 1, op_lushr},
        {"iand",            0x7E, 1, op_iand},
        {"land",            0x7F, 1, op_land},
        {"ior",             0x80, 1, op_ior},
        {"lor",             0x81, 1, op_lor},
        {"ixor",            0x82, 1, op_ixor},
        {"lxor",            0x83, 1, op_lxor},
        {"iinc",            0x84, 1, op_iinc},
        {"i2l",             0x85, 1, op_i2l},
        {"i2f",             0x86, 1, op_i2f},
        {"i2d",             0x87, 1, op_i2d},
        {"l2i",             0x88, 1, op_l2i},
        {"l2f",             0x89, 1, op_l2f},
        {"l2d",             0x8A, 1, op_l2d},
        {"f2i",             0x8B, 1, op_f2i},
        {"f2l",             0x8C, 1, op_f2l},
        {"f2d",             0x8D, 1, op_f2d},
        {"d2i",             0x8E, 1, op_d2i},
        {"d2l",             0x8F, 1, op_d2l},
        {"d2f",             0x90, 1, op_d2f},
        {"i2b",             0x91, 1, op_i2b},
        {"i2c",             0x92, 1, op_i2c},
        {"i2s",             0x93, 1, op_i2s},
        {"lcmp",            0x94, 1, op_lcmp},
        {"fcmpl",           0x95, 1, op_fcmpl},
        {"fcmpg",           0x96, 1, op_fcmpg},
        {"dcmpl",           0x97, 1, op_dcmpl},
        {"dcmpg",           0x98, 1, op_dcmpg},
        {"ifeq",            0x99, 3, op_ifeq},
        {"ifne",            0x9A, 3, op_ifne},
        {"iflt",            0x9B, 3, op_iflt},
        {"ifge",            0x9C, 3, op_ifge},
        {"ifgt",            0x9D, 3, op_ifgt},
        {"ifle",            0x9E, 3, op_ifle},
        {"if_icmpeq",       0x9F, 3, op_if_icmpeq},
        {"if_icmpne",       0xA0, 3, op_if_icmpne},
        {"if_icmplt",       0xA1, 3, op_if_icmplt},
        {"if_icmpge",       0xA2, 3, op_if_icmpge},
        {"if_icmpgt",       0xA3, 3, op_if_icmpgt},
        {"if_icmple",       0xA4, 3, op_if_icmple},
        {"if_acmpeq",       0xA5, 3, op_if_acmpeq},
        {"if_acmpne",       0xA6, 3, op_if_acmpne},
        {"goto",            0xA7, 3, op_goto},
        {"jsr",             0xA8, 3, op_jsr},
        {"ret",             0xA9, 3, op_ret},
        {"tableswitch",     0xAA, 0, op_tableswitch},
        {"lookupswitch",    0xAB, 0, op_lookupswitch},
        {"ireturn",         0xAC, 1, op_ireturn},
        {"lreturn",         0xAD, 1, op_ireturn},
        {"dreturn",         0xAE, 1, op_ireturn},
        {"dreturn",         0xAF, 1, op_ireturn},
        {"areturn",         0xB0, 1, op_ireturn},
        {"return",          0xB1, 1, op_return},
        {"getstatic",       0xB2, 3, op_getstatic},
        {"putstatic",       0xB3, 3, op_putstatic},
        {"getfield",        0xB4, 3, op_getfield},
        {"putfield",        0xB5, 3, op_putfield},
        {"invokevirtual",   0xB6, 3, op_invokevirtual},
        {"invokespecial",   0xB7, 3, op_invokespecial},
        {"invokestatic",    0xB8, 3, op_invokestatic},
        {"invokeinterface", 0xB9, 3, op_invokeinterface},
        {"invokedynamic",   0xBA, 3, op_invokedynamic},
        {"new",             0xBB, 3, op_new},
        {"newarray",        0xBC, 2, op_newarray},
        {"anewarray",       0xBD, 3, op_anewarray},
        {"arraylength",     0xBE, 1, op_arraylength},
        {"athrow",          0xBF, 1, op_athrow},
        {"checkcast",       0xC0, 3, op_checkcast},
        {"instanceof",      0xC1, 3, op_instanceof},
        {"monitorenter",    0xC2, 1, op_monitorenter},
        {"monitorexit",     0xC3, 1, op_monitorexit},
        {"wide",            0xC4, 1, op_wide},
        {"multianewarray",  0xC5, 1, op_multianewarray},
        {"ifnull",          0xC6, 3, op_ifnull},
        {"ifnonnull",       0xC7, 3, op_ifnonnull},
        {"breakpoint",      0xCA, 1, op_breakpoint},
};

Instruction **instruct_indexies_create() {
    size_t byteCode_size = sizeof(instructionSet) / sizeof(Instruction);
    Instruction **indexies = jvm_alloc(0xff * sizeof(Instruction *));
    s32 i;
    for (i = 0; i < byteCode_size; i++) {
        s32 opCode = instructionSet[i].opCode;
        if (indexies[opCode]) {
            jvm_printf("ERRROR : instruction dup : %x\n", opCode);
        }
        indexies[opCode] = &(instructionSet[i]);
    }
#if _JVM_DEBUG > 5
    for (i = 0; i < 256; i++) {
        if (!indexies[i]) {
            jvm_printf("[%x] ", i);
        }
    }
#endif
    return indexies;
}

void instruct_indexies_destory(Instruction **instcts) {
    jvm_free(instcts);
}


static c8 *find_instruct_name(u8 op) {
    return instructionsIndexies[op]->name;
}

static InstructFunc find_instruct_func(u8 op) {
    Instruction *i = instructionsIndexies[op];
    if (!i) {
        jvm_printf("instruct not found :[%x]\n", op);
        exit(1);
        return 0;
    }
    return i->func;
}

s32 find_instruct_offset(u8 op) {
    return instructionsIndexies[op]->offset;
}


static ExceptionTable *
find_exception_handler(Runtime *runtime, Instance *exception, CodeAttribute *ca, s32 offset, __refer exception_ref) {
    Instance *ins = (Instance *) exception_ref;

    s32 i;
    ExceptionTable *e = ca->exception_table;
    for (i = 0; i < ca->exception_table_length; i++) {

        if (offset >= (e + i)->start_pc
            && offset < (e + i)->end_pc) {
            if (!(e + i)->catch_type) {
                return e + i;
            }
            ConstantClassRef *ccr = find_constant_classref(runtime->clazz, (e + i)->catch_type);
            Class *catchClass = classes_load_get(ccr->name, runtime);
            if (instance_of(catchClass, exception))
                return e + i;
        }
    }
    return NULL;
}

static s32 find_line_num(CodeAttribute *ca, s32 offset) {
    s32 i, j;

    for (j = 0; j < ca->line_number_table_length; j++) {
        LineNumberTable *node = &(ca->line_number_table[j]);
        if (offset >= node->start_pc) {
            if (j + 1 < ca->line_number_table_length) {
                LineNumberTable *next_node = &(ca->line_number_table[j + 1]);

                if (offset < next_node->start_pc) {
                    return node->line_number;
                }
            } else {
                return node->line_number;
            }
        }
    }
    return -1;
}

static void printCodeAttribute(CodeAttribute *ca, Class *p) {
    s32 i = 0;
    s32 tmp = 0;
    u8 opCode = 0;
    jvm_printf("attribute name : %s\n", utf8_cstr(get_utf8_string(p, ca->attribute_name_index)));
    jvm_printf("attribute arr_length: %d\n", ca->attribute_length);

    jvm_printf("max_stack: %d\n", ca->max_stack);
    jvm_printf("max_locals: %d\n", ca->max_locals);
    jvm_printf("code_length: %d\n", ca->code_length);
    u8 *pc = ca->code;
    i = 0;
    do {
        c8 *opName = find_instruct_name(pc[0]);
        if (opName == 0) {
            jvm_printf("Unknow OpCode %02X\n", pc[0]);
            exit(1);
        }
        jvm_printf("%s \n", opName);
        tmp = find_instruct_offset(pc[0]);
        pc += tmp;
        i += tmp;
    } while (i < ca->code_length);
}

/**
 * 把堆栈中的方法调用参数存入方法本地变量
 * 调用方法前，父程序把函数参数推入堆栈，方法调用时，需要把堆栈中的参数存到本地变量
 * @param method
 * @param father
 * @param son
 */
void stack2localvar(MethodInfo *method, Runtime *father, Runtime *son) {

    Utf8String *paraType = method->paraType;
    s32 i;
    StackEntry entry;
    s32 i_local = 0;
    s32 paraLen = paraType->length;
    s32 ins_this = 0;
    s32 stack_size = father->stack->size;
    s32 stack_pointer = stack_size - paraLen;
    if (!(method->access_flags & ACC_STATIC)) {//非静态方法需要把局部变量位置0设为this
        peek_entry(father->stack, &entry, stack_pointer - 1);
        (son->localVariables + i_local++)->refer = entry_2_refer(&entry);
        ins_this = 1;
    }
    for (i = 0; i < paraType->length; i++) {
        char type = utf8_char_at(paraType, i);
        peek_entry(father->stack, &entry, stack_pointer++);
        switch (type) {
            case 'R': {
                (son->localVariables + i_local++)->refer = entry_2_refer(&entry);
                break;
            }
            case '8': {
                //把双字类型拆成两个单元放入本地变量
                Long2Double l2d;
                l2d.l = entry_2_long(&entry);
                (son->localVariables + i_local++)->integer = l2d.i2l.i1;
                (son->localVariables + i_local++)->integer = l2d.i2l.i0;
                break;
            }
            case '4': {
                (son->localVariables + i_local++)->integer = entry_2_int(&entry);
                break;
            }
        }
    }
    father->stack->size -= paraLen + ins_this;
}

s32 synchronized_lock_method(MethodInfo *method, Runtime *runtime) {
    //synchronized process
    if (method->access_flags & ACC_SYNCHRONIZED) {
        if (method->access_flags & ACC_STATIC) {
            jthread_lock((MemoryBlock *) runtime->clazz, runtime);
        } else {
            jthread_lock(&((Instance *) runtime->localVariables[0].refer)->mb, runtime);
        }
    }
}

s32 synchronized_unlock_method(MethodInfo *method, Runtime *runtime) {
    //synchronized process
    if (method->access_flags & ACC_SYNCHRONIZED) {
        if (method->access_flags & ACC_STATIC) {
            jthread_unlock(&runtime->clazz->mb, runtime);
        } else {
            jthread_unlock(&((Instance *) runtime->localVariables[0].refer)->mb, runtime);
        }
    }
}

s32 execute_method(MethodInfo *method, Runtime *pruntime, Class *clazz) {
    s32 j = 0, ret = 0;

    Runtime runtime;
    memset(&(runtime), 0, sizeof(runtime));
    runtime.stack = pruntime->stack;
    runtime.threadInfo = pruntime->threadInfo;

    pruntime->son = &runtime;
    runtime.parent = pruntime;

    runtime.methodInfo = method;
    runtime.clazz = clazz;
    s32 stackSize;
//    if (utf8_equals_c(method->name, "createPacketAnalyzer")) {
//        int debug = 1;
//    }
    if (method->access_flags & ACC_NATIVE) {//本地方法
        localvar_init(&runtime, method->para_count + 1);//可能有非静态本地方法调用，因此+1
        stack2localvar(method, pruntime, &runtime);
        stackSize = pruntime->stack->size;
        //缓存调用本地方法
        if (!method->native_func) { //把本地方法找出来缓存
            java_native_method *native = find_native_method(utf8_cstr(clazz->name), utf8_cstr(method->name),
                                                            utf8_cstr(method->descriptor));
            method->native_func = native->func_pointer;
        }
#if _JVM_DEBUG > 3
        invoke_deepth(&runtime);
        java_native_method *native = find_native_method(utf8_cstr(clazz->name), utf8_cstr(method->name),
                                                        utf8_cstr(method->descriptor));
        jvm_printf("%s.%s()  {\n", native->clzname, native->methodname);
#endif
        synchronized_lock_method(method, &runtime);
        ret = method->native_func(&runtime, clazz);
        synchronized_unlock_method(method, &runtime);
        localvar_dispose(&runtime);
//        if (java_debug) {
//            //process jdwp suspend
//            while (runtime.threadInfo->suspend_count) {
//                threadSleep(20);
//            }
//        }
#if _JVM_DEBUG > 3
        invoke_deepth(&runtime);
        jvm_printf("}\n");
#endif

    } else {

        for (j = 0; j < method->attributes_count; j++) {
            if (!method->attributes[j].converted_code)continue;
            CodeAttribute *ca = method->attributes[j].converted_code;
            localvar_init(&runtime, ca->max_locals + 1);
            stack2localvar(method, pruntime, &runtime);
            stackSize = pruntime->stack->size;
            synchronized_lock_method(method, &runtime);

#if _JVM_DEBUG_BYTECODE_DUMP
            jvm_printf("---------------------------------\n");
            jvm_printf("code dump\n");
            printCodeAttribute(&ca, obj_of_clazz);
            jvm_printf("---------------------------------\n");
#endif
#if _JVM_DEBUG > 3
            if (utf8_char_at(clazz->name, 3) != '/' && utf8_char_at(clazz->name, 4) != 's') {
                invoke_deepth(&runtime);
                jvm_printf("%s.%s()  {\n", utf8_cstr(clazz->name), utf8_cstr(method->name));
            }
#endif
            runtime.pc = ca->code;
            runtime.ca = ca;
            s32 i = 0;
            do {
                if (java_debug) {
                    //breakpoint
                    if (method->breakpoint) {
                        jdwp_check_breakpoint(&runtime);
                    }
                    //debug step
                    if (runtime.threadInfo->jdwp_step.active) {//单步状态
                        runtime.threadInfo->jdwp_step.bytecode_count++;
                        jdwp_check_debug_step(&runtime);

                    }
                }
                //process thread suspend
                check_suspend_and_pause(&runtime);

                InstructFunc func = find_instruct_func(runtime.pc[0]);
                if (func != 0) {
#if _JVM_DEBUG_PROFILE
                    u8 instruct_code = pc[0];
                    s64 start_at = nanoTime();
#endif
                    i = func(&runtime.pc, &runtime);

#if _JVM_DEBUG_PROFILE
                    s64 spent = nanoTime() - start_at;
                    spent = 1;
                    HashtableValue v = hashtable_get(instruct_profile, (HashtableKey) (long) instruct_code);
                    if (v == NULL) {
                        hashtable_put(instruct_profile, (HashtableKey) (long) instruct_code, (HashtableKey)(long) (spent));
                    } else {
                        hashtable_put(instruct_profile, (HashtableKey) (long) instruct_code,
                                      (HashtableKey) (v + spent));
                    }
#endif
                }
                if (i == RUNTIME_STATUS_RETURN) break;
                else if (i == RUNTIME_STATUS_EXCEPTION) {
                    __refer ref = pop_ref(runtime.stack);
                    //jvm_printf("stack size:%d , enter size:%d\n", runtime.stack->size, stackSize);
                    runtime.stack->size = stackSize;//恢复堆栈大小
                    push_ref(runtime.stack, ref);

                    Instance *ins = (Instance *) ref;
                    s32 lineNum = find_line_num(ca, runtime.pc - ca->code);
                    printf("   at %s.%s(%s.java:%d)\n",
                           utf8_cstr(clazz->name), utf8_cstr(method->name),
                           utf8_cstr(clazz->name),
                           lineNum
                    );
                    ExceptionTable *et = find_exception_handler(&runtime, ins, ca, runtime.pc - ca->code, ref);
                    if (et == NULL) {
                        ret = RUNTIME_STATUS_EXCEPTION;
                        break;
                    } else {
                        //jvm_printf("Exception : %s\n", utf8_cstr(ins->mb.obj_of_clazz->name));
                        runtime.pc = (ca->code + et->handler_pc);
                        ret = RUNTIME_STATUS_NORMAL;
                    }
                }
            } while (1);
            synchronized_unlock_method(method, &runtime);
            localvar_dispose(&runtime);
#if _JVM_DEBUG > 3
            if (utf8_char_at(clazz->name, 3) != '/' && utf8_char_at(clazz->name, 4) != 's') {//   that com/sun
                invoke_deepth(&runtime);
                jvm_printf("}\n");
            } else {
                int debug = 1;
            }
            //jvm_printf("}  %s.%s  \n", utf8_cstr(clazz->name), utf8_cstr(method->name));
            Utf8String *ustr = method->descriptor;
            //jvm_printf("stack size in:%d out:%d  topof stack:\n", stackSize, pruntime->stack->size);
            if (ret != RUNTIME_STATUS_EXCEPTION) {
                if (utf8_indexof_c(ustr, ")V") >= 0) {//无反回值
                    if (pruntime->stack->size < stackSize) {
                        exit(1);
                    }
                } else {
                    if (pruntime->stack->size < stackSize + 1) {
                        exit(1);
                    }
                }
            }
#endif
        }
    }
    pruntime->son = NULL;
    return ret;
}
