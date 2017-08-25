//
// Created by Gust on 2017/8/20 0020.
//
#include "jvm.h"
//======================= global var =============================


Utf8String *classpath;
Hashtable *classes;  //key =  package+classname value =  class_ptr
ArrayList *thread_list; //all thread
s32 STACK_LENGHT = 10240;


//extern const s32 METHOD_MAX_PARA_LENGHT = 32;


Hashtable *son_2_father; //key=mem_ptr, value=我被别人引用的列表
Hashtable *father_2_son; //key=mem_ptr, value=别人被我引用的列表
s64 MAX_HEAP_SIZE = 20 * 1024 * 1024;
s64 heap_size = 0; //当前已经分配的内存总数

Instruction **instructionsIndexies;


#if _JVM_DEBUG_PROFILE
Hashtable* instruct_profile;
#endif