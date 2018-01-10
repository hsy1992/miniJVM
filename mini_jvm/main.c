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
#include <errno.h>
#include <signal.h>
#include "jvm/jvm_util.h"
#include "jvm/jvm.h"


void _on_jvm_sig(int no) {

    jvm_printf("signo:%d  errno: %d , %s\n", no, errno, strerror(errno));
}

/*
 *
 */
int main(int argc, char **argv) {
//    jvm_printf("void* size:%d\n", sizeof(void *));
//    jvm_printf("long size:%d\n", sizeof(long));
//    jvm_printf("intptr_t size:%d\n", sizeof(intptr_t));
    signal(SIGABRT, _on_jvm_sig);
    signal(SIGFPE, _on_jvm_sig);
    signal(SIGSEGV, _on_jvm_sig);
    signal(SIGTERM, _on_jvm_sig);

    char *classpath = NULL;
    char *main_name = NULL;
    ArrayList *java_para = arraylist_create(0);
    s32 ret;
    if (argc >
        1) {//  mini_jvm   -Xmx1024M -cp ../../javalib/dist/minijvm_rt.jar;../../javalib_test/dist/minijvm_test.jar;./ test/Foo1 999
        s32 i;
        for (i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-cp") == 0 || strcmp(argv[i], "-classpath") == 0) {
                classpath = argv[i + 1];
                i++;
            } else if (argv[i][0] == '-') {
                //other jvm para
            } else if (main_name == NULL) {
                main_name = argv[i];
            } else {
                arraylist_push_back(java_para, argv[i]);
            }
        }
    } else {
//        classpath = "../../javalib/dist/minijvm_rt.jar;../../win_gui/gui_lib/dist/gui_lib.jar;./";
//        main_name = "test/GuiTest";

//        classpath = "../../javalib/dist/minijvm_rt.jar;../../jni_test/java/dist/jni_test.jar;./";
//        main_name = "test/JniTest";


        classpath = "../../javalib/dist/minijvm_rt.jar;../../javalib_test/dist/minijvm_test.jar;./";
        main_name = "test/Foo1";
//        main_name = "test/Foo2";
//        main_name = "test/TestFile";
//        main_name = "test/HttpServer";
//        main_name = "test/BpDeepTest";
//        main_name = "test/ReflectTest";

    }
    ret = execute_jvm(classpath, main_name, java_para);
    arraylist_destory(java_para);
    return ret;
}

