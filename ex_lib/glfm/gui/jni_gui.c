//
// Created by Gust on 2018/2/1.
//


#include "jvm.h"
#include "jni_gui.h"


void JNI_OnLoad_mini(JniEnv *env) {
    memset(&refers, 0, sizeof(GlobeRefer));
    refers.env = env;

    refers._runtime_surface_error = runtime_create(NULL);
    refers._runtime_key = runtime_create(NULL);
    refers._runtime_character = runtime_create(NULL);
    refers._runtime_mainloop = runtime_create(NULL);
    refers._runtime_memory_warning = runtime_create(NULL);
    refers._runtime_keyboard_visible = runtime_create(NULL);
    refers._runtime_touch = runtime_create(NULL);
    refers._runtime_surface_destroyed = runtime_create(NULL);
    refers._runtime_surface_resized = runtime_create(NULL);
    refers._runtime_app_focus = runtime_create(NULL);
    refers._runtime_surface_created = runtime_create(NULL);

    env->native_reg_lib(ptr_GlfmFuncTable(), count_GlfmFuncTable());
    env->native_reg_lib(ptr_GLFuncTable(), count_GLFuncTable());
    env->native_reg_lib(ptr_NanovgFuncTable(), count_NanovgFuncTable());
}

void JNI_OnUnload_mini(JniEnv *env) {
    env->native_remove_lib(ptr_GlfmFuncTable());
    env->native_remove_lib(ptr_GLFuncTable());
    env->native_remove_lib(ptr_NanovgFuncTable());

    runtime_destory(refers._runtime_surface_error);
    runtime_destory(refers._runtime_key);
    runtime_destory(refers._runtime_character);
    runtime_destory(refers._runtime_mainloop);
    runtime_destory(refers._runtime_memory_warning);
    runtime_destory(refers._runtime_keyboard_visible);
    runtime_destory(refers._runtime_touch);
    runtime_destory(refers._runtime_surface_destroyed);
    runtime_destory(refers._runtime_surface_resized);
    runtime_destory(refers._runtime_app_focus);
    runtime_destory(refers._runtime_surface_created);
}


/* ===============================================================
 *
 *                          DEMO
 *
 * ===============================================================*/

int main(void) {

    return 0;
}
