//
// Created by Gust on 2017/8/20 0020.
//
#include "jvm.h"
//======================= global var =============================

Class *JVM_CLASS;
Utf8String *classpath;
Hashtable *classes;  //key =  package+classname value =  class_ptr
Hashtable *array_classes;

ArrayList *native_libs;
ArrayList *thread_list; //all thread
s32 STACK_LENGHT = 10240;
c8 *data_type_str = "    ZCFDBSIJL[";

//extern const s32 METHOD_MAX_PARA_LENGHT = 32;
s32 _garbage_thread_stop = 0;
s32 _garbage_thread_pause = 1;
s64 GARBAGE_PERIOD_MS = 1000;
ArrayList *_garbage_refer_set_pool;
s64 _garbage_count = 0;
Hashtable *sys_prop;
//
Instance *jdwp_jthread;
u8 volatile java_debug = 1;
u8 JDWP_BREAK_POINT = 0xca;

Hashtable *son_2_father; //key=mem_ptr, value=我被别人引用的列表
Hashtable *father_2_son; //key=mem_ptr, value=别人被我引用的列表
s64 MAX_HEAP_SIZE = 20 * 1024 * 1024;
s64 heap_size = 0; //当前已经分配的内存总数

Instruction **instructionsIndexies;
Instance *main_thread;//



/**
 * 在分配的内存块前面加4个字节用于存放此块内存的长度
 * @param size
 * @return
 */
void *jvm_alloc(u32 size) {
    if (!size)return NULL;
//    if (size > 5000) {
//        int debug = 1;
//    }
    size += 4;
    void *ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
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

#if _JVM_DEBUG_PROFILE
Hashtable* instruct_profile;
#endif

