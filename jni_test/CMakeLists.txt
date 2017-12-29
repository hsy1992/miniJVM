cmake_minimum_required(VERSION 3.8)
project(jnitest)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        jni_ex.c)

#add_executable(test1 ${SOURCE_FILES})

add_library(jnitest SHARED  ${SOURCE_FILES})