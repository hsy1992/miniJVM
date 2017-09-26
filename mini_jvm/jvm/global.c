//
// Created by Gust on 2017/8/20 0020.
//

#include "jvm.h"
//======================= global var =============================

ClassLoader *sys_classloader;
ClassLoader *array_classloader;

ArrayList *native_libs;
ArrayList *thread_list; //all thread
Hashtable *sys_prop;

Collector *collector;

Instruction **instructionsIndexies;
Instance *main_thread;//
Runtime *main_runtime = NULL;

c8 *data_type_str = "    ZCFDBSIJL[";
s32 STACK_LENGHT = 10240;
s64 GARBAGE_PERIOD_MS = 1000;
//
//
u8 volatile java_debug = 0;
Instance *jdwp_jthread;

s64 MAX_HEAP_SIZE = 20 * 1024 * 1024;
s64 heap_size = 0; //当前已经分配的内存总数

//======================= memory manage =============================

static s64 alloc_count = 0;
static s64 free_count = 0;
Pairlist *mem_size_2_count;

void mem_mgr_create() {
    if (!mem_size_2_count) {
        mem_size_2_count = pairlist_create(0);
    }
}

void mem_mgr_distory() {
    if (mem_size_2_count) {
        pairlist_destory(mem_size_2_count);
        mem_size_2_count = NULL;
    }
}

#ifndef __MEM_LEAK_DETECT

/**
 * 在分配的内存块前面加4个字节用于存放此块内存的长度
 * @param size
 * @return
 */
void *jvm_alloc(u32 size) {

    if (!size)return NULL;
    size += 4;
    void *ptr = calloc(size, 1);
    if (ptr) {
        heap_size += size;
        alloc_count++;
        if (mem_size_2_count) {
            s32 count = pairlist_getl(mem_size_2_count, size);
            count++;
            pairlist_putl(mem_size_2_count, size, count);
        }
        setFieldInt(ptr, size);
        return ptr + 4;
    }
    return NULL;
}

s32 jvm_free(void *ptr) {
    if (ptr) {
        s32 size = getFieldInt(ptr - 4);
        heap_size -= size;
        free_count++;
        if (mem_size_2_count) {
            s32 count = pairlist_getl(mem_size_2_count, size);
            count--;
            pairlist_putl(mem_size_2_count, size, count);
        }
//        if (size == 176) {
//            int debug = 1;
//        }
        free(ptr - 4);
        return size;
    }
    return 0;
}

void *jvm_realloc(void *pPtr, u32 size) {
    if (!pPtr)return NULL;
    if (!size)return NULL;
    size += 4;
    heap_size -= getFieldInt(pPtr - 4);
    void *ptr = realloc(pPtr - 4, size);
    if (ptr) {
        heap_size += size;
        setFieldInt(ptr, size);
        return ptr + 4;
    }
    return NULL;
}

#endif //_DEBUG

void mem_mgr_print() {
    jvm_printf("=============\n");
    s32 i;
    for (i = 0; i < mem_size_2_count->count; i++) {
        Pair p = pairlist_get_pair(mem_size_2_count, i);
        if (p.right)
            jvm_printf("%8lld : %8lld\n", (s64) p.leftl, (s64) p.rightl);
    }
}

#if _JVM_DEBUG_PROFILE
Hashtable* instruct_profile;
#endif

