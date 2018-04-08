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

GcCollector *collector;

JniEnv jnienv;

Instance *main_thread;//
Runtime *main_runtime = NULL;

c8 *data_type_str = "    ZCFDBSIJL[";
__refer data_type_classes[DATATYPE_COUNT];
s32 data_type_bytes[DATATYPE_COUNT] = {0, 0, 0, 0,
                                       sizeof(c8),
                                       sizeof(u16),
                                       sizeof(f32),
                                       sizeof(f64),
                                       sizeof(c8),
                                       sizeof(s16),
                                       sizeof(s32),
                                       sizeof(s64),
                                       sizeof(__refer),
                                       sizeof(__refer),
};
s32 STACK_LENGHT = 10240;
s64 GARBAGE_PERIOD_MS = 30 * 1000;

s64 MAX_HEAP_SIZE = 50 * 1024 * 1024;


//
InstanceFieldInfo ins_field_offset;
//
u8 volatile java_debug = 0;


#if _JVM_DEBUG_PROFILE
Hashtable* instruct_profile_sum;
Hashtable* instruct_profile_count;
#endif

