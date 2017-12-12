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

#include "utils/hashtable.h"

#include "jvm/jvm.h"
#include "jvm/jvm_util.h"
#include "jvm/garbage.h"


/*
 *
 */
int main(int argc, char **argv) {
#ifdef __MEM_LEAK_DETECT
    dbg_init(0);
    //dbg_catch_sigsegv();
#endif //__MEM_LEAK_DETECT


    s32 ret;
//    ret = execute("../../javalib_test/build/classes/", "test/Foo1", argc, (c8 **)argv);
    ret = execute("../../javalib/build/classes/", "test/Foo1", argc, (c8 **) argv);
//    testMinizWrapper();


#ifdef __MEM_LEAK_DETECT
    dbg_heap_dump("");
    dbg_mem_stat();
#endif //__MEM_LEAK_DETECT
    return ret;
}

