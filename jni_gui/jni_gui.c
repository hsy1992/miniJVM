//
// Created by Gust on 2018/2/1.
//


#include "../mini_jvm/jvm/jvm.h"
#include "jni_gui.h"


void JNI_OnLoad(JniEnv *env) {
    memset(&refers, 0, sizeof(GlobeRefer));
    refers.env = env;
    env->native_reg_lib(ptr_GlfwFuncTable(), count_GlfwFuncTable());
    env->native_reg_lib(ptr_NkFuncTable(), count_NkFuncTable());
    env->native_reg_lib(ptr_GLFuncTable(), count_GLFuncTable());
    env->native_reg_lib(ptr_NutilFuncTable(), count_NutilFuncTable());
}

void JNI_OnUnload(JniEnv *env) {
    env->native_remove_lib(ptr_GlfwFuncTable());
    env->native_remove_lib(ptr_NkFuncTable());
    env->native_remove_lib(ptr_GLFuncTable());
    env->native_remove_lib(ptr_NutilFuncTable());
}



/* ===============================================================
 *
 *                          DEMO
 *
 * ===============================================================*/

int main(void) {

    return 0;
}
