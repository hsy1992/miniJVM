//
// Created by Gust on 2018/2/1.
//


#include "jvm.h"
#include "jni_gui.h"


void JNI_OnLoad(JniEnv *env) {
    memset(&refers, 0, sizeof(GlobeRefer));
    refers.env = env;
    env->native_reg_lib(ptr_GlfmFuncTable(), count_GlfmFuncTable());
    env->native_reg_lib(ptr_GLFuncTable(), count_GLFuncTable());
    env->native_reg_lib(ptr_NanovgFuncTable(), count_NanovgFuncTable());
}

void JNI_OnUnload(JniEnv *env) {
    env->native_remove_lib(ptr_GlfmFuncTable());
    env->native_remove_lib(ptr_GLFuncTable());
    env->native_remove_lib(ptr_NanovgFuncTable());
}



/* ===============================================================
 *
 *                          DEMO
 *
 * ===============================================================*/

int main(void) {

    return 0;
}
