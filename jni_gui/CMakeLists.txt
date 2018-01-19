cmake_minimum_required(VERSION 3.8)
project(jni_gui)


#set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

include_directories(deps/include/)

set(SOURCE_FILES
        glad.c
        jni_gui.h
        jni_glfw.c
        jni_gl.c
        jni_nk.c
        )
find_package(OpenGL REQUIRED)
add_executable(test_gui ${SOURCE_FILES})

add_library(gui SHARED ${SOURCE_FILES})


MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")

IF (APPLE)
    MESSAGE("apple os here.")
    find_library(MY_LIB libglfw3.a deps/lib/mac/)
    find_library(COCOA_LIB Cocoa)
    find_library(IOKIT_LIB IOKit)
    find_library(OPENGL_LIB OpenGL)
    find_library(COREFOUNDATION_LIB CoreFoundation)
    find_library(COREVIDEO_LIB CoreVideo)
    MARK_AS_ADVANCED(COCOA_LIB IOKIT_LIB OPENGL_LIB COREFOUNDATION_LIB COREVIDEO_LIB)
    SET(EXTRA_LIBS ${COCOA_LIB} ${IOKIT_LIB} ${OPENGL_LIB} ${COREFOUNDATION_LIB} ${COREVIDEO_LIB})

    TARGET_LINK_LIBRARIES(gui ${EXTRA_LIBS})
ELSEIF (LINUX)
ELSEIF (WINDOWS)
    find_library(MY_LIB libglfw3.a deps/lib/win_i686/)
    TARGET_LINK_LIBRARIES(gui opengl32)
ENDIF ()

TARGET_LINK_LIBRARIES(gui ${MY_LIB})


#set(CMAKE_EXE_LINKER_FLAGS "-mwindows")