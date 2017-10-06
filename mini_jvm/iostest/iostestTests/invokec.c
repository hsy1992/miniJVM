//
//  call_c.c
//  iostestTests
//
//  Created by Gust on 2017/10/6.
//  Copyright © 2017年 Gust. All rights reserved.
//

#include "../../jvm/jvm.h"
#include "iostestTests-Bridging-Header.h"

int call_jvm(char* app_path) {
    s32 argc=0;
    char ** argv=NULL;
    Utf8String * ustr=utf8_create_c(app_path);
    utf8_append_c(ustr, "/classes/");
    s32 ret ;
    ret= execute(utf8_cstr(ustr), "test/Foo1", argc, argv);
    utf8_destory(ustr);
    return ret;
}

