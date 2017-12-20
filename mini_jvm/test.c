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
#include "jvm/jvm.h"


/*
 *
 */
int main(int argc, char **argv) {

    s32 ret;
    ret = execute_jvm("../../javalib_test/dist/minijvm_javatest.jar:../../javalib_test/build/classes/", "test/Foo1",
                      argc, (c8 **) argv);
    ret = execute_jvm("../../javalib/dist/mini_jvm_java.jar;./", "test/Foo1", argc,
                      (c8 **) argv);

    return ret;
}

