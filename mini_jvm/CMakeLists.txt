cmake_minimum_required(VERSION 3.8)
project(mini_jvm)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD 99)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    ADD_DEFINITIONS(-D_DEBUG -D__JVM_DEBUG__)
    message("this is a debug compile")
endif ()

include_directories()

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
        jvm/global.c
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
        main.c
        utils/tinycthread.h
        utils/tinycthread.c
        utils/linkedlist.c
        utils/linkedlist.h
        utils/spinlock.c
        utils/spinlock.h
        utils/bytebuf.c
        utils/bytebuf.h
        utils/miniz/miniz_wrapper.c
        utils/miniz/miniz_wrapper.h
        utils/dirent_win.h)

add_executable(mini_jvm ${SOURCE_FILES})

#Set link library.
TARGET_LINK_LIBRARIES(mini_jvm pthread m)

MESSAGE(STATUS "platform: ${CMAKE_SYSTEM_NAME}")

IF (APPLE)
    SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../binary/macos)
ELSEIF (UNIX)
    TARGET_LINK_LIBRARIES(mini_jvm dl)
    SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../binary/centos_x64)
ELSEIF (MINGW)
    TARGET_LINK_LIBRARIES(mini_jvm ws2_32)
    IF (CMAKE_SIZEOF_VOID_P EQUAL 4)
        SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../binary/win32)
    elseif (CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../binary/win_x64)
    endif ()
ENDIF ()

