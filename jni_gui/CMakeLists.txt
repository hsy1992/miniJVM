cmake_minimum_required(VERSION 3.8)
project(jni_gui)


#set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

include_directories(deps/include/)
find_library(MY_LIB libglfw3.a deps/lib/win_i686/)

set(SOURCE_FILES
        glad.c
        jni_glfw.c
        )
find_package( OpenGL REQUIRED )
add_executable(test_gui ${SOURCE_FILES})

add_library(gui SHARED ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(gui ${MY_LIB} opengl32)

#set(CMAKE_EXE_LINKER_FLAGS "-mwindows")