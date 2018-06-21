

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


s32 is_ref(StackEntry *entry) {
    if (entry->type & STACK_ENTRY_REF)
        return 1;
    return 0;
}

//======================= runtime =============================
enum {
    RUNTIME_LOCALVAR_SIZE = 10,
};

/**
 * runtime 的创建和销毁会极大影响性能，因此对其进行缓存
 * @param parent runtime of parent
 * @return runtime
 */
Runtime *runtime_create(Runtime *parent) {
    Runtime *top_runtime = NULL;

    Runtime *runtime = NULL;
    if (parent) {
        top_runtime = parent->threadInfo->top_runtime;
    }

    if (top_runtime) {
        runtime = top_runtime->runtime_pool_header;
        if (runtime) {
            top_runtime->runtime_pool_header = runtime->next;
            runtime->next = NULL;
        }
    }
    if (runtime == NULL) {
        runtime = jvm_calloc(sizeof(Runtime));
        runtime->localvar = jvm_calloc(RUNTIME_LOCALVAR_SIZE * sizeof(LocalVarItem));
        runtime->localvar_max = RUNTIME_LOCALVAR_SIZE;
        runtime->jnienv = &jnienv;
        if (parent) {
            runtime->stack = parent->stack;
            runtime->threadInfo = parent->threadInfo;
        }
    }
    //
    if (parent != NULL) {
        runtime->parent = parent;
        parent->son = runtime;
    } else {
        runtime->stack = stack_create(STACK_LENGHT);
        runtime->threadInfo = threadinfo_create();
        runtime->threadInfo->top_runtime = runtime;
    }
    return runtime;
}


void runtime_destory(Runtime *runtime) {
    Runtime *top_runtime = runtime->threadInfo->top_runtime;
    if (top_runtime != runtime) {
        runtime->next = top_runtime->runtime_pool_header;
        top_runtime->runtime_pool_header = runtime;
    } else {
        stack_destory(runtime->stack);
        threadinfo_destory(runtime->threadInfo);

        Runtime *next = top_runtime->runtime_pool_header;
        while (next) {
            Runtime *r = next;
            next = r->next;
            jvm_free(r->localvar);
            jvm_free(r);
        }
        runtime->runtime_pool_header = NULL;
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

Runtime *getTopRuntime(Runtime *runtime) {
    if (runtime) {
        return runtime->threadInfo->top_runtime;
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




void localvar_setInt_jni(LocalVarItem *localvar, s32 index, s32 val) {
    localvar_setInt(localvar, index, val);
}

void localvar_setRefer_jni(LocalVarItem *localvar, s32 index, __refer val) {
    localvar_setRefer(localvar, index, val);
}

s32 localvar_getInt_jni(LocalVarItem *localvar, s32 index) {
    return localvar_getInt(localvar, index);
}

__refer localvar_getRefer_jni(LocalVarItem *localvar, s32 index) {
    return localvar_getRefer(localvar, index);
}

void localvar_setLong_2slot_jni(LocalVarItem *localvar, s32 index, s64 val) {
    localvar_setLong(localvar, index, val);
}

s64 localvar_getLong_2slot_jni(LocalVarItem *localvar, s32 index) {
    return localvar_getLong(localvar, index);
}