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


void t1() {

}

void t2() {
}

void t3() {

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

void t5() {


}

/*
 *
 */
int main(int argc, char **argv) {
    s32 ret = execute("../java/build/classes/", "test/Foo1", argc, argv);
    return ret;
}

