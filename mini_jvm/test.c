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
#include "utils/hashtable.h"
#include "utils/utf8_string.h"
#include "jvm/jvm.h"
#include "jvm/jvm_util.h"
#include "jvm/garbage.h"


typedef struct SA {
    int a;

    int (*getv)();
} TA;

int _getv(TA *_this) {
    return _this->a;
}

/*
 * 
 */
int t1() {
    printf("start test\n");
//
//    TA ta;
//    ta.a = 999;
//    ta.getv = _getv;
//    int ia = ta.getv(&ta);
//    printf("struct a.getv():%d\n", ia);
//
//    hmap_t map = hashtable_new(_DEFAULT_HashtableHash, _DEFAULT_HashtableEquals);
//    //    int a = 1, b = 2, c = 3;
//    char* a = "aaa", *b = "bbb", *c = "ccc";
//    hashtable_put(map, a, a);
//    hashtable_put(map, b, b);
//    hashtable_put(map, c, c);
//    int* iptr = NULL;
//    int v = 0;
//    int i = 0;
//    static const int BUF_SIZE = 10;
//    char cbuf[BUF_SIZE];
//    HashtableIterator hti;
//    hashtable_iterate(map, &hti);
//    printf("hmap_t:%d\n", hashtable_num_entries(map));
//    for (; hashtable_iter_has_more(&hti); i++) {
//        itoa(i, cbuf, BUF_SIZE);
//        char* k = hashtable_iter_next_key(&hti);
//        char* v = hashtable_get(map, k);
//        printf("%s,%s\n", k, v);
//    }
//
//    hmap_t imap = hashtable_new(_DEFAULT_HashtableHash, _DEFAULT_HashtableEquals);
//    hashtable_put(imap, 1, a);
//
//    printf("imap : %s\n",hashtable_get(imap,1));
//
//    Utf8String *s = utf8_create_c("test is is .");
//    utf8_append_c(s, "ok");
//    printf("string:%s\n", utf8_cstr(s));
//    utf8_replace_c(s, "is", "was");
//    utf8_replace_c(s, "ok", "");
//    printf("string:%s\n", utf8_cstr(s));
//    printf("string:%d\n", utf8_indexof_c(s, "ok"));
//
//    utf8_destory(s);
//    int a=0;
//    char b=-20;
//    char arr_body[]={-20,10,10,20};
//    int bytes=1;
//    switch(bytes){
//        case 1:
//            a=arr_body[0]<<8|arr_body[1];
//            break;
//    }
//    printf("%d\n",a);
}

void t2() {

    StackFrame *stack = stack_init(10);

    double d = 3.1415926535;
    int ix = 0;

    memcpy(&ix, &d, sizeof(int));
    push_int(stack, ix);
    memcpy(&ix, ((char *) &d) + 4, sizeof(int));
    push_int(stack, ix);

    long long int v;
    v = pop_int(stack);
    v <<= 32;
    v |= pop_int(stack);
    double d1 = 0.0;
    memcpy(&d1, &v, sizeof(v));

    printf("%f\n", d1);

    int i = 9;
    push_int(stack, i);
    int a = pop_int(stack);
    printf("a=%x\n", a);
    long long int l = 0x1000000120000002L;
    push_long(stack, l);
    long long int value = pop_long(stack);
    printf("value=%llx\n", value);

    //16f311e0000000 << 8 =65f65816f311e0
    long long int aval = 0x16f311e0000000L;
    aval = aval << 8;
    printf("shift bit  %llx\n", aval);

}

void t3() {
    double d = 88888888.12345678765f;
    float f = 66666666.66;
    u8 buf[20];
    int i = 0;
    for (i = 0; i < 20; i++)buf[i] = 'U';
    sprintf(buf, "%lf\n", d);
    printf("%f, %8.7f\n", f, d);
    printf("%s", buf);
}

void t4() {
    printf("pointer size:%d\n", sizeof(void *));
    printf("jint size:%d\n", sizeof(s32));
    printf("jlong size:%d\n", sizeof(s64));
    printf("jshort size:%d\n", sizeof(s16));
    printf("jbyte size:%d\n", sizeof(c8));
    printf("jdouble size:%d\n", sizeof(f64));
    printf("jfloat size:%d\n", sizeof(f32));

    printf("long size:%d\n", sizeof(long));
}

void t5() {
    s64 start, spent;
    s32 max = 200000000;
    s32 i, v;
    Long2Double l2d;
    v = 0;
    start = currentTimeMillis();
    for (i = 0; i < max; i++) {
        v = i;
    }
    spent = currentTimeMillis() - start;
    printf("v=%lld, spent %lld\n", v, spent);
    v = 0;
    start = currentTimeMillis();
    for (i = 0; i < max; i++) {
        //memcpy(&v, &i, sizeof(s32));
        l2d.i2l.i1 = i;
    }
    spent = currentTimeMillis() - start;

    printf("v=%lld, spent %lld\n", l2d.i2l.i1, spent);
}

int main(int argc, char **argv) {
    s32 ret=execute("../java/build/classes/", "com/egls/test/Foo1", argc, argv);
    return ret;
//    t5();
//    return 0;
}

