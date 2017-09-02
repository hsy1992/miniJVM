cmake_minimum_required(VERSION 3.8)
project(mini_jvm)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        jvm/interpreter.c
        jvm/class.c
        jvm/class_loader.c
        jvm/class_loader_constant.c
        jvm/class_loader_field.c
        jvm/class_loader_interface.c
        jvm/java_native.c
        jvm/java_native.h
        jvm/jvm.c
        jvm/class_loader_method.c
        jvm/jvm.h
        jvm/stack.c
        utils/arraylist.c
        utils/arraylist.h
        utils/hashtable.c
        utils/hashtable.h
        utils/utf8_string.c
        utils/utf8_string.h
        test.c jvm/garbage.h
        jvm/garbage.c
        jvm/jvm_util.c
        jvm/jvm_util.h
        jvm/os_linux.c
        jvm/os_win.c
        jvm/global.c
        utils/pairlist.h
        utils/pairlist.c
        jvm/global.h
        utils/hashset.c
        utils/hashset.h
        jvm/java_native_net.c)

add_executable(mini_jvm ${SOURCE_FILES})