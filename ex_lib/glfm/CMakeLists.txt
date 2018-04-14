cmake_minimum_required(VERSION 3.1.0)

project(GLFM C)

option(GLFM_BUILD_EXAMPLE "Build the GLFM example" ON)

set(GLFM_HEADERS include/glfm.h)

include_directories(gui/ gui/stb gui/other ../../mini_jvm/jvm)

set(NANOVG_SOURCE_FILES
	gui/stb/stb_image_write.h
	gui/stb/stb_image.h
	gui/stb/stb_truetype.h
	gui/nanovg/fontstash.h
	gui/nanovg/nanovg_gl.h
	gui/nanovg/nanovg_gl_utils.h
	gui/nanovg/nanovg.h
	gui/nanovg/nanovg.c
	gui/glad/glad.c
	gui/glad/glad.h
	gui/KHR/khrplatform.h
	gui/other/linmath.h
	gui/jni_gui.h
	gui/jni_nanovg_assist.h
	gui/jni_stb_assist.h
	gui/jni_nanovg.c
	gui/jni_gui.c
	gui/jni_glfm.c
)


set(MINI_JVM_SOURCE_FILES
        ../../mini_jvm/utils/d_type.h
        ../../mini_jvm/utils/d_utils.c
        ../../mini_jvm/utils/arraylist.c
        ../../mini_jvm/utils/arraylist.h
        ../../mini_jvm/utils/hashtable.c
        ../../mini_jvm/utils/hashtable.h
        ../../mini_jvm/utils/utf8_string.c
        ../../mini_jvm/utils/utf8_string.h
        ../../mini_jvm/utils/pairlist.h
        ../../mini_jvm/utils/pairlist.c
        ../../mini_jvm/utils/hashset.c
        ../../mini_jvm/utils/hashset.h
        ../../mini_jvm/jvm/jvm.h
        ../../mini_jvm/jvm/interpreter.c
        ../../mini_jvm/jvm/jvm.c
        ../../mini_jvm/jvm/class.c
        ../../mini_jvm/jvm/runtime.c
        ../../mini_jvm/jvm/garbage.c
        ../../mini_jvm/jvm/jvm_util.c
        ../../mini_jvm/jvm/jvm_util.h
        ../../mini_jvm/jvm/global.c
        ../../mini_jvm/jvm/java_native_std.h
        ../../mini_jvm/jvm/java_native_io.c
        ../../mini_jvm/jvm/java_native_std.c
        ../../mini_jvm/jvm/java_native_reflect.c
        ../../mini_jvm/jvm/java_native_reflect.h
        ../../mini_jvm/jvm/jdwp.c
        ../../mini_jvm/jvm/jdwp.h
        ../../mini_jvm/jvm/java_native_io.h
        ../../mini_jvm/jvm/class_load.c
        ../../mini_jvm/jvm/garbage.h
        ../../mini_jvm/utils/tinycthread.h
        ../../mini_jvm/utils/tinycthread.c
        ../../mini_jvm/utils/linkedlist.c
        ../../mini_jvm/utils/linkedlist.h
        ../../mini_jvm/utils/spinlock.c
        ../../mini_jvm/utils/spinlock.h
        ../../mini_jvm/utils/bytebuf.c
        ../../mini_jvm/utils/bytebuf.h
        ../../mini_jvm/utils/miniz/miniz_wrapper.c
        ../../mini_jvm/utils/miniz/miniz_wrapper.h
        ../../mini_jvm/utils/dirent_win.h)


if (CMAKE_SYSTEM_NAME MATCHES "Emscripten")
    set(GLFM_SRC src/glfm_platform.h src/glfm_platform_emscripten.c)
elseif (CMAKE_SYSTEM_NAME MATCHES "Android")
    set(GLFM_SRC src/glfm_platform.h src/glfm_platform_android.c ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
    # Set NDEBUG for android_native_app_glue to remove some superfluous logging
    set_source_files_properties(${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c PROPERTIES COMPILE_FLAGS "-DNDEBUG")
else() # Assume iOS
    set(IOS TRUE)
    set(GLFM_SRC src/glfm_platform.h src/glfm_platform_ios.m)
    set(CMAKE_OSX_SYSROOT "iphoneos")
    set(GLFM_COMPILE_FLAGS "-Wno-objc-interface-ivars -Wno-objc-missing-property-synthesis -Wno-direct-ivar-access")
endif()

add_library(glfm ${GLFM_SRC} ${GLFM_HEADERS} ${MINI_JVM_SOURCE_FILES} ${NANOVG_SOURCE_FILES})
target_include_directories(glfm PUBLIC include)
target_include_directories(glfm PRIVATE src)

source_group(include FILES ${GLFM_HEADERS})
source_group(src FILES ${GLFM_SRC})



if (CMAKE_C_COMPILER_ID MATCHES "Clang")
    set_target_properties(glfm PROPERTIES COMPILE_FLAGS "-Weverything -Wwrite-strings -Wno-padded -Wno-covered-switch-default ${GLFM_COMPILE_FLAGS}")
elseif (CMAKE_C_COMPILER_ID MATCHES "GNU")
    set_target_properties(glfm PROPERTIES COMPILE_FLAGS "-Wall -Wextra -Wwrite-strings ${GLFM_COMPILE_FLAGS}")
elseif (CMAKE_C_COMPILER_ID MATCHES "MSVC")
    set_target_properties(glfm PROPERTIES COMPILE_FLAGS "/Wall ${GLFM_COMPILE_FLAGS}")
endif()

if (CMAKE_SYSTEM_NAME MATCHES "Android")
    find_library(log-lib log)
    find_library(android-lib android)
    find_library(EGL-lib EGL)
    find_library(GLESv2-lib GLESv2)
    target_link_libraries(glfm ${log-lib} ${android-lib} ${EGL-lib} ${GLESv2-lib})
    target_include_directories(glfm PRIVATE ${ANDROID_NDK}/sources/android/native_app_glue/)
    set_target_properties(glfm PROPERTIES
        C_STANDARD 99)
elseif (IOS)
    target_link_libraries(glfm "-framework Foundation -framework CoreGraphics -framework UIKit -framework OpenGLES -framework QuartzCore")
    set_target_properties(glfm PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/GLFM.build/lib # For Archiving
        XCODE_ATTRIBUTE_SUPPORTED_PLATFORMS "iphoneos iphonesimulator appletvos appletvsimulator"
        XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 8.0
        XCODE_ATTRIBUTE_TVOS_DEPLOYMENT_TARGET 9.0
        XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
    )
endif()

if (GLFM_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
