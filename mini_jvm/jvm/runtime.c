

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jvm.h"
#include "garbage.h"
#include "jvm_util.h"


/* Stack Initialization */
RuntimeStack *stack_create(s32 entry_size) {
    RuntimeStack *stack = jvm_alloc(sizeof(RuntimeStack));
    stack->store = (StackEntry *) jvm_alloc(sizeof(StackEntry) * entry_size);
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

void push_entry(RuntimeStack *stack, StackEntry *entry) {
    u8 *tmp = stack->store[stack->size].entry;
    memcpy(tmp, entry, sizeof(StackEntry));
    stack->size++;
}

/* push Integer */
void push_int(RuntimeStack *stack, s32 value) {
    //memset(&stack->store[stack->size], 0, sizeof(StackEntry));
    u8 *tmp = stack->store[stack->size].entry;
    memcpy(tmp, &value, sizeof(s32));
    stack->store[stack->size].type = STACK_ENTRY_INT;
    stack->size++;
}


/* push Double */
void push_double(RuntimeStack *stack, f64 value) {
    //memset(&stack->store[stack->size], 0, sizeof(StackEntry));
    u8 *tmp = stack->store[stack->size].entry;
    memcpy(tmp, &value, sizeof(f64));
    stack->store[stack->size].type = STACK_ENTRY_DOUBLE;
    stack->size++;
}

/* push Float */
void push_float(RuntimeStack *stack, f32 value) {
    //memset(&stack->store[stack->size], 0, sizeof(StackEntry));
    u8 *tmp = stack->store[stack->size].entry;
    memcpy(tmp, &value, sizeof(f32));
    stack->store[stack->size].type = STACK_ENTRY_FLOAT;
    stack->size++;
}


/* pop Integer */
s32 pop_int(RuntimeStack *stack) {
    stack->size--;
    u8 *tmp = stack->store[stack->size].entry;
    s32 value = 0;
    memcpy(&value, tmp, sizeof(s32));
    memset(&stack->store[stack->size], 0, sizeof(StackEntry));
    return value;
}

/* push Long */
void push_long(RuntimeStack *stack, s64 value) {
    //memset(&stack->store[stack->size], 0, sizeof(StackEntry));
    u8 *tmp = stack->store[stack->size].entry;
    memcpy(tmp, &value, sizeof(s64));
    stack->store[stack->size].type = STACK_ENTRY_LONG;
    stack->size++;
}

/* pop Long */
s64 pop_long(RuntimeStack *stack) {
    stack->size--;
    s64 value = 0;
    u8 *tmp = stack->store[stack->size].entry;
    memcpy(&value, tmp, sizeof(s64));
    memset(&stack->store[stack->size], 0, sizeof(StackEntry));
    return value;
}

/* push Ref */
void push_ref(RuntimeStack *stack, __refer value) {
    //memset(&stack->store[stack->size], 0, sizeof(StackEntry));
    u8 *tmp = stack->store[stack->size].entry;
    memcpy(tmp, &value, sizeof(__refer));
    garbage_refer_count_inc(value);
    stack->store[stack->size].type = STACK_ENTRY_REF;
    stack->size++;
}

__refer pop_ref(RuntimeStack *stack) {
    stack->size--;
    __refer value = 0;
    u8 *tmp = stack->store[stack->size].entry;
    memcpy(&value, tmp, sizeof(__refer));
    memset(&stack->store[stack->size], 0, sizeof(StackEntry));
    garbage_refer_count_dec(value);
    return value;
}

/* pop Double */
f64 pop_double(RuntimeStack *stack) {
    stack->size--;
    f64 value = 0;
    u8 *tmp = stack->store[stack->size].entry;
    memcpy(&value, tmp, sizeof(f64));
    memset(&stack->store[stack->size], 0, sizeof(StackEntry));
    return value;
}

/* pop Float */
f32 pop_float(RuntimeStack *stack) {
    stack->size--;
    f32 value = 0;
    u8 *tmp = stack->store[stack->size].entry;
    memcpy(&value, tmp, sizeof(f32));
    memset(&stack->store[stack->size], 0, sizeof(StackEntry));
    return value;
}

/* Pop Stack Entry */
void pop_entry(RuntimeStack *stack, StackEntry *entry) {
    stack->size--;
    memcpy(entry, stack->store[stack->size].entry, sizeof(StackEntry));
    memset(&stack->store[stack->size], 0, sizeof(StackEntry));
    if (entry->type == STACK_ENTRY_REF) {
        garbage_refer_count_dec(entry_2_refer(entry));
    }
}

/* Entry to Int */
s32 entry_2_int(StackEntry *entry) {
    s32 value = 0;
    memcpy(&value, entry->entry, sizeof(s32));
    return value;
}

s64 entry_2_long(StackEntry *entry) {
    s64 value = 0;
    memcpy(&value, entry->entry, sizeof(s64));
    return value;
}

__refer entry_2_refer(StackEntry *entry) {
    __refer value = 0;
    memcpy(&value, entry->entry, sizeof(__refer));
    return value;
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

void peek_entry(RuntimeStack *stack, StackEntry *entry, int index) {
    memcpy(entry, stack->store[index].entry, sizeof(StackEntry));
}

//======================= localvar =============================



Runtime *runtime_create(Runtime *parent) {
    Runtime *runtime = jvm_alloc(sizeof(Runtime));
    if (parent) {
        runtime->stack = parent->stack;
        runtime->threadInfo = parent->threadInfo;
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
    if (runtime->threadInfo->top_runtime == runtime) {
        stack_destory(runtime->stack);
        threadinfo_destory(runtime->threadInfo);
    }
    if (runtime->parent) {
        runtime->parent = NULL;
    }
    jvm_free(runtime);

}


s32 localvar_init(Runtime *runtime, s32 count) {
    runtime->localVariables = jvm_alloc(sizeof(LocalVarItem) * count);
    runtime->localvar_count = count;
    return 0;
}

s32 localvar_dispose(Runtime *runtime) {
    s32 i;
    for (i = 0; i < runtime->localvar_count; i++) {
        __refer ref = localvar_getRefer(runtime, i);
        if (ref) {
            garbage_refer_count_dec(ref);
        }
    }
    jvm_free(runtime->localVariables);
    runtime->localVariables = NULL;
    runtime->localvar_count = 0;
    return 0;
}

void localvar_clear_refer(s32 index, Runtime *runtime) {
    __refer old = runtime->localVariables[index].refer;
    if (old) {
        garbage_refer_count_dec(old);
        runtime->localVariables[index].refer = NULL;
    }
}

void localvar_setInt(Runtime *runtime, s32 index, s32 val) {
    localvar_clear_refer(index, runtime);
    runtime->localVariables[index].integer = val;
}

void localvar_setRefer(Runtime *runtime, s32 index, __refer val) {
    localvar_clear_refer(index, runtime);
    runtime->localVariables[index].refer = val;
}

s32 localvar_getInt(Runtime *runtime, s32 index) {
    return runtime->localVariables[index].integer;
}

__refer localvar_getRefer(Runtime *runtime, s32 index) {
    return runtime->localVariables[index].refer;
}