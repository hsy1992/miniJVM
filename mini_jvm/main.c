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
    signal(SIGABRT, _on_jvm_sig);
    signal(SIGFPE, _on_jvm_sig);
    signal(SIGSEGV, _on_jvm_sig);
    signal(SIGTERM, _on_jvm_sig);

    char *classpath = NULL;
    char *main_name = NULL;
    ArrayList *java_para = arraylist_create(0);
    s32 ret;
    if (argc >
        1) {//  mini_jvm   -Xmx16M -cp ../../javalib/dist/minijvm_rt.jar;../../javalib_test/dist/minijvm_test.jar;./ test/Foo1 999
        s32 i;
        for (i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-cp") == 0 || strcmp(argv[i], "-classpath") == 0) {
                classpath = argv[i + 1];
                i++;
            } else if (strcmp(argv[i], "-Xdebug") == 0) {
                java_debug = 1;
            } else if (argv[i][0] == '-') {
                jvm_printf("skiped argv: %s", argv[i]);
                //other jvm para
            } else if (main_name == NULL) {
                main_name = argv[i];
            } else {
                arraylist_push_back(java_para, argv[i]);
            }
        }
    } else {
        java_debug = 0;
        classpath = "../../javalib/dist/minijvm_rt.jar;../../jni_gui/java/dist/gui_lib.jar;./";
//        main_name = "test/Gears";
//        main_name = "test/TestGL";
//        main_name = "test/GuiTest";
//        main_name = "test/RenderTexure";
//        main_name = "test/Alpha";
//        main_name = "test/Light";
//        main_name = "test/Shader";
//        main_name = "test/Boing";
        main_name = "test/TestNanovg";

//        classpath = "../../javalib/dist/minijvm_rt.jar;../../jni_test/java/dist/jni_test.jar;./";
//        main_name = "test/JniTest";


//        classpath = "../../javalib/dist/minijvm_rt.jar;../../javalib_test/dist/minijvm_test.jar;./";
//        main_name = "test/Foo1";
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

