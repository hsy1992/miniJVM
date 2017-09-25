//
// Created by gust on 2017/8/25.
//

#ifndef MINI_JVM_GLOBAL_H
#define MINI_JVM_GLOBAL_H

#include <sys/types.h>

typedef unsigned char u8;
typedef char c8;
typedef unsigned short int u16;
typedef signed short int s16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef double f64;
typedef unsigned long long u64;
typedef signed long long s64;
typedef void *__refer;



void *jvm_alloc(u32 size);

s32 jvm_free(void *ptr);

void *jvm_realloc(void *pPtr, u32 size);

#endif //MINI_JVM_GLOBAL_H
