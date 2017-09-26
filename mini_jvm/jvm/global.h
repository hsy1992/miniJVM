//
// Created by gust on 2017/8/25.
//

#ifndef MINI_JVM_GLOBAL_H
#define MINI_JVM_GLOBAL_H

#include <sys/types.h>
#include "../utils/d_type.h"

#ifndef __MEM_LEAK_DETECT
void *jvm_alloc(u32 size);

s32 jvm_free(void *ptr);

void *jvm_realloc(void *pPtr, u32 size);
#endif //_DEBUG

void mem_mgr_print();

void mem_mgr_distory();

void mem_mgr_create();

#endif //MINI_JVM_GLOBAL_H
