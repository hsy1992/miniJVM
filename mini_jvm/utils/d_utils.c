//
// Created by Gust on 2017/10/5.
//

#include <memory.h>
#include "d_type.h"
#include "ltalloc.cc"



//========================     var     =========================


s64 heap_size = 0;


//========================     mem alloc     =========================

#ifdef MEM_ALLOC_LTALLOC

void *jvm_calloc(u32 size) {
    return ltcalloc(1, size);
}

void *jvm_malloc(u32 size) {
    return ltmalloc(size);
}

void jvm_free(void *ptr) {
    ltfree(ptr);
}

void *jvm_realloc(void *pPtr, u32 size) {
    return ltrealloc(pPtr, size);
}

void jvm_squeeze(u32 padsz) {
    ltsqueeze(padsz);
}

#endif