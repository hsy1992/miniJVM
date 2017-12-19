//
//  call_c.c
//  iostestTests
//
//  Created by Gust on 2017/10/6.
//  Copyright © 2017年 Gust. All rights reserved.
//

#include "../jvm/jvm.h"
#include "iostestTests-Bridging-Header.h"

int call_jvm(char* app_path) {
    s32 argc=0;
    char ** argv=NULL;
    s32 ret ;
    char path[256];
    memset(&path,0,256);
    strcat(path,app_path);
    strcat(path,"/dist/mini_jvm_java.jar");
    printf("classpath: %s\n",path);
    ret= execute_jvm(path, "test/Foo1", argc, argv);
    return ret;
}

