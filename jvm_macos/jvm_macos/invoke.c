//
//  invoke.c
//  jvm_macos
//
//  Created by Gust on 2018/1/30.
//  Copyright © 2018年 Gust. All rights reserved.
//

#include "../../mini_jvm/jvm/jvm.h"
#include "../../mini_jvm/utils/arraylist.h"
#include "jvm_macos-Bridging-Header.h"

int call_jvm(char* app_path) {
    s32 ret ;
    char path[512];
    memset(&path,0,512);
    strcat(path,app_path);
    strcat(path,"/dist/lib/minijvm_rt.jar");
    strcat(path,";");
    strcat(path,app_path);
    strcat(path,"/dist/minijvm_test.jar");
    printf("classpath: %s\n",path);
    ArrayList * java_para=arraylist_create(0);
    ret= execute_jvm(path, "test/Foo1", java_para);
    arraylist_destory(java_para);
    return ret;
}

