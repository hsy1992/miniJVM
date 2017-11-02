//
// Created by Gust on 2017/8/20 0020.
//

#include "jvm.h"
#include "jvm_util.h"
//======================= global var =============================

ClassLoader *sys_classloader;
ClassLoader *array_classloader;

ThreadLock threadlist_lock;

ArrayList *native_libs;
ArrayList *thread_list; //all thread
Hashtable *sys_prop;

Collector *collector;

Instruction **instructionsIndexies;
Instance *main_thread;//
Runtime *main_runtime = NULL;

c8 *data_type_str = "    ZCFDBSIJL[";
s32 STACK_LENGHT = 10240;
s64 GARBAGE_PERIOD_MS = 10000;
//
//
u8 volatile java_debug = 1;
Instance *jdwp_jthread;





#if _JVM_DEBUG_PROFILE
Hashtable* instruct_profile;
#endif

