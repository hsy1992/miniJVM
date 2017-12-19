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
#include "utils/bytebuf.h"

#include "jvm/jvm.h"
#include "jvm/jvm_util.h"
#include "jvm/garbage.h"


/*
 *
 */
int main(int argc, char **argv) {

    s32 ret;
    ret = execute_jvm("../../javalib_test/dist/minijvm_javatest.jar:../../javalib_test/build/classes/", "test/Foo1", argc, (c8 **)argv);
    ret = execute_jvm("../../javalib/dist/mini_jvm_java.jar:../../javalib/build/classes/", "test/Foo1", argc,
                      (c8 **) argv);
//    testMinizWrapper();

    return ret;
}

