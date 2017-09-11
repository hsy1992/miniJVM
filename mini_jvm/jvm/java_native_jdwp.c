//
// Created by gust on 2017/9/1.
//

#include "jvm.h"
#include "java_native_std.h"
#include "garbage.h"


#ifdef __cplusplus
extern "C" {
#endif


s32 javax_mini_jdwp_reflect_JdwpNative_referenceTyepSize(Runtime *runtime, Class *clazz) {
    push_int(runtime->stack, sizeof(__refer));

#if _JVM_DEBUG
    printf("javax_mini_jdwp_reflect_JdwpNative_referenceTyepSize\n");
#endif
    return 0;
}

static java_native_method method_jdwp_table[] = {
        {"javax/mini/jdwp/reflect/JdwpNative", "referenceTyepSize", "()I", javax_mini_jdwp_reflect_JdwpNative_referenceTyepSize},

};


void reg_jdwp_native_lib() {
    native_reg_lib(&(method_jdwp_table[0]), sizeof(method_jdwp_table) / sizeof(java_native_method));
}


#ifdef __cplusplus
}
#endif