/* nuklear - 1.32.0 - public domain */
#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <string.h>
#include "deps/include/glad/glad.h"
#include "deps/include/GLFW/glfw3.h"

#include "../mini_jvm/jvm/jvm.h"


typedef struct _GlobeRefer GlobeRefer;

struct _GlobeRefer {
    JniEnv *env;
    Instance *glfw_callback;
    MethodInfo *_callback_error;
    MethodInfo *_callback_key;
    MethodInfo *_callback_character;
    MethodInfo *_button_callback_mouse;
    MethodInfo *_callback_cursor_pos;
    MethodInfo *_callback_cursor_enter;
    MethodInfo *_callback_window_close;
    MethodInfo *_callback_window_size;
    MethodInfo *_callback_window_pos;
    MethodInfo *_callback_window_focus;
    MethodInfo *_callback_window_iconify;
    MethodInfo *_callback_window_refresh;
    MethodInfo *_callback_framebuffer_size;
    Runtime *runtime;
};
GlobeRefer refers;


/* ==============================   local tools  =================================*/

s64 getParaLong(Runtime *runtime, s32 index) {
    JniEnv *env = runtime->jnienv;
    Long2Double l2d;
    l2d.i2l.i1 = env->localvar_getInt(runtime, index);
    l2d.i2l.i0 = env->localvar_getInt(runtime, index + 1);
    return l2d.l;
}

c8 *jbytearr2c8arr(JniEnv *env, Instance *jbytearr) {
    c8 *arr = env->jvm_malloc(jbytearr->arr_length + 1);
    memcpy(arr, jbytearr->arr_body, jbytearr->arr_length);
    arr[jbytearr->arr_length] = 0;
    return arr;
}

/* ==============================   jni callback =================================*/

static void _callback_error(int error, const char *description) {
    if (refers._callback_error) {
        JniEnv *env = refers.env;
        Utf8String *ustr = env->utf8_create_part_c((c8 *) description, 0, strlen(description));
        Instance *jstr = refers.env->jstring_create(ustr, refers.runtime);
        env->utf8_destory(ustr);
        env->push_ref(refers.runtime->stack, refers.glfw_callback);
        env->push_int(refers.runtime->stack, error);
        env->push_ref(refers.runtime->stack, jstr);
        env->execute_method(refers._callback_error, refers.runtime, refers.glfw_callback->mb.clazz);
    }
}

static void _callback_key(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (refers._callback_key) {
        JniEnv *env = refers.env;
        env->push_ref(refers.runtime->stack, refers.glfw_callback);
        env->push_long(refers.runtime->stack, (s64) (intptr_t) window);
        env->push_int(refers.runtime->stack, key);
        env->push_int(refers.runtime->stack, scancode);
        env->push_int(refers.runtime->stack, action);
        env->push_int(refers.runtime->stack, mods);
        env->execute_method(refers._callback_key, refers.runtime, refers.glfw_callback->mb.clazz);
    }
}

static void _callback_character(GLFWwindow *window, u32 ch) {
    if (refers._callback_character) {
        JniEnv *env = refers.env;
        env->push_ref(refers.runtime->stack, refers.glfw_callback);
        env->push_long(refers.runtime->stack, (s64) (intptr_t) window);
        env->push_int(refers.runtime->stack, ch);
        env->execute_method(refers._callback_character, refers.runtime, refers.glfw_callback->mb.clazz);
    }
}


void _button_callback_mouse(GLFWwindow *window, int button, int action, int mods) {
    if (refers._button_callback_mouse) {
        JniEnv *env = refers.env;
        env->push_ref(refers.runtime->stack, refers.glfw_callback);
        env->push_long(refers.runtime->stack, (s64) (intptr_t) window);
        env->push_int(refers.runtime->stack, button);
        env->push_int(refers.runtime->stack, action == GLFW_PRESS);
        env->execute_method(refers._button_callback_mouse, refers.runtime, refers.glfw_callback->mb.clazz);
    }
}

void _callback_cursor_pos(GLFWwindow *window, f64 x, f64 y) {
    if (refers._callback_cursor_pos) {
        JniEnv *env = refers.env;
        env->push_ref(refers.runtime->stack, refers.glfw_callback);
        env->push_long(refers.runtime->stack, (s64) (intptr_t) window);
        env->push_int(refers.runtime->stack, x);
        env->push_int(refers.runtime->stack, y);
        env->execute_method(refers._callback_cursor_pos, refers.runtime, refers.glfw_callback->mb.clazz);
    }
}

void _callback_cursor_enter(GLFWwindow *window, s32 enter) {
    if (refers._callback_cursor_enter) {
        JniEnv *env = refers.env;
        env->push_ref(refers.runtime->stack, refers.glfw_callback);
        env->push_long(refers.runtime->stack, (s64) (intptr_t) window);
        env->push_int(refers.runtime->stack, enter);
        env->execute_method(refers._callback_cursor_enter, refers.runtime, refers.glfw_callback->mb.clazz);
    }
}

void _callback_window_size(GLFWwindow *window, s32 w, s32 h) {
    if (refers._callback_window_size) {
        JniEnv *env = refers.env;
        env->push_ref(refers.runtime->stack, refers.glfw_callback);
        env->push_long(refers.runtime->stack, (s64) (intptr_t) window);
        env->push_int(refers.runtime->stack, w);
        env->push_int(refers.runtime->stack, h);
        env->execute_method(refers._callback_window_size, refers.runtime, refers.glfw_callback->mb.clazz);
    }
}

void _callback_window_pos(GLFWwindow *window, s32 w, s32 h) {
    if (refers._callback_window_pos) {
        JniEnv *env = refers.env;
        env->push_ref(refers.runtime->stack, refers.glfw_callback);
        env->push_long(refers.runtime->stack, (s64) (intptr_t) window);
        env->push_int(refers.runtime->stack, w);
        env->push_int(refers.runtime->stack, h);
        env->execute_method(refers._callback_window_pos, refers.runtime, refers.glfw_callback->mb.clazz);
    }
}

void _callback_window_close(GLFWwindow *window) {
    if (refers._callback_window_close) {
        JniEnv *env = refers.env;
        env->push_ref(refers.runtime->stack, refers.glfw_callback);
        env->push_long(refers.runtime->stack, (s64) (intptr_t) window);
        env->execute_method(refers._callback_window_close, refers.runtime, refers.glfw_callback->mb.clazz);
        env->pop_empty(refers.runtime->stack);
    }
}

void _callback_window_focus(GLFWwindow *window, s32 focus) {
    if (refers._callback_window_focus) {
        JniEnv *env = refers.env;
        env->push_ref(refers.runtime->stack, refers.glfw_callback);
        env->push_long(refers.runtime->stack, (s64) (intptr_t) window);
        env->push_int(refers.runtime->stack, focus);
        env->execute_method(refers._callback_window_focus, refers.runtime, refers.glfw_callback->mb.clazz);
    }
}

void _callback_window_iconify(GLFWwindow *window, s32 iconified) {
    if (refers._callback_window_iconify) {
        JniEnv *env = refers.env;
        env->push_ref(refers.runtime->stack, refers.glfw_callback);
        env->push_long(refers.runtime->stack, (s64) (intptr_t) window);
        env->push_int(refers.runtime->stack, iconified);
        env->execute_method(refers._callback_window_iconify, refers.runtime, refers.glfw_callback->mb.clazz);
    }
}

void _callback_window_refresh(GLFWwindow *window) {
    if (refers._callback_window_refresh) {
        JniEnv *env = refers.env;
        env->push_ref(refers.runtime->stack, refers.glfw_callback);
        env->push_long(refers.runtime->stack, (s64) (intptr_t) window);
        env->execute_method(refers._callback_window_refresh, refers.runtime, refers.glfw_callback->mb.clazz);
    }
}

void _callback_framebuffer_size(GLFWwindow *window, s32 w, s32 h) {
    if (refers._callback_framebuffer_size) {
        JniEnv *env = refers.env;
        env->push_ref(refers.runtime->stack, refers.glfw_callback);
        env->push_long(refers.runtime->stack, (s64) (intptr_t) window);
        env->push_int(refers.runtime->stack, w);
        env->push_int(refers.runtime->stack, h);
        env->execute_method(refers._callback_framebuffer_size, refers.runtime, refers.glfw_callback->mb.clazz);
    }
}

int org_mini_glfw_utils_GlUtils_f2b(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *farr = env->localvar_getRefer(runtime, pos++);
    Instance *barr = env->localvar_getRefer(runtime, pos++);
    if (farr->arr_length == barr->arr_length * 4) {
        memcpy(barr->arr_body, farr->arr_body, barr->arr_length);
    }
    env->push_ref(runtime->stack, barr);
    return 0;
}

/* ==============================   jni glfw =================================*/

int org_mini_glfw_Glfw_glfwSetCallbackJni(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    GLFWwindow *window = (__refer) (intptr_t) getParaLong(runtime, pos);
    pos += 2;
    refers.glfw_callback = env->localvar_getRefer(runtime, pos++);

    //this object not refered by jvm , so needs to hold by jni manaul
    if (refers.glfw_callback) env->instance_release_from_thread(refers.glfw_callback, runtime);
    env->instance_hold_to_thread(refers.glfw_callback, runtime);

    glfwSetErrorCallback(_callback_error);
    glfwSetKeyCallback(window, _callback_key);
    glfwSetMouseButtonCallback(window, _button_callback_mouse);
    glfwSetCursorPosCallback(window, _callback_cursor_pos);
    glfwSetCursorEnterCallback(window, _callback_cursor_enter);
    glfwSetWindowCloseCallback(window, _callback_window_close);
    glfwSetWindowSizeCallback(window, _callback_window_size);
    glfwSetWindowPosCallback(window, _callback_window_pos);
    glfwSetWindowFocusCallback(window, _callback_window_focus);
    glfwSetWindowIconifyCallback(window, _callback_window_iconify);
    glfwSetWindowRefreshCallback(window, _callback_window_refresh);
    glfwSetCharCallback(window, _callback_character);
    glfwSetFramebufferSizeCallback(window, _callback_framebuffer_size);

    c8 *name_s, *type_s;
    {
        name_s = "error";
        type_s = "(ILjava/lang/String;)V";
        Utf8String *name = env->utf8_create_part_c(name_s, 0, strlen(name_s));
        Utf8String *type = env->utf8_create_part_c(type_s, 0, strlen(type_s));
        refers._callback_error =
                env->find_methodInfo_by_name(refers.glfw_callback->mb.clazz->name, name, type);
        env->utf8_destory(name);
        env->utf8_destory(type);
    }
    {
        name_s = "key";
        type_s = "(JIIII)V";
        Utf8String *name = env->utf8_create_part_c(name_s, 0, strlen(name_s));
        Utf8String *type = env->utf8_create_part_c(type_s, 0, strlen(type_s));
        refers._callback_key =
                env->find_methodInfo_by_name(refers.glfw_callback->mb.clazz->name, name, type);
        env->utf8_destory(name);
        env->utf8_destory(type);
    }
    {
        name_s = "character";
        type_s = "(JC)V";
        Utf8String *name = env->utf8_create_part_c(name_s, 0, strlen(name_s));
        Utf8String *type = env->utf8_create_part_c(type_s, 0, strlen(type_s));
        refers._callback_key =
                env->find_methodInfo_by_name(refers.glfw_callback->mb.clazz->name, name, type);
        env->utf8_destory(name);
        env->utf8_destory(type);
    }
    {
        name_s = "mouseButton";
        type_s = "(JIZ)V";
        Utf8String *name = env->utf8_create_part_c(name_s, 0, strlen(name_s));
        Utf8String *type = env->utf8_create_part_c(type_s, 0, strlen(type_s));
        refers._button_callback_mouse =
                env->find_methodInfo_by_name(refers.glfw_callback->mb.clazz->name, name, type);
        env->utf8_destory(name);
        env->utf8_destory(type);
    }
    {
        name_s = "cursorPos";
        type_s = "(JII)V";
        Utf8String *name = env->utf8_create_part_c(name_s, 0, strlen(name_s));
        Utf8String *type = env->utf8_create_part_c(type_s, 0, strlen(type_s));
        refers._callback_cursor_pos =
                env->find_methodInfo_by_name(refers.glfw_callback->mb.clazz->name, name, type);
        env->utf8_destory(name);
        env->utf8_destory(type);
    }
    {
        name_s = "cursorEnter";
        type_s = "(JZ)V";
        Utf8String *name = env->utf8_create_part_c(name_s, 0, strlen(name_s));
        Utf8String *type = env->utf8_create_part_c(type_s, 0, strlen(type_s));
        refers._callback_cursor_enter =
                env->find_methodInfo_by_name(refers.glfw_callback->mb.clazz->name, name, type);
        env->utf8_destory(name);
        env->utf8_destory(type);
    }
    {
        name_s = "windowPos";
        type_s = "(JII)V";
        Utf8String *name = env->utf8_create_part_c(name_s, 0, strlen(name_s));
        Utf8String *type = env->utf8_create_part_c(type_s, 0, strlen(type_s));
        refers._callback_window_pos =
                env->find_methodInfo_by_name(refers.glfw_callback->mb.clazz->name, name, type);
        env->utf8_destory(name);
        env->utf8_destory(type);
    }
    {
        name_s = "windowSize";
        type_s = "(JII)V";
        Utf8String *name = env->utf8_create_part_c(name_s, 0, strlen(name_s));
        Utf8String *type = env->utf8_create_part_c(type_s, 0, strlen(type_s));
        refers._callback_window_size =
                env->find_methodInfo_by_name(refers.glfw_callback->mb.clazz->name, name, type);
        env->utf8_destory(name);
        env->utf8_destory(type);
    }
    {
        name_s = "windowClose";
        type_s = "(J)Z";
        Utf8String *name = env->utf8_create_part_c(name_s, 0, strlen(name_s));
        Utf8String *type = env->utf8_create_part_c(type_s, 0, strlen(type_s));
        refers._callback_window_close =
                env->find_methodInfo_by_name(refers.glfw_callback->mb.clazz->name, name, type);
        env->utf8_destory(name);
        env->utf8_destory(type);
    }
    {
        name_s = "windowRefresh";
        type_s = "(J)V";
        Utf8String *name = env->utf8_create_part_c(name_s, 0, strlen(name_s));
        Utf8String *type = env->utf8_create_part_c(type_s, 0, strlen(type_s));
        refers._callback_window_refresh =
                env->find_methodInfo_by_name(refers.glfw_callback->mb.clazz->name, name, type);
        env->utf8_destory(name);
        env->utf8_destory(type);
    }
    {
        name_s = "windowFocus";
        type_s = "(JZ)V";
        Utf8String *name = env->utf8_create_part_c(name_s, 0, strlen(name_s));
        Utf8String *type = env->utf8_create_part_c(type_s, 0, strlen(type_s));
        refers._callback_window_focus =
                env->find_methodInfo_by_name(refers.glfw_callback->mb.clazz->name, name, type);
        env->utf8_destory(name);
        env->utf8_destory(type);
    }
    {
        name_s = "windowIconify";
        type_s = "(JZ)V";
        Utf8String *name = env->utf8_create_part_c(name_s, 0, strlen(name_s));
        Utf8String *type = env->utf8_create_part_c(type_s, 0, strlen(type_s));
        refers._callback_window_iconify =
                env->find_methodInfo_by_name(refers.glfw_callback->mb.clazz->name, name, type);
        env->utf8_destory(name);
        env->utf8_destory(type);
    }
    {
        name_s = "framebufferSize";
        type_s = "(JII)V";
        Utf8String *name = env->utf8_create_part_c(name_s, 0, strlen(name_s));
        Utf8String *type = env->utf8_create_part_c(type_s, 0, strlen(type_s));
        refers._callback_framebuffer_size =
                env->find_methodInfo_by_name(refers.glfw_callback->mb.clazz->name, name, type);
        env->utf8_destory(name);
        env->utf8_destory(type);
    }
    return 0;
}

int org_mini_glfw_Glfw_glfwGetTime(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 list = env->localvar_getInt(runtime, pos++);
    env->push_double(runtime->stack, glfwGetTime());
    return 0;
}

int org_mini_glfw_Glfw_glfwCreateWindowJni(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);
    Instance *title_arr = env->localvar_getRefer(runtime, pos++);
    c8 *title = jbytearr2c8arr(env, title_arr);
    GLFWmonitor *monitor = (__refer) (intptr_t) getParaLong(runtime, pos);
    pos += 2;
    GLFWwindow *share = (__refer) (intptr_t) getParaLong(runtime, pos);
    pos += 2;

    GLFWwindow *window = glfwCreateWindow(width, height, title, monitor, share);
    if (!window) {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
    }
    //
    env->jvm_free(title);
    env->push_long(runtime->stack, (s64) (intptr_t) window);
    return 0;
}

int org_mini_glfw_Glfw_glfwDestroyWindow(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    GLFWwindow *window = (__refer) (intptr_t) getParaLong(runtime, pos);
    glfwDestroyWindow(window);
    return 0;
}

int org_mini_glfw_Glfw_glfwWindowShouldClose(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    GLFWwindow *window = (__refer) (intptr_t) getParaLong(runtime, pos);
    env->push_int(runtime->stack, GL_TRUE == glfwWindowShouldClose((GLFWwindow *) window));
    return 0;
}

int org_mini_glfw_Glfw_glfwInitJni(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    env->push_int(runtime->stack, glfwInit() == GLFW_TRUE);
    return 0;
}

int org_mini_glfw_Glfw_glfwTerminate(Runtime *runtime, Class *clazz) {
    glfwTerminate();
    return 0;
}

int org_mini_glfw_Glfw_glfwWindowHint(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 hint = env->localvar_getInt(runtime, pos++);
    s32 value = env->localvar_getInt(runtime, pos++);
    glfwWindowHint(hint, value);
    return 0;
}

int org_mini_glfw_Glfw_glfwPollEvents(Runtime *runtime, Class *clazz) {
    refers.runtime = runtime;
    glfwPollEvents();
    refers.runtime = NULL;
    return 0;
}

int org_mini_glfw_Glfw_glfwSetWindowShouldClose(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    GLFWwindow *window = (__refer) (intptr_t) getParaLong(runtime, pos);
    pos += 2;
    s32 value = env->localvar_getInt(runtime, pos++);
    glfwSetWindowShouldClose(window, value);
    return 0;
}

int org_mini_glfw_Glfw_glfwMakeContextCurrentJni(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    GLFWwindow *window = (__refer) (intptr_t) getParaLong(runtime, pos);
    pos += 2;
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    return 0;
}

int org_mini_glfw_Glfw_glfwSwapInterval(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 interval = env->localvar_getInt(runtime, pos++);
    glfwSwapInterval(interval);
    return 0;
}

int org_mini_glfw_Glfw_glfwSwapBuffers(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    GLFWwindow *window = (__refer) (intptr_t) getParaLong(runtime, pos);
    pos += 2;
    glfwSwapBuffers(window);
    return 0;
}

int org_mini_glfw_Glfw_glfwGetFramebufferSizeW(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    GLFWwindow *window = (__refer) (intptr_t) getParaLong(runtime, pos);
    s32 w, h;
    glfwGetFramebufferSize(window, &w, &h);
    env->push_int(runtime->stack, w);
    return 0;
}

int org_mini_glfw_Glfw_glfwGetFramebufferSizeH(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    GLFWwindow *window = (__refer) (intptr_t) getParaLong(runtime, pos);
    s32 w, h;
    glfwGetFramebufferSize(window, &w, &h);
    env->push_int(runtime->stack, h);
    return 0;
}

/* ==============================   jni gl =================================*/


int org_mini_gl_GL_init(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    //gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    return 0;
}

int org_mini_gl_GL_glViewport(Runtime *runtime, Class *clazz) {
    //glViewport(GLint x,GLint y,GLsizei width,GLsizei height);
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 w = env->localvar_getInt(runtime, pos++);
    s32 h = env->localvar_getInt(runtime, pos++);
    glViewport(x, y, (GLsizei) w, (GLsizei) h);
    return 0;
}

int org_mini_gl_GL_glVertexAttribPointer(Runtime *runtime, Class *clazz) {
    //glViewport(GLint x,GLint y,GLsizei width,GLsizei height);
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 index = env->localvar_getInt(runtime, pos++);
    s32 size = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    s32 normalized = env->localvar_getInt(runtime, pos++);
    s32 stride = env->localvar_getInt(runtime, pos++);
    __refer pointer = (__refer) (intptr_t) getParaLong(runtime, pos);
    glVertexAttribPointer((GLuint) index, (GLint) size, (GLenum) type, (GLboolean) normalized, (GLsizei) stride,
                          (const GLvoid *) pointer);
    return 0;
}


int org_mini_gl_GL_glMatrixMode(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 mode = env->localvar_getInt(runtime, pos++);
    glMatrixMode((GLenum) mode);
    return 0;
}

int org_mini_gl_GL_glLoadMatrixf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Instance *arr = env->localvar_getRefer(runtime, pos++);
    s32 offset = env->localvar_getInt(runtime, pos++);
    offset *= env->data_type_bytes[arr->mb.arr_type_index];
    glLoadMatrixf((const GLfloat *) (arr + offset));
    return 0;
}

int org_mini_gl_GL_glPushMatrix(Runtime *runtime, Class *clazz) {
    glPushMatrix();
    return 0;
}

int org_mini_gl_GL_glPopMatrix(Runtime *runtime, Class *clazz) {
    glPopMatrix();
    return 0;
}

int org_mini_gl_GL_glEnable(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 cap = env->localvar_getInt(runtime, pos++);
    glEnable((GLenum) cap);
    return 0;
}

int org_mini_gl_GL_glDisable(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 cap = env->localvar_getInt(runtime, pos++);
    glDisable((GLenum) cap);
    return 0;
}

int org_mini_gl_GL_glShadeModel(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 mode = env->localvar_getInt(runtime, pos++);
    glShadeModel((GLenum) mode);
    return 0;
}

int org_mini_gl_GL_glClear(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 mask = env->localvar_getInt(runtime, pos++);
    glClear((GLbitfield) mask);
    return 0;
}

int org_mini_gl_GL_glDrawBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 mode = env->localvar_getInt(runtime, pos++);
    glDrawBuffer((GLenum) mode);
    return 0;
}

int org_mini_gl_GL_glReadBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 mode = env->localvar_getInt(runtime, pos++);
    glReadBuffer((GLenum) mode);
    return 0;
}

int org_mini_gl_GL_glTranslatef(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Int2Float x, y, z;
    x.i = env->localvar_getInt(runtime, pos++);
    y.i = env->localvar_getInt(runtime, pos++);
    z.i = env->localvar_getInt(runtime, pos++);
    glTranslatef((GLfloat) x.f, (GLfloat) y.f, (GLfloat) z.f);
    return 0;
}

int org_mini_gl_GL_glRotatef(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Int2Float a, x, y, z;
    a.i = env->localvar_getInt(runtime, pos++);
    x.i = env->localvar_getInt(runtime, pos++);
    y.i = env->localvar_getInt(runtime, pos++);
    z.i = env->localvar_getInt(runtime, pos++);
    glRotatef((GLfloat) a.f, (GLfloat) x.f, (GLfloat) y.f, (GLfloat) z.f);
    return 0;
}

int org_mini_gl_GL_glRotated(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Long2Double a, x, y, z;
    a.l = env->localvar_getInt(runtime, pos++);
    x.l = env->localvar_getInt(runtime, pos++);
    y.l = env->localvar_getInt(runtime, pos++);
    z.l = env->localvar_getInt(runtime, pos++);
    glRotated((GLdouble) a.d, (GLdouble) x.d, (GLdouble) y.d, (GLdouble) z.d);
    return 0;
}

int org_mini_gl_GL_glScalef(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Int2Float x, y, z;
    x.i = env->localvar_getInt(runtime, pos++);
    y.i = env->localvar_getInt(runtime, pos++);
    z.i = env->localvar_getInt(runtime, pos++);
    glScalef((GLfloat) x.f, (GLfloat) y.f, (GLfloat) z.f);
    return 0;
}

int org_mini_gl_GL_glCullFace(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 mode = env->localvar_getInt(runtime, pos++);
    glCullFace((GLenum) mode);
    return 0;
}

int org_mini_gl_GL_glFrontFace(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 mode = env->localvar_getInt(runtime, pos++);
    glFrontFace((GLenum) mode);
    return 0;
}

int org_mini_gl_GL_glBegin(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 mode = env->localvar_getInt(runtime, pos++);
    glBegin((GLenum) mode);
    return 0;
}

int org_mini_gl_GL_glEnd(Runtime *runtime, Class *clazz) {
    glEnd();
    return 0;
}

int org_mini_gl_GL_glFlush(Runtime *runtime, Class *clazz) {
    glFlush();
    return 0;
}

int org_mini_gl_GL_glFinish(Runtime *runtime, Class *clazz) {
    glFinish();
    return 0;
}

int org_mini_gl_GL_glGetError(Runtime *runtime, Class *clazz) {
    glGetError();
    return 0;
}

int org_mini_gl_GL_glVertex2s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    int x, y;
    x = env->localvar_getInt(runtime, pos++);
    y = env->localvar_getInt(runtime, pos++);
    glVertex2s((GLshort) x, (GLshort) y);
    return 0;
}

int org_mini_gl_GL_glVertex3s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    int x, y, z, w;
    x = env->localvar_getInt(runtime, pos++);
    y = env->localvar_getInt(runtime, pos++);
    z = env->localvar_getInt(runtime, pos++);
    glVertex3s((GLshort) x, (GLshort) y, (GLshort) z);
    return 0;
}

int org_mini_gl_GL_glVertex4s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    int x, y, z, w;
    x = env->localvar_getInt(runtime, pos++);
    y = env->localvar_getInt(runtime, pos++);
    z = env->localvar_getInt(runtime, pos++);
    w = env->localvar_getInt(runtime, pos++);
    glVertex4s((GLshort) x, (GLshort) y, (GLshort) z, (GLshort) w);
    return 0;
}

int org_mini_gl_GL_glVertex2i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    int x, y;
    x = env->localvar_getInt(runtime, pos++);
    y = env->localvar_getInt(runtime, pos++);
    glVertex2i((GLint) x, (GLint) y);
    return 0;
}

int org_mini_gl_GL_glVertex3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    int x, y, z;
    x = env->localvar_getInt(runtime, pos++);
    y = env->localvar_getInt(runtime, pos++);
    z = env->localvar_getInt(runtime, pos++);
    glVertex3i((GLint) x, (GLint) y, (GLint) z);
    return 0;
}

int org_mini_gl_GL_glVertex4i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    int x, y, z, w;
    x = env->localvar_getInt(runtime, pos++);
    y = env->localvar_getInt(runtime, pos++);
    z = env->localvar_getInt(runtime, pos++);
    w = env->localvar_getInt(runtime, pos++);
    glVertex4i((GLint) x, (GLint) y, (GLint) z, (GLint) w);
    return 0;
}

int org_mini_gl_GL_glVertex2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Int2Float x, y;
    x.i = env->localvar_getInt(runtime, pos++);
    y.i = env->localvar_getInt(runtime, pos++);
    glVertex2f((GLfloat) x.f, (GLfloat) y.f);
    return 0;
}

int org_mini_gl_GL_glColor3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Int2Float x, y, z;
    x.i = env->localvar_getInt(runtime, pos++);
    y.i = env->localvar_getInt(runtime, pos++);
    z.i = env->localvar_getInt(runtime, pos++);
    glColor3f((GLfloat) x.f, (GLfloat) y.f, (GLfloat) z.f);
    return 0;
}

int org_mini_gl_GL_glVertex3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Int2Float x, y, z;
    x.i = env->localvar_getInt(runtime, pos++);
    y.i = env->localvar_getInt(runtime, pos++);
    z.i = env->localvar_getInt(runtime, pos++);
    glVertex3f((GLfloat) x.f, (GLfloat) y.f, (GLfloat) z.f);
    return 0;
}

int org_mini_gl_GL_glVertex4f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Int2Float x, y, z, w;
    x.i = env->localvar_getInt(runtime, pos++);
    y.i = env->localvar_getInt(runtime, pos++);
    z.i = env->localvar_getInt(runtime, pos++);
    w.i = env->localvar_getInt(runtime, pos++);
    glVertex4f((GLfloat) x.f, (GLfloat) y.f, (GLfloat) z.f, (GLfloat) w.f);
    return 0;
}

int org_mini_gl_GL_glVertex2d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Long2Double x, y, z;
    x.l = env->localvar_getInt(runtime, pos++);
    y.l = env->localvar_getInt(runtime, pos++);
    glVertex2d((GLdouble) x.d, (GLdouble) y.d);
    return 0;
}

int org_mini_gl_GL_glVertex3d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Long2Double x, y, z;
    x.l = env->localvar_getInt(runtime, pos++);
    y.l = env->localvar_getInt(runtime, pos++);
    z.l = env->localvar_getInt(runtime, pos++);
    glVertex3d((GLdouble) x.d, (GLdouble) y.d, (GLdouble) z.d);
    return 0;
}

int org_mini_gl_GL_glVertex4d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Long2Double x, y, z, w;
    x.l = env->localvar_getInt(runtime, pos++);
    y.l = env->localvar_getInt(runtime, pos++);
    z.l = env->localvar_getInt(runtime, pos++);
    w.l = env->localvar_getInt(runtime, pos++);
    glVertex4d((GLdouble) x.d, (GLdouble) y.d, (GLdouble) z.d, (GLdouble) w.d);
    return 0;
}

int org_mini_gl_GL_glNormal3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Int2Float x, y, z;
    x.i = env->localvar_getInt(runtime, pos++);
    y.i = env->localvar_getInt(runtime, pos++);
    z.i = env->localvar_getInt(runtime, pos++);
    glNormal3f((GLfloat) x.f, (GLfloat) y.f, (GLfloat) z.f);
    return 0;
}

int org_mini_gl_GL_glClearColor(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Int2Float x, y, z, a;
    x.i = env->localvar_getInt(runtime, pos++);
    y.i = env->localvar_getInt(runtime, pos++);
    z.i = env->localvar_getInt(runtime, pos++);
    a.i = env->localvar_getInt(runtime, pos++);
    glClearColor((GLfloat) x.f, (GLfloat) y.f, (GLfloat) z.f, (GLfloat) a.f);
    return 0;
}

int org_mini_gl_GL_glRectf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Int2Float x1, y1, x2, y2;
    x1.i = env->localvar_getInt(runtime, pos++);
    y1.i = env->localvar_getInt(runtime, pos++);
    x2.i = env->localvar_getInt(runtime, pos++);
    y2.i = env->localvar_getInt(runtime, pos++);
    glRectf((GLfloat) x1.f, (GLfloat) y1.f, (GLfloat) x2.f, (GLfloat) y2.f);
    return 0;
}

int org_mini_gl_GL_glLightfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 light = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *arr = env->localvar_getRefer(runtime, pos++);
    s32 offset = env->localvar_getInt(runtime, pos++);
    offset *= env->data_type_bytes[arr->mb.arr_type_index];
    glLightfv((GLenum) light, (GLenum) pname, (const GLfloat *) (arr->arr_body + offset));
    return 0;
}

int org_mini_gl_GL_glMaterialfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 light = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *arr = env->localvar_getRefer(runtime, pos++);
    s32 offset = env->localvar_getInt(runtime, pos++);
    offset *= env->data_type_bytes[arr->mb.arr_type_index];
    glMaterialfv((GLenum) light, (GLenum) pname, (const GLfloat *) (arr->arr_body + offset));
    return 0;
}

int org_mini_gl_GL_glNewList(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 list = env->localvar_getInt(runtime, pos++);
    s32 mode = env->localvar_getInt(runtime, pos++);
    glNewList((GLuint) list, (GLenum) mode);
    return 0;
}

int org_mini_gl_GL_glGenLists(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 range = env->localvar_getInt(runtime, pos++);
    env->push_int(runtime->stack, glGenLists((GLsizei) range));
    return 0;
}

int org_mini_gl_GL_glEndList(Runtime *runtime, Class *clazz) {
    glEndList();
    return 0;
}

int org_mini_gl_GL_glCallList(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 list = env->localvar_getInt(runtime, pos++);
    glCallList((GLuint) list);
    return 0;
}

int org_mini_gl_GL_glDrawArrays(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 mode = env->localvar_getInt(runtime, pos++);
    s32 first = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    glDrawArrays((GLenum) mode, (GLint) first, (GLsizei) count);
    return 0;
}

int org_mini_gl_GL_glBindVertexArray(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 array = env->localvar_getInt(runtime, pos++);
    glBindVertexArray((GLuint) array);
    return 0;
}

int org_mini_gl_GL_glEnableVertexAttribArray(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 index = env->localvar_getInt(runtime, pos++);
    glEnableVertexAttribArray((GLuint) index);
    return 0;
}

int org_mini_gl_GL_glLinkProgram(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 program = env->localvar_getInt(runtime, pos++);
    glLinkProgram((GLuint) program);
    return 0;
}

int org_mini_gl_GL_glCreateShader(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 type = env->localvar_getInt(runtime, pos++);
    env->push_int(runtime->stack, glCreateShader((GLenum) type));
    return 0;
}

int org_mini_gl_GL_glCreateProgram(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    env->push_int(runtime->stack, glCreateProgram());
    return 0;
}

int org_mini_gl_GL_glCompileShader(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 shader = env->localvar_getInt(runtime, pos++);
    glCompileShader((GLuint) shader);
    return 0;
}

int org_mini_gl_GL_glShaderSource(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 shader = env->localvar_getInt(runtime, pos++);
    Instance *barr = env->localvar_getRefer(runtime, pos++);
    glShaderSource((GLuint) shader, 1, (const GLchar *const *) &barr->arr_body, NULL);
    return 0;
}

int org_mini_gl_GL_glAttachShader(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 program = env->localvar_getInt(runtime, pos++);
    s32 shader = env->localvar_getInt(runtime, pos++);
    glAttachShader((GLuint) program, (GLuint) shader);
    return 0;
}

int org_mini_gl_GL_glLoadIdentity(Runtime *runtime, Class *clazz) {
    glLoadIdentity();
    return 0;
}

int org_mini_gl_GL_glFrustum(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Long2Double left, right, bottom, top, near_val, far_val;
    left.l = getParaLong(runtime, pos);
    pos += 2;
    right.l = getParaLong(runtime, pos);
    pos += 2;
    bottom.l = getParaLong(runtime, pos);
    pos += 2;
    top.l = getParaLong(runtime, pos);
    pos += 2;
    near_val.l = getParaLong(runtime, pos);
    pos += 2;
    far_val.l = getParaLong(runtime, pos);
    pos += 2;
    glFrustum((GLdouble) left.d, (GLdouble) right.d, (GLdouble) bottom.d, (GLdouble) top.d, (GLdouble) near_val.d,
              (GLdouble) far_val.d);
    return 0;
}

int org_mini_gl_GL_glBufferData(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 target = env->localvar_getInt(runtime, pos++);
    s64 size = getParaLong(runtime, pos);
    pos += 2;
    Instance *arr = env->localvar_getRefer(runtime, pos++);
    s32 offset = env->localvar_getInt(runtime, pos++);
    s32 usage = env->localvar_getInt(runtime, pos++);
    offset *= env->data_type_bytes[arr->mb.arr_type_index];
    glBufferData((GLenum) target, (GLsizeiptr) size, (const GLvoid *) (arr->arr_body + offset), (GLenum) usage);
    return 0;
}

int org_mini_gl_GL_glGenBuffers(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 n = env->localvar_getInt(runtime, pos++);
    Instance *arr = env->localvar_getRefer(runtime, pos++);
    s32 offset = env->localvar_getInt(runtime, pos++);
    offset *= env->data_type_bytes[arr->mb.arr_type_index];
    glGenBuffers((GLsizei) n, (GLuint *) (arr->arr_body + offset));
    return 0;
}

int org_mini_gl_GL_glGenVertexArrays(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 n = env->localvar_getInt(runtime, pos++);
    Instance *arr = env->localvar_getRefer(runtime, pos++);
    s32 offset = env->localvar_getInt(runtime, pos++);
    offset *= env->data_type_bytes[arr->mb.arr_type_index];
    glGenVertexArrays((GLsizei) n, (GLuint *) (arr->arr_body + offset));
    return 0;
}

int org_mini_gl_GL_glBindBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 target = env->localvar_getInt(runtime, pos++);
    s32 buffer = env->localvar_getInt(runtime, pos++);
    glBindBuffer((GLenum) target, (GLuint) buffer);
    return 0;
}

int org_mini_gl_GL_glPointSize(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Int2Float i2f;
    i2f.i = env->localvar_getInt(runtime, pos++);
    glPointSize((GLfloat) i2f.f);
    return 0;
}

int org_mini_gl_GL_glLineWidth(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Int2Float i2f;
    i2f.i = env->localvar_getInt(runtime, pos++);
    glLineWidth((GLfloat) i2f.f);
    return 0;
}


int org_mini_gl_GL_glPolygonStipple(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Instance *arr = env->localvar_getRefer(runtime, pos++);
    s32 offset = env->localvar_getInt(runtime, pos++);
    offset *= env->data_type_bytes[arr->mb.arr_type_index];
    glPolygonStipple((const GLubyte *) (arr->arr_body + offset));
    return 0;
}

static java_native_method method_test2_table[] = {
        {"org/mini/glfw/utils/GlUtils", "f2b",                       "([F[B)[B",                         org_mini_glfw_utils_GlUtils_f2b},
        {"org/mini/glfw/Glfw",          "glfwGetTime",               "()D",                              org_mini_glfw_Glfw_glfwGetTime},
        {"org/mini/glfw/Glfw",          "glfwCreateWindowJni",       "(II[BJJ)J",                        org_mini_glfw_Glfw_glfwCreateWindowJni},
        {"org/mini/glfw/Glfw",          "glfwDestroyWindow",         "(J)V",                             org_mini_glfw_Glfw_glfwDestroyWindow},
        {"org/mini/glfw/Glfw",          "glfwWindowShouldClose",     "(J)Z",                             org_mini_glfw_Glfw_glfwWindowShouldClose},
        {"org/mini/glfw/Glfw",          "glfwSetCallbackJni",        "(JLorg/mini/glfw/GlfwCallback;)V", org_mini_glfw_Glfw_glfwSetCallbackJni},
        {"org/mini/glfw/Glfw",          "glfwInitJni",               "()Z",                              org_mini_glfw_Glfw_glfwInitJni},
        {"org/mini/glfw/Glfw",          "glfwTerminate",             "()V",                              org_mini_glfw_Glfw_glfwTerminate},
        {"org/mini/glfw/Glfw",          "glfwWindowHint",            "(II)V",                            org_mini_glfw_Glfw_glfwWindowHint},
        {"org/mini/glfw/Glfw",          "glfwPollEvents",            "()V",                              org_mini_glfw_Glfw_glfwPollEvents},
        {"org/mini/glfw/Glfw",          "glfwSetWindowShouldClose",  "(JI)V",                            org_mini_glfw_Glfw_glfwSetWindowShouldClose},
        {"org/mini/glfw/Glfw",          "glfwMakeContextCurrentJni", "(J)V",                             org_mini_glfw_Glfw_glfwMakeContextCurrentJni},
        {"org/mini/glfw/Glfw",          "glfwSwapInterval",          "(I)V",                             org_mini_glfw_Glfw_glfwSwapInterval},
        {"org/mini/glfw/Glfw",          "glfwSwapBuffers",           "(J)V",                             org_mini_glfw_Glfw_glfwSwapBuffers},
        {"org/mini/glfw/Glfw",          "glfwGetFramebufferSizeW",   "(J)I",                             org_mini_glfw_Glfw_glfwGetFramebufferSizeW},
        {"org/mini/glfw/Glfw",          "glfwGetFramebufferSizeH",   "(J)I",                             org_mini_glfw_Glfw_glfwGetFramebufferSizeH},
        {"org/mini/gl/GL",              "init",                      "()V",                              org_mini_gl_GL_init},
        {"org/mini/gl/GL",              "glViewport",                "(IIII)V",                          org_mini_gl_GL_glViewport},
        {"org/mini/gl/GL",              "glVertexAttribPointer",     "(IIIIIJ)V",                        org_mini_gl_GL_glVertexAttribPointer},
        {"org/mini/gl/GL",              "glMatrixMode",              "(I)V",                             org_mini_gl_GL_glMatrixMode},
        {"org/mini/gl/GL",              "glLoadMatrixf",             "(I)V",                             org_mini_gl_GL_glLoadMatrixf},
        {"org/mini/gl/GL",              "glPushMatrix",              "()V",                              org_mini_gl_GL_glPushMatrix},
        {"org/mini/gl/GL",              "glPopMatrix",               "()V",                              org_mini_gl_GL_glPopMatrix},
        {"org/mini/gl/GL",              "glEnable",                  "(I)V",                             org_mini_gl_GL_glEnable},
        {"org/mini/gl/GL",              "glDisable",                 "(I)V",                             org_mini_gl_GL_glDisable},
        {"org/mini/gl/GL",              "glShadeModel",              "(I)V",                             org_mini_gl_GL_glShadeModel},
        {"org/mini/gl/GL",              "glClear",                   "(I)V",                             org_mini_gl_GL_glClear},
        {"org/mini/gl/GL",              "glDrawBuffer",              "(I)V",                             org_mini_gl_GL_glDrawBuffer},
        {"org/mini/gl/GL",              "glReadBuffer",              "(I)V",                             org_mini_gl_GL_glReadBuffer},
        {"org/mini/gl/GL",              "glTranslatef",              "(FFF)V",                           org_mini_gl_GL_glTranslatef},
        {"org/mini/gl/GL",              "glScalef",                  "(FFF)V",                           org_mini_gl_GL_glScalef},
        {"org/mini/gl/GL",              "glRotatef",                 "(FFFF)V",                          org_mini_gl_GL_glRotatef},
        {"org/mini/gl/GL",              "glRotated",                 "(DDDD)V",                          org_mini_gl_GL_glRotated},
        {"org/mini/gl/GL",              "glCullFace",                "(I)V",                             org_mini_gl_GL_glCullFace},
        {"org/mini/gl/GL",              "glFrontFace",               "(I)V",                             org_mini_gl_GL_glFrontFace},
        {"org/mini/gl/GL",              "glBegin",                   "(I)V",                             org_mini_gl_GL_glBegin},
        {"org/mini/gl/GL",              "glEnd",                     "()V",                              org_mini_gl_GL_glEnd},
        {"org/mini/gl/GL",              "glFlush",                   "()V",                              org_mini_gl_GL_glFlush},
        {"org/mini/gl/GL",              "glFinish",                  "()V",                              org_mini_gl_GL_glFinish},
        {"org/mini/gl/GL",              "glGetError",                "()V",                              org_mini_gl_GL_glGetError},
        {"org/mini/gl/GL",              "glColor3f",                 "(FFF)V",                           org_mini_gl_GL_glColor3f},
        {"org/mini/gl/GL",              "glVertex2s",                "(SS)V",                            org_mini_gl_GL_glVertex2s},
        {"org/mini/gl/GL",              "glVertex3s",                "(SSS)V",                           org_mini_gl_GL_glVertex3s},
        {"org/mini/gl/GL",              "glVertex4s",                "(SSSS)V",                          org_mini_gl_GL_glVertex4s},
        {"org/mini/gl/GL",              "glVertex2i",                "(II)V",                            org_mini_gl_GL_glVertex2i},
        {"org/mini/gl/GL",              "glVertex3i",                "(III)V",                           org_mini_gl_GL_glVertex3i},
        {"org/mini/gl/GL",              "glVertex4i",                "(IIII)V",                          org_mini_gl_GL_glVertex4i},
        {"org/mini/gl/GL",              "glVertex2f",                "(FF)V",                            org_mini_gl_GL_glVertex2f},
        {"org/mini/gl/GL",              "glVertex3f",                "(FFF)V",                           org_mini_gl_GL_glVertex3f},
        {"org/mini/gl/GL",              "glVertex4f",                "(FFFF)V",                          org_mini_gl_GL_glVertex4f},
        {"org/mini/gl/GL",              "glVertex2d",                "(DD)V",                            org_mini_gl_GL_glVertex2d},
        {"org/mini/gl/GL",              "glVertex3d",                "(DDD)V",                           org_mini_gl_GL_glVertex3d},
        {"org/mini/gl/GL",              "glVertex4d",                "(DDDD)V",                          org_mini_gl_GL_glVertex4d},
        {"org/mini/gl/GL",              "glNormal3f",                "(FFF)V",                           org_mini_gl_GL_glNormal3f},
        {"org/mini/gl/GL",              "glClearColor",              "(FFFF)V",                          org_mini_gl_GL_glClearColor},
        {"org/mini/gl/GL",              "glRectf",                   "(FFFF)V",                          org_mini_gl_GL_glRectf},
        {"org/mini/gl/GL",              "glLightfv",                 "(IILjava/lang/Object;I)V",         org_mini_gl_GL_glLightfv},
        {"org/mini/gl/GL",              "glMaterialfv",              "(IILjava/lang/Object;I)V",         org_mini_gl_GL_glMaterialfv},
        {"org/mini/gl/GL",              "glNewList",                 "(II)V",                            org_mini_gl_GL_glNewList},
        {"org/mini/gl/GL",              "glGenLists",                "(I)I",                             org_mini_gl_GL_glGenLists},
        {"org/mini/gl/GL",              "glEndList",                 "()V",                              org_mini_gl_GL_glEndList},
        {"org/mini/gl/GL",              "glCallList",                "(I)V",                             org_mini_gl_GL_glCallList},
        {"org/mini/gl/GL",              "glDrawArrays",              "(III)V",                           org_mini_gl_GL_glDrawArrays},
        {"org/mini/gl/GL",              "glGenVertexArrays",         "(ILjava/lang/Object;I)V",          org_mini_gl_GL_glGenVertexArrays},
        {"org/mini/gl/GL",              "glBindVertexArray",         "(I)V",                             org_mini_gl_GL_glBindVertexArray},
        {"org/mini/gl/GL",              "glEnableVertexAttribArray", "(I)V",                             org_mini_gl_GL_glEnableVertexAttribArray},
        {"org/mini/gl/GL",              "glLoadIdentity",            "()V",                              org_mini_gl_GL_glLoadIdentity},
        {"org/mini/gl/GL",              "glFrustum",                 "(DDDDDD)V",                        org_mini_gl_GL_glFrustum},
        {"org/mini/gl/GL",              "glShaderSource",            "(I[B)V",                           org_mini_gl_GL_glShaderSource},
        {"org/mini/gl/GL",              "glCompileShader",           "(I)V",                             org_mini_gl_GL_glCompileShader},
        {"org/mini/gl/GL",              "glAttachShader",            "(II)V",                            org_mini_gl_GL_glAttachShader},
        {"org/mini/gl/GL",              "glLinkProgram",             "(I)V",                             org_mini_gl_GL_glLinkProgram},
        {"org/mini/gl/GL",              "glCreateShader",            "(I)I",                             org_mini_gl_GL_glCreateShader},
        {"org/mini/gl/GL",              "glCreateProgram",           "()I",                              org_mini_gl_GL_glCreateProgram},
        {"org/mini/gl/GL",              "glBufferData",              "(IJLjava/lang/Object;II)V",        org_mini_gl_GL_glBufferData},
        {"org/mini/gl/GL",              "glGenBuffers",              "(ILjava/lang/Object;I)V",          org_mini_gl_GL_glGenBuffers},
        {"org/mini/gl/GL",              "glBindBuffer",              "(II)V",                            org_mini_gl_GL_glBindBuffer},
        {"org/mini/gl/GL",              "glPointSize",               "(F)V",                             org_mini_gl_GL_glPointSize},
        {"org/mini/gl/GL",              "glLineWidth",               "(F)V",                             org_mini_gl_GL_glLineWidth},
        {"org/mini/gl/GL",              "glPolygonStipple",          "(Ljava/lang/Object;I)V",           org_mini_gl_GL_glPolygonStipple},
};

void JNI_OnLoad(JniEnv *env) {
    memset(&refers, 0, sizeof(GlobeRefer));
    refers.env = env;
    env->native_reg_lib(&(method_test2_table[0]), sizeof(method_test2_table) / sizeof(java_native_method));
}

void JNI_OnUnload(JniEnv *env) {
    env->native_remove_lib((__refer) &method_test2_table[0]);
}


/* ===============================================================
 *
 *                          DEMO
 *
 * ===============================================================*/

int main(void) {

    return 0;
}

