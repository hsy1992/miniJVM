//
// Created by gust on 2018/1/17.
//

#ifndef JNI_GUI_JNI_GLFW_H
#define JNI_GUI_JNI_GLFW_H

//tag dont delete this line, builder will auto insert here



#define NUTIL_API extern

typedef struct _GlobeRefer GlobeRefer;
extern GlobeRefer refers;

//__refer ptr_NkFuncTable();
//
//s32 count_NkFuncTable();

__refer ptr_GLFuncTable();

s32 count_GLFuncTable();

__refer ptr_GlfwFuncTable();

s32 count_GlfwFuncTable();

__refer ptr_NutilFuncTable();

s32 count_NutilFuncTable();

static void init() {}

struct _GlobeRefer {
    JniEnv *env;
    Runtime *runtime;
    Instance *glfm_callback;
    MethodInfo *_callback_surface_error;
    MethodInfo *_callback_key;
    MethodInfo *_callback_character;
    MethodInfo *_callback_mainloop;
    MethodInfo *_callback_memory_warning;
    MethodInfo *_callback_keyboard_visible;
    MethodInfo *_callback_touch;
    MethodInfo *_callback_cursor_enter;
    MethodInfo *_callback_surface_destroyed;
    MethodInfo *_callback_surface_resized;
    MethodInfo *_callback_window_pos;
    MethodInfo *_callback_app_focus;
    MethodInfo *_callback_surface_created;
    MethodInfo *_callback_window_refresh;
    MethodInfo *_callback_framebuffer_size;
};


// dont delete the comment .for generate jni
/*

 */
//implementation



#endif //JNI_GUI_JNI_GLFW_H
