

#ifndef _GARBAGE_H
#define _GARBAGE_H

#include "../utils/hashtable.h"
#include "jvm.h"




void *jvm_alloc(u32 size);

void jvm_free(void *ptr);

void *jvm_realloc(void *pPtr, u32 size);

s32 garbage_collector_create();

s32 garbage_collect();

void dump_refer();

void *garbage_refer(void *sonPtr, void *parentPtr);

void garbage_derefer(void *sonPtr, void *parentPtr);

void garbage_derefer_all(void *parentPtr);

#endif //_GARBAGE_H
