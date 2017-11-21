/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test.c
 * Author: gust
 *
 * Created on 2017年7月19日, 下午3:14
 */

#include <stdio.h>
#include <stdlib.h>
#include <miniz/miniz_wrapper.h>
#include "utils/hashtable.h"
#include "utils/utf8_string.h"
#include "jvm/jvm.h"
#include "jvm/jvm_util.h"
#include "jvm/garbage.h"


void t1() {

}

void t2() {
}

void t3() {
    LinkedList* list=linkedlist_create();
    linkedlist_push_front(list,(__refer)(long)1);
    linkedlist_push_front(list,(__refer)(long)2);
    linkedlist_push_front(list,(__refer)(long)3);
    linkedlist_push_front(list,(__refer)(long)4);
    __refer ref;
    while (NULL != (ref = linkedlist_pop_end(list))) {
        printf("%lld\n",(s64)(long)ref);
    }
    linkedlist_destory(list);
}

void t4() {
    printf("pointer size:%d\n", (s32) sizeof(void *));
    printf("jint size:%d\n", (s32) sizeof(s32));
    printf("jlong size:%d\n", (s32) sizeof(s64));
    printf("jshort size:%d\n", (s32) sizeof(s16));
    printf("jbyte size:%d\n", (s32) sizeof(c8));
    printf("jdouble size:%d\n", (s32) sizeof(f64));
    printf("jfloat size:%d\n", (s32) sizeof(f32));
    printf("long size:%d\n", (s32) sizeof(long));
}


typedef struct _BB {
    __refer ref;
    s32 count;
    autoptr *a;
} BB;

void t5() {
    __refer r = jvm_calloc(100);
    autoptr *a = autoptr_new(r);

    BB *b = jvm_calloc(sizeof(BB));
    b->a = autoptr_get(a);
    autoptr_NULL(&a);
    autoptr_NULL(&b->a);
    jvm_printf("a = %llx , b->a = %llx\n", (s64) (long) a, (s64) (long) b->a);
    jvm_free(b);
}

void _on_jvm_sig(s32 signo) {
    int debug = 1;
}

/*
 *
 */
int main(int argc, char **argv) {
#ifdef __MEM_LEAK_DETECT
    dbg_init(0);
    //dbg_catch_sigsegv();
#endif //__MEM_LEAK_DETECT




    s32 ret;
//    ret = execute("../../javalib_test/build/classes/", "test/Foo1", argc, argv);
    ret = execute("../../javalib/build/classes/", "test/Foo1", argc, argv);
//    t3();
//    testMinizWrapper();


#ifdef __MEM_LEAK_DETECT
    dbg_heap_dump("");
    dbg_mem_stat();
#endif //__MEM_LEAK_DETECT
    return ret;
}

