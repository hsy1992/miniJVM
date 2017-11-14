cmake_minimum_required(VERSION 3.8)
project(mini_jvm)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD 99)

include_directories(jvm utils utils/cmem)

set(SOURCE_FILES
        utils/d_type.h
        utils/d_utils.c
        utils/arraylist.c
        utils/arraylist.h
        utils/hashtable.c
        utils/hashtable.h
        utils/utf8_string.c
        utils/utf8_string.h
        utils/pairlist.h
        utils/pairlist.c
        utils/hashset.c
        utils/hashset.h
        jvm/jvm.h
        jvm/interpreter.c
        jvm/jvm.c
        jvm/class.c
        jvm/runtime.c
        jvm/garbage.c
        jvm/jvm_util.c
        jvm/jvm_util.h
        jvm/os_linux.c
        jvm/os_win.c
        jvm/global.c
        jvm/global.h
        jvm/java_native_std.h
        jvm/java_native_io.c
        jvm/java_native_std.c
        jvm/java_native_reflect.c
        jvm/java_native_reflect.h
        jvm/jdwp.c
        jvm/jdwp.h
        jvm/java_native_io.h
        jvm/class_load.c
        jvm/garbage.h
        utils/cmem/memleak.c
        utils/cmem/memleak.h
        utils/cmem/example.c
        utils/cmem/phashtable.c
        utils/cmem/phashtable.h
        test.c
        utils/linkedlist.c
        utils/linkedlist.h utils/spinlock.c utils/spinlock.h)

add_executable(mini_jvm ${SOURCE_FILES})

#Set link library.
TARGET_LINK_LIBRARIES(mini_jvm pthread m )
if ( (CMAKE_SYSTEM_NAME MATCHES "Windows") OR (CMAKE_SYSTEM_NAME MATCHES "CYGWIN"))
    TARGET_LINK_LIBRARIES(mini_jvm  ws2_32)
endif ()
MESSAGE(STATUS "platform: ${CMAKE_SYSTEM_NAME}")