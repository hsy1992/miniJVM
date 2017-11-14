//
// Created by Gust on 2017/8/20 0020.
//

#include "jvm.h"
#include "jvm_util.h"
//======================= global var =============================

ClassLoader *sys_classloader;
ClassLoader *array_classloader;


ArrayList *native_libs;
ArrayList *thread_list; //all thread
Hashtable *sys_prop;
ArrayList *runtime_cache;

GcCollector *collector;

Instruction **instructionsIndexies;
Instance *main_thread;//
Runtime *main_runtime = NULL;

c8 *data_type_str = "    ZCFDBSIJL[";
s32 STACK_LENGHT = 10240;
s64 GARBAGE_PERIOD_MS = 10000;

s64 MAX_HEAP_SIZE = 20 * 1024 * 1024;



//
InstanceFieldInfo ins_field_offset;
//
u8 volatile java_debug = 1;


#if _JVM_DEBUG_PROFILE
Hashtable* instruct_profile;
#endif

