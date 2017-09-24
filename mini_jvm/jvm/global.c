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
s64 GARBAGE_PERIOD_MS = 5000;
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


#if _JVM_DEBUG_PROFILE
Hashtable* instruct_profile;
#endif