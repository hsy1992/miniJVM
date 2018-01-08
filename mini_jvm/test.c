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
#include <jvm_util.h>
#include "jvm/jvm.h"


/*
 *
 */
int main(int argc, char **argv) {
    jvm_printf("void* size:%d\n", sizeof(void *));
    jvm_printf("long size:%d\n", sizeof(long));
    jvm_printf("intptr_t size:%d\n", sizeof(intptr_t));
    s32 ret;

//    ret = execute_jvm("../../javalib/dist/mini_jvm_java.jar;../../win_gui/gui_lib/dist/gui_lib.jar;./",
//                      "test/GuiTest",
//                      argc,
//                      (c8 **) argv);



//    ret = execute_jvm("../../javalib/dist/mini_jvm_java.jar;../../javalib_test/dist/minijvm_javatest.jar;./",
//                      "test/Foo1",
//                      argc,
//                      (c8 **) argv);

    ret = execute_jvm("../../javalib/dist/mini_jvm_java.jar;../../jni_test/java/dist/jni_test.jar;./",
                      "test/JniTest",
                      argc,
                      (c8 **) argv);

//    ret = execute_jvm("../../javalib/dist/mini_jvm_java.jar;../../javalib_test/dist/minijvm_javatest.jar;./",
//                      "test/Foo2",
//                      argc,
//                      (c8 **) argv);

//    ret = execute_jvm("../../javalib/dist/mini_jvm_java.jar;../../javalib_test/dist/minijvm_javatest.jar;./",
//                      "test/TestFile",
//                      argc,
//                      (c8 **) argv);

//    ret = execute_jvm("../../javalib/dist/mini_jvm_java.jar;../../javalib_test/dist/minijvm_javatest.jar;./",
//                      "test/HttpServer",
//                      argc,
//                      (c8 **) argv);

//    ret = execute_jvm("../../javalib/dist/mini_jvm_java.jar;../../javalib_test/dist/minijvm_javatest.jar;./",
//                      "test/BpDeepTest",
//                      argc,
//                      (c8 **) argv);

//    ret = execute_jvm("../../javalib/dist/mini_jvm_java.jar;../../javalib_test/dist/minijvm_javatest.jar;./",
//                      "test/ReflectTest",
//                      argc,
//                      (c8 **) argv);

    return ret;
}

