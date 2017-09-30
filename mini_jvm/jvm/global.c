//
// Created by Gust on 2017/8/20 0020.
//

#include "jvm.h"
#include "jvm_util.h"
//======================= global var =============================

ClassLoader *sys_classloader;
ClassLoader *array_classloader;

ThreadLock sys_lock;

ArrayList *native_libs;
ArrayList *thread_list; //all thread
Hashtable *sys_prop;

Collector *collector;

Instruction **instructionsIndexies;
Instance *main_thread;//
Runtime *main_runtime = NULL;

c8 *data_type_str = "    ZCFDBSIJL[";
s32 STACK_LENGHT = 10240;
s64 GARBAGE_PERIOD_MS = 5000;
//
//
u8 volatile java_debug = 0;
Instance *jdwp_jthread;

s64 MAX_HEAP_SIZE = 20 * 1024 * 1024;
s64 heap_size = 0; //当前已经分配的内存总数

//======================= memory manage =============================


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
        setFieldInt(ptr, size);
        return ptr + 4;
    }
    return NULL;
}

s32 jvm_free(void *ptr) {
    if (ptr) {
        s32 size = getFieldInt(ptr - 4);
        heap_size -= size;
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



#if _JVM_DEBUG_PROFILE
Hashtable* instruct_profile;
#endif

