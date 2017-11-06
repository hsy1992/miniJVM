

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/d_type.h"
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


/* push Integer */
void push_int(RuntimeStack *stack, s32 value) {
    StackEntry *entry = &stack->store[stack->size];
    memcpy(&entry->value, &value, sizeof(s32));
    entry->type = STACK_ENTRY_INT;
    stack->size++;
}


/* pop Integer */
s32 pop_int(RuntimeStack *stack) {
    stack->size--;
    StackEntry *entry = &stack->store[stack->size];

    s32 value = 0;
    memcpy(&value, &entry->value, sizeof(s32));
    memset(&stack->store[stack->size], 0, sizeof(StackEntry));
    return value;
}

/* push Double */
void push_double(RuntimeStack *stack, f64 value) {
    //memset(&stack->store[stack->size], 0, sizeof(StackEntry));
    void *tmp = &stack->store[stack->size].value;
    memcpy(tmp, &value, sizeof(f64));
    stack->store[stack->size].type = STACK_ENTRY_DOUBLE;
    stack->size++;
}

/* pop Double */
f64 pop_double(RuntimeStack *stack) {
    stack->size--;
    StackEntry *entry = &stack->store[stack->size];
    f64 value = 0;
    memcpy(&value, &entry->value, sizeof(f64));
    memset(&stack->store[stack->size], 0, sizeof(StackEntry));
    return value;
}

/* push Float */
void push_float(RuntimeStack *stack, f32 value) {
    //memset(&stack->store[stack->size], 0, sizeof(StackEntry));
    void *tmp = &stack->store[stack->size].value;
    memcpy(tmp, &value, sizeof(f32));
    stack->store[stack->size].type = STACK_ENTRY_FLOAT;
    stack->size++;
}

/* pop Float */
f32 pop_float(RuntimeStack *stack) {
    stack->size--;
    StackEntry *entry = &stack->store[stack->size];

    f32 value = 0;

    memcpy(&value, &entry->value, sizeof(f32));
    memset(&stack->store[stack->size], 0, sizeof(StackEntry));
    return value;
}


/* push Long */
void push_long(RuntimeStack *stack, s64 value) {
    //memset(&stack->store[stack->size], 0, sizeof(StackEntry));
    void *tmp = &stack->store[stack->size].value;
    memcpy(tmp, &value, sizeof(s64));
    stack->store[stack->size].type = STACK_ENTRY_LONG;
    stack->size++;
}

/* pop Long */
s64 pop_long(RuntimeStack *stack) {
    stack->size--;
    StackEntry *entry = &stack->store[stack->size];

    s64 value = 0;
    memcpy(&value, &entry->value, sizeof(s64));
    memset(&stack->store[stack->size], 0, sizeof(StackEntry));
    return value;
}

/* push Ref */
void push_ref(RuntimeStack *stack, __refer value) {
    //memset(&stack->store[stack->size], 0, sizeof(StackEntry));
    void *tmp = &stack->store[stack->size].value;
    memcpy(tmp, &value, sizeof(__refer));
    stack->store[stack->size].type = STACK_ENTRY_REF;
    stack->size++;
}

__refer pop_ref(RuntimeStack *stack) {
    stack->size--;
    StackEntry *entry = &stack->store[stack->size];
    __refer value = 0;
    memcpy(&value, &entry->value, sizeof(__refer));
    memset(&stack->store[stack->size], 0, sizeof(StackEntry));

    return value;
}


void push_entry(RuntimeStack *stack, StackEntry *entry) {
    void *tmp = &stack->store[stack->size];
    memcpy(tmp, entry, sizeof(StackEntry));
    stack->size++;
}

/* Pop Stack Entry */
void pop_entry(RuntimeStack *stack, StackEntry *entry) {
    stack->size--;
    void *ptr = &stack->store[stack->size];
    memcpy(entry, ptr, sizeof(StackEntry));
    memset(ptr, 0, sizeof(StackEntry));

}

void pop_empty(RuntimeStack *stack) {
    stack->size--;
    memset(&stack->store[stack->size], 0, sizeof(StackEntry));
}

/* Entry to Int */
s32 entry_2_int(StackEntry *entry) {
    s32 value = 0;
    memcpy(&value, &entry->value, sizeof(s32));
    return value;
}

s64 entry_2_long(StackEntry *entry) {
    s64 value = 0;
    memcpy(&value, &entry->value, sizeof(s64));
    return value;
}

__refer entry_2_refer(StackEntry *entry) {
    __refer value = 0;
    memcpy(&value, &entry->value, sizeof(__refer));
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
    memcpy(entry, &stack->store[index].value, sizeof(StackEntry));
}

//======================= runtime =============================



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
//======================= localvar =============================


s32 localvar_init(Runtime *runtime, s32 count) {
    runtime->localVariables = jvm_alloc(sizeof(LocalVarItem) * count);
    runtime->localvar_count = count;
    return 0;
}

s32 localvar_dispose(Runtime *runtime) {
    s32 i;

//    if (runtime->localVariables == NULL) {
//        int debug = 1;
//    }
    jvm_free(runtime->localVariables);
    runtime->localVariables = NULL;
    runtime->localvar_count = 0;
    return 0;
}

void localvar_clear_refer(s32 index, Runtime *runtime) {
    __refer old = runtime->localVariables[index].refer;
    if (old) {
        runtime->localVariables[index].refer = NULL;
    }
}

void localvar_setInt(Runtime *runtime, s32 index, s32 val) {
    runtime->localVariables[index].integer = val;
}

void localvar_setRefer(Runtime *runtime, s32 index, __refer val) {
    runtime->localVariables[index].refer = val;
}

s32 localvar_getInt(Runtime *runtime, s32 index) {
    return runtime->localVariables[index].integer;
}

__refer localvar_getRefer(Runtime *runtime, s32 index) {
    return runtime->localVariables[index].refer;
}