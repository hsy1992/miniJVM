

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/d_type.h"
#include "jvm.h"
#include "garbage.h"
#include "jvm_util.h"


/* Stack Initialization */
RuntimeStack *stack_create(s32 entry_size) {
    RuntimeStack *stack = jvm_calloc(sizeof(RuntimeStack));
    stack->store = (StackEntry *) jvm_calloc(sizeof(StackEntry) * entry_size);
    stack->size = 0;
    stack->max_size = entry_size;
    return stack;
}

void stack_destory(RuntimeStack *stack) {
    if (stack->store) {
        jvm_free(stack->store);
        stack->store = NULL;
    }
    jvm_free(stack);
}


/* push Integer */
void push_int_jni(RuntimeStack *stack, s32 value) {
    push_int(stack, value);
}


/* pop Integer */
s32 pop_int_jni(RuntimeStack *stack) {
    return pop_int(stack);
}

/* push Double */
void push_double_jni(RuntimeStack *stack, f64 value) {
    push_double(stack, value);
}

/* pop Double */
f64 pop_double_jni(RuntimeStack *stack) {
    return pop_double(stack);
}

/* push Float */
void push_float_jni(RuntimeStack *stack, f32 value) {
    push_float(stack, value);
}

/* pop Float */
f32 pop_float_jni(RuntimeStack *stack) {
    return pop_float(stack);
}


/* push Long */
void push_long_jni(RuntimeStack *stack, s64 value) {
    push_long(stack, value);
}

/* pop Long */
s64 pop_long_jni(RuntimeStack *stack) {
    return pop_long(stack);
}

/* push Ref */
void push_ref_jni(RuntimeStack *stack, __refer value) {
    push_ref(stack, value);
}

__refer pop_ref_jni(RuntimeStack *stack) {
    return pop_ref(stack);
}


void push_entry_jni(RuntimeStack *stack, StackEntry *entry) {
    push_entry(stack, entry);
}

/* Pop Stack Entry */
void pop_entry_jni(RuntimeStack *stack, StackEntry *entry) {
    pop_entry(stack, entry);
}

void pop_empty_jni(RuntimeStack *stack) {
    pop_empty(stack);
}

/* Entry to Int */
s32 entry_2_int_jni(StackEntry *entry) {
    return entry_2_int(entry);
}

s64 entry_2_long_jni(StackEntry *entry) {
    return entry_2_long(entry);
}

__refer entry_2_refer_jni(StackEntry *entry) {
    return entry_2_refer(entry);
}

void peek_entry_jni(RuntimeStack *stack, StackEntry *entry, int index) {
    peek_entry(stack, entry, index);
}

s32 is_cat1(StackEntry *entry) {
    if (entry->type & STACK_ENTRY_INT || entry->type & STACK_ENTRY_FLOAT || entry->type & STACK_ENTRY_REF) {
        return 1;
    }
    return 0;
}

s32 is_cat2(StackEntry *entry) {
    if (entry->type & STACK_ENTRY_LONG || entry->type & STACK_ENTRY_DOUBLE) {
        return 1;
    }
    return 0;
}

s32 is_ref(StackEntry *entry) {
    if (entry->type & STACK_ENTRY_REF)
        return 1;
    return 0;
}

//======================= runtime =============================
enum {
    RUNTIME_POOL_SIZE = 20,
    RUNTIME_LOCALVAR_SIZE = 10,
};

/**
 * runtime 的创建和销毁会极大影响性能，因此对其进行缓存
 * @param parent runtime of parent
 * @return runtime
 */
Runtime *runtime_create(Runtime *parent) {
    s32 is_top = parent == NULL;
    Runtime *runtime = NULL;
    if (!is_top) {
        runtime = arraylist_pop_back_unsafe(parent->threadInfo->top_runtime->runtime_pool);
    }
    if (runtime == NULL) {
        runtime = jvm_calloc(sizeof(Runtime));
        runtime->localvar = jvm_calloc(RUNTIME_LOCALVAR_SIZE * sizeof(LocalVarItem));
        runtime->localvar_max = RUNTIME_LOCALVAR_SIZE;
    }
    runtime->jnienv = &jnienv;
    //
    if (!is_top) {
        runtime->stack = parent->stack;
        runtime->threadInfo = parent->threadInfo;
        runtime->parent = parent;
        parent->son = runtime;
    } else {
        runtime->stack = stack_create(STACK_LENGHT);
        runtime->threadInfo = threadinfo_create();
        runtime->threadInfo->top_runtime = runtime;
        runtime->runtime_pool = arraylist_create(RUNTIME_POOL_SIZE);
    }
    return runtime;
}


void runtime_destory(Runtime *runtime) {
    s32 is_top = runtime->threadInfo->top_runtime == runtime;
    if (!is_top) {
        arraylist_push_back_unsafe(runtime->threadInfo->top_runtime->runtime_pool, runtime);
//        __refer lv = runtime->localvar;
//        s32 max = runtime->localvar_max;
//        memset(runtime, 0, sizeof(Runtime));
//        runtime->localvar = lv;
//        memset(lv, 0, RUNTIME_LOCALVAR_SIZE * sizeof(LocalVarItem));
//        runtime->localvar_max = max;
    } else {
        stack_destory(runtime->stack);
        threadinfo_destory(runtime->threadInfo);
        s32 i;
        for (i = 0; i < runtime->runtime_pool->length; i++) {
            Runtime *r = arraylist_get_value(runtime->runtime_pool, i);
            jvm_free(r->localvar);
            jvm_free(r);
        }
        arraylist_destory(runtime->runtime_pool);
        runtime->runtime_pool = NULL;
        jvm_free(runtime->localvar);
        jvm_free(runtime);
    }
}

s32 getRuntimeDepth(Runtime *top) {
    s32 deep = 0;
    while (top) {
        deep++;
        top = top->son;
    }
    deep--;//top need not
    return deep;
}

Runtime *getLastSon(Runtime *top) {
    while (top) {
        if (!top->son)return top;
        top = top->son;
    }
    return NULL;
}

s64 getInstructPointer(Runtime *runtime) {
    if (runtime && runtime->method && runtime->ca) {
        return runtime->pc - runtime->ca->code;
    }
    return -1;
}

void getRuntimeStack(Runtime *runtime, Utf8String *ustr) {
    Runtime *last = getLastSon(runtime);
    while (last) {
        utf8_append_s64(ustr, (s64) (intptr_t) runtime, 16);
        utf8_append_c(ustr, " ");
        if (last->method) {
            utf8_append(ustr, last->method->_this_class->name);
            utf8_append_c(ustr, ".");
            utf8_append(ustr, last->method->name);
            utf8_append(ustr, last->method->descriptor);
            utf8_append_c(ustr, ":");
            utf8_append_s64(ustr, (s64) ((last->pc) - (last->ca ? last->ca->code : 0)), 10);
        }
        utf8_append_c(ustr, "\n");

        last = last->parent;
        if (last == NULL || last->parent == NULL || last->parent->parent == NULL)break;
    }
}

//======================= localvar =============================


s32 localvar_init(Runtime *runtime, s32 count) {
    if (count > runtime->localvar_max) {
        jvm_free(runtime->localvar);
        runtime->localvar = jvm_calloc(sizeof(LocalVarItem) * count);
        runtime->localvar_max = count;
    } else {
        memset(runtime->localvar, 0, count * sizeof(LocalVarItem));
    }
    runtime->localvar_count = count;
    return 0;
}

s32 localvar_dispose(Runtime *runtime) {
    runtime->localvar_count = 0;
    return 0;
}

void localvar_setInt_jni(Runtime *runtime, s32 index, s32 val) {
    localvar_setInt(runtime, index, val);
}

void localvar_setRefer_jni(Runtime *runtime, s32 index, __refer val) {
    localvar_setRefer(runtime, index, val);
}

s32 localvar_getInt_jni(Runtime *runtime, s32 index) {
    return localvar_getInt(runtime, index);
}

__refer localvar_getRefer_jni(Runtime *runtime, s32 index) {
    return localvar_getRefer(runtime, index);
}

void localvar_setLong_2slot_jni(Runtime *runtime, s32 index, s64 val) {
    Long2Double l2d;
    l2d.l = val;
    runtime->localvar[index].integer = l2d.i2l.i1;
    runtime->localvar[index + 1].integer = l2d.i2l.i0;
}

s64 localvar_getLong_2slot_jni(Runtime *runtime, s32 index) {
    Long2Double l2d;
    l2d.i2l.i1 = runtime->localvar[index].integer;
    l2d.i2l.i0 = runtime->localvar[index + 1].integer;
    return l2d.l;
}