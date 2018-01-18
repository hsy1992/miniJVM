/* nuklear - 1.32.0 - public domain */
#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <string.h>
#include "deps/include/glad/glad.h"
#include "deps/include/GLFW/glfw3.h"
#include "deps/include/linmath.h"

#include "../mini_jvm/jvm/jvm.h"


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

Instance *createJavaString(Runtime *runtime, c8 *cstr) {
    JniEnv *env = runtime->jnienv;
    Utf8String *ustr = env->utf8_create_part_c(cstr, 0, strlen(cstr));
    Instance *jstr = env->jstring_create(ustr, runtime);
    env->utf8_destory(ustr);
    return jstr;
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

static void _callback_drop(GLFWwindow *window, s32 count, const c8 **cstrs) {
    if (refers._callback_drop) {
        JniEnv *env = refers.env;
        env->push_ref(refers.runtime->stack, refers.glfw_callback);
        env->push_long(refers.runtime->stack, (s64) (intptr_t) window);
        env->push_int(refers.runtime->stack, count);
        Utf8String *cls = env->utf8_create_part_c(STR_CLASS_JAVA_LANG_STRING, 0, strlen(STR_CLASS_JAVA_LANG_STRING));
        Instance *jstrs = env->jarray_create(count, 0, cls);
        env->utf8_destory(cls);
        s32 i;
        Long2Double l2d;
        for (i = 0; i < count; i++) {
            l2d.r = createJavaString(refers.runtime, (c8 *) cstrs[i]);
            env->jarray_set_field(jstrs, i, &l2d);
        }
        env->push_ref(refers.runtime->stack, jstrs);
        env->execute_method(refers._callback_drop, refers.runtime, refers.glfw_callback->mb.clazz);
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

int org_mini_glfw_utils_Gutil_f2b(Runtime *runtime, Class *clazz) {
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

void vec_add(Instance *ra, Instance *aa, Instance *ba) {
    GLfloat *r = (GLfloat *) ra->arr_body;
    GLfloat *a = (GLfloat *) aa->arr_body;
    GLfloat *b = (GLfloat *) ba->arr_body;
    int i;
    for (i = 0; i < ra->arr_length; ++i)
        r[i] = a[i] + b[i];
}

int org_mini_glfw_utils_Gutil_vec_add(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *ra = env->localvar_getRefer(runtime, pos++);
    Instance *aa = env->localvar_getRefer(runtime, pos++);
    Instance *ba = env->localvar_getRefer(runtime, pos++);
    vec_add(ra, aa, ba);
    env->push_ref(runtime->stack, ra);
    return 0;
}

void vec_sub(Instance *ra, Instance *aa, Instance *ba) {
    GLfloat *r = (GLfloat *) ra->arr_body;
    GLfloat *a = (GLfloat *) aa->arr_body;
    GLfloat *b = (GLfloat *) ba->arr_body;
    int i;
    for (i = 0; i < ra->arr_length; ++i)
        r[i] = a[i] - b[i];
}

int org_mini_glfw_utils_Gutil_vec_sub(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *ra = env->localvar_getRefer(runtime, pos++);
    Instance *aa = env->localvar_getRefer(runtime, pos++);
    Instance *ba = env->localvar_getRefer(runtime, pos++);
    vec_sub(ra, aa, ba);
    env->push_ref(runtime->stack, ra);
    return 0;
}

float vec_mul_inner(Instance *aa, Instance *ba) {
    int i;
    float r;
    for (i = 0; i < aa->arr_length; ++i)
        r += aa->arr_body[i] * ba->arr_body[i];
    return r;
}

int org_mini_glfw_utils_Gutil_vec_mul_inner(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *aa = env->localvar_getRefer(runtime, pos++);
    Instance *ba = env->localvar_getRefer(runtime, pos++);
    float r = vec_mul_inner(aa, ba);
    env->push_float(runtime->stack, r);
    return 0;
}

void vec_scale(Instance *ra, Instance *aa, float f) {
    GLfloat *r = (GLfloat *) ra->arr_body;
    GLfloat *a = (GLfloat *) aa->arr_body;
    int i;
    for (i = 0; i < ra->arr_length; ++i)
        r[i] = a[i] * f;
}

int org_mini_glfw_utils_Gutil_vec_scale(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *ra = env->localvar_getRefer(runtime, pos++);
    Instance *aa = env->localvar_getRefer(runtime, pos++);
    Int2Float f;
    f.i = env->localvar_getInt(runtime, pos++);
    vec_scale(ra, aa, f.f);
    env->push_ref(runtime->stack, ra);
    return 0;
}

float vec_len(Instance *ra) {
    return (float) sqrt(vec_mul_inner(ra, ra));
}

int org_mini_glfw_utils_Gutil_vec_len(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *ra = env->localvar_getRefer(runtime, pos++);
    float f = vec_len(ra);
    env->push_float(runtime->stack, f);
    return 0;
}

int org_mini_glfw_utils_Gutil_vec_normal(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *ra = env->localvar_getRefer(runtime, pos++);
    Instance *aa = env->localvar_getRefer(runtime, pos++);
    float k = 1.f / vec_len(aa);
    vec_scale(ra, aa, k);
    env->push_ref(runtime->stack, ra);
    return 0;
}

int org_mini_glfw_utils_Gutil_vec_reflect(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *ra = env->localvar_getRefer(runtime, pos++);
    Instance *aa = env->localvar_getRefer(runtime, pos++);
    Instance *ba = env->localvar_getRefer(runtime, pos++);
    GLfloat *r = (GLfloat *) ra->arr_body;
    GLfloat *a = (GLfloat *) aa->arr_body;
    GLfloat *b = (GLfloat *) ba->arr_body;
    float p = 2.f * vec_mul_inner(aa, ba);
    int i;
    for (i = 0; i < 4; ++i)
        r[i] = a[i] - p * b[i];
    env->push_ref(runtime->stack, ra);
    return 0;
}

int org_mini_glfw_utils_Gutil_vec_mul_cross(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *ra = env->localvar_getRefer(runtime, pos++);
    Instance *aa = env->localvar_getRefer(runtime, pos++);
    Instance *ba = env->localvar_getRefer(runtime, pos++);
    GLfloat *r = (GLfloat *) ra->arr_body;
    GLfloat *a = (GLfloat *) aa->arr_body;
    GLfloat *b = (GLfloat *) ba->arr_body;
    r[0] = a[1] * b[2] - a[2] * b[1];
    r[1] = a[2] * b[0] - a[0] * b[2];
    r[2] = a[0] * b[1] - a[1] * b[0];
    if (ra->arr_length > 3)r[3] = 1.f;
    env->push_ref(runtime->stack, ra);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_identity(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    mat4x4_identity((vec4 *) r->arr_body);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_dup(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Instance *m1 = env->localvar_getRefer(runtime, pos++);
    mat4x4_dup((vec4 *) r->arr_body, (vec4 *) m1->arr_body);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_row(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Instance *m1 = env->localvar_getRefer(runtime, pos++);
    int row = env->localvar_getInt(runtime, pos++);
    mat4x4_row((GLfloat *) r->arr_body, (vec4 *) m1->arr_body, row);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_col(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Instance *m1 = env->localvar_getRefer(runtime, pos++);
    int col = env->localvar_getInt(runtime, pos++);
    mat4x4_col((GLfloat *) r->arr_body, (vec4 *) m1->arr_body, col);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_transpose(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Instance *m1 = env->localvar_getRefer(runtime, pos++);
    mat4x4_transpose((vec4 *) r->arr_body, (vec4 *) m1->arr_body);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_add(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Instance *m1 = env->localvar_getRefer(runtime, pos++);
    Instance *m2 = env->localvar_getRefer(runtime, pos++);
    mat4x4_add((vec4 *) r->arr_body, (vec4 *) m1->arr_body, (vec4 *) m2->arr_body);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_sub(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Instance *m1 = env->localvar_getRefer(runtime, pos++);
    Instance *m2 = env->localvar_getRefer(runtime, pos++);
    mat4x4_sub((vec4 *) r->arr_body, (vec4 *) m1->arr_body, (vec4 *) m2->arr_body);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_mul(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Instance *m1 = env->localvar_getRefer(runtime, pos++);
    Instance *m2 = env->localvar_getRefer(runtime, pos++);
    mat4x4_mul((vec4 *) r->arr_body, (vec4 *) m1->arr_body, (vec4 *) m2->arr_body);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_mul_vec4(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Instance *m1 = env->localvar_getRefer(runtime, pos++);
    Instance *m2 = env->localvar_getRefer(runtime, pos++);
    mat4x4_mul_vec4((GLfloat *) r->arr_body, (vec4 *) m1->arr_body, (GLfloat *) m2->arr_body);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_from_vec3_mul_outer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Instance *m1 = env->localvar_getRefer(runtime, pos++);
    Instance *m2 = env->localvar_getRefer(runtime, pos++);
    mat4x4_from_vec3_mul_outer((vec4 *) r->arr_body, (GLfloat *) m1->arr_body, (GLfloat *) m2->arr_body);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_translate(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Int2Float x, y, z;
    x.i = env->localvar_getInt(runtime, pos++);
    y.i = env->localvar_getInt(runtime, pos++);
    z.i = env->localvar_getInt(runtime, pos++);
    mat4x4_translate((vec4 *) r->arr_body, x.f, y.f, z.f);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_translate_in_place(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Int2Float x, y, z;
    x.i = env->localvar_getInt(runtime, pos++);
    y.i = env->localvar_getInt(runtime, pos++);
    z.i = env->localvar_getInt(runtime, pos++);
    mat4x4_translate_in_place((vec4 *) r->arr_body, x.f, y.f, z.f);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_scale(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Instance *m1 = env->localvar_getRefer(runtime, pos++);
    Int2Float f;
    f.i = env->localvar_getInt(runtime, pos++);
    mat4x4_scale((vec4 *) r->arr_body, (vec4 *) m1->arr_body, f.f);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_scale_aniso(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Instance *m1 = env->localvar_getRefer(runtime, pos++);
    Int2Float x, y, z;
    x.i = env->localvar_getInt(runtime, pos++);
    y.i = env->localvar_getInt(runtime, pos++);
    z.i = env->localvar_getInt(runtime, pos++);
    mat4x4_scale_aniso((vec4 *) r->arr_body, (vec4 *) m1->arr_body, x.f, y.f, z.f);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_rotate(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Instance *m1 = env->localvar_getRefer(runtime, pos++);
    Int2Float x, y, z, a;
    x.i = env->localvar_getInt(runtime, pos++);
    y.i = env->localvar_getInt(runtime, pos++);
    z.i = env->localvar_getInt(runtime, pos++);
    a.i = env->localvar_getInt(runtime, pos++);
    mat4x4_rotate((vec4 *) r->arr_body, (vec4 *) m1->arr_body, x.f, y.f, z.f, a.f);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_rotateX(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Instance *m1 = env->localvar_getRefer(runtime, pos++);
    Int2Float f;
    f.i = env->localvar_getInt(runtime, pos++);
    mat4x4_rotate_X((vec4 *) r->arr_body, (vec4 *) m1->arr_body, f.f);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_rotateY(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Instance *m1 = env->localvar_getRefer(runtime, pos++);
    Int2Float f;
    f.i = env->localvar_getInt(runtime, pos++);
    mat4x4_rotate_Y((vec4 *) r->arr_body, (vec4 *) m1->arr_body, f.f);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_rotateZ(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Instance *m1 = env->localvar_getRefer(runtime, pos++);
    Int2Float f;
    f.i = env->localvar_getInt(runtime, pos++);
    mat4x4_rotate_Z((vec4 *) r->arr_body, (vec4 *) m1->arr_body, f.f);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_invert(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Instance *m1 = env->localvar_getRefer(runtime, pos++);
    mat4x4_invert((vec4 *) r->arr_body, (vec4 *) m1->arr_body);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_orthonormalize(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Instance *m1 = env->localvar_getRefer(runtime, pos++);
    mat4x4_orthonormalize((vec4 *) r->arr_body, (vec4 *) m1->arr_body);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_ortho(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *ra = env->localvar_getRefer(runtime, pos++);
    Int2Float l, r, b, t, n, f;
    l.i = env->localvar_getInt(runtime, pos++);
    r.i = env->localvar_getInt(runtime, pos++);
    b.i = env->localvar_getInt(runtime, pos++);
    t.i = env->localvar_getInt(runtime, pos++);
    n.i = env->localvar_getInt(runtime, pos++);
    f.i = env->localvar_getInt(runtime, pos++);
    mat4x4_ortho((vec4 *) ra->arr_body, l.f, r.f, b.f, t.f, n.f, f.f);
    env->push_ref(runtime->stack, ra);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_frustum(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *ra = env->localvar_getRefer(runtime, pos++);
    Int2Float l, r, b, t, n, f;
    l.i = env->localvar_getInt(runtime, pos++);
    r.i = env->localvar_getInt(runtime, pos++);
    b.i = env->localvar_getInt(runtime, pos++);
    t.i = env->localvar_getInt(runtime, pos++);
    n.i = env->localvar_getInt(runtime, pos++);
    f.i = env->localvar_getInt(runtime, pos++);
    mat4x4_frustum((vec4 *) ra->arr_body, l.f, r.f, b.f, t.f, n.f, f.f);
    env->push_ref(runtime->stack, ra);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_perspective(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Int2Float y_fov, aspect, n, f;
    y_fov.i = env->localvar_getInt(runtime, pos++);
    aspect.i = env->localvar_getInt(runtime, pos++);
    n.i = env->localvar_getInt(runtime, pos++);
    f.i = env->localvar_getInt(runtime, pos++);
    mat4x4_perspective((vec4 *) r->arr_body, y_fov.f, aspect.f, n.f, f.f);
    env->push_ref(runtime->stack, r);
    return 0;
}

int org_mini_glfw_utils_Gutil_mat4x4_look_at(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    int pos = 0;
    Instance *r = env->localvar_getRefer(runtime, pos++);
    Instance *vec3_eye = env->localvar_getRefer(runtime, pos++);
    Instance *vec3_center = env->localvar_getRefer(runtime, pos++);
    Instance *vec3_up = env->localvar_getRefer(runtime, pos++);
    mat4x4_look_at((vec4 *) r->arr_body, (float *) vec3_eye->arr_body,
                   (float *) vec3_center->arr_body,
                   (float *) vec3_up->arr_body);
    env->push_ref(runtime->stack, r);
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
    glfwSetCharCallback(window, _callback_character);
    glfwSetDropCallback(window, _callback_drop);
    glfwSetMouseButtonCallback(window, _button_callback_mouse);
    glfwSetCursorPosCallback(window, _callback_cursor_pos);
    glfwSetCursorEnterCallback(window, _callback_cursor_enter);
    glfwSetWindowCloseCallback(window, _callback_window_close);
    glfwSetWindowSizeCallback(window, _callback_window_size);
    glfwSetWindowPosCallback(window, _callback_window_pos);
    glfwSetWindowFocusCallback(window, _callback_window_focus);
    glfwSetWindowIconifyCallback(window, _callback_window_iconify);
    glfwSetWindowRefreshCallback(window, _callback_window_refresh);
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
        refers._callback_character =
                env->find_methodInfo_by_name(refers.glfw_callback->mb.clazz->name, name, type);
        env->utf8_destory(name);
        env->utf8_destory(type);
    }
    {
        name_s = "drop";
        type_s = "(JI[Ljava/lang/String;)V";
        Utf8String *name = env->utf8_create_part_c(name_s, 0, strlen(name_s));
        Utf8String *type = env->utf8_create_part_c(type_s, 0, strlen(type_s));
        refers._callback_drop =
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

int org_mini_glfw_Glfw_glfwSetTime(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    Long2Double t;
    t.l = getParaLong(runtime, pos);
    pos += 2;
    glfwSetTime(t.d);
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

int org_mini_glfw_Glfw_glfwSetWindowAspectRatio(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    GLFWwindow *window = (__refer) (intptr_t) getParaLong(runtime, pos);
    pos += 2;
    s32 numer = env->localvar_getInt(runtime, pos++);
    s32 denom = env->localvar_getInt(runtime, pos++);
    glfwSetWindowAspectRatio(window, numer, denom);
    return 0;
}

int org_mini_glfw_Glfw_glfwGetClipboardString(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    GLFWwindow *window = (__refer) (intptr_t) getParaLong(runtime, pos);
    c8 *cstr = (c8 *) glfwGetClipboardString(window);
    if (cstr) {
        Utf8String *ustr = env->utf8_create_part_c(cstr, 0, strlen(cstr));
        Instance *jstr = env->jstring_create(ustr, runtime);
        env->utf8_destory(ustr);
        env->push_ref(runtime->stack, jstr);
    } else {
        env->push_ref(runtime->stack, NULL);
    }
    return 0;
}

/* ==============================   jni gl =================================*/


int org_mini_gl_GL_init(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    return 0;
}


int org_mini_gl_GL_glCompileShader(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 shader = env->localvar_getInt(runtime, pos++);
    glCompileShader((GLuint) shader);

    GLint compileResult = GL_TRUE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
    if (compileResult == GL_FALSE) {
        char szLog[1024] = {0};
        GLsizei logLen = 0;
        glGetShaderInfoLog(shader, 1024, &logLen, szLog);
        fprintf(stderr, "Compile Shader fail error log: %s \nshader code:\n", szLog);
        glDeleteShader(shader);
        shader = 0;
    }
    return 0;
}


int org_mini_gl_GL_glGetString(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    s32 name = env->localvar_getInt(runtime, pos++);
    c8 *cstr = (c8 *) glGetString((GLenum) name);
    if (cstr) {
        Instance *jstr = createJavaString(runtime, cstr);
        env->push_ref(runtime->stack, jstr);
    } else {
        env->push_ref(runtime->stack, NULL);
    }
    return 0;
}
/*
 * ===============================================================================
 * ===============================================================================
 * =================================  auto gen code ==============================
 * ===============================================================================
 * ===============================================================================
 * ===============================================================================
*/


int org_mini_gl_GL_glCullFace(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);

    glCullFace((GLenum)mode);

    return 0;
}

int org_mini_gl_GL_glFrontFace(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);

    glFrontFace((GLenum)mode);

    return 0;
}

int org_mini_gl_GL_glHint(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 mode = env->localvar_getInt(runtime, pos++);

    glHint((GLenum)target, (GLenum)mode);

    return 0;
}

int org_mini_gl_GL_glLineWidth(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float width;width.i = env->localvar_getInt(runtime, pos++);

    glLineWidth((GLfloat)width.f);

    return 0;
}

int org_mini_gl_GL_glPointSize(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float size;size.i = env->localvar_getInt(runtime, pos++);

    glPointSize((GLfloat)size.f);

    return 0;
}

int org_mini_gl_GL_glPolygonMode(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 face = env->localvar_getInt(runtime, pos++);
    s32 mode = env->localvar_getInt(runtime, pos++);

    glPolygonMode((GLenum)face, (GLenum)mode);

    return 0;
}

int org_mini_gl_GL_glScissor(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);

    glScissor((GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height);

    return 0;
}

int org_mini_gl_GL_glTexParameterf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Int2Float param;param.i = env->localvar_getInt(runtime, pos++);

    glTexParameterf((GLenum)target, (GLenum)pname, (GLfloat)param.f);

    return 0;
}

int org_mini_gl_GL_glTexParameterfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glTexParameterfv((GLenum)target, (GLenum)pname, (const GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glTexParameteri(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    s32 param = env->localvar_getInt(runtime, pos++);

    glTexParameteri((GLenum)target, (GLenum)pname, (GLint)param);

    return 0;
}

int org_mini_gl_GL_glTexParameteriv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glTexParameteriv((GLenum)target, (GLenum)pname, (const GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glTexImage1D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 internalformat = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 border = env->localvar_getInt(runtime, pos++);
    s32 format = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    Instance *pixels = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset7 = env->localvar_getInt(runtime, pos++);
    __refer ptr7 = NULL;
    if(pixels){
        arr_offset7 *= env->data_type_bytes[pixels->mb.arr_type_index];
        ptr7 = pixels->arr_body + arr_offset7;
    }

    glTexImage1D((GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLint)border, (GLenum)format, (GLenum)type, (const void *)(ptr7));

    return 0;
}

int org_mini_gl_GL_glTexImage2D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 internalformat = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);
    s32 border = env->localvar_getInt(runtime, pos++);
    s32 format = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    Instance *pixels = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset8 = env->localvar_getInt(runtime, pos++);
    __refer ptr8 = NULL;
    if(pixels){
        arr_offset8 *= env->data_type_bytes[pixels->mb.arr_type_index];
        ptr8 = pixels->arr_body + arr_offset8;
    }

    glTexImage2D((GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLenum)format, (GLenum)type, (const void *)(ptr8));

    return 0;
}

int org_mini_gl_GL_glDrawBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 buf = env->localvar_getInt(runtime, pos++);

    glDrawBuffer((GLenum)buf);

    return 0;
}

int org_mini_gl_GL_glClear(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mask = env->localvar_getInt(runtime, pos++);

    glClear((GLbitfield)mask);

    return 0;
}

int org_mini_gl_GL_glClearColor(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float red;red.i = env->localvar_getInt(runtime, pos++);
    Int2Float green;green.i = env->localvar_getInt(runtime, pos++);
    Int2Float blue;blue.i = env->localvar_getInt(runtime, pos++);
    Int2Float alpha;alpha.i = env->localvar_getInt(runtime, pos++);

    glClearColor((GLfloat)red.f, (GLfloat)green.f, (GLfloat)blue.f, (GLfloat)alpha.f);

    return 0;
}

int org_mini_gl_GL_glClearStencil(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 s = env->localvar_getInt(runtime, pos++);

    glClearStencil((GLint)s);

    return 0;
}

int org_mini_gl_GL_glClearDepth(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double depth;depth.l = getParaLong(runtime, pos);pos += 2;

    glClearDepth((GLdouble)depth.d);

    return 0;
}

int org_mini_gl_GL_glStencilMask(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mask = env->localvar_getInt(runtime, pos++);

    glStencilMask((GLuint)mask);

    return 0;
}

int org_mini_gl_GL_glColorMask(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 red = env->localvar_getInt(runtime, pos++);
    s32 green = env->localvar_getInt(runtime, pos++);
    s32 blue = env->localvar_getInt(runtime, pos++);
    s32 alpha = env->localvar_getInt(runtime, pos++);

    glColorMask((GLboolean)red, (GLboolean)green, (GLboolean)blue, (GLboolean)alpha);

    return 0;
}

int org_mini_gl_GL_glDepthMask(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 flag = env->localvar_getInt(runtime, pos++);

    glDepthMask((GLboolean)flag);

    return 0;
}

int org_mini_gl_GL_glDisable(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 cap = env->localvar_getInt(runtime, pos++);

    glDisable((GLenum)cap);

    return 0;
}

int org_mini_gl_GL_glEnable(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 cap = env->localvar_getInt(runtime, pos++);

    glEnable((GLenum)cap);

    return 0;
}

int org_mini_gl_GL_glFinish(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;


    glFinish();

    return 0;
}

int org_mini_gl_GL_glFlush(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;


    glFlush();

    return 0;
}

int org_mini_gl_GL_glBlendFunc(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 sfactor = env->localvar_getInt(runtime, pos++);
    s32 dfactor = env->localvar_getInt(runtime, pos++);

    glBlendFunc((GLenum)sfactor, (GLenum)dfactor);

    return 0;
}

int org_mini_gl_GL_glLogicOp(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 opcode = env->localvar_getInt(runtime, pos++);

    glLogicOp((GLenum)opcode);

    return 0;
}

int org_mini_gl_GL_glStencilFunc(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 func = env->localvar_getInt(runtime, pos++);
    s32 ref = env->localvar_getInt(runtime, pos++);
    s32 mask = env->localvar_getInt(runtime, pos++);

    glStencilFunc((GLenum)func, (GLint)ref, (GLuint)mask);

    return 0;
}

int org_mini_gl_GL_glStencilOp(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 fail = env->localvar_getInt(runtime, pos++);
    s32 zfail = env->localvar_getInt(runtime, pos++);
    s32 zpass = env->localvar_getInt(runtime, pos++);

    glStencilOp((GLenum)fail, (GLenum)zfail, (GLenum)zpass);

    return 0;
}

int org_mini_gl_GL_glDepthFunc(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 func = env->localvar_getInt(runtime, pos++);

    glDepthFunc((GLenum)func);

    return 0;
}

int org_mini_gl_GL_glPixelStoref(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    Int2Float param;param.i = env->localvar_getInt(runtime, pos++);

    glPixelStoref((GLenum)pname, (GLfloat)param.f);

    return 0;
}

int org_mini_gl_GL_glPixelStorei(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    s32 param = env->localvar_getInt(runtime, pos++);

    glPixelStorei((GLenum)pname, (GLint)param);

    return 0;
}

int org_mini_gl_GL_glReadBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 src = env->localvar_getInt(runtime, pos++);

    glReadBuffer((GLenum)src);

    return 0;
}

int org_mini_gl_GL_glReadPixels(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);
    s32 format = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    Instance *pixels = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset6 = env->localvar_getInt(runtime, pos++);
    __refer ptr6 = NULL;
    if(pixels){
        arr_offset6 *= env->data_type_bytes[pixels->mb.arr_type_index];
        ptr6 = pixels->arr_body + arr_offset6;
    }

    glReadPixels((GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, (void *)(ptr6));

    return 0;
}

int org_mini_gl_GL_glGetBooleanv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *data = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(data){
        arr_offset1 *= env->data_type_bytes[data->mb.arr_type_index];
        ptr1 = data->arr_body + arr_offset1;
    }

    glGetBooleanv((GLenum)pname, (GLboolean *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glGetDoublev(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *data = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(data){
        arr_offset1 *= env->data_type_bytes[data->mb.arr_type_index];
        ptr1 = data->arr_body + arr_offset1;
    }

    glGetDoublev((GLenum)pname, (GLdouble *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glGetError(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;


    s32 ret_value = (s32)glGetError();
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glGetFloatv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *data = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(data){
        arr_offset1 *= env->data_type_bytes[data->mb.arr_type_index];
        ptr1 = data->arr_body + arr_offset1;
    }

    glGetFloatv((GLenum)pname, (GLfloat *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glGetIntegerv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *data = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(data){
        arr_offset1 *= env->data_type_bytes[data->mb.arr_type_index];
        ptr1 = data->arr_body + arr_offset1;
    }

    glGetIntegerv((GLenum)pname, (GLint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glGetTexImage(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 format = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    Instance *pixels = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset4 = env->localvar_getInt(runtime, pos++);
    __refer ptr4 = NULL;
    if(pixels){
        arr_offset4 *= env->data_type_bytes[pixels->mb.arr_type_index];
        ptr4 = pixels->arr_body + arr_offset4;
    }

    glGetTexImage((GLenum)target, (GLint)level, (GLenum)format, (GLenum)type, (void *)(ptr4));

    return 0;
}

int org_mini_gl_GL_glGetTexParameterfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetTexParameterfv((GLenum)target, (GLenum)pname, (GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetTexParameteriv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetTexParameteriv((GLenum)target, (GLenum)pname, (GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetTexLevelParameterfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(params){
        arr_offset3 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr3 = params->arr_body + arr_offset3;
    }

    glGetTexLevelParameterfv((GLenum)target, (GLint)level, (GLenum)pname, (GLfloat *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glGetTexLevelParameteriv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(params){
        arr_offset3 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr3 = params->arr_body + arr_offset3;
    }

    glGetTexLevelParameteriv((GLenum)target, (GLint)level, (GLenum)pname, (GLint *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glIsEnabled(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 cap = env->localvar_getInt(runtime, pos++);

    s32 ret_value = (s32)glIsEnabled((GLenum)cap);
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glDepthRange(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double pnear;pnear.l = getParaLong(runtime, pos);pos += 2;
    Long2Double pfar;pfar.l = getParaLong(runtime, pos);pos += 2;

    glDepthRange((GLdouble)pnear.d, (GLdouble)pfar.d);

    return 0;
}

int org_mini_gl_GL_glViewport(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);

    glViewport((GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height);

    return 0;
}

int org_mini_gl_GL_glNewList(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 list = env->localvar_getInt(runtime, pos++);
    s32 mode = env->localvar_getInt(runtime, pos++);

    glNewList((GLuint)list, (GLenum)mode);

    return 0;
}

int org_mini_gl_GL_glEndList(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;


    glEndList();

    return 0;
}

int org_mini_gl_GL_glCallList(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 list = env->localvar_getInt(runtime, pos++);

    glCallList((GLuint)list);

    return 0;
}

int org_mini_gl_GL_glCallLists(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 n = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    Instance *lists = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(lists){
        arr_offset2 *= env->data_type_bytes[lists->mb.arr_type_index];
        ptr2 = lists->arr_body + arr_offset2;
    }

    glCallLists((GLsizei)n, (GLenum)type, (const void *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glDeleteLists(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 list = env->localvar_getInt(runtime, pos++);
    s32 range = env->localvar_getInt(runtime, pos++);

    glDeleteLists((GLuint)list, (GLsizei)range);

    return 0;
}

int org_mini_gl_GL_glGenLists(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 range = env->localvar_getInt(runtime, pos++);

    s32 ret_value = (s32)glGenLists((GLsizei)range);
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glListBase(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 base = env->localvar_getInt(runtime, pos++);

    glListBase((GLuint)base);

    return 0;
}

int org_mini_gl_GL_glBegin(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);

    glBegin((GLenum)mode);

    return 0;
}

int org_mini_gl_GL_glBitmap(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);
    Int2Float xorig;xorig.i = env->localvar_getInt(runtime, pos++);
    Int2Float yorig;yorig.i = env->localvar_getInt(runtime, pos++);
    Int2Float xmove;xmove.i = env->localvar_getInt(runtime, pos++);
    Int2Float ymove;ymove.i = env->localvar_getInt(runtime, pos++);
    Instance *bitmap = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset6 = env->localvar_getInt(runtime, pos++);
    __refer ptr6 = NULL;
    if(bitmap){
        arr_offset6 *= env->data_type_bytes[bitmap->mb.arr_type_index];
        ptr6 = bitmap->arr_body + arr_offset6;
    }

    glBitmap((GLsizei)width, (GLsizei)height, (GLfloat)xorig.f, (GLfloat)yorig.f, (GLfloat)xmove.f, (GLfloat)ymove.f, (const GLubyte *)(ptr6));

    return 0;
}

int org_mini_gl_GL_glColor3b(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 red = env->localvar_getInt(runtime, pos++);
    s32 green = env->localvar_getInt(runtime, pos++);
    s32 blue = env->localvar_getInt(runtime, pos++);

    glColor3b((GLbyte)red, (GLbyte)green, (GLbyte)blue);

    return 0;
}

int org_mini_gl_GL_glColor3bv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glColor3bv((const GLbyte *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glColor3d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double red;red.l = getParaLong(runtime, pos);pos += 2;
    Long2Double green;green.l = getParaLong(runtime, pos);pos += 2;
    Long2Double blue;blue.l = getParaLong(runtime, pos);pos += 2;

    glColor3d((GLdouble)red.d, (GLdouble)green.d, (GLdouble)blue.d);

    return 0;
}

int org_mini_gl_GL_glColor3dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glColor3dv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glColor3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float red;red.i = env->localvar_getInt(runtime, pos++);
    Int2Float green;green.i = env->localvar_getInt(runtime, pos++);
    Int2Float blue;blue.i = env->localvar_getInt(runtime, pos++);

    glColor3f((GLfloat)red.f, (GLfloat)green.f, (GLfloat)blue.f);

    return 0;
}

int org_mini_gl_GL_glColor3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glColor3fv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glColor3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 red = env->localvar_getInt(runtime, pos++);
    s32 green = env->localvar_getInt(runtime, pos++);
    s32 blue = env->localvar_getInt(runtime, pos++);

    glColor3i((GLint)red, (GLint)green, (GLint)blue);

    return 0;
}

int org_mini_gl_GL_glColor3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glColor3iv((const GLint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glColor3s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 red = env->localvar_getInt(runtime, pos++);
    s32 green = env->localvar_getInt(runtime, pos++);
    s32 blue = env->localvar_getInt(runtime, pos++);

    glColor3s((GLshort)red, (GLshort)green, (GLshort)blue);

    return 0;
}

int org_mini_gl_GL_glColor3sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glColor3sv((const GLshort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glColor3ub(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 red = env->localvar_getInt(runtime, pos++);
    s32 green = env->localvar_getInt(runtime, pos++);
    s32 blue = env->localvar_getInt(runtime, pos++);

    glColor3ub((GLubyte)red, (GLubyte)green, (GLubyte)blue);

    return 0;
}

int org_mini_gl_GL_glColor3ubv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glColor3ubv((const GLubyte *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glColor3ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 red = env->localvar_getInt(runtime, pos++);
    s32 green = env->localvar_getInt(runtime, pos++);
    s32 blue = env->localvar_getInt(runtime, pos++);

    glColor3ui((GLuint)red, (GLuint)green, (GLuint)blue);

    return 0;
}

int org_mini_gl_GL_glColor3uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glColor3uiv((const GLuint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glColor3us(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 red = env->localvar_getInt(runtime, pos++);
    s32 green = env->localvar_getInt(runtime, pos++);
    s32 blue = env->localvar_getInt(runtime, pos++);

    glColor3us((GLushort)red, (GLushort)green, (GLushort)blue);

    return 0;
}

int org_mini_gl_GL_glColor3usv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glColor3usv((const GLushort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glColor4b(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 red = env->localvar_getInt(runtime, pos++);
    s32 green = env->localvar_getInt(runtime, pos++);
    s32 blue = env->localvar_getInt(runtime, pos++);
    s32 alpha = env->localvar_getInt(runtime, pos++);

    glColor4b((GLbyte)red, (GLbyte)green, (GLbyte)blue, (GLbyte)alpha);

    return 0;
}

int org_mini_gl_GL_glColor4bv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glColor4bv((const GLbyte *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glColor4d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double red;red.l = getParaLong(runtime, pos);pos += 2;
    Long2Double green;green.l = getParaLong(runtime, pos);pos += 2;
    Long2Double blue;blue.l = getParaLong(runtime, pos);pos += 2;
    Long2Double alpha;alpha.l = getParaLong(runtime, pos);pos += 2;

    glColor4d((GLdouble)red.d, (GLdouble)green.d, (GLdouble)blue.d, (GLdouble)alpha.d);

    return 0;
}

int org_mini_gl_GL_glColor4dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glColor4dv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glColor4f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float red;red.i = env->localvar_getInt(runtime, pos++);
    Int2Float green;green.i = env->localvar_getInt(runtime, pos++);
    Int2Float blue;blue.i = env->localvar_getInt(runtime, pos++);
    Int2Float alpha;alpha.i = env->localvar_getInt(runtime, pos++);

    glColor4f((GLfloat)red.f, (GLfloat)green.f, (GLfloat)blue.f, (GLfloat)alpha.f);

    return 0;
}

int org_mini_gl_GL_glColor4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glColor4fv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glColor4i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 red = env->localvar_getInt(runtime, pos++);
    s32 green = env->localvar_getInt(runtime, pos++);
    s32 blue = env->localvar_getInt(runtime, pos++);
    s32 alpha = env->localvar_getInt(runtime, pos++);

    glColor4i((GLint)red, (GLint)green, (GLint)blue, (GLint)alpha);

    return 0;
}

int org_mini_gl_GL_glColor4iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glColor4iv((const GLint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glColor4s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 red = env->localvar_getInt(runtime, pos++);
    s32 green = env->localvar_getInt(runtime, pos++);
    s32 blue = env->localvar_getInt(runtime, pos++);
    s32 alpha = env->localvar_getInt(runtime, pos++);

    glColor4s((GLshort)red, (GLshort)green, (GLshort)blue, (GLshort)alpha);

    return 0;
}

int org_mini_gl_GL_glColor4sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glColor4sv((const GLshort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glColor4ub(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 red = env->localvar_getInt(runtime, pos++);
    s32 green = env->localvar_getInt(runtime, pos++);
    s32 blue = env->localvar_getInt(runtime, pos++);
    s32 alpha = env->localvar_getInt(runtime, pos++);

    glColor4ub((GLubyte)red, (GLubyte)green, (GLubyte)blue, (GLubyte)alpha);

    return 0;
}

int org_mini_gl_GL_glColor4ubv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glColor4ubv((const GLubyte *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glColor4ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 red = env->localvar_getInt(runtime, pos++);
    s32 green = env->localvar_getInt(runtime, pos++);
    s32 blue = env->localvar_getInt(runtime, pos++);
    s32 alpha = env->localvar_getInt(runtime, pos++);

    glColor4ui((GLuint)red, (GLuint)green, (GLuint)blue, (GLuint)alpha);

    return 0;
}

int org_mini_gl_GL_glColor4uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glColor4uiv((const GLuint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glColor4us(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 red = env->localvar_getInt(runtime, pos++);
    s32 green = env->localvar_getInt(runtime, pos++);
    s32 blue = env->localvar_getInt(runtime, pos++);
    s32 alpha = env->localvar_getInt(runtime, pos++);

    glColor4us((GLushort)red, (GLushort)green, (GLushort)blue, (GLushort)alpha);

    return 0;
}

int org_mini_gl_GL_glColor4usv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glColor4usv((const GLushort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glEdgeFlag(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 flag = env->localvar_getInt(runtime, pos++);

    glEdgeFlag((GLboolean)flag);

    return 0;
}

int org_mini_gl_GL_glEdgeFlagv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *flag = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(flag){
        arr_offset0 *= env->data_type_bytes[flag->mb.arr_type_index];
        ptr0 = flag->arr_body + arr_offset0;
    }

    glEdgeFlagv((const GLboolean *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glEnd(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;


    glEnd();

    return 0;
}

int org_mini_gl_GL_glIndexd(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double c;c.l = getParaLong(runtime, pos);pos += 2;

    glIndexd((GLdouble)c.d);

    return 0;
}

int org_mini_gl_GL_glIndexdv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *c = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(c){
        arr_offset0 *= env->data_type_bytes[c->mb.arr_type_index];
        ptr0 = c->arr_body + arr_offset0;
    }

    glIndexdv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glIndexf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float c;c.i = env->localvar_getInt(runtime, pos++);

    glIndexf((GLfloat)c.f);

    return 0;
}

int org_mini_gl_GL_glIndexfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *c = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(c){
        arr_offset0 *= env->data_type_bytes[c->mb.arr_type_index];
        ptr0 = c->arr_body + arr_offset0;
    }

    glIndexfv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glIndexi(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 c = env->localvar_getInt(runtime, pos++);

    glIndexi((GLint)c);

    return 0;
}

int org_mini_gl_GL_glIndexiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *c = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(c){
        arr_offset0 *= env->data_type_bytes[c->mb.arr_type_index];
        ptr0 = c->arr_body + arr_offset0;
    }

    glIndexiv((const GLint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glIndexs(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 c = env->localvar_getInt(runtime, pos++);

    glIndexs((GLshort)c);

    return 0;
}

int org_mini_gl_GL_glIndexsv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *c = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(c){
        arr_offset0 *= env->data_type_bytes[c->mb.arr_type_index];
        ptr0 = c->arr_body + arr_offset0;
    }

    glIndexsv((const GLshort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glNormal3b(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 nx = env->localvar_getInt(runtime, pos++);
    s32 ny = env->localvar_getInt(runtime, pos++);
    s32 nz = env->localvar_getInt(runtime, pos++);

    glNormal3b((GLbyte)nx, (GLbyte)ny, (GLbyte)nz);

    return 0;
}

int org_mini_gl_GL_glNormal3bv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glNormal3bv((const GLbyte *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glNormal3d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double nx;nx.l = getParaLong(runtime, pos);pos += 2;
    Long2Double ny;ny.l = getParaLong(runtime, pos);pos += 2;
    Long2Double nz;nz.l = getParaLong(runtime, pos);pos += 2;

    glNormal3d((GLdouble)nx.d, (GLdouble)ny.d, (GLdouble)nz.d);

    return 0;
}

int org_mini_gl_GL_glNormal3dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glNormal3dv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glNormal3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float nx;nx.i = env->localvar_getInt(runtime, pos++);
    Int2Float ny;ny.i = env->localvar_getInt(runtime, pos++);
    Int2Float nz;nz.i = env->localvar_getInt(runtime, pos++);

    glNormal3f((GLfloat)nx.f, (GLfloat)ny.f, (GLfloat)nz.f);

    return 0;
}

int org_mini_gl_GL_glNormal3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glNormal3fv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glNormal3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 nx = env->localvar_getInt(runtime, pos++);
    s32 ny = env->localvar_getInt(runtime, pos++);
    s32 nz = env->localvar_getInt(runtime, pos++);

    glNormal3i((GLint)nx, (GLint)ny, (GLint)nz);

    return 0;
}

int org_mini_gl_GL_glNormal3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glNormal3iv((const GLint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glNormal3s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 nx = env->localvar_getInt(runtime, pos++);
    s32 ny = env->localvar_getInt(runtime, pos++);
    s32 nz = env->localvar_getInt(runtime, pos++);

    glNormal3s((GLshort)nx, (GLshort)ny, (GLshort)nz);

    return 0;
}

int org_mini_gl_GL_glNormal3sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glNormal3sv((const GLshort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glRasterPos2d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double x;x.l = getParaLong(runtime, pos);pos += 2;
    Long2Double y;y.l = getParaLong(runtime, pos);pos += 2;

    glRasterPos2d((GLdouble)x.d, (GLdouble)y.d);

    return 0;
}

int org_mini_gl_GL_glRasterPos2dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glRasterPos2dv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glRasterPos2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float x;x.i = env->localvar_getInt(runtime, pos++);
    Int2Float y;y.i = env->localvar_getInt(runtime, pos++);

    glRasterPos2f((GLfloat)x.f, (GLfloat)y.f);

    return 0;
}

int org_mini_gl_GL_glRasterPos2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glRasterPos2fv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glRasterPos2i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);

    glRasterPos2i((GLint)x, (GLint)y);

    return 0;
}

int org_mini_gl_GL_glRasterPos2iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glRasterPos2iv((const GLint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glRasterPos2s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);

    glRasterPos2s((GLshort)x, (GLshort)y);

    return 0;
}

int org_mini_gl_GL_glRasterPos2sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glRasterPos2sv((const GLshort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glRasterPos3d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double x;x.l = getParaLong(runtime, pos);pos += 2;
    Long2Double y;y.l = getParaLong(runtime, pos);pos += 2;
    Long2Double z;z.l = getParaLong(runtime, pos);pos += 2;

    glRasterPos3d((GLdouble)x.d, (GLdouble)y.d, (GLdouble)z.d);

    return 0;
}

int org_mini_gl_GL_glRasterPos3dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glRasterPos3dv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glRasterPos3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float x;x.i = env->localvar_getInt(runtime, pos++);
    Int2Float y;y.i = env->localvar_getInt(runtime, pos++);
    Int2Float z;z.i = env->localvar_getInt(runtime, pos++);

    glRasterPos3f((GLfloat)x.f, (GLfloat)y.f, (GLfloat)z.f);

    return 0;
}

int org_mini_gl_GL_glRasterPos3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glRasterPos3fv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glRasterPos3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 z = env->localvar_getInt(runtime, pos++);

    glRasterPos3i((GLint)x, (GLint)y, (GLint)z);

    return 0;
}

int org_mini_gl_GL_glRasterPos3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glRasterPos3iv((const GLint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glRasterPos3s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 z = env->localvar_getInt(runtime, pos++);

    glRasterPos3s((GLshort)x, (GLshort)y, (GLshort)z);

    return 0;
}

int org_mini_gl_GL_glRasterPos3sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glRasterPos3sv((const GLshort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glRasterPos4d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double x;x.l = getParaLong(runtime, pos);pos += 2;
    Long2Double y;y.l = getParaLong(runtime, pos);pos += 2;
    Long2Double z;z.l = getParaLong(runtime, pos);pos += 2;
    Long2Double w;w.l = getParaLong(runtime, pos);pos += 2;

    glRasterPos4d((GLdouble)x.d, (GLdouble)y.d, (GLdouble)z.d, (GLdouble)w.d);

    return 0;
}

int org_mini_gl_GL_glRasterPos4dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glRasterPos4dv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glRasterPos4f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float x;x.i = env->localvar_getInt(runtime, pos++);
    Int2Float y;y.i = env->localvar_getInt(runtime, pos++);
    Int2Float z;z.i = env->localvar_getInt(runtime, pos++);
    Int2Float w;w.i = env->localvar_getInt(runtime, pos++);

    glRasterPos4f((GLfloat)x.f, (GLfloat)y.f, (GLfloat)z.f, (GLfloat)w.f);

    return 0;
}

int org_mini_gl_GL_glRasterPos4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glRasterPos4fv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glRasterPos4i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 z = env->localvar_getInt(runtime, pos++);
    s32 w = env->localvar_getInt(runtime, pos++);

    glRasterPos4i((GLint)x, (GLint)y, (GLint)z, (GLint)w);

    return 0;
}

int org_mini_gl_GL_glRasterPos4iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glRasterPos4iv((const GLint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glRasterPos4s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 z = env->localvar_getInt(runtime, pos++);
    s32 w = env->localvar_getInt(runtime, pos++);

    glRasterPos4s((GLshort)x, (GLshort)y, (GLshort)z, (GLshort)w);

    return 0;
}

int org_mini_gl_GL_glRasterPos4sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glRasterPos4sv((const GLshort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glRectd(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double x1;x1.l = getParaLong(runtime, pos);pos += 2;
    Long2Double y1;y1.l = getParaLong(runtime, pos);pos += 2;
    Long2Double x2;x2.l = getParaLong(runtime, pos);pos += 2;
    Long2Double y2;y2.l = getParaLong(runtime, pos);pos += 2;

    glRectd((GLdouble)x1.d, (GLdouble)y1.d, (GLdouble)x2.d, (GLdouble)y2.d);

    return 0;
}

int org_mini_gl_GL_glRectdv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v1 = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v1){
        arr_offset0 *= env->data_type_bytes[v1->mb.arr_type_index];
        ptr0 = v1->arr_body + arr_offset0;
    }
    Instance *v2 = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(v2){
        arr_offset2 *= env->data_type_bytes[v2->mb.arr_type_index];
        ptr2 = v2->arr_body + arr_offset2;
    }

    glRectdv((const GLdouble *)(ptr0), (const GLdouble *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glRectf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float x1;x1.i = env->localvar_getInt(runtime, pos++);
    Int2Float y1;y1.i = env->localvar_getInt(runtime, pos++);
    Int2Float x2;x2.i = env->localvar_getInt(runtime, pos++);
    Int2Float y2;y2.i = env->localvar_getInt(runtime, pos++);

    glRectf((GLfloat)x1.f, (GLfloat)y1.f, (GLfloat)x2.f, (GLfloat)y2.f);

    return 0;
}

int org_mini_gl_GL_glRectfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v1 = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v1){
        arr_offset0 *= env->data_type_bytes[v1->mb.arr_type_index];
        ptr0 = v1->arr_body + arr_offset0;
    }
    Instance *v2 = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(v2){
        arr_offset2 *= env->data_type_bytes[v2->mb.arr_type_index];
        ptr2 = v2->arr_body + arr_offset2;
    }

    glRectfv((const GLfloat *)(ptr0), (const GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glRecti(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x1 = env->localvar_getInt(runtime, pos++);
    s32 y1 = env->localvar_getInt(runtime, pos++);
    s32 x2 = env->localvar_getInt(runtime, pos++);
    s32 y2 = env->localvar_getInt(runtime, pos++);

    glRecti((GLint)x1, (GLint)y1, (GLint)x2, (GLint)y2);

    return 0;
}

int org_mini_gl_GL_glRectiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v1 = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v1){
        arr_offset0 *= env->data_type_bytes[v1->mb.arr_type_index];
        ptr0 = v1->arr_body + arr_offset0;
    }
    Instance *v2 = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(v2){
        arr_offset2 *= env->data_type_bytes[v2->mb.arr_type_index];
        ptr2 = v2->arr_body + arr_offset2;
    }

    glRectiv((const GLint *)(ptr0), (const GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glRects(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x1 = env->localvar_getInt(runtime, pos++);
    s32 y1 = env->localvar_getInt(runtime, pos++);
    s32 x2 = env->localvar_getInt(runtime, pos++);
    s32 y2 = env->localvar_getInt(runtime, pos++);

    glRects((GLshort)x1, (GLshort)y1, (GLshort)x2, (GLshort)y2);

    return 0;
}

int org_mini_gl_GL_glRectsv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v1 = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v1){
        arr_offset0 *= env->data_type_bytes[v1->mb.arr_type_index];
        ptr0 = v1->arr_body + arr_offset0;
    }
    Instance *v2 = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(v2){
        arr_offset2 *= env->data_type_bytes[v2->mb.arr_type_index];
        ptr2 = v2->arr_body + arr_offset2;
    }

    glRectsv((const GLshort *)(ptr0), (const GLshort *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glTexCoord1d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double s;s.l = getParaLong(runtime, pos);pos += 2;

    glTexCoord1d((GLdouble)s.d);

    return 0;
}

int org_mini_gl_GL_glTexCoord1dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glTexCoord1dv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glTexCoord1f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float s;s.i = env->localvar_getInt(runtime, pos++);

    glTexCoord1f((GLfloat)s.f);

    return 0;
}

int org_mini_gl_GL_glTexCoord1fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glTexCoord1fv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glTexCoord1i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 s = env->localvar_getInt(runtime, pos++);

    glTexCoord1i((GLint)s);

    return 0;
}

int org_mini_gl_GL_glTexCoord1iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glTexCoord1iv((const GLint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glTexCoord1s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 s = env->localvar_getInt(runtime, pos++);

    glTexCoord1s((GLshort)s);

    return 0;
}

int org_mini_gl_GL_glTexCoord1sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glTexCoord1sv((const GLshort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glTexCoord2d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double s;s.l = getParaLong(runtime, pos);pos += 2;
    Long2Double t;t.l = getParaLong(runtime, pos);pos += 2;

    glTexCoord2d((GLdouble)s.d, (GLdouble)t.d);

    return 0;
}

int org_mini_gl_GL_glTexCoord2dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glTexCoord2dv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glTexCoord2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float s;s.i = env->localvar_getInt(runtime, pos++);
    Int2Float t;t.i = env->localvar_getInt(runtime, pos++);

    glTexCoord2f((GLfloat)s.f, (GLfloat)t.f);

    return 0;
}

int org_mini_gl_GL_glTexCoord2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glTexCoord2fv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glTexCoord2i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 s = env->localvar_getInt(runtime, pos++);
    s32 t = env->localvar_getInt(runtime, pos++);

    glTexCoord2i((GLint)s, (GLint)t);

    return 0;
}

int org_mini_gl_GL_glTexCoord2iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glTexCoord2iv((const GLint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glTexCoord2s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 s = env->localvar_getInt(runtime, pos++);
    s32 t = env->localvar_getInt(runtime, pos++);

    glTexCoord2s((GLshort)s, (GLshort)t);

    return 0;
}

int org_mini_gl_GL_glTexCoord2sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glTexCoord2sv((const GLshort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glTexCoord3d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double s;s.l = getParaLong(runtime, pos);pos += 2;
    Long2Double t;t.l = getParaLong(runtime, pos);pos += 2;
    Long2Double r;r.l = getParaLong(runtime, pos);pos += 2;

    glTexCoord3d((GLdouble)s.d, (GLdouble)t.d, (GLdouble)r.d);

    return 0;
}

int org_mini_gl_GL_glTexCoord3dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glTexCoord3dv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glTexCoord3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float s;s.i = env->localvar_getInt(runtime, pos++);
    Int2Float t;t.i = env->localvar_getInt(runtime, pos++);
    Int2Float r;r.i = env->localvar_getInt(runtime, pos++);

    glTexCoord3f((GLfloat)s.f, (GLfloat)t.f, (GLfloat)r.f);

    return 0;
}

int org_mini_gl_GL_glTexCoord3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glTexCoord3fv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glTexCoord3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 s = env->localvar_getInt(runtime, pos++);
    s32 t = env->localvar_getInt(runtime, pos++);
    s32 r = env->localvar_getInt(runtime, pos++);

    glTexCoord3i((GLint)s, (GLint)t, (GLint)r);

    return 0;
}

int org_mini_gl_GL_glTexCoord3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glTexCoord3iv((const GLint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glTexCoord3s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 s = env->localvar_getInt(runtime, pos++);
    s32 t = env->localvar_getInt(runtime, pos++);
    s32 r = env->localvar_getInt(runtime, pos++);

    glTexCoord3s((GLshort)s, (GLshort)t, (GLshort)r);

    return 0;
}

int org_mini_gl_GL_glTexCoord3sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glTexCoord3sv((const GLshort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glTexCoord4d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double s;s.l = getParaLong(runtime, pos);pos += 2;
    Long2Double t;t.l = getParaLong(runtime, pos);pos += 2;
    Long2Double r;r.l = getParaLong(runtime, pos);pos += 2;
    Long2Double q;q.l = getParaLong(runtime, pos);pos += 2;

    glTexCoord4d((GLdouble)s.d, (GLdouble)t.d, (GLdouble)r.d, (GLdouble)q.d);

    return 0;
}

int org_mini_gl_GL_glTexCoord4dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glTexCoord4dv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glTexCoord4f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float s;s.i = env->localvar_getInt(runtime, pos++);
    Int2Float t;t.i = env->localvar_getInt(runtime, pos++);
    Int2Float r;r.i = env->localvar_getInt(runtime, pos++);
    Int2Float q;q.i = env->localvar_getInt(runtime, pos++);

    glTexCoord4f((GLfloat)s.f, (GLfloat)t.f, (GLfloat)r.f, (GLfloat)q.f);

    return 0;
}

int org_mini_gl_GL_glTexCoord4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glTexCoord4fv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glTexCoord4i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 s = env->localvar_getInt(runtime, pos++);
    s32 t = env->localvar_getInt(runtime, pos++);
    s32 r = env->localvar_getInt(runtime, pos++);
    s32 q = env->localvar_getInt(runtime, pos++);

    glTexCoord4i((GLint)s, (GLint)t, (GLint)r, (GLint)q);

    return 0;
}

int org_mini_gl_GL_glTexCoord4iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glTexCoord4iv((const GLint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glTexCoord4s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 s = env->localvar_getInt(runtime, pos++);
    s32 t = env->localvar_getInt(runtime, pos++);
    s32 r = env->localvar_getInt(runtime, pos++);
    s32 q = env->localvar_getInt(runtime, pos++);

    glTexCoord4s((GLshort)s, (GLshort)t, (GLshort)r, (GLshort)q);

    return 0;
}

int org_mini_gl_GL_glTexCoord4sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glTexCoord4sv((const GLshort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glVertex2d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double x;x.l = getParaLong(runtime, pos);pos += 2;
    Long2Double y;y.l = getParaLong(runtime, pos);pos += 2;

    glVertex2d((GLdouble)x.d, (GLdouble)y.d);

    return 0;
}

int org_mini_gl_GL_glVertex2dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glVertex2dv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glVertex2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float x;x.i = env->localvar_getInt(runtime, pos++);
    Int2Float y;y.i = env->localvar_getInt(runtime, pos++);

    glVertex2f((GLfloat)x.f, (GLfloat)y.f);

    return 0;
}

int org_mini_gl_GL_glVertex2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glVertex2fv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glVertex2i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);

    glVertex2i((GLint)x, (GLint)y);

    return 0;
}

int org_mini_gl_GL_glVertex2iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glVertex2iv((const GLint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glVertex2s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);

    glVertex2s((GLshort)x, (GLshort)y);

    return 0;
}

int org_mini_gl_GL_glVertex2sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glVertex2sv((const GLshort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glVertex3d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double x;x.l = getParaLong(runtime, pos);pos += 2;
    Long2Double y;y.l = getParaLong(runtime, pos);pos += 2;
    Long2Double z;z.l = getParaLong(runtime, pos);pos += 2;

    glVertex3d((GLdouble)x.d, (GLdouble)y.d, (GLdouble)z.d);

    return 0;
}

int org_mini_gl_GL_glVertex3dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glVertex3dv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glVertex3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float x;x.i = env->localvar_getInt(runtime, pos++);
    Int2Float y;y.i = env->localvar_getInt(runtime, pos++);
    Int2Float z;z.i = env->localvar_getInt(runtime, pos++);

    glVertex3f((GLfloat)x.f, (GLfloat)y.f, (GLfloat)z.f);

    return 0;
}

int org_mini_gl_GL_glVertex3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glVertex3fv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glVertex3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 z = env->localvar_getInt(runtime, pos++);

    glVertex3i((GLint)x, (GLint)y, (GLint)z);

    return 0;
}

int org_mini_gl_GL_glVertex3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glVertex3iv((const GLint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glVertex3s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 z = env->localvar_getInt(runtime, pos++);

    glVertex3s((GLshort)x, (GLshort)y, (GLshort)z);

    return 0;
}

int org_mini_gl_GL_glVertex3sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glVertex3sv((const GLshort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glVertex4d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double x;x.l = getParaLong(runtime, pos);pos += 2;
    Long2Double y;y.l = getParaLong(runtime, pos);pos += 2;
    Long2Double z;z.l = getParaLong(runtime, pos);pos += 2;
    Long2Double w;w.l = getParaLong(runtime, pos);pos += 2;

    glVertex4d((GLdouble)x.d, (GLdouble)y.d, (GLdouble)z.d, (GLdouble)w.d);

    return 0;
}

int org_mini_gl_GL_glVertex4dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glVertex4dv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glVertex4f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float x;x.i = env->localvar_getInt(runtime, pos++);
    Int2Float y;y.i = env->localvar_getInt(runtime, pos++);
    Int2Float z;z.i = env->localvar_getInt(runtime, pos++);
    Int2Float w;w.i = env->localvar_getInt(runtime, pos++);

    glVertex4f((GLfloat)x.f, (GLfloat)y.f, (GLfloat)z.f, (GLfloat)w.f);

    return 0;
}

int org_mini_gl_GL_glVertex4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glVertex4fv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glVertex4i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 z = env->localvar_getInt(runtime, pos++);
    s32 w = env->localvar_getInt(runtime, pos++);

    glVertex4i((GLint)x, (GLint)y, (GLint)z, (GLint)w);

    return 0;
}

int org_mini_gl_GL_glVertex4iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glVertex4iv((const GLint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glVertex4s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 z = env->localvar_getInt(runtime, pos++);
    s32 w = env->localvar_getInt(runtime, pos++);

    glVertex4s((GLshort)x, (GLshort)y, (GLshort)z, (GLshort)w);

    return 0;
}

int org_mini_gl_GL_glVertex4sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glVertex4sv((const GLshort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glClipPlane(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 plane = env->localvar_getInt(runtime, pos++);
    Instance *equation = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(equation){
        arr_offset1 *= env->data_type_bytes[equation->mb.arr_type_index];
        ptr1 = equation->arr_body + arr_offset1;
    }

    glClipPlane((GLenum)plane, (const GLdouble *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glColorMaterial(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 face = env->localvar_getInt(runtime, pos++);
    s32 mode = env->localvar_getInt(runtime, pos++);

    glColorMaterial((GLenum)face, (GLenum)mode);

    return 0;
}

int org_mini_gl_GL_glFogf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    Int2Float param;param.i = env->localvar_getInt(runtime, pos++);

    glFogf((GLenum)pname, (GLfloat)param.f);

    return 0;
}

int org_mini_gl_GL_glFogfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(params){
        arr_offset1 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr1 = params->arr_body + arr_offset1;
    }

    glFogfv((GLenum)pname, (const GLfloat *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glFogi(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    s32 param = env->localvar_getInt(runtime, pos++);

    glFogi((GLenum)pname, (GLint)param);

    return 0;
}

int org_mini_gl_GL_glFogiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(params){
        arr_offset1 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr1 = params->arr_body + arr_offset1;
    }

    glFogiv((GLenum)pname, (const GLint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glLightf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 light = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Int2Float param;param.i = env->localvar_getInt(runtime, pos++);

    glLightf((GLenum)light, (GLenum)pname, (GLfloat)param.f);

    return 0;
}

int org_mini_gl_GL_glLightfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 light = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glLightfv((GLenum)light, (GLenum)pname, (const GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glLighti(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 light = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    s32 param = env->localvar_getInt(runtime, pos++);

    glLighti((GLenum)light, (GLenum)pname, (GLint)param);

    return 0;
}

int org_mini_gl_GL_glLightiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 light = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glLightiv((GLenum)light, (GLenum)pname, (const GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glLightModelf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    Int2Float param;param.i = env->localvar_getInt(runtime, pos++);

    glLightModelf((GLenum)pname, (GLfloat)param.f);

    return 0;
}

int org_mini_gl_GL_glLightModelfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(params){
        arr_offset1 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr1 = params->arr_body + arr_offset1;
    }

    glLightModelfv((GLenum)pname, (const GLfloat *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glLightModeli(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    s32 param = env->localvar_getInt(runtime, pos++);

    glLightModeli((GLenum)pname, (GLint)param);

    return 0;
}

int org_mini_gl_GL_glLightModeliv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(params){
        arr_offset1 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr1 = params->arr_body + arr_offset1;
    }

    glLightModeliv((GLenum)pname, (const GLint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glLineStipple(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 factor = env->localvar_getInt(runtime, pos++);
    s32 pattern = env->localvar_getInt(runtime, pos++);

    glLineStipple((GLint)factor, (GLushort)pattern);

    return 0;
}

int org_mini_gl_GL_glMaterialf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 face = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Int2Float param;param.i = env->localvar_getInt(runtime, pos++);

    glMaterialf((GLenum)face, (GLenum)pname, (GLfloat)param.f);

    return 0;
}

int org_mini_gl_GL_glMaterialfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 face = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glMaterialfv((GLenum)face, (GLenum)pname, (const GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glMateriali(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 face = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    s32 param = env->localvar_getInt(runtime, pos++);

    glMateriali((GLenum)face, (GLenum)pname, (GLint)param);

    return 0;
}

int org_mini_gl_GL_glMaterialiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 face = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glMaterialiv((GLenum)face, (GLenum)pname, (const GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glPolygonStipple(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *mask = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(mask){
        arr_offset0 *= env->data_type_bytes[mask->mb.arr_type_index];
        ptr0 = mask->arr_body + arr_offset0;
    }

    glPolygonStipple((const GLubyte *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glShadeModel(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);

    glShadeModel((GLenum)mode);

    return 0;
}

int org_mini_gl_GL_glTexEnvf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Int2Float param;param.i = env->localvar_getInt(runtime, pos++);

    glTexEnvf((GLenum)target, (GLenum)pname, (GLfloat)param.f);

    return 0;
}

int org_mini_gl_GL_glTexEnvfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glTexEnvfv((GLenum)target, (GLenum)pname, (const GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glTexEnvi(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    s32 param = env->localvar_getInt(runtime, pos++);

    glTexEnvi((GLenum)target, (GLenum)pname, (GLint)param);

    return 0;
}

int org_mini_gl_GL_glTexEnviv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glTexEnviv((GLenum)target, (GLenum)pname, (const GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glTexGend(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 coord = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Long2Double param;param.l = getParaLong(runtime, pos);pos += 2;

    glTexGend((GLenum)coord, (GLenum)pname, (GLdouble)param.d);

    return 0;
}

int org_mini_gl_GL_glTexGendv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 coord = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glTexGendv((GLenum)coord, (GLenum)pname, (const GLdouble *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glTexGenf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 coord = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Int2Float param;param.i = env->localvar_getInt(runtime, pos++);

    glTexGenf((GLenum)coord, (GLenum)pname, (GLfloat)param.f);

    return 0;
}

int org_mini_gl_GL_glTexGenfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 coord = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glTexGenfv((GLenum)coord, (GLenum)pname, (const GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glTexGeni(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 coord = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    s32 param = env->localvar_getInt(runtime, pos++);

    glTexGeni((GLenum)coord, (GLenum)pname, (GLint)param);

    return 0;
}

int org_mini_gl_GL_glTexGeniv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 coord = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glTexGeniv((GLenum)coord, (GLenum)pname, (const GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glFeedbackBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 size = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    Instance *buffer = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(buffer){
        arr_offset2 *= env->data_type_bytes[buffer->mb.arr_type_index];
        ptr2 = buffer->arr_body + arr_offset2;
    }

    glFeedbackBuffer((GLsizei)size, (GLenum)type, (GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glSelectBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 size = env->localvar_getInt(runtime, pos++);
    Instance *buffer = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(buffer){
        arr_offset1 *= env->data_type_bytes[buffer->mb.arr_type_index];
        ptr1 = buffer->arr_body + arr_offset1;
    }

    glSelectBuffer((GLsizei)size, (GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glRenderMode(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);

    s32 ret_value = (s32)glRenderMode((GLenum)mode);
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glInitNames(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;


    glInitNames();

    return 0;
}

int org_mini_gl_GL_glLoadName(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 name = env->localvar_getInt(runtime, pos++);

    glLoadName((GLuint)name);

    return 0;
}

int org_mini_gl_GL_glPassThrough(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float token;token.i = env->localvar_getInt(runtime, pos++);

    glPassThrough((GLfloat)token.f);

    return 0;
}

int org_mini_gl_GL_glPopName(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;


    glPopName();

    return 0;
}

int org_mini_gl_GL_glPushName(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 name = env->localvar_getInt(runtime, pos++);

    glPushName((GLuint)name);

    return 0;
}

int org_mini_gl_GL_glClearAccum(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float red;red.i = env->localvar_getInt(runtime, pos++);
    Int2Float green;green.i = env->localvar_getInt(runtime, pos++);
    Int2Float blue;blue.i = env->localvar_getInt(runtime, pos++);
    Int2Float alpha;alpha.i = env->localvar_getInt(runtime, pos++);

    glClearAccum((GLfloat)red.f, (GLfloat)green.f, (GLfloat)blue.f, (GLfloat)alpha.f);

    return 0;
}

int org_mini_gl_GL_glClearIndex(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float c;c.i = env->localvar_getInt(runtime, pos++);

    glClearIndex((GLfloat)c.f);

    return 0;
}

int org_mini_gl_GL_glIndexMask(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mask = env->localvar_getInt(runtime, pos++);

    glIndexMask((GLuint)mask);

    return 0;
}

int org_mini_gl_GL_glAccum(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 op = env->localvar_getInt(runtime, pos++);
    Int2Float value;value.i = env->localvar_getInt(runtime, pos++);

    glAccum((GLenum)op, (GLfloat)value.f);

    return 0;
}

int org_mini_gl_GL_glPopAttrib(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;


    glPopAttrib();

    return 0;
}

int org_mini_gl_GL_glPushAttrib(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mask = env->localvar_getInt(runtime, pos++);

    glPushAttrib((GLbitfield)mask);

    return 0;
}

int org_mini_gl_GL_glMap1d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Long2Double u1;u1.l = getParaLong(runtime, pos);pos += 2;
    Long2Double u2;u2.l = getParaLong(runtime, pos);pos += 2;
    s32 stride = env->localvar_getInt(runtime, pos++);
    s32 order = env->localvar_getInt(runtime, pos++);
    Instance *points = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(points){
        arr_offset5 *= env->data_type_bytes[points->mb.arr_type_index];
        ptr5 = points->arr_body + arr_offset5;
    }

    glMap1d((GLenum)target, (GLdouble)u1.d, (GLdouble)u2.d, (GLint)stride, (GLint)order, (const GLdouble *)(ptr5));

    return 0;
}

int org_mini_gl_GL_glMap1f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Int2Float u1;u1.i = env->localvar_getInt(runtime, pos++);
    Int2Float u2;u2.i = env->localvar_getInt(runtime, pos++);
    s32 stride = env->localvar_getInt(runtime, pos++);
    s32 order = env->localvar_getInt(runtime, pos++);
    Instance *points = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(points){
        arr_offset5 *= env->data_type_bytes[points->mb.arr_type_index];
        ptr5 = points->arr_body + arr_offset5;
    }

    glMap1f((GLenum)target, (GLfloat)u1.f, (GLfloat)u2.f, (GLint)stride, (GLint)order, (const GLfloat *)(ptr5));

    return 0;
}

int org_mini_gl_GL_glMap2d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Long2Double u1;u1.l = getParaLong(runtime, pos);pos += 2;
    Long2Double u2;u2.l = getParaLong(runtime, pos);pos += 2;
    s32 ustride = env->localvar_getInt(runtime, pos++);
    s32 uorder = env->localvar_getInt(runtime, pos++);
    Long2Double v1;v1.l = getParaLong(runtime, pos);pos += 2;
    Long2Double v2;v2.l = getParaLong(runtime, pos);pos += 2;
    s32 vstride = env->localvar_getInt(runtime, pos++);
    s32 vorder = env->localvar_getInt(runtime, pos++);
    Instance *points = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset9 = env->localvar_getInt(runtime, pos++);
    __refer ptr9 = NULL;
    if(points){
        arr_offset9 *= env->data_type_bytes[points->mb.arr_type_index];
        ptr9 = points->arr_body + arr_offset9;
    }

    glMap2d((GLenum)target, (GLdouble)u1.d, (GLdouble)u2.d, (GLint)ustride, (GLint)uorder, (GLdouble)v1.d, (GLdouble)v2.d, (GLint)vstride, (GLint)vorder, (const GLdouble *)(ptr9));

    return 0;
}

int org_mini_gl_GL_glMap2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Int2Float u1;u1.i = env->localvar_getInt(runtime, pos++);
    Int2Float u2;u2.i = env->localvar_getInt(runtime, pos++);
    s32 ustride = env->localvar_getInt(runtime, pos++);
    s32 uorder = env->localvar_getInt(runtime, pos++);
    Int2Float v1;v1.i = env->localvar_getInt(runtime, pos++);
    Int2Float v2;v2.i = env->localvar_getInt(runtime, pos++);
    s32 vstride = env->localvar_getInt(runtime, pos++);
    s32 vorder = env->localvar_getInt(runtime, pos++);
    Instance *points = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset9 = env->localvar_getInt(runtime, pos++);
    __refer ptr9 = NULL;
    if(points){
        arr_offset9 *= env->data_type_bytes[points->mb.arr_type_index];
        ptr9 = points->arr_body + arr_offset9;
    }

    glMap2f((GLenum)target, (GLfloat)u1.f, (GLfloat)u2.f, (GLint)ustride, (GLint)uorder, (GLfloat)v1.f, (GLfloat)v2.f, (GLint)vstride, (GLint)vorder, (const GLfloat *)(ptr9));

    return 0;
}

int org_mini_gl_GL_glMapGrid1d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 un = env->localvar_getInt(runtime, pos++);
    Long2Double u1;u1.l = getParaLong(runtime, pos);pos += 2;
    Long2Double u2;u2.l = getParaLong(runtime, pos);pos += 2;

    glMapGrid1d((GLint)un, (GLdouble)u1.d, (GLdouble)u2.d);

    return 0;
}

int org_mini_gl_GL_glMapGrid1f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 un = env->localvar_getInt(runtime, pos++);
    Int2Float u1;u1.i = env->localvar_getInt(runtime, pos++);
    Int2Float u2;u2.i = env->localvar_getInt(runtime, pos++);

    glMapGrid1f((GLint)un, (GLfloat)u1.f, (GLfloat)u2.f);

    return 0;
}

int org_mini_gl_GL_glMapGrid2d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 un = env->localvar_getInt(runtime, pos++);
    Long2Double u1;u1.l = getParaLong(runtime, pos);pos += 2;
    Long2Double u2;u2.l = getParaLong(runtime, pos);pos += 2;
    s32 vn = env->localvar_getInt(runtime, pos++);
    Long2Double v1;v1.l = getParaLong(runtime, pos);pos += 2;
    Long2Double v2;v2.l = getParaLong(runtime, pos);pos += 2;

    glMapGrid2d((GLint)un, (GLdouble)u1.d, (GLdouble)u2.d, (GLint)vn, (GLdouble)v1.d, (GLdouble)v2.d);

    return 0;
}

int org_mini_gl_GL_glMapGrid2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 un = env->localvar_getInt(runtime, pos++);
    Int2Float u1;u1.i = env->localvar_getInt(runtime, pos++);
    Int2Float u2;u2.i = env->localvar_getInt(runtime, pos++);
    s32 vn = env->localvar_getInt(runtime, pos++);
    Int2Float v1;v1.i = env->localvar_getInt(runtime, pos++);
    Int2Float v2;v2.i = env->localvar_getInt(runtime, pos++);

    glMapGrid2f((GLint)un, (GLfloat)u1.f, (GLfloat)u2.f, (GLint)vn, (GLfloat)v1.f, (GLfloat)v2.f);

    return 0;
}

int org_mini_gl_GL_glEvalCoord1d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double u;u.l = getParaLong(runtime, pos);pos += 2;

    glEvalCoord1d((GLdouble)u.d);

    return 0;
}

int org_mini_gl_GL_glEvalCoord1dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *u = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(u){
        arr_offset0 *= env->data_type_bytes[u->mb.arr_type_index];
        ptr0 = u->arr_body + arr_offset0;
    }

    glEvalCoord1dv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glEvalCoord1f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float u;u.i = env->localvar_getInt(runtime, pos++);

    glEvalCoord1f((GLfloat)u.f);

    return 0;
}

int org_mini_gl_GL_glEvalCoord1fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *u = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(u){
        arr_offset0 *= env->data_type_bytes[u->mb.arr_type_index];
        ptr0 = u->arr_body + arr_offset0;
    }

    glEvalCoord1fv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glEvalCoord2d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double u;u.l = getParaLong(runtime, pos);pos += 2;
    Long2Double v;v.l = getParaLong(runtime, pos);pos += 2;

    glEvalCoord2d((GLdouble)u.d, (GLdouble)v.d);

    return 0;
}

int org_mini_gl_GL_glEvalCoord2dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *u = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(u){
        arr_offset0 *= env->data_type_bytes[u->mb.arr_type_index];
        ptr0 = u->arr_body + arr_offset0;
    }

    glEvalCoord2dv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glEvalCoord2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float u;u.i = env->localvar_getInt(runtime, pos++);
    Int2Float v;v.i = env->localvar_getInt(runtime, pos++);

    glEvalCoord2f((GLfloat)u.f, (GLfloat)v.f);

    return 0;
}

int org_mini_gl_GL_glEvalCoord2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *u = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(u){
        arr_offset0 *= env->data_type_bytes[u->mb.arr_type_index];
        ptr0 = u->arr_body + arr_offset0;
    }

    glEvalCoord2fv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glEvalMesh1(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);
    s32 i1 = env->localvar_getInt(runtime, pos++);
    s32 i2 = env->localvar_getInt(runtime, pos++);

    glEvalMesh1((GLenum)mode, (GLint)i1, (GLint)i2);

    return 0;
}

int org_mini_gl_GL_glEvalPoint1(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 i = env->localvar_getInt(runtime, pos++);

    glEvalPoint1((GLint)i);

    return 0;
}

int org_mini_gl_GL_glEvalMesh2(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);
    s32 i1 = env->localvar_getInt(runtime, pos++);
    s32 i2 = env->localvar_getInt(runtime, pos++);
    s32 j1 = env->localvar_getInt(runtime, pos++);
    s32 j2 = env->localvar_getInt(runtime, pos++);

    glEvalMesh2((GLenum)mode, (GLint)i1, (GLint)i2, (GLint)j1, (GLint)j2);

    return 0;
}

int org_mini_gl_GL_glEvalPoint2(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 i = env->localvar_getInt(runtime, pos++);
    s32 j = env->localvar_getInt(runtime, pos++);

    glEvalPoint2((GLint)i, (GLint)j);

    return 0;
}

int org_mini_gl_GL_glAlphaFunc(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 func = env->localvar_getInt(runtime, pos++);
    Int2Float ref;ref.i = env->localvar_getInt(runtime, pos++);

    glAlphaFunc((GLenum)func, (GLfloat)ref.f);

    return 0;
}

int org_mini_gl_GL_glPixelZoom(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float xfactor;xfactor.i = env->localvar_getInt(runtime, pos++);
    Int2Float yfactor;yfactor.i = env->localvar_getInt(runtime, pos++);

    glPixelZoom((GLfloat)xfactor.f, (GLfloat)yfactor.f);

    return 0;
}

int org_mini_gl_GL_glPixelTransferf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    Int2Float param;param.i = env->localvar_getInt(runtime, pos++);

    glPixelTransferf((GLenum)pname, (GLfloat)param.f);

    return 0;
}

int org_mini_gl_GL_glPixelTransferi(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    s32 param = env->localvar_getInt(runtime, pos++);

    glPixelTransferi((GLenum)pname, (GLint)param);

    return 0;
}

int org_mini_gl_GL_glPixelMapfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 map = env->localvar_getInt(runtime, pos++);
    s32 mapsize = env->localvar_getInt(runtime, pos++);
    Instance *values = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(values){
        arr_offset2 *= env->data_type_bytes[values->mb.arr_type_index];
        ptr2 = values->arr_body + arr_offset2;
    }

    glPixelMapfv((GLenum)map, (GLsizei)mapsize, (const GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glPixelMapuiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 map = env->localvar_getInt(runtime, pos++);
    s32 mapsize = env->localvar_getInt(runtime, pos++);
    Instance *values = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(values){
        arr_offset2 *= env->data_type_bytes[values->mb.arr_type_index];
        ptr2 = values->arr_body + arr_offset2;
    }

    glPixelMapuiv((GLenum)map, (GLsizei)mapsize, (const GLuint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glPixelMapusv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 map = env->localvar_getInt(runtime, pos++);
    s32 mapsize = env->localvar_getInt(runtime, pos++);
    Instance *values = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(values){
        arr_offset2 *= env->data_type_bytes[values->mb.arr_type_index];
        ptr2 = values->arr_body + arr_offset2;
    }

    glPixelMapusv((GLenum)map, (GLsizei)mapsize, (const GLushort *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glCopyPixels(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);

    glCopyPixels((GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLenum)type);

    return 0;
}

int org_mini_gl_GL_glDrawPixels(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);
    s32 format = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    Instance *pixels = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset4 = env->localvar_getInt(runtime, pos++);
    __refer ptr4 = NULL;
    if(pixels){
        arr_offset4 *= env->data_type_bytes[pixels->mb.arr_type_index];
        ptr4 = pixels->arr_body + arr_offset4;
    }

    glDrawPixels((GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, (const void *)(ptr4));

    return 0;
}

int org_mini_gl_GL_glGetClipPlane(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 plane = env->localvar_getInt(runtime, pos++);
    Instance *equation = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(equation){
        arr_offset1 *= env->data_type_bytes[equation->mb.arr_type_index];
        ptr1 = equation->arr_body + arr_offset1;
    }

    glGetClipPlane((GLenum)plane, (GLdouble *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glGetLightfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 light = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetLightfv((GLenum)light, (GLenum)pname, (GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetLightiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 light = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetLightiv((GLenum)light, (GLenum)pname, (GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetMapdv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 query = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(v){
        arr_offset2 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr2 = v->arr_body + arr_offset2;
    }

    glGetMapdv((GLenum)target, (GLenum)query, (GLdouble *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetMapfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 query = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(v){
        arr_offset2 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr2 = v->arr_body + arr_offset2;
    }

    glGetMapfv((GLenum)target, (GLenum)query, (GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetMapiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 query = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(v){
        arr_offset2 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr2 = v->arr_body + arr_offset2;
    }

    glGetMapiv((GLenum)target, (GLenum)query, (GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetMaterialfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 face = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetMaterialfv((GLenum)face, (GLenum)pname, (GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetMaterialiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 face = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetMaterialiv((GLenum)face, (GLenum)pname, (GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetPixelMapfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 map = env->localvar_getInt(runtime, pos++);
    Instance *values = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(values){
        arr_offset1 *= env->data_type_bytes[values->mb.arr_type_index];
        ptr1 = values->arr_body + arr_offset1;
    }

    glGetPixelMapfv((GLenum)map, (GLfloat *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glGetPixelMapuiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 map = env->localvar_getInt(runtime, pos++);
    Instance *values = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(values){
        arr_offset1 *= env->data_type_bytes[values->mb.arr_type_index];
        ptr1 = values->arr_body + arr_offset1;
    }

    glGetPixelMapuiv((GLenum)map, (GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glGetPixelMapusv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 map = env->localvar_getInt(runtime, pos++);
    Instance *values = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(values){
        arr_offset1 *= env->data_type_bytes[values->mb.arr_type_index];
        ptr1 = values->arr_body + arr_offset1;
    }

    glGetPixelMapusv((GLenum)map, (GLushort *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glGetPolygonStipple(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *mask = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(mask){
        arr_offset0 *= env->data_type_bytes[mask->mb.arr_type_index];
        ptr0 = mask->arr_body + arr_offset0;
    }

    glGetPolygonStipple((GLubyte *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glGetTexEnvfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetTexEnvfv((GLenum)target, (GLenum)pname, (GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetTexEnviv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetTexEnviv((GLenum)target, (GLenum)pname, (GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetTexGendv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 coord = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetTexGendv((GLenum)coord, (GLenum)pname, (GLdouble *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetTexGenfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 coord = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetTexGenfv((GLenum)coord, (GLenum)pname, (GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetTexGeniv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 coord = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetTexGeniv((GLenum)coord, (GLenum)pname, (GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glIsList(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 list = env->localvar_getInt(runtime, pos++);

    s32 ret_value = (s32)glIsList((GLuint)list);
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glFrustum(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double left;left.l = getParaLong(runtime, pos);pos += 2;
    Long2Double right;right.l = getParaLong(runtime, pos);pos += 2;
    Long2Double bottom;bottom.l = getParaLong(runtime, pos);pos += 2;
    Long2Double top;top.l = getParaLong(runtime, pos);pos += 2;
    Long2Double zNear;zNear.l = getParaLong(runtime, pos);pos += 2;
    Long2Double zFar;zFar.l = getParaLong(runtime, pos);pos += 2;

    glFrustum((GLdouble)left.d, (GLdouble)right.d, (GLdouble)bottom.d, (GLdouble)top.d, (GLdouble)zNear.d, (GLdouble)zFar.d);

    return 0;
}

int org_mini_gl_GL_glLoadIdentity(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;


    glLoadIdentity();

    return 0;
}

int org_mini_gl_GL_glLoadMatrixf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *m = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(m){
        arr_offset0 *= env->data_type_bytes[m->mb.arr_type_index];
        ptr0 = m->arr_body + arr_offset0;
    }

    glLoadMatrixf((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glLoadMatrixd(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *m = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(m){
        arr_offset0 *= env->data_type_bytes[m->mb.arr_type_index];
        ptr0 = m->arr_body + arr_offset0;
    }

    glLoadMatrixd((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glMatrixMode(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);

    glMatrixMode((GLenum)mode);

    return 0;
}

int org_mini_gl_GL_glMultMatrixf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *m = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(m){
        arr_offset0 *= env->data_type_bytes[m->mb.arr_type_index];
        ptr0 = m->arr_body + arr_offset0;
    }

    glMultMatrixf((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glMultMatrixd(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *m = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(m){
        arr_offset0 *= env->data_type_bytes[m->mb.arr_type_index];
        ptr0 = m->arr_body + arr_offset0;
    }

    glMultMatrixd((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glOrtho(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double left;left.l = getParaLong(runtime, pos);pos += 2;
    Long2Double right;right.l = getParaLong(runtime, pos);pos += 2;
    Long2Double bottom;bottom.l = getParaLong(runtime, pos);pos += 2;
    Long2Double top;top.l = getParaLong(runtime, pos);pos += 2;
    Long2Double zNear;zNear.l = getParaLong(runtime, pos);pos += 2;
    Long2Double zFar;zFar.l = getParaLong(runtime, pos);pos += 2;

    glOrtho((GLdouble)left.d, (GLdouble)right.d, (GLdouble)bottom.d, (GLdouble)top.d, (GLdouble)zNear.d, (GLdouble)zFar.d);

    return 0;
}

int org_mini_gl_GL_glPopMatrix(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;


    glPopMatrix();

    return 0;
}

int org_mini_gl_GL_glPushMatrix(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;


    glPushMatrix();

    return 0;
}

int org_mini_gl_GL_glRotated(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double angle;angle.l = getParaLong(runtime, pos);pos += 2;
    Long2Double x;x.l = getParaLong(runtime, pos);pos += 2;
    Long2Double y;y.l = getParaLong(runtime, pos);pos += 2;
    Long2Double z;z.l = getParaLong(runtime, pos);pos += 2;

    glRotated((GLdouble)angle.d, (GLdouble)x.d, (GLdouble)y.d, (GLdouble)z.d);

    return 0;
}

int org_mini_gl_GL_glRotatef(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float angle;angle.i = env->localvar_getInt(runtime, pos++);
    Int2Float x;x.i = env->localvar_getInt(runtime, pos++);
    Int2Float y;y.i = env->localvar_getInt(runtime, pos++);
    Int2Float z;z.i = env->localvar_getInt(runtime, pos++);

    glRotatef((GLfloat)angle.f, (GLfloat)x.f, (GLfloat)y.f, (GLfloat)z.f);

    return 0;
}

int org_mini_gl_GL_glScaled(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double x;x.l = getParaLong(runtime, pos);pos += 2;
    Long2Double y;y.l = getParaLong(runtime, pos);pos += 2;
    Long2Double z;z.l = getParaLong(runtime, pos);pos += 2;

    glScaled((GLdouble)x.d, (GLdouble)y.d, (GLdouble)z.d);

    return 0;
}

int org_mini_gl_GL_glScalef(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float x;x.i = env->localvar_getInt(runtime, pos++);
    Int2Float y;y.i = env->localvar_getInt(runtime, pos++);
    Int2Float z;z.i = env->localvar_getInt(runtime, pos++);

    glScalef((GLfloat)x.f, (GLfloat)y.f, (GLfloat)z.f);

    return 0;
}

int org_mini_gl_GL_glTranslated(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double x;x.l = getParaLong(runtime, pos);pos += 2;
    Long2Double y;y.l = getParaLong(runtime, pos);pos += 2;
    Long2Double z;z.l = getParaLong(runtime, pos);pos += 2;

    glTranslated((GLdouble)x.d, (GLdouble)y.d, (GLdouble)z.d);

    return 0;
}

int org_mini_gl_GL_glTranslatef(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float x;x.i = env->localvar_getInt(runtime, pos++);
    Int2Float y;y.i = env->localvar_getInt(runtime, pos++);
    Int2Float z;z.i = env->localvar_getInt(runtime, pos++);

    glTranslatef((GLfloat)x.f, (GLfloat)y.f, (GLfloat)z.f);

    return 0;
}

int org_mini_gl_GL_glDrawArrays(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);
    s32 first = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);

    glDrawArrays((GLenum)mode, (GLint)first, (GLsizei)count);

    return 0;
}

int org_mini_gl_GL_glDrawElements(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    Instance *indices = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(indices){
        arr_offset3 *= env->data_type_bytes[indices->mb.arr_type_index];
        ptr3 = indices->arr_body + arr_offset3;
    }

    glDrawElements((GLenum)mode, (GLsizei)count, (GLenum)type, (const void *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glGetPointerv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(params){
        arr_offset1 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr1 = params->arr_body + arr_offset1;
    }

    glGetPointerv((GLenum)pname, (void **)(ptr1));

    return 0;
}

int org_mini_gl_GL_glPolygonOffset(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float factor;factor.i = env->localvar_getInt(runtime, pos++);
    Int2Float units;units.i = env->localvar_getInt(runtime, pos++);

    glPolygonOffset((GLfloat)factor.f, (GLfloat)units.f);

    return 0;
}

int org_mini_gl_GL_glCopyTexImage1D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 internalformat = env->localvar_getInt(runtime, pos++);
    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 border = env->localvar_getInt(runtime, pos++);

    glCopyTexImage1D((GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLint)border);

    return 0;
}

int org_mini_gl_GL_glCopyTexImage2D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 internalformat = env->localvar_getInt(runtime, pos++);
    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);
    s32 border = env->localvar_getInt(runtime, pos++);

    glCopyTexImage2D((GLenum)target, (GLint)level, (GLenum)internalformat, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLint)border);

    return 0;
}

int org_mini_gl_GL_glCopyTexSubImage1D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 xoffset = env->localvar_getInt(runtime, pos++);
    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);

    glCopyTexSubImage1D((GLenum)target, (GLint)level, (GLint)xoffset, (GLint)x, (GLint)y, (GLsizei)width);

    return 0;
}

int org_mini_gl_GL_glCopyTexSubImage2D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 xoffset = env->localvar_getInt(runtime, pos++);
    s32 yoffset = env->localvar_getInt(runtime, pos++);
    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);

    glCopyTexSubImage2D((GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height);

    return 0;
}

int org_mini_gl_GL_glTexSubImage1D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 xoffset = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 format = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    Instance *pixels = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset6 = env->localvar_getInt(runtime, pos++);
    __refer ptr6 = NULL;
    if(pixels){
        arr_offset6 *= env->data_type_bytes[pixels->mb.arr_type_index];
        ptr6 = pixels->arr_body + arr_offset6;
    }

    glTexSubImage1D((GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLenum)type, (const void *)(ptr6));

    return 0;
}

int org_mini_gl_GL_glTexSubImage2D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 xoffset = env->localvar_getInt(runtime, pos++);
    s32 yoffset = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);
    s32 format = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    Instance *pixels = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset8 = env->localvar_getInt(runtime, pos++);
    __refer ptr8 = NULL;
    if(pixels){
        arr_offset8 *= env->data_type_bytes[pixels->mb.arr_type_index];
        ptr8 = pixels->arr_body + arr_offset8;
    }

    glTexSubImage2D((GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, (const void *)(ptr8));

    return 0;
}

int org_mini_gl_GL_glBindTexture(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 texture = env->localvar_getInt(runtime, pos++);

    glBindTexture((GLenum)target, (GLuint)texture);

    return 0;
}

int org_mini_gl_GL_glDeleteTextures(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 n = env->localvar_getInt(runtime, pos++);
    Instance *textures = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(textures){
        arr_offset1 *= env->data_type_bytes[textures->mb.arr_type_index];
        ptr1 = textures->arr_body + arr_offset1;
    }

    glDeleteTextures((GLsizei)n, (const GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glGenTextures(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 n = env->localvar_getInt(runtime, pos++);
    Instance *textures = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(textures){
        arr_offset1 *= env->data_type_bytes[textures->mb.arr_type_index];
        ptr1 = textures->arr_body + arr_offset1;
    }

    glGenTextures((GLsizei)n, (GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glIsTexture(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 texture = env->localvar_getInt(runtime, pos++);

    s32 ret_value = (s32)glIsTexture((GLuint)texture);
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glArrayElement(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 i = env->localvar_getInt(runtime, pos++);

    glArrayElement((GLint)i);

    return 0;
}

int org_mini_gl_GL_glColorPointer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 size = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    s32 stride = env->localvar_getInt(runtime, pos++);
    Instance *pointer = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(pointer){
        arr_offset3 *= env->data_type_bytes[pointer->mb.arr_type_index];
        ptr3 = pointer->arr_body + arr_offset3;
    }

    glColorPointer((GLint)size, (GLenum)type, (GLsizei)stride, (const void *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glDisableClientState(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 array = env->localvar_getInt(runtime, pos++);

    glDisableClientState((GLenum)array);

    return 0;
}

int org_mini_gl_GL_glEdgeFlagPointer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 stride = env->localvar_getInt(runtime, pos++);
    Instance *pointer = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(pointer){
        arr_offset1 *= env->data_type_bytes[pointer->mb.arr_type_index];
        ptr1 = pointer->arr_body + arr_offset1;
    }

    glEdgeFlagPointer((GLsizei)stride, (const void *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glEnableClientState(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 array = env->localvar_getInt(runtime, pos++);

    glEnableClientState((GLenum)array);

    return 0;
}

int org_mini_gl_GL_glIndexPointer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 type = env->localvar_getInt(runtime, pos++);
    s32 stride = env->localvar_getInt(runtime, pos++);
    Instance *pointer = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(pointer){
        arr_offset2 *= env->data_type_bytes[pointer->mb.arr_type_index];
        ptr2 = pointer->arr_body + arr_offset2;
    }

    glIndexPointer((GLenum)type, (GLsizei)stride, (const void *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glInterleavedArrays(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 format = env->localvar_getInt(runtime, pos++);
    s32 stride = env->localvar_getInt(runtime, pos++);
    Instance *pointer = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(pointer){
        arr_offset2 *= env->data_type_bytes[pointer->mb.arr_type_index];
        ptr2 = pointer->arr_body + arr_offset2;
    }

    glInterleavedArrays((GLenum)format, (GLsizei)stride, (const void *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glNormalPointer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 type = env->localvar_getInt(runtime, pos++);
    s32 stride = env->localvar_getInt(runtime, pos++);
    Instance *pointer = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(pointer){
        arr_offset2 *= env->data_type_bytes[pointer->mb.arr_type_index];
        ptr2 = pointer->arr_body + arr_offset2;
    }

    glNormalPointer((GLenum)type, (GLsizei)stride, (const void *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glTexCoordPointer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 size = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    s32 stride = env->localvar_getInt(runtime, pos++);
    Instance *pointer = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(pointer){
        arr_offset3 *= env->data_type_bytes[pointer->mb.arr_type_index];
        ptr3 = pointer->arr_body + arr_offset3;
    }

    glTexCoordPointer((GLint)size, (GLenum)type, (GLsizei)stride, (const void *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glVertexPointer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 size = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    s32 stride = env->localvar_getInt(runtime, pos++);
    Instance *pointer = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(pointer){
        arr_offset3 *= env->data_type_bytes[pointer->mb.arr_type_index];
        ptr3 = pointer->arr_body + arr_offset3;
    }

    glVertexPointer((GLint)size, (GLenum)type, (GLsizei)stride, (const void *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glAreTexturesResident(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 n = env->localvar_getInt(runtime, pos++);
    Instance *textures = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(textures){
        arr_offset1 *= env->data_type_bytes[textures->mb.arr_type_index];
        ptr1 = textures->arr_body + arr_offset1;
    }
    Instance *residences = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(residences){
        arr_offset3 *= env->data_type_bytes[residences->mb.arr_type_index];
        ptr3 = residences->arr_body + arr_offset3;
    }

    s32 ret_value = (s32)glAreTexturesResident((GLsizei)n, (const GLuint *)(ptr1), (GLboolean *)(ptr3));
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glPrioritizeTextures(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 n = env->localvar_getInt(runtime, pos++);
    Instance *textures = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(textures){
        arr_offset1 *= env->data_type_bytes[textures->mb.arr_type_index];
        ptr1 = textures->arr_body + arr_offset1;
    }
    Instance *priorities = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(priorities){
        arr_offset3 *= env->data_type_bytes[priorities->mb.arr_type_index];
        ptr3 = priorities->arr_body + arr_offset3;
    }

    glPrioritizeTextures((GLsizei)n, (const GLuint *)(ptr1), (const GLfloat *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glIndexub(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 c = env->localvar_getInt(runtime, pos++);

    glIndexub((GLubyte)c);

    return 0;
}

int org_mini_gl_GL_glIndexubv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *c = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(c){
        arr_offset0 *= env->data_type_bytes[c->mb.arr_type_index];
        ptr0 = c->arr_body + arr_offset0;
    }

    glIndexubv((const GLubyte *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glPopClientAttrib(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;


    glPopClientAttrib();

    return 0;
}

int org_mini_gl_GL_glPushClientAttrib(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mask = env->localvar_getInt(runtime, pos++);

    glPushClientAttrib((GLbitfield)mask);

    return 0;
}

int org_mini_gl_GL_glDrawRangeElements(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);
    s32 start = env->localvar_getInt(runtime, pos++);
    s32 end = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    Instance *indices = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(indices){
        arr_offset5 *= env->data_type_bytes[indices->mb.arr_type_index];
        ptr5 = indices->arr_body + arr_offset5;
    }

    glDrawRangeElements((GLenum)mode, (GLuint)start, (GLuint)end, (GLsizei)count, (GLenum)type, (const void *)(ptr5));

    return 0;
}

int org_mini_gl_GL_glTexImage3D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 internalformat = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);
    s32 depth = env->localvar_getInt(runtime, pos++);
    s32 border = env->localvar_getInt(runtime, pos++);
    s32 format = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    Instance *pixels = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset9 = env->localvar_getInt(runtime, pos++);
    __refer ptr9 = NULL;
    if(pixels){
        arr_offset9 *= env->data_type_bytes[pixels->mb.arr_type_index];
        ptr9 = pixels->arr_body + arr_offset9;
    }

    glTexImage3D((GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLenum)format, (GLenum)type, (const void *)(ptr9));

    return 0;
}

int org_mini_gl_GL_glTexSubImage3D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 xoffset = env->localvar_getInt(runtime, pos++);
    s32 yoffset = env->localvar_getInt(runtime, pos++);
    s32 zoffset = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);
    s32 depth = env->localvar_getInt(runtime, pos++);
    s32 format = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    Instance *pixels = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset10 = env->localvar_getInt(runtime, pos++);
    __refer ptr10 = NULL;
    if(pixels){
        arr_offset10 *= env->data_type_bytes[pixels->mb.arr_type_index];
        ptr10 = pixels->arr_body + arr_offset10;
    }

    glTexSubImage3D((GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLenum)type, (const void *)(ptr10));

    return 0;
}

int org_mini_gl_GL_glCopyTexSubImage3D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 xoffset = env->localvar_getInt(runtime, pos++);
    s32 yoffset = env->localvar_getInt(runtime, pos++);
    s32 zoffset = env->localvar_getInt(runtime, pos++);
    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);

    glCopyTexSubImage3D((GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height);

    return 0;
}

int org_mini_gl_GL_glActiveTexture(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 texture = env->localvar_getInt(runtime, pos++);

    glActiveTexture((GLenum)texture);

    return 0;
}

int org_mini_gl_GL_glSampleCoverage(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float value;value.i = env->localvar_getInt(runtime, pos++);
    s32 invert = env->localvar_getInt(runtime, pos++);

    glSampleCoverage((GLfloat)value.f, (GLboolean)invert);

    return 0;
}

int org_mini_gl_GL_glCompressedTexImage3D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 internalformat = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);
    s32 depth = env->localvar_getInt(runtime, pos++);
    s32 border = env->localvar_getInt(runtime, pos++);
    s32 imageSize = env->localvar_getInt(runtime, pos++);
    Instance *data = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset8 = env->localvar_getInt(runtime, pos++);
    __refer ptr8 = NULL;
    if(data){
        arr_offset8 *= env->data_type_bytes[data->mb.arr_type_index];
        ptr8 = data->arr_body + arr_offset8;
    }

    glCompressedTexImage3D((GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLint)border, (GLsizei)imageSize, (const void *)(ptr8));

    return 0;
}

int org_mini_gl_GL_glCompressedTexImage2D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 internalformat = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);
    s32 border = env->localvar_getInt(runtime, pos++);
    s32 imageSize = env->localvar_getInt(runtime, pos++);
    Instance *data = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset7 = env->localvar_getInt(runtime, pos++);
    __refer ptr7 = NULL;
    if(data){
        arr_offset7 *= env->data_type_bytes[data->mb.arr_type_index];
        ptr7 = data->arr_body + arr_offset7;
    }

    glCompressedTexImage2D((GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLsizei)imageSize, (const void *)(ptr7));

    return 0;
}

int org_mini_gl_GL_glCompressedTexImage1D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 internalformat = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 border = env->localvar_getInt(runtime, pos++);
    s32 imageSize = env->localvar_getInt(runtime, pos++);
    Instance *data = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset6 = env->localvar_getInt(runtime, pos++);
    __refer ptr6 = NULL;
    if(data){
        arr_offset6 *= env->data_type_bytes[data->mb.arr_type_index];
        ptr6 = data->arr_body + arr_offset6;
    }

    glCompressedTexImage1D((GLenum)target, (GLint)level, (GLenum)internalformat, (GLsizei)width, (GLint)border, (GLsizei)imageSize, (const void *)(ptr6));

    return 0;
}

int org_mini_gl_GL_glCompressedTexSubImage3D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 xoffset = env->localvar_getInt(runtime, pos++);
    s32 yoffset = env->localvar_getInt(runtime, pos++);
    s32 zoffset = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);
    s32 depth = env->localvar_getInt(runtime, pos++);
    s32 format = env->localvar_getInt(runtime, pos++);
    s32 imageSize = env->localvar_getInt(runtime, pos++);
    Instance *data = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset10 = env->localvar_getInt(runtime, pos++);
    __refer ptr10 = NULL;
    if(data){
        arr_offset10 *= env->data_type_bytes[data->mb.arr_type_index];
        ptr10 = data->arr_body + arr_offset10;
    }

    glCompressedTexSubImage3D((GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLint)zoffset, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLenum)format, (GLsizei)imageSize, (const void *)(ptr10));

    return 0;
}

int org_mini_gl_GL_glCompressedTexSubImage2D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 xoffset = env->localvar_getInt(runtime, pos++);
    s32 yoffset = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);
    s32 format = env->localvar_getInt(runtime, pos++);
    s32 imageSize = env->localvar_getInt(runtime, pos++);
    Instance *data = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset8 = env->localvar_getInt(runtime, pos++);
    __refer ptr8 = NULL;
    if(data){
        arr_offset8 *= env->data_type_bytes[data->mb.arr_type_index];
        ptr8 = data->arr_body + arr_offset8;
    }

    glCompressedTexSubImage2D((GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLsizei)imageSize, (const void *)(ptr8));

    return 0;
}

int org_mini_gl_GL_glCompressedTexSubImage1D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 xoffset = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 format = env->localvar_getInt(runtime, pos++);
    s32 imageSize = env->localvar_getInt(runtime, pos++);
    Instance *data = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset6 = env->localvar_getInt(runtime, pos++);
    __refer ptr6 = NULL;
    if(data){
        arr_offset6 *= env->data_type_bytes[data->mb.arr_type_index];
        ptr6 = data->arr_body + arr_offset6;
    }

    glCompressedTexSubImage1D((GLenum)target, (GLint)level, (GLint)xoffset, (GLsizei)width, (GLenum)format, (GLsizei)imageSize, (const void *)(ptr6));

    return 0;
}

int org_mini_gl_GL_glGetCompressedTexImage(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    Instance *img = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(img){
        arr_offset2 *= env->data_type_bytes[img->mb.arr_type_index];
        ptr2 = img->arr_body + arr_offset2;
    }

    glGetCompressedTexImage((GLenum)target, (GLint)level, (void *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glClientActiveTexture(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 texture = env->localvar_getInt(runtime, pos++);

    glClientActiveTexture((GLenum)texture);

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord1d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Long2Double s;s.l = getParaLong(runtime, pos);pos += 2;

    glMultiTexCoord1d((GLenum)target, (GLdouble)s.d);

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord1dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glMultiTexCoord1dv((GLenum)target, (const GLdouble *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord1f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Int2Float s;s.i = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord1f((GLenum)target, (GLfloat)s.f);

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord1fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glMultiTexCoord1fv((GLenum)target, (const GLfloat *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord1i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 s = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord1i((GLenum)target, (GLint)s);

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord1iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glMultiTexCoord1iv((GLenum)target, (const GLint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord1s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 s = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord1s((GLenum)target, (GLshort)s);

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord1sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glMultiTexCoord1sv((GLenum)target, (const GLshort *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord2d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Long2Double s;s.l = getParaLong(runtime, pos);pos += 2;
    Long2Double t;t.l = getParaLong(runtime, pos);pos += 2;

    glMultiTexCoord2d((GLenum)target, (GLdouble)s.d, (GLdouble)t.d);

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord2dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glMultiTexCoord2dv((GLenum)target, (const GLdouble *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Int2Float s;s.i = env->localvar_getInt(runtime, pos++);
    Int2Float t;t.i = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord2f((GLenum)target, (GLfloat)s.f, (GLfloat)t.f);

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glMultiTexCoord2fv((GLenum)target, (const GLfloat *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord2i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 s = env->localvar_getInt(runtime, pos++);
    s32 t = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord2i((GLenum)target, (GLint)s, (GLint)t);

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord2iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glMultiTexCoord2iv((GLenum)target, (const GLint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord2s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 s = env->localvar_getInt(runtime, pos++);
    s32 t = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord2s((GLenum)target, (GLshort)s, (GLshort)t);

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord2sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glMultiTexCoord2sv((GLenum)target, (const GLshort *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord3d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Long2Double s;s.l = getParaLong(runtime, pos);pos += 2;
    Long2Double t;t.l = getParaLong(runtime, pos);pos += 2;
    Long2Double r;r.l = getParaLong(runtime, pos);pos += 2;

    glMultiTexCoord3d((GLenum)target, (GLdouble)s.d, (GLdouble)t.d, (GLdouble)r.d);

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord3dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glMultiTexCoord3dv((GLenum)target, (const GLdouble *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Int2Float s;s.i = env->localvar_getInt(runtime, pos++);
    Int2Float t;t.i = env->localvar_getInt(runtime, pos++);
    Int2Float r;r.i = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord3f((GLenum)target, (GLfloat)s.f, (GLfloat)t.f, (GLfloat)r.f);

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glMultiTexCoord3fv((GLenum)target, (const GLfloat *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 s = env->localvar_getInt(runtime, pos++);
    s32 t = env->localvar_getInt(runtime, pos++);
    s32 r = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord3i((GLenum)target, (GLint)s, (GLint)t, (GLint)r);

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glMultiTexCoord3iv((GLenum)target, (const GLint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord3s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 s = env->localvar_getInt(runtime, pos++);
    s32 t = env->localvar_getInt(runtime, pos++);
    s32 r = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord3s((GLenum)target, (GLshort)s, (GLshort)t, (GLshort)r);

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord3sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glMultiTexCoord3sv((GLenum)target, (const GLshort *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord4d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Long2Double s;s.l = getParaLong(runtime, pos);pos += 2;
    Long2Double t;t.l = getParaLong(runtime, pos);pos += 2;
    Long2Double r;r.l = getParaLong(runtime, pos);pos += 2;
    Long2Double q;q.l = getParaLong(runtime, pos);pos += 2;

    glMultiTexCoord4d((GLenum)target, (GLdouble)s.d, (GLdouble)t.d, (GLdouble)r.d, (GLdouble)q.d);

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord4dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glMultiTexCoord4dv((GLenum)target, (const GLdouble *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord4f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Int2Float s;s.i = env->localvar_getInt(runtime, pos++);
    Int2Float t;t.i = env->localvar_getInt(runtime, pos++);
    Int2Float r;r.i = env->localvar_getInt(runtime, pos++);
    Int2Float q;q.i = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord4f((GLenum)target, (GLfloat)s.f, (GLfloat)t.f, (GLfloat)r.f, (GLfloat)q.f);

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glMultiTexCoord4fv((GLenum)target, (const GLfloat *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord4i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 s = env->localvar_getInt(runtime, pos++);
    s32 t = env->localvar_getInt(runtime, pos++);
    s32 r = env->localvar_getInt(runtime, pos++);
    s32 q = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord4i((GLenum)target, (GLint)s, (GLint)t, (GLint)r, (GLint)q);

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord4iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glMultiTexCoord4iv((GLenum)target, (const GLint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord4s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 s = env->localvar_getInt(runtime, pos++);
    s32 t = env->localvar_getInt(runtime, pos++);
    s32 r = env->localvar_getInt(runtime, pos++);
    s32 q = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord4s((GLenum)target, (GLshort)s, (GLshort)t, (GLshort)r, (GLshort)q);

    return 0;
}

int org_mini_gl_GL_glMultiTexCoord4sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glMultiTexCoord4sv((GLenum)target, (const GLshort *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glLoadTransposeMatrixf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *m = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(m){
        arr_offset0 *= env->data_type_bytes[m->mb.arr_type_index];
        ptr0 = m->arr_body + arr_offset0;
    }

    glLoadTransposeMatrixf((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glLoadTransposeMatrixd(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *m = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(m){
        arr_offset0 *= env->data_type_bytes[m->mb.arr_type_index];
        ptr0 = m->arr_body + arr_offset0;
    }

    glLoadTransposeMatrixd((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glMultTransposeMatrixf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *m = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(m){
        arr_offset0 *= env->data_type_bytes[m->mb.arr_type_index];
        ptr0 = m->arr_body + arr_offset0;
    }

    glMultTransposeMatrixf((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glMultTransposeMatrixd(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *m = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(m){
        arr_offset0 *= env->data_type_bytes[m->mb.arr_type_index];
        ptr0 = m->arr_body + arr_offset0;
    }

    glMultTransposeMatrixd((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glBlendFuncSeparate(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 sfactorRGB = env->localvar_getInt(runtime, pos++);
    s32 dfactorRGB = env->localvar_getInt(runtime, pos++);
    s32 sfactorAlpha = env->localvar_getInt(runtime, pos++);
    s32 dfactorAlpha = env->localvar_getInt(runtime, pos++);

    glBlendFuncSeparate((GLenum)sfactorRGB, (GLenum)dfactorRGB, (GLenum)sfactorAlpha, (GLenum)dfactorAlpha);

    return 0;
}

int org_mini_gl_GL_glMultiDrawArrays(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);
    Instance *first = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(first){
        arr_offset1 *= env->data_type_bytes[first->mb.arr_type_index];
        ptr1 = first->arr_body + arr_offset1;
    }
    Instance *count = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(count){
        arr_offset3 *= env->data_type_bytes[count->mb.arr_type_index];
        ptr3 = count->arr_body + arr_offset3;
    }
    s32 drawcount = env->localvar_getInt(runtime, pos++);

    glMultiDrawArrays((GLenum)mode, (const GLint *)(ptr1), (const GLsizei *)(ptr3), (GLsizei)drawcount);

    return 0;
}

int org_mini_gl_GL_glMultiDrawElements(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);
    Instance *count = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(count){
        arr_offset1 *= env->data_type_bytes[count->mb.arr_type_index];
        ptr1 = count->arr_body + arr_offset1;
    }
    s32 type = env->localvar_getInt(runtime, pos++);
    Instance *indices = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset4 = env->localvar_getInt(runtime, pos++);
    __refer ptr4 = NULL;
    if(indices){
        arr_offset4 *= env->data_type_bytes[indices->mb.arr_type_index];
        ptr4 = indices->arr_body + arr_offset4;
    }
    s32 drawcount = env->localvar_getInt(runtime, pos++);

    glMultiDrawElements((GLenum)mode, (const GLsizei *)(ptr1), (GLenum)type, (const void *const*)(ptr4), (GLsizei)drawcount);

    return 0;
}

int org_mini_gl_GL_glPointParameterf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    Int2Float param;param.i = env->localvar_getInt(runtime, pos++);

    glPointParameterf((GLenum)pname, (GLfloat)param.f);

    return 0;
}

int org_mini_gl_GL_glPointParameterfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(params){
        arr_offset1 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr1 = params->arr_body + arr_offset1;
    }

    glPointParameterfv((GLenum)pname, (const GLfloat *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glPointParameteri(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    s32 param = env->localvar_getInt(runtime, pos++);

    glPointParameteri((GLenum)pname, (GLint)param);

    return 0;
}

int org_mini_gl_GL_glPointParameteriv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(params){
        arr_offset1 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr1 = params->arr_body + arr_offset1;
    }

    glPointParameteriv((GLenum)pname, (const GLint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glFogCoordf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float coord;coord.i = env->localvar_getInt(runtime, pos++);

    glFogCoordf((GLfloat)coord.f);

    return 0;
}

int org_mini_gl_GL_glFogCoordfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *coord = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(coord){
        arr_offset0 *= env->data_type_bytes[coord->mb.arr_type_index];
        ptr0 = coord->arr_body + arr_offset0;
    }

    glFogCoordfv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glFogCoordd(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double coord;coord.l = getParaLong(runtime, pos);pos += 2;

    glFogCoordd((GLdouble)coord.d);

    return 0;
}

int org_mini_gl_GL_glFogCoorddv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *coord = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(coord){
        arr_offset0 *= env->data_type_bytes[coord->mb.arr_type_index];
        ptr0 = coord->arr_body + arr_offset0;
    }

    glFogCoorddv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glFogCoordPointer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 type = env->localvar_getInt(runtime, pos++);
    s32 stride = env->localvar_getInt(runtime, pos++);
    Instance *pointer = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(pointer){
        arr_offset2 *= env->data_type_bytes[pointer->mb.arr_type_index];
        ptr2 = pointer->arr_body + arr_offset2;
    }

    glFogCoordPointer((GLenum)type, (GLsizei)stride, (const void *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glSecondaryColor3b(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 red = env->localvar_getInt(runtime, pos++);
    s32 green = env->localvar_getInt(runtime, pos++);
    s32 blue = env->localvar_getInt(runtime, pos++);

    glSecondaryColor3b((GLbyte)red, (GLbyte)green, (GLbyte)blue);

    return 0;
}

int org_mini_gl_GL_glSecondaryColor3bv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glSecondaryColor3bv((const GLbyte *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glSecondaryColor3d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double red;red.l = getParaLong(runtime, pos);pos += 2;
    Long2Double green;green.l = getParaLong(runtime, pos);pos += 2;
    Long2Double blue;blue.l = getParaLong(runtime, pos);pos += 2;

    glSecondaryColor3d((GLdouble)red.d, (GLdouble)green.d, (GLdouble)blue.d);

    return 0;
}

int org_mini_gl_GL_glSecondaryColor3dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glSecondaryColor3dv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glSecondaryColor3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float red;red.i = env->localvar_getInt(runtime, pos++);
    Int2Float green;green.i = env->localvar_getInt(runtime, pos++);
    Int2Float blue;blue.i = env->localvar_getInt(runtime, pos++);

    glSecondaryColor3f((GLfloat)red.f, (GLfloat)green.f, (GLfloat)blue.f);

    return 0;
}

int org_mini_gl_GL_glSecondaryColor3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glSecondaryColor3fv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glSecondaryColor3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 red = env->localvar_getInt(runtime, pos++);
    s32 green = env->localvar_getInt(runtime, pos++);
    s32 blue = env->localvar_getInt(runtime, pos++);

    glSecondaryColor3i((GLint)red, (GLint)green, (GLint)blue);

    return 0;
}

int org_mini_gl_GL_glSecondaryColor3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glSecondaryColor3iv((const GLint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glSecondaryColor3s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 red = env->localvar_getInt(runtime, pos++);
    s32 green = env->localvar_getInt(runtime, pos++);
    s32 blue = env->localvar_getInt(runtime, pos++);

    glSecondaryColor3s((GLshort)red, (GLshort)green, (GLshort)blue);

    return 0;
}

int org_mini_gl_GL_glSecondaryColor3sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glSecondaryColor3sv((const GLshort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glSecondaryColor3ub(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 red = env->localvar_getInt(runtime, pos++);
    s32 green = env->localvar_getInt(runtime, pos++);
    s32 blue = env->localvar_getInt(runtime, pos++);

    glSecondaryColor3ub((GLubyte)red, (GLubyte)green, (GLubyte)blue);

    return 0;
}

int org_mini_gl_GL_glSecondaryColor3ubv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glSecondaryColor3ubv((const GLubyte *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glSecondaryColor3ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 red = env->localvar_getInt(runtime, pos++);
    s32 green = env->localvar_getInt(runtime, pos++);
    s32 blue = env->localvar_getInt(runtime, pos++);

    glSecondaryColor3ui((GLuint)red, (GLuint)green, (GLuint)blue);

    return 0;
}

int org_mini_gl_GL_glSecondaryColor3uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glSecondaryColor3uiv((const GLuint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glSecondaryColor3us(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 red = env->localvar_getInt(runtime, pos++);
    s32 green = env->localvar_getInt(runtime, pos++);
    s32 blue = env->localvar_getInt(runtime, pos++);

    glSecondaryColor3us((GLushort)red, (GLushort)green, (GLushort)blue);

    return 0;
}

int org_mini_gl_GL_glSecondaryColor3usv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glSecondaryColor3usv((const GLushort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glSecondaryColorPointer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 size = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    s32 stride = env->localvar_getInt(runtime, pos++);
    Instance *pointer = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(pointer){
        arr_offset3 *= env->data_type_bytes[pointer->mb.arr_type_index];
        ptr3 = pointer->arr_body + arr_offset3;
    }

    glSecondaryColorPointer((GLint)size, (GLenum)type, (GLsizei)stride, (const void *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glWindowPos2d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double x;x.l = getParaLong(runtime, pos);pos += 2;
    Long2Double y;y.l = getParaLong(runtime, pos);pos += 2;

    glWindowPos2d((GLdouble)x.d, (GLdouble)y.d);

    return 0;
}

int org_mini_gl_GL_glWindowPos2dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glWindowPos2dv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glWindowPos2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float x;x.i = env->localvar_getInt(runtime, pos++);
    Int2Float y;y.i = env->localvar_getInt(runtime, pos++);

    glWindowPos2f((GLfloat)x.f, (GLfloat)y.f);

    return 0;
}

int org_mini_gl_GL_glWindowPos2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glWindowPos2fv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glWindowPos2i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);

    glWindowPos2i((GLint)x, (GLint)y);

    return 0;
}

int org_mini_gl_GL_glWindowPos2iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glWindowPos2iv((const GLint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glWindowPos2s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);

    glWindowPos2s((GLshort)x, (GLshort)y);

    return 0;
}

int org_mini_gl_GL_glWindowPos2sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glWindowPos2sv((const GLshort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glWindowPos3d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Long2Double x;x.l = getParaLong(runtime, pos);pos += 2;
    Long2Double y;y.l = getParaLong(runtime, pos);pos += 2;
    Long2Double z;z.l = getParaLong(runtime, pos);pos += 2;

    glWindowPos3d((GLdouble)x.d, (GLdouble)y.d, (GLdouble)z.d);

    return 0;
}

int org_mini_gl_GL_glWindowPos3dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glWindowPos3dv((const GLdouble *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glWindowPos3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float x;x.i = env->localvar_getInt(runtime, pos++);
    Int2Float y;y.i = env->localvar_getInt(runtime, pos++);
    Int2Float z;z.i = env->localvar_getInt(runtime, pos++);

    glWindowPos3f((GLfloat)x.f, (GLfloat)y.f, (GLfloat)z.f);

    return 0;
}

int org_mini_gl_GL_glWindowPos3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glWindowPos3fv((const GLfloat *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glWindowPos3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 z = env->localvar_getInt(runtime, pos++);

    glWindowPos3i((GLint)x, (GLint)y, (GLint)z);

    return 0;
}

int org_mini_gl_GL_glWindowPos3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glWindowPos3iv((const GLint *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glWindowPos3s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 z = env->localvar_getInt(runtime, pos++);

    glWindowPos3s((GLshort)x, (GLshort)y, (GLshort)z);

    return 0;
}

int org_mini_gl_GL_glWindowPos3sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(v){
        arr_offset0 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr0 = v->arr_body + arr_offset0;
    }

    glWindowPos3sv((const GLshort *)(ptr0));

    return 0;
}

int org_mini_gl_GL_glBlendColor(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float red;red.i = env->localvar_getInt(runtime, pos++);
    Int2Float green;green.i = env->localvar_getInt(runtime, pos++);
    Int2Float blue;blue.i = env->localvar_getInt(runtime, pos++);
    Int2Float alpha;alpha.i = env->localvar_getInt(runtime, pos++);

    glBlendColor((GLfloat)red.f, (GLfloat)green.f, (GLfloat)blue.f, (GLfloat)alpha.f);

    return 0;
}

int org_mini_gl_GL_glBlendEquation(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);

    glBlendEquation((GLenum)mode);

    return 0;
}

int org_mini_gl_GL_glGenQueries(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 n = env->localvar_getInt(runtime, pos++);
    Instance *ids = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(ids){
        arr_offset1 *= env->data_type_bytes[ids->mb.arr_type_index];
        ptr1 = ids->arr_body + arr_offset1;
    }

    glGenQueries((GLsizei)n, (GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glDeleteQueries(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 n = env->localvar_getInt(runtime, pos++);
    Instance *ids = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(ids){
        arr_offset1 *= env->data_type_bytes[ids->mb.arr_type_index];
        ptr1 = ids->arr_body + arr_offset1;
    }

    glDeleteQueries((GLsizei)n, (const GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glIsQuery(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 id = env->localvar_getInt(runtime, pos++);

    s32 ret_value = (s32)glIsQuery((GLuint)id);
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glBeginQuery(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 id = env->localvar_getInt(runtime, pos++);

    glBeginQuery((GLenum)target, (GLuint)id);

    return 0;
}

int org_mini_gl_GL_glEndQuery(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);

    glEndQuery((GLenum)target);

    return 0;
}

int org_mini_gl_GL_glGetQueryiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetQueryiv((GLenum)target, (GLenum)pname, (GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetQueryObjectiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 id = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetQueryObjectiv((GLuint)id, (GLenum)pname, (GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetQueryObjectuiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 id = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetQueryObjectuiv((GLuint)id, (GLenum)pname, (GLuint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glBindBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 buffer = env->localvar_getInt(runtime, pos++);

    glBindBuffer((GLenum)target, (GLuint)buffer);

    return 0;
}

int org_mini_gl_GL_glDeleteBuffers(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 n = env->localvar_getInt(runtime, pos++);
    Instance *buffers = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(buffers){
        arr_offset1 *= env->data_type_bytes[buffers->mb.arr_type_index];
        ptr1 = buffers->arr_body + arr_offset1;
    }

    glDeleteBuffers((GLsizei)n, (const GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glGenBuffers(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 n = env->localvar_getInt(runtime, pos++);
    Instance *buffers = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(buffers){
        arr_offset1 *= env->data_type_bytes[buffers->mb.arr_type_index];
        ptr1 = buffers->arr_body + arr_offset1;
    }

    glGenBuffers((GLsizei)n, (GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glIsBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 buffer = env->localvar_getInt(runtime, pos++);

    s32 ret_value = (s32)glIsBuffer((GLuint)buffer);
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glBufferData(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s64 size = getParaLong(runtime, pos);pos += 2;
    Instance *data = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(data){
        arr_offset2 *= env->data_type_bytes[data->mb.arr_type_index];
        ptr2 = data->arr_body + arr_offset2;
    }
    s32 usage = env->localvar_getInt(runtime, pos++);

    glBufferData((GLenum)target, (GLsizeiptr)size, (const void *)(ptr2), (GLenum)usage);

    return 0;
}

int org_mini_gl_GL_glBufferSubData(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s64 offset = getParaLong(runtime, pos);pos += 2;
    s64 size = getParaLong(runtime, pos);pos += 2;
    Instance *data = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(data){
        arr_offset3 *= env->data_type_bytes[data->mb.arr_type_index];
        ptr3 = data->arr_body + arr_offset3;
    }

    glBufferSubData((GLenum)target, (GLintptr)offset, (GLsizeiptr)size, (const void *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glGetBufferSubData(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s64 offset = getParaLong(runtime, pos);pos += 2;
    s64 size = getParaLong(runtime, pos);pos += 2;
    Instance *data = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(data){
        arr_offset3 *= env->data_type_bytes[data->mb.arr_type_index];
        ptr3 = data->arr_body + arr_offset3;
    }

    glGetBufferSubData((GLenum)target, (GLintptr)offset, (GLsizeiptr)size, (void *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glMapBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 access = env->localvar_getInt(runtime, pos++);

    __refer ptr = glMapBuffer((GLenum)target, (GLenum)access);

    //Instance *barr = env->jarray_create(count, DATATYPE_LONG, NULL);
    env->push_ref(runtime->stack, NULL);

    return 0;
}

int org_mini_gl_GL_glUnmapBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);

    s32 ret_value = (s32)glUnmapBuffer((GLenum)target);
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glGetBufferParameteriv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetBufferParameteriv((GLenum)target, (GLenum)pname, (GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetBufferPointerv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetBufferPointerv((GLenum)target, (GLenum)pname, (void **)(ptr2));

    return 0;
}

int org_mini_gl_GL_glBlendEquationSeparate(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 modeRGB = env->localvar_getInt(runtime, pos++);
    s32 modeAlpha = env->localvar_getInt(runtime, pos++);

    glBlendEquationSeparate((GLenum)modeRGB, (GLenum)modeAlpha);

    return 0;
}

int org_mini_gl_GL_glDrawBuffers(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 n = env->localvar_getInt(runtime, pos++);
    Instance *bufs = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(bufs){
        arr_offset1 *= env->data_type_bytes[bufs->mb.arr_type_index];
        ptr1 = bufs->arr_body + arr_offset1;
    }

    glDrawBuffers((GLsizei)n, (const GLenum *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glStencilOpSeparate(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 face = env->localvar_getInt(runtime, pos++);
    s32 sfail = env->localvar_getInt(runtime, pos++);
    s32 dpfail = env->localvar_getInt(runtime, pos++);
    s32 dppass = env->localvar_getInt(runtime, pos++);

    glStencilOpSeparate((GLenum)face, (GLenum)sfail, (GLenum)dpfail, (GLenum)dppass);

    return 0;
}

int org_mini_gl_GL_glStencilFuncSeparate(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 face = env->localvar_getInt(runtime, pos++);
    s32 func = env->localvar_getInt(runtime, pos++);
    s32 ref = env->localvar_getInt(runtime, pos++);
    s32 mask = env->localvar_getInt(runtime, pos++);

    glStencilFuncSeparate((GLenum)face, (GLenum)func, (GLint)ref, (GLuint)mask);

    return 0;
}

int org_mini_gl_GL_glStencilMaskSeparate(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 face = env->localvar_getInt(runtime, pos++);
    s32 mask = env->localvar_getInt(runtime, pos++);

    glStencilMaskSeparate((GLenum)face, (GLuint)mask);

    return 0;
}

int org_mini_gl_GL_glAttachShader(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 shader = env->localvar_getInt(runtime, pos++);

    glAttachShader((GLuint)program, (GLuint)shader);

    return 0;
}

int org_mini_gl_GL_glBindAttribLocation(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *name = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(name){
        arr_offset2 *= env->data_type_bytes[name->mb.arr_type_index];
        ptr2 = name->arr_body + arr_offset2;
    }

    glBindAttribLocation((GLuint)program, (GLuint)index, (const GLchar *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glCreateProgram(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;


    s32 ret_value = (s32)glCreateProgram();
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glCreateShader(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 type = env->localvar_getInt(runtime, pos++);

    s32 ret_value = (s32)glCreateShader((GLenum)type);
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glDeleteProgram(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);

    glDeleteProgram((GLuint)program);

    return 0;
}

int org_mini_gl_GL_glDeleteShader(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 shader = env->localvar_getInt(runtime, pos++);

    glDeleteShader((GLuint)shader);

    return 0;
}

int org_mini_gl_GL_glDetachShader(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 shader = env->localvar_getInt(runtime, pos++);

    glDetachShader((GLuint)program, (GLuint)shader);

    return 0;
}

int org_mini_gl_GL_glDisableVertexAttribArray(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);

    glDisableVertexAttribArray((GLuint)index);

    return 0;
}

int org_mini_gl_GL_glEnableVertexAttribArray(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);

    glEnableVertexAttribArray((GLuint)index);

    return 0;
}

int org_mini_gl_GL_glGetActiveAttrib(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 index = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *length = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(length){
        arr_offset3 *= env->data_type_bytes[length->mb.arr_type_index];
        ptr3 = length->arr_body + arr_offset3;
    }
    Instance *size = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(size){
        arr_offset5 *= env->data_type_bytes[size->mb.arr_type_index];
        ptr5 = size->arr_body + arr_offset5;
    }
    Instance *type = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset7 = env->localvar_getInt(runtime, pos++);
    __refer ptr7 = NULL;
    if(type){
        arr_offset7 *= env->data_type_bytes[type->mb.arr_type_index];
        ptr7 = type->arr_body + arr_offset7;
    }
    Instance *name = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset9 = env->localvar_getInt(runtime, pos++);
    __refer ptr9 = NULL;
    if(name){
        arr_offset9 *= env->data_type_bytes[name->mb.arr_type_index];
        ptr9 = name->arr_body + arr_offset9;
    }

    glGetActiveAttrib((GLuint)program, (GLuint)index, (GLsizei)bufSize, (GLsizei *)(ptr3), (GLint *)(ptr5), (GLenum *)(ptr7), (GLchar *)(ptr9));

    return 0;
}

int org_mini_gl_GL_glGetActiveUniform(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 index = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *length = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(length){
        arr_offset3 *= env->data_type_bytes[length->mb.arr_type_index];
        ptr3 = length->arr_body + arr_offset3;
    }
    Instance *size = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(size){
        arr_offset5 *= env->data_type_bytes[size->mb.arr_type_index];
        ptr5 = size->arr_body + arr_offset5;
    }
    Instance *type = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset7 = env->localvar_getInt(runtime, pos++);
    __refer ptr7 = NULL;
    if(type){
        arr_offset7 *= env->data_type_bytes[type->mb.arr_type_index];
        ptr7 = type->arr_body + arr_offset7;
    }
    Instance *name = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset9 = env->localvar_getInt(runtime, pos++);
    __refer ptr9 = NULL;
    if(name){
        arr_offset9 *= env->data_type_bytes[name->mb.arr_type_index];
        ptr9 = name->arr_body + arr_offset9;
    }

    glGetActiveUniform((GLuint)program, (GLuint)index, (GLsizei)bufSize, (GLsizei *)(ptr3), (GLint *)(ptr5), (GLenum *)(ptr7), (GLchar *)(ptr9));

    return 0;
}

int org_mini_gl_GL_glGetAttachedShaders(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 maxCount = env->localvar_getInt(runtime, pos++);
    Instance *count = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(count){
        arr_offset2 *= env->data_type_bytes[count->mb.arr_type_index];
        ptr2 = count->arr_body + arr_offset2;
    }
    Instance *shaders = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset4 = env->localvar_getInt(runtime, pos++);
    __refer ptr4 = NULL;
    if(shaders){
        arr_offset4 *= env->data_type_bytes[shaders->mb.arr_type_index];
        ptr4 = shaders->arr_body + arr_offset4;
    }

    glGetAttachedShaders((GLuint)program, (GLsizei)maxCount, (GLsizei *)(ptr2), (GLuint *)(ptr4));

    return 0;
}

int org_mini_gl_GL_glGetAttribLocation(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    Instance *name = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(name){
        arr_offset1 *= env->data_type_bytes[name->mb.arr_type_index];
        ptr1 = name->arr_body + arr_offset1;
    }

    s32 ret_value = (s32)glGetAttribLocation((GLuint)program, (const GLchar *)(ptr1));
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glGetProgramiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetProgramiv((GLuint)program, (GLenum)pname, (GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetProgramInfoLog(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *length = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(length){
        arr_offset2 *= env->data_type_bytes[length->mb.arr_type_index];
        ptr2 = length->arr_body + arr_offset2;
    }
    Instance *infoLog = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset4 = env->localvar_getInt(runtime, pos++);
    __refer ptr4 = NULL;
    if(infoLog){
        arr_offset4 *= env->data_type_bytes[infoLog->mb.arr_type_index];
        ptr4 = infoLog->arr_body + arr_offset4;
    }

    glGetProgramInfoLog((GLuint)program, (GLsizei)bufSize, (GLsizei *)(ptr2), (GLchar *)(ptr4));

    return 0;
}

int org_mini_gl_GL_glGetShaderiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 shader = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetShaderiv((GLuint)shader, (GLenum)pname, (GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetShaderInfoLog(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 shader = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *length = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(length){
        arr_offset2 *= env->data_type_bytes[length->mb.arr_type_index];
        ptr2 = length->arr_body + arr_offset2;
    }
    Instance *infoLog = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset4 = env->localvar_getInt(runtime, pos++);
    __refer ptr4 = NULL;
    if(infoLog){
        arr_offset4 *= env->data_type_bytes[infoLog->mb.arr_type_index];
        ptr4 = infoLog->arr_body + arr_offset4;
    }

    glGetShaderInfoLog((GLuint)shader, (GLsizei)bufSize, (GLsizei *)(ptr2), (GLchar *)(ptr4));

    return 0;
}

int org_mini_gl_GL_glGetShaderSource(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 shader = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *length = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(length){
        arr_offset2 *= env->data_type_bytes[length->mb.arr_type_index];
        ptr2 = length->arr_body + arr_offset2;
    }
    Instance *source = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset4 = env->localvar_getInt(runtime, pos++);
    __refer ptr4 = NULL;
    if(source){
        arr_offset4 *= env->data_type_bytes[source->mb.arr_type_index];
        ptr4 = source->arr_body + arr_offset4;
    }

    glGetShaderSource((GLuint)shader, (GLsizei)bufSize, (GLsizei *)(ptr2), (GLchar *)(ptr4));

    return 0;
}

int org_mini_gl_GL_glGetUniformLocation(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    Instance *name = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(name){
        arr_offset1 *= env->data_type_bytes[name->mb.arr_type_index];
        ptr1 = name->arr_body + arr_offset1;
    }

    s32 ret_value = (s32)glGetUniformLocation((GLuint)program, (const GLchar *)(ptr1));
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glGetUniformfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 location = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetUniformfv((GLuint)program, (GLint)location, (GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetUniformiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 location = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetUniformiv((GLuint)program, (GLint)location, (GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetVertexAttribdv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetVertexAttribdv((GLuint)index, (GLenum)pname, (GLdouble *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetVertexAttribfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetVertexAttribfv((GLuint)index, (GLenum)pname, (GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetVertexAttribiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetVertexAttribiv((GLuint)index, (GLenum)pname, (GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetVertexAttribPointerv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *pointer = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(pointer){
        arr_offset2 *= env->data_type_bytes[pointer->mb.arr_type_index];
        ptr2 = pointer->arr_body + arr_offset2;
    }

    glGetVertexAttribPointerv((GLuint)index, (GLenum)pname, (void **)(ptr2));

    return 0;
}

int org_mini_gl_GL_glIsProgram(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);

    s32 ret_value = (s32)glIsProgram((GLuint)program);
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glIsShader(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 shader = env->localvar_getInt(runtime, pos++);

    s32 ret_value = (s32)glIsShader((GLuint)shader);
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glLinkProgram(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);

    glLinkProgram((GLuint)program);

    return 0;
}

int org_mini_gl_GL_glShaderSource(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 shader = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    Instance *string = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(string){
        arr_offset2 *= env->data_type_bytes[string->mb.arr_type_index];
        ptr2 = string->arr_body + arr_offset2;
    }
    Instance *length = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset4 = env->localvar_getInt(runtime, pos++);
    __refer ptr4 = NULL;
    if(length){
        arr_offset4 *= env->data_type_bytes[length->mb.arr_type_index];
        ptr4 = length->arr_body + arr_offset4;
    }

    glShaderSource((GLuint)shader, (GLsizei)count, (const GLchar *const*)(ptr2), (const GLint *)(ptr4));

    return 0;
}

int org_mini_gl_GL_glUseProgram(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);

    glUseProgram((GLuint)program);

    return 0;
}

int org_mini_gl_GL_glUniform1f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    Int2Float v0;v0.i = env->localvar_getInt(runtime, pos++);

    glUniform1f((GLint)location, (GLfloat)v0.f);

    return 0;
}

int org_mini_gl_GL_glUniform2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    Int2Float v0;v0.i = env->localvar_getInt(runtime, pos++);
    Int2Float v1;v1.i = env->localvar_getInt(runtime, pos++);

    glUniform2f((GLint)location, (GLfloat)v0.f, (GLfloat)v1.f);

    return 0;
}

int org_mini_gl_GL_glUniform3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    Int2Float v0;v0.i = env->localvar_getInt(runtime, pos++);
    Int2Float v1;v1.i = env->localvar_getInt(runtime, pos++);
    Int2Float v2;v2.i = env->localvar_getInt(runtime, pos++);

    glUniform3f((GLint)location, (GLfloat)v0.f, (GLfloat)v1.f, (GLfloat)v2.f);

    return 0;
}

int org_mini_gl_GL_glUniform4f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    Int2Float v0;v0.i = env->localvar_getInt(runtime, pos++);
    Int2Float v1;v1.i = env->localvar_getInt(runtime, pos++);
    Int2Float v2;v2.i = env->localvar_getInt(runtime, pos++);
    Int2Float v3;v3.i = env->localvar_getInt(runtime, pos++);

    glUniform4f((GLint)location, (GLfloat)v0.f, (GLfloat)v1.f, (GLfloat)v2.f, (GLfloat)v3.f);

    return 0;
}

int org_mini_gl_GL_glUniform1i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 v0 = env->localvar_getInt(runtime, pos++);

    glUniform1i((GLint)location, (GLint)v0);

    return 0;
}

int org_mini_gl_GL_glUniform2i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 v0 = env->localvar_getInt(runtime, pos++);
    s32 v1 = env->localvar_getInt(runtime, pos++);

    glUniform2i((GLint)location, (GLint)v0, (GLint)v1);

    return 0;
}

int org_mini_gl_GL_glUniform3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 v0 = env->localvar_getInt(runtime, pos++);
    s32 v1 = env->localvar_getInt(runtime, pos++);
    s32 v2 = env->localvar_getInt(runtime, pos++);

    glUniform3i((GLint)location, (GLint)v0, (GLint)v1, (GLint)v2);

    return 0;
}

int org_mini_gl_GL_glUniform4i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 v0 = env->localvar_getInt(runtime, pos++);
    s32 v1 = env->localvar_getInt(runtime, pos++);
    s32 v2 = env->localvar_getInt(runtime, pos++);
    s32 v3 = env->localvar_getInt(runtime, pos++);

    glUniform4i((GLint)location, (GLint)v0, (GLint)v1, (GLint)v2, (GLint)v3);

    return 0;
}

int org_mini_gl_GL_glUniform1fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(value){
        arr_offset2 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr2 = value->arr_body + arr_offset2;
    }

    glUniform1fv((GLint)location, (GLsizei)count, (const GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glUniform2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(value){
        arr_offset2 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr2 = value->arr_body + arr_offset2;
    }

    glUniform2fv((GLint)location, (GLsizei)count, (const GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glUniform3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(value){
        arr_offset2 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr2 = value->arr_body + arr_offset2;
    }

    glUniform3fv((GLint)location, (GLsizei)count, (const GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glUniform4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(value){
        arr_offset2 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr2 = value->arr_body + arr_offset2;
    }

    glUniform4fv((GLint)location, (GLsizei)count, (const GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glUniform1iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(value){
        arr_offset2 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr2 = value->arr_body + arr_offset2;
    }

    glUniform1iv((GLint)location, (GLsizei)count, (const GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glUniform2iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(value){
        arr_offset2 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr2 = value->arr_body + arr_offset2;
    }

    glUniform2iv((GLint)location, (GLsizei)count, (const GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glUniform3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(value){
        arr_offset2 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr2 = value->arr_body + arr_offset2;
    }

    glUniform3iv((GLint)location, (GLsizei)count, (const GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glUniform4iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(value){
        arr_offset2 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr2 = value->arr_body + arr_offset2;
    }

    glUniform4iv((GLint)location, (GLsizei)count, (const GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glUniformMatrix2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    s32 transpose = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(value){
        arr_offset3 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr3 = value->arr_body + arr_offset3;
    }

    glUniformMatrix2fv((GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glUniformMatrix3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    s32 transpose = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(value){
        arr_offset3 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr3 = value->arr_body + arr_offset3;
    }

    glUniformMatrix3fv((GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glUniformMatrix4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    s32 transpose = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(value){
        arr_offset3 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr3 = value->arr_body + arr_offset3;
    }

    glUniformMatrix4fv((GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glValidateProgram(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);

    glValidateProgram((GLuint)program);

    return 0;
}

int org_mini_gl_GL_glVertexAttrib1d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Long2Double x;x.l = getParaLong(runtime, pos);pos += 2;

    glVertexAttrib1d((GLuint)index, (GLdouble)x.d);

    return 0;
}

int org_mini_gl_GL_glVertexAttrib1dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib1dv((GLuint)index, (const GLdouble *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib1f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Int2Float x;x.i = env->localvar_getInt(runtime, pos++);

    glVertexAttrib1f((GLuint)index, (GLfloat)x.f);

    return 0;
}

int org_mini_gl_GL_glVertexAttrib1fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib1fv((GLuint)index, (const GLfloat *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib1s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 x = env->localvar_getInt(runtime, pos++);

    glVertexAttrib1s((GLuint)index, (GLshort)x);

    return 0;
}

int org_mini_gl_GL_glVertexAttrib1sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib1sv((GLuint)index, (const GLshort *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib2d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Long2Double x;x.l = getParaLong(runtime, pos);pos += 2;
    Long2Double y;y.l = getParaLong(runtime, pos);pos += 2;

    glVertexAttrib2d((GLuint)index, (GLdouble)x.d, (GLdouble)y.d);

    return 0;
}

int org_mini_gl_GL_glVertexAttrib2dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib2dv((GLuint)index, (const GLdouble *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Int2Float x;x.i = env->localvar_getInt(runtime, pos++);
    Int2Float y;y.i = env->localvar_getInt(runtime, pos++);

    glVertexAttrib2f((GLuint)index, (GLfloat)x.f, (GLfloat)y.f);

    return 0;
}

int org_mini_gl_GL_glVertexAttrib2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib2fv((GLuint)index, (const GLfloat *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib2s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);

    glVertexAttrib2s((GLuint)index, (GLshort)x, (GLshort)y);

    return 0;
}

int org_mini_gl_GL_glVertexAttrib2sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib2sv((GLuint)index, (const GLshort *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib3d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Long2Double x;x.l = getParaLong(runtime, pos);pos += 2;
    Long2Double y;y.l = getParaLong(runtime, pos);pos += 2;
    Long2Double z;z.l = getParaLong(runtime, pos);pos += 2;

    glVertexAttrib3d((GLuint)index, (GLdouble)x.d, (GLdouble)y.d, (GLdouble)z.d);

    return 0;
}

int org_mini_gl_GL_glVertexAttrib3dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib3dv((GLuint)index, (const GLdouble *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Int2Float x;x.i = env->localvar_getInt(runtime, pos++);
    Int2Float y;y.i = env->localvar_getInt(runtime, pos++);
    Int2Float z;z.i = env->localvar_getInt(runtime, pos++);

    glVertexAttrib3f((GLuint)index, (GLfloat)x.f, (GLfloat)y.f, (GLfloat)z.f);

    return 0;
}

int org_mini_gl_GL_glVertexAttrib3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib3fv((GLuint)index, (const GLfloat *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib3s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 z = env->localvar_getInt(runtime, pos++);

    glVertexAttrib3s((GLuint)index, (GLshort)x, (GLshort)y, (GLshort)z);

    return 0;
}

int org_mini_gl_GL_glVertexAttrib3sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib3sv((GLuint)index, (const GLshort *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib4Nbv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib4Nbv((GLuint)index, (const GLbyte *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib4Niv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib4Niv((GLuint)index, (const GLint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib4Nsv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib4Nsv((GLuint)index, (const GLshort *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib4Nub(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 z = env->localvar_getInt(runtime, pos++);
    s32 w = env->localvar_getInt(runtime, pos++);

    glVertexAttrib4Nub((GLuint)index, (GLubyte)x, (GLubyte)y, (GLubyte)z, (GLubyte)w);

    return 0;
}

int org_mini_gl_GL_glVertexAttrib4Nubv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib4Nubv((GLuint)index, (const GLubyte *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib4Nuiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib4Nuiv((GLuint)index, (const GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib4Nusv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib4Nusv((GLuint)index, (const GLushort *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib4bv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib4bv((GLuint)index, (const GLbyte *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib4d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Long2Double x;x.l = getParaLong(runtime, pos);pos += 2;
    Long2Double y;y.l = getParaLong(runtime, pos);pos += 2;
    Long2Double z;z.l = getParaLong(runtime, pos);pos += 2;
    Long2Double w;w.l = getParaLong(runtime, pos);pos += 2;

    glVertexAttrib4d((GLuint)index, (GLdouble)x.d, (GLdouble)y.d, (GLdouble)z.d, (GLdouble)w.d);

    return 0;
}

int org_mini_gl_GL_glVertexAttrib4dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib4dv((GLuint)index, (const GLdouble *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib4f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Int2Float x;x.i = env->localvar_getInt(runtime, pos++);
    Int2Float y;y.i = env->localvar_getInt(runtime, pos++);
    Int2Float z;z.i = env->localvar_getInt(runtime, pos++);
    Int2Float w;w.i = env->localvar_getInt(runtime, pos++);

    glVertexAttrib4f((GLuint)index, (GLfloat)x.f, (GLfloat)y.f, (GLfloat)z.f, (GLfloat)w.f);

    return 0;
}

int org_mini_gl_GL_glVertexAttrib4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib4fv((GLuint)index, (const GLfloat *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib4iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib4iv((GLuint)index, (const GLint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib4s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 z = env->localvar_getInt(runtime, pos++);
    s32 w = env->localvar_getInt(runtime, pos++);

    glVertexAttrib4s((GLuint)index, (GLshort)x, (GLshort)y, (GLshort)z, (GLshort)w);

    return 0;
}

int org_mini_gl_GL_glVertexAttrib4sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib4sv((GLuint)index, (const GLshort *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib4ubv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib4ubv((GLuint)index, (const GLubyte *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib4uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib4uiv((GLuint)index, (const GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttrib4usv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttrib4usv((GLuint)index, (const GLushort *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttribPointer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 size = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    s32 normalized = env->localvar_getInt(runtime, pos++);
    s32 stride = env->localvar_getInt(runtime, pos++);
    Instance *pointer = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(pointer){
        arr_offset5 *= env->data_type_bytes[pointer->mb.arr_type_index];
        ptr5 = pointer->arr_body + arr_offset5;
    }

    glVertexAttribPointer((GLuint)index, (GLint)size, (GLenum)type, (GLboolean)normalized, (GLsizei)stride, (const void *)(ptr5));

    return 0;
}

int org_mini_gl_GL_glUniformMatrix2x3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    s32 transpose = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(value){
        arr_offset3 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr3 = value->arr_body + arr_offset3;
    }

    glUniformMatrix2x3fv((GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glUniformMatrix3x2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    s32 transpose = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(value){
        arr_offset3 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr3 = value->arr_body + arr_offset3;
    }

    glUniformMatrix3x2fv((GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glUniformMatrix2x4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    s32 transpose = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(value){
        arr_offset3 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr3 = value->arr_body + arr_offset3;
    }

    glUniformMatrix2x4fv((GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glUniformMatrix4x2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    s32 transpose = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(value){
        arr_offset3 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr3 = value->arr_body + arr_offset3;
    }

    glUniformMatrix4x2fv((GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glUniformMatrix3x4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    s32 transpose = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(value){
        arr_offset3 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr3 = value->arr_body + arr_offset3;
    }

    glUniformMatrix3x4fv((GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glUniformMatrix4x3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    s32 transpose = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(value){
        arr_offset3 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr3 = value->arr_body + arr_offset3;
    }

    glUniformMatrix4x3fv((GLint)location, (GLsizei)count, (GLboolean)transpose, (const GLfloat *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glColorMaski(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 r = env->localvar_getInt(runtime, pos++);
    s32 g = env->localvar_getInt(runtime, pos++);
    s32 b = env->localvar_getInt(runtime, pos++);
    s32 a = env->localvar_getInt(runtime, pos++);

    glColorMaski((GLuint)index, (GLboolean)r, (GLboolean)g, (GLboolean)b, (GLboolean)a);

    return 0;
}

int org_mini_gl_GL_glGetBooleani_v(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *data = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(data){
        arr_offset2 *= env->data_type_bytes[data->mb.arr_type_index];
        ptr2 = data->arr_body + arr_offset2;
    }

    glGetBooleani_v((GLenum)target, (GLuint)index, (GLboolean *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetIntegeri_v(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *data = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(data){
        arr_offset2 *= env->data_type_bytes[data->mb.arr_type_index];
        ptr2 = data->arr_body + arr_offset2;
    }

    glGetIntegeri_v((GLenum)target, (GLuint)index, (GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glEnablei(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 index = env->localvar_getInt(runtime, pos++);

    glEnablei((GLenum)target, (GLuint)index);

    return 0;
}

int org_mini_gl_GL_glDisablei(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 index = env->localvar_getInt(runtime, pos++);

    glDisablei((GLenum)target, (GLuint)index);

    return 0;
}

int org_mini_gl_GL_glIsEnabledi(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 index = env->localvar_getInt(runtime, pos++);

    s32 ret_value = (s32)glIsEnabledi((GLenum)target, (GLuint)index);
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glBeginTransformFeedback(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 primitiveMode = env->localvar_getInt(runtime, pos++);

    glBeginTransformFeedback((GLenum)primitiveMode);

    return 0;
}

int org_mini_gl_GL_glEndTransformFeedback(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;


    glEndTransformFeedback();

    return 0;
}

int org_mini_gl_GL_glBindBufferRange(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 index = env->localvar_getInt(runtime, pos++);
    s32 buffer = env->localvar_getInt(runtime, pos++);
    s64 offset = getParaLong(runtime, pos);pos += 2;
    s64 size = getParaLong(runtime, pos);pos += 2;

    glBindBufferRange((GLenum)target, (GLuint)index, (GLuint)buffer, (GLintptr)offset, (GLsizeiptr)size);

    return 0;
}

int org_mini_gl_GL_glBindBufferBase(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 index = env->localvar_getInt(runtime, pos++);
    s32 buffer = env->localvar_getInt(runtime, pos++);

    glBindBufferBase((GLenum)target, (GLuint)index, (GLuint)buffer);

    return 0;
}

int org_mini_gl_GL_glTransformFeedbackVaryings(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    Instance *varyings = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(varyings){
        arr_offset2 *= env->data_type_bytes[varyings->mb.arr_type_index];
        ptr2 = varyings->arr_body + arr_offset2;
    }
    s32 bufferMode = env->localvar_getInt(runtime, pos++);

    glTransformFeedbackVaryings((GLuint)program, (GLsizei)count, (const GLchar *const*)(ptr2), (GLenum)bufferMode);

    return 0;
}

int org_mini_gl_GL_glGetTransformFeedbackVarying(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 index = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *length = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(length){
        arr_offset3 *= env->data_type_bytes[length->mb.arr_type_index];
        ptr3 = length->arr_body + arr_offset3;
    }
    Instance *size = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(size){
        arr_offset5 *= env->data_type_bytes[size->mb.arr_type_index];
        ptr5 = size->arr_body + arr_offset5;
    }
    Instance *type = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset7 = env->localvar_getInt(runtime, pos++);
    __refer ptr7 = NULL;
    if(type){
        arr_offset7 *= env->data_type_bytes[type->mb.arr_type_index];
        ptr7 = type->arr_body + arr_offset7;
    }
    Instance *name = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset9 = env->localvar_getInt(runtime, pos++);
    __refer ptr9 = NULL;
    if(name){
        arr_offset9 *= env->data_type_bytes[name->mb.arr_type_index];
        ptr9 = name->arr_body + arr_offset9;
    }

    glGetTransformFeedbackVarying((GLuint)program, (GLuint)index, (GLsizei)bufSize, (GLsizei *)(ptr3), (GLsizei *)(ptr5), (GLenum *)(ptr7), (GLchar *)(ptr9));

    return 0;
}

int org_mini_gl_GL_glClampColor(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 clamp = env->localvar_getInt(runtime, pos++);

    glClampColor((GLenum)target, (GLenum)clamp);

    return 0;
}

int org_mini_gl_GL_glBeginConditionalRender(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 id = env->localvar_getInt(runtime, pos++);
    s32 mode = env->localvar_getInt(runtime, pos++);

    glBeginConditionalRender((GLuint)id, (GLenum)mode);

    return 0;
}

int org_mini_gl_GL_glEndConditionalRender(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;


    glEndConditionalRender();

    return 0;
}

int org_mini_gl_GL_glVertexAttribIPointer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 size = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    s32 stride = env->localvar_getInt(runtime, pos++);
    Instance *pointer = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset4 = env->localvar_getInt(runtime, pos++);
    __refer ptr4 = NULL;
    if(pointer){
        arr_offset4 *= env->data_type_bytes[pointer->mb.arr_type_index];
        ptr4 = pointer->arr_body + arr_offset4;
    }

    glVertexAttribIPointer((GLuint)index, (GLint)size, (GLenum)type, (GLsizei)stride, (const void *)(ptr4));

    return 0;
}

int org_mini_gl_GL_glGetVertexAttribIiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetVertexAttribIiv((GLuint)index, (GLenum)pname, (GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetVertexAttribIuiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetVertexAttribIuiv((GLuint)index, (GLenum)pname, (GLuint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glVertexAttribI1i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 x = env->localvar_getInt(runtime, pos++);

    glVertexAttribI1i((GLuint)index, (GLint)x);

    return 0;
}

int org_mini_gl_GL_glVertexAttribI2i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);

    glVertexAttribI2i((GLuint)index, (GLint)x, (GLint)y);

    return 0;
}

int org_mini_gl_GL_glVertexAttribI3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 z = env->localvar_getInt(runtime, pos++);

    glVertexAttribI3i((GLuint)index, (GLint)x, (GLint)y, (GLint)z);

    return 0;
}

int org_mini_gl_GL_glVertexAttribI4i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 z = env->localvar_getInt(runtime, pos++);
    s32 w = env->localvar_getInt(runtime, pos++);

    glVertexAttribI4i((GLuint)index, (GLint)x, (GLint)y, (GLint)z, (GLint)w);

    return 0;
}

int org_mini_gl_GL_glVertexAttribI1ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 x = env->localvar_getInt(runtime, pos++);

    glVertexAttribI1ui((GLuint)index, (GLuint)x);

    return 0;
}

int org_mini_gl_GL_glVertexAttribI2ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);

    glVertexAttribI2ui((GLuint)index, (GLuint)x, (GLuint)y);

    return 0;
}

int org_mini_gl_GL_glVertexAttribI3ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 z = env->localvar_getInt(runtime, pos++);

    glVertexAttribI3ui((GLuint)index, (GLuint)x, (GLuint)y, (GLuint)z);

    return 0;
}

int org_mini_gl_GL_glVertexAttribI4ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 z = env->localvar_getInt(runtime, pos++);
    s32 w = env->localvar_getInt(runtime, pos++);

    glVertexAttribI4ui((GLuint)index, (GLuint)x, (GLuint)y, (GLuint)z, (GLuint)w);

    return 0;
}

int org_mini_gl_GL_glVertexAttribI1iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttribI1iv((GLuint)index, (const GLint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttribI2iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttribI2iv((GLuint)index, (const GLint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttribI3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttribI3iv((GLuint)index, (const GLint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttribI4iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttribI4iv((GLuint)index, (const GLint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttribI1uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttribI1uiv((GLuint)index, (const GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttribI2uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttribI2uiv((GLuint)index, (const GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttribI3uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttribI3uiv((GLuint)index, (const GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttribI4uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttribI4uiv((GLuint)index, (const GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttribI4bv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttribI4bv((GLuint)index, (const GLbyte *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttribI4sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttribI4sv((GLuint)index, (const GLshort *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttribI4ubv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttribI4ubv((GLuint)index, (const GLubyte *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glVertexAttribI4usv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(v){
        arr_offset1 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr1 = v->arr_body + arr_offset1;
    }

    glVertexAttribI4usv((GLuint)index, (const GLushort *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glGetUniformuiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 location = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetUniformuiv((GLuint)program, (GLint)location, (GLuint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glBindFragDataLocation(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 color = env->localvar_getInt(runtime, pos++);
    Instance *name = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(name){
        arr_offset2 *= env->data_type_bytes[name->mb.arr_type_index];
        ptr2 = name->arr_body + arr_offset2;
    }

    glBindFragDataLocation((GLuint)program, (GLuint)color, (const GLchar *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetFragDataLocation(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    Instance *name = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(name){
        arr_offset1 *= env->data_type_bytes[name->mb.arr_type_index];
        ptr1 = name->arr_body + arr_offset1;
    }

    s32 ret_value = (s32)glGetFragDataLocation((GLuint)program, (const GLchar *)(ptr1));
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glUniform1ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 v0 = env->localvar_getInt(runtime, pos++);

    glUniform1ui((GLint)location, (GLuint)v0);

    return 0;
}

int org_mini_gl_GL_glUniform2ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 v0 = env->localvar_getInt(runtime, pos++);
    s32 v1 = env->localvar_getInt(runtime, pos++);

    glUniform2ui((GLint)location, (GLuint)v0, (GLuint)v1);

    return 0;
}

int org_mini_gl_GL_glUniform3ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 v0 = env->localvar_getInt(runtime, pos++);
    s32 v1 = env->localvar_getInt(runtime, pos++);
    s32 v2 = env->localvar_getInt(runtime, pos++);

    glUniform3ui((GLint)location, (GLuint)v0, (GLuint)v1, (GLuint)v2);

    return 0;
}

int org_mini_gl_GL_glUniform4ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 v0 = env->localvar_getInt(runtime, pos++);
    s32 v1 = env->localvar_getInt(runtime, pos++);
    s32 v2 = env->localvar_getInt(runtime, pos++);
    s32 v3 = env->localvar_getInt(runtime, pos++);

    glUniform4ui((GLint)location, (GLuint)v0, (GLuint)v1, (GLuint)v2, (GLuint)v3);

    return 0;
}

int org_mini_gl_GL_glUniform1uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(value){
        arr_offset2 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr2 = value->arr_body + arr_offset2;
    }

    glUniform1uiv((GLint)location, (GLsizei)count, (const GLuint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glUniform2uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(value){
        arr_offset2 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr2 = value->arr_body + arr_offset2;
    }

    glUniform2uiv((GLint)location, (GLsizei)count, (const GLuint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glUniform3uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(value){
        arr_offset2 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr2 = value->arr_body + arr_offset2;
    }

    glUniform3uiv((GLint)location, (GLsizei)count, (const GLuint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glUniform4uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 location = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(value){
        arr_offset2 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr2 = value->arr_body + arr_offset2;
    }

    glUniform4uiv((GLint)location, (GLsizei)count, (const GLuint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glTexParameterIiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glTexParameterIiv((GLenum)target, (GLenum)pname, (const GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glTexParameterIuiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glTexParameterIuiv((GLenum)target, (GLenum)pname, (const GLuint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetTexParameterIiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetTexParameterIiv((GLenum)target, (GLenum)pname, (GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetTexParameterIuiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetTexParameterIuiv((GLenum)target, (GLenum)pname, (GLuint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glClearBufferiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 buffer = env->localvar_getInt(runtime, pos++);
    s32 drawbuffer = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(value){
        arr_offset2 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr2 = value->arr_body + arr_offset2;
    }

    glClearBufferiv((GLenum)buffer, (GLint)drawbuffer, (const GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glClearBufferuiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 buffer = env->localvar_getInt(runtime, pos++);
    s32 drawbuffer = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(value){
        arr_offset2 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr2 = value->arr_body + arr_offset2;
    }

    glClearBufferuiv((GLenum)buffer, (GLint)drawbuffer, (const GLuint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glClearBufferfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 buffer = env->localvar_getInt(runtime, pos++);
    s32 drawbuffer = env->localvar_getInt(runtime, pos++);
    Instance *value = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(value){
        arr_offset2 *= env->data_type_bytes[value->mb.arr_type_index];
        ptr2 = value->arr_body + arr_offset2;
    }

    glClearBufferfv((GLenum)buffer, (GLint)drawbuffer, (const GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glClearBufferfi(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 buffer = env->localvar_getInt(runtime, pos++);
    s32 drawbuffer = env->localvar_getInt(runtime, pos++);
    Int2Float depth;depth.i = env->localvar_getInt(runtime, pos++);
    s32 stencil = env->localvar_getInt(runtime, pos++);

    glClearBufferfi((GLenum)buffer, (GLint)drawbuffer, (GLfloat)depth.f, (GLint)stencil);

    return 0;
}

int org_mini_gl_GL_glGetStringi(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 name = env->localvar_getInt(runtime, pos++);
    s32 index = env->localvar_getInt(runtime, pos++);

    glGetStringi((GLenum)name, (GLuint)index);

    //Instance *barr = env->jarray_create(count, DATATYPE_LONG, NULL);
    env->push_ref(runtime->stack, NULL);

    return 0;
}

int org_mini_gl_GL_glIsRenderbuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 renderbuffer = env->localvar_getInt(runtime, pos++);

    s32 ret_value = (s32)glIsRenderbuffer((GLuint)renderbuffer);
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glBindRenderbuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 renderbuffer = env->localvar_getInt(runtime, pos++);

    glBindRenderbuffer((GLenum)target, (GLuint)renderbuffer);

    return 0;
}

int org_mini_gl_GL_glDeleteRenderbuffers(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 n = env->localvar_getInt(runtime, pos++);
    Instance *renderbuffers = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(renderbuffers){
        arr_offset1 *= env->data_type_bytes[renderbuffers->mb.arr_type_index];
        ptr1 = renderbuffers->arr_body + arr_offset1;
    }

    glDeleteRenderbuffers((GLsizei)n, (const GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glGenRenderbuffers(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 n = env->localvar_getInt(runtime, pos++);
    Instance *renderbuffers = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(renderbuffers){
        arr_offset1 *= env->data_type_bytes[renderbuffers->mb.arr_type_index];
        ptr1 = renderbuffers->arr_body + arr_offset1;
    }

    glGenRenderbuffers((GLsizei)n, (GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glRenderbufferStorage(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 internalformat = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);

    glRenderbufferStorage((GLenum)target, (GLenum)internalformat, (GLsizei)width, (GLsizei)height);

    return 0;
}

int org_mini_gl_GL_glGetRenderbufferParameteriv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetRenderbufferParameteriv((GLenum)target, (GLenum)pname, (GLint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glIsFramebuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 framebuffer = env->localvar_getInt(runtime, pos++);

    s32 ret_value = (s32)glIsFramebuffer((GLuint)framebuffer);
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glBindFramebuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 framebuffer = env->localvar_getInt(runtime, pos++);

    glBindFramebuffer((GLenum)target, (GLuint)framebuffer);

    return 0;
}

int org_mini_gl_GL_glDeleteFramebuffers(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 n = env->localvar_getInt(runtime, pos++);
    Instance *framebuffers = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(framebuffers){
        arr_offset1 *= env->data_type_bytes[framebuffers->mb.arr_type_index];
        ptr1 = framebuffers->arr_body + arr_offset1;
    }

    glDeleteFramebuffers((GLsizei)n, (const GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glGenFramebuffers(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 n = env->localvar_getInt(runtime, pos++);
    Instance *framebuffers = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(framebuffers){
        arr_offset1 *= env->data_type_bytes[framebuffers->mb.arr_type_index];
        ptr1 = framebuffers->arr_body + arr_offset1;
    }

    glGenFramebuffers((GLsizei)n, (GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glCheckFramebufferStatus(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);

    s32 ret_value = (s32)glCheckFramebufferStatus((GLenum)target);
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glFramebufferTexture1D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 attachment = env->localvar_getInt(runtime, pos++);
    s32 textarget = env->localvar_getInt(runtime, pos++);
    s32 texture = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);

    glFramebufferTexture1D((GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level);

    return 0;
}

int org_mini_gl_GL_glFramebufferTexture2D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 attachment = env->localvar_getInt(runtime, pos++);
    s32 textarget = env->localvar_getInt(runtime, pos++);
    s32 texture = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);

    glFramebufferTexture2D((GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level);

    return 0;
}

int org_mini_gl_GL_glFramebufferTexture3D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 attachment = env->localvar_getInt(runtime, pos++);
    s32 textarget = env->localvar_getInt(runtime, pos++);
    s32 texture = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 zoffset = env->localvar_getInt(runtime, pos++);

    glFramebufferTexture3D((GLenum)target, (GLenum)attachment, (GLenum)textarget, (GLuint)texture, (GLint)level, (GLint)zoffset);

    return 0;
}

int org_mini_gl_GL_glFramebufferRenderbuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 attachment = env->localvar_getInt(runtime, pos++);
    s32 renderbuffertarget = env->localvar_getInt(runtime, pos++);
    s32 renderbuffer = env->localvar_getInt(runtime, pos++);

    glFramebufferRenderbuffer((GLenum)target, (GLenum)attachment, (GLenum)renderbuffertarget, (GLuint)renderbuffer);

    return 0;
}

int org_mini_gl_GL_glGetFramebufferAttachmentParameteriv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 attachment = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(params){
        arr_offset3 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr3 = params->arr_body + arr_offset3;
    }

    glGetFramebufferAttachmentParameteriv((GLenum)target, (GLenum)attachment, (GLenum)pname, (GLint *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glGenerateMipmap(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);

    glGenerateMipmap((GLenum)target);

    return 0;
}

int org_mini_gl_GL_glBlitFramebuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 srcX0 = env->localvar_getInt(runtime, pos++);
    s32 srcY0 = env->localvar_getInt(runtime, pos++);
    s32 srcX1 = env->localvar_getInt(runtime, pos++);
    s32 srcY1 = env->localvar_getInt(runtime, pos++);
    s32 dstX0 = env->localvar_getInt(runtime, pos++);
    s32 dstY0 = env->localvar_getInt(runtime, pos++);
    s32 dstX1 = env->localvar_getInt(runtime, pos++);
    s32 dstY1 = env->localvar_getInt(runtime, pos++);
    s32 mask = env->localvar_getInt(runtime, pos++);
    s32 filter = env->localvar_getInt(runtime, pos++);

    glBlitFramebuffer((GLint)srcX0, (GLint)srcY0, (GLint)srcX1, (GLint)srcY1, (GLint)dstX0, (GLint)dstY0, (GLint)dstX1, (GLint)dstY1, (GLbitfield)mask, (GLenum)filter);

    return 0;
}

int org_mini_gl_GL_glRenderbufferStorageMultisample(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 samples = env->localvar_getInt(runtime, pos++);
    s32 internalformat = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);

    glRenderbufferStorageMultisample((GLenum)target, (GLsizei)samples, (GLenum)internalformat, (GLsizei)width, (GLsizei)height);

    return 0;
}

int org_mini_gl_GL_glFramebufferTextureLayer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 attachment = env->localvar_getInt(runtime, pos++);
    s32 texture = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 layer = env->localvar_getInt(runtime, pos++);

    glFramebufferTextureLayer((GLenum)target, (GLenum)attachment, (GLuint)texture, (GLint)level, (GLint)layer);

    return 0;
}

int org_mini_gl_GL_glMapBufferRange(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s64 offset = getParaLong(runtime, pos);pos += 2;
    s64 length = getParaLong(runtime, pos);pos += 2;
    s32 access = env->localvar_getInt(runtime, pos++);

    __refer ptr = glMapBufferRange((GLenum)target, (GLintptr)offset, (GLsizeiptr)length, (GLbitfield)access);

    //Instance *barr = env->jarray_create(count, DATATYPE_LONG, NULL);
    env->push_ref(runtime->stack, NULL);

    return 0;
}

int org_mini_gl_GL_glFlushMappedBufferRange(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s64 offset = getParaLong(runtime, pos);pos += 2;
    s64 length = getParaLong(runtime, pos);pos += 2;

    glFlushMappedBufferRange((GLenum)target, (GLintptr)offset, (GLsizeiptr)length);

    return 0;
}

int org_mini_gl_GL_glBindVertexArray(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 array = env->localvar_getInt(runtime, pos++);

    glBindVertexArray((GLuint)array);

    return 0;
}

int org_mini_gl_GL_glDeleteVertexArrays(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 n = env->localvar_getInt(runtime, pos++);
    Instance *arrays = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(arrays){
        arr_offset1 *= env->data_type_bytes[arrays->mb.arr_type_index];
        ptr1 = arrays->arr_body + arr_offset1;
    }

    glDeleteVertexArrays((GLsizei)n, (const GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glGenVertexArrays(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 n = env->localvar_getInt(runtime, pos++);
    Instance *arrays = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(arrays){
        arr_offset1 *= env->data_type_bytes[arrays->mb.arr_type_index];
        ptr1 = arrays->arr_body + arr_offset1;
    }

    glGenVertexArrays((GLsizei)n, (GLuint *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glIsVertexArray(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 array = env->localvar_getInt(runtime, pos++);

    s32 ret_value = (s32)glIsVertexArray((GLuint)array);
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glDrawArraysInstanced(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);
    s32 first = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    s32 instancecount = env->localvar_getInt(runtime, pos++);

    glDrawArraysInstanced((GLenum)mode, (GLint)first, (GLsizei)count, (GLsizei)instancecount);

    return 0;
}

int org_mini_gl_GL_glDrawElementsInstanced(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    Instance *indices = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(indices){
        arr_offset3 *= env->data_type_bytes[indices->mb.arr_type_index];
        ptr3 = indices->arr_body + arr_offset3;
    }
    s32 instancecount = env->localvar_getInt(runtime, pos++);

    glDrawElementsInstanced((GLenum)mode, (GLsizei)count, (GLenum)type, (const void *)(ptr3), (GLsizei)instancecount);

    return 0;
}

int org_mini_gl_GL_glTexBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 internalformat = env->localvar_getInt(runtime, pos++);
    s32 buffer = env->localvar_getInt(runtime, pos++);

    glTexBuffer((GLenum)target, (GLenum)internalformat, (GLuint)buffer);

    return 0;
}

int org_mini_gl_GL_glPrimitiveRestartIndex(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 index = env->localvar_getInt(runtime, pos++);

    glPrimitiveRestartIndex((GLuint)index);

    return 0;
}

int org_mini_gl_GL_glCopyBufferSubData(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 readTarget = env->localvar_getInt(runtime, pos++);
    s32 writeTarget = env->localvar_getInt(runtime, pos++);
    s64 readOffset = getParaLong(runtime, pos);pos += 2;
    s64 writeOffset = getParaLong(runtime, pos);pos += 2;
    s64 size = getParaLong(runtime, pos);pos += 2;

    glCopyBufferSubData((GLenum)readTarget, (GLenum)writeTarget, (GLintptr)readOffset, (GLintptr)writeOffset, (GLsizeiptr)size);

    return 0;
}

int org_mini_gl_GL_glGetUniformIndices(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 uniformCount = env->localvar_getInt(runtime, pos++);
    Instance *uniformNames = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(uniformNames){
        arr_offset2 *= env->data_type_bytes[uniformNames->mb.arr_type_index];
        ptr2 = uniformNames->arr_body + arr_offset2;
    }
    Instance *uniformIndices = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset4 = env->localvar_getInt(runtime, pos++);
    __refer ptr4 = NULL;
    if(uniformIndices){
        arr_offset4 *= env->data_type_bytes[uniformIndices->mb.arr_type_index];
        ptr4 = uniformIndices->arr_body + arr_offset4;
    }

    glGetUniformIndices((GLuint)program, (GLsizei)uniformCount, (const GLchar *const*)(ptr2), (GLuint *)(ptr4));

    return 0;
}

int org_mini_gl_GL_glGetActiveUniformsiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 uniformCount = env->localvar_getInt(runtime, pos++);
    Instance *uniformIndices = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(uniformIndices){
        arr_offset2 *= env->data_type_bytes[uniformIndices->mb.arr_type_index];
        ptr2 = uniformIndices->arr_body + arr_offset2;
    }
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(params){
        arr_offset5 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr5 = params->arr_body + arr_offset5;
    }

    glGetActiveUniformsiv((GLuint)program, (GLsizei)uniformCount, (const GLuint *)(ptr2), (GLenum)pname, (GLint *)(ptr5));

    return 0;
}

int org_mini_gl_GL_glGetActiveUniformName(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 uniformIndex = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *length = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(length){
        arr_offset3 *= env->data_type_bytes[length->mb.arr_type_index];
        ptr3 = length->arr_body + arr_offset3;
    }
    Instance *uniformName = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(uniformName){
        arr_offset5 *= env->data_type_bytes[uniformName->mb.arr_type_index];
        ptr5 = uniformName->arr_body + arr_offset5;
    }

    glGetActiveUniformName((GLuint)program, (GLuint)uniformIndex, (GLsizei)bufSize, (GLsizei *)(ptr3), (GLchar *)(ptr5));

    return 0;
}

int org_mini_gl_GL_glGetUniformBlockIndex(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    Instance *uniformBlockName = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(uniformBlockName){
        arr_offset1 *= env->data_type_bytes[uniformBlockName->mb.arr_type_index];
        ptr1 = uniformBlockName->arr_body + arr_offset1;
    }

    s32 ret_value = (s32)glGetUniformBlockIndex((GLuint)program, (const GLchar *)(ptr1));
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glGetActiveUniformBlockiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 uniformBlockIndex = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(params){
        arr_offset3 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr3 = params->arr_body + arr_offset3;
    }

    glGetActiveUniformBlockiv((GLuint)program, (GLuint)uniformBlockIndex, (GLenum)pname, (GLint *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glGetActiveUniformBlockName(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 uniformBlockIndex = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *length = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(length){
        arr_offset3 *= env->data_type_bytes[length->mb.arr_type_index];
        ptr3 = length->arr_body + arr_offset3;
    }
    Instance *uniformBlockName = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(uniformBlockName){
        arr_offset5 *= env->data_type_bytes[uniformBlockName->mb.arr_type_index];
        ptr5 = uniformBlockName->arr_body + arr_offset5;
    }

    glGetActiveUniformBlockName((GLuint)program, (GLuint)uniformBlockIndex, (GLsizei)bufSize, (GLsizei *)(ptr3), (GLchar *)(ptr5));

    return 0;
}

int org_mini_gl_GL_glUniformBlockBinding(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 uniformBlockIndex = env->localvar_getInt(runtime, pos++);
    s32 uniformBlockBinding = env->localvar_getInt(runtime, pos++);

    glUniformBlockBinding((GLuint)program, (GLuint)uniformBlockIndex, (GLuint)uniformBlockBinding);

    return 0;
}

int org_mini_gl_GL_glDrawElementsBaseVertex(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    Instance *indices = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(indices){
        arr_offset3 *= env->data_type_bytes[indices->mb.arr_type_index];
        ptr3 = indices->arr_body + arr_offset3;
    }
    s32 basevertex = env->localvar_getInt(runtime, pos++);

    glDrawElementsBaseVertex((GLenum)mode, (GLsizei)count, (GLenum)type, (const void *)(ptr3), (GLint)basevertex);

    return 0;
}

int org_mini_gl_GL_glDrawRangeElementsBaseVertex(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);
    s32 start = env->localvar_getInt(runtime, pos++);
    s32 end = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    Instance *indices = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(indices){
        arr_offset5 *= env->data_type_bytes[indices->mb.arr_type_index];
        ptr5 = indices->arr_body + arr_offset5;
    }
    s32 basevertex = env->localvar_getInt(runtime, pos++);

    glDrawRangeElementsBaseVertex((GLenum)mode, (GLuint)start, (GLuint)end, (GLsizei)count, (GLenum)type, (const void *)(ptr5), (GLint)basevertex);

    return 0;
}

int org_mini_gl_GL_glDrawElementsInstancedBaseVertex(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    Instance *indices = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(indices){
        arr_offset3 *= env->data_type_bytes[indices->mb.arr_type_index];
        ptr3 = indices->arr_body + arr_offset3;
    }
    s32 instancecount = env->localvar_getInt(runtime, pos++);
    s32 basevertex = env->localvar_getInt(runtime, pos++);

    glDrawElementsInstancedBaseVertex((GLenum)mode, (GLsizei)count, (GLenum)type, (const void *)(ptr3), (GLsizei)instancecount, (GLint)basevertex);

    return 0;
}

int org_mini_gl_GL_glMultiDrawElementsBaseVertex(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);
    Instance *count = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(count){
        arr_offset1 *= env->data_type_bytes[count->mb.arr_type_index];
        ptr1 = count->arr_body + arr_offset1;
    }
    s32 type = env->localvar_getInt(runtime, pos++);
    Instance *indices = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset4 = env->localvar_getInt(runtime, pos++);
    __refer ptr4 = NULL;
    if(indices){
        arr_offset4 *= env->data_type_bytes[indices->mb.arr_type_index];
        ptr4 = indices->arr_body + arr_offset4;
    }
    s32 drawcount = env->localvar_getInt(runtime, pos++);
    Instance *basevertex = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset7 = env->localvar_getInt(runtime, pos++);
    __refer ptr7 = NULL;
    if(basevertex){
        arr_offset7 *= env->data_type_bytes[basevertex->mb.arr_type_index];
        ptr7 = basevertex->arr_body + arr_offset7;
    }

    glMultiDrawElementsBaseVertex((GLenum)mode, (const GLsizei *)(ptr1), (GLenum)type, (const void *const*)(ptr4), (GLsizei)drawcount, (const GLint *)(ptr7));

    return 0;
}

int org_mini_gl_GL_glProvokingVertex(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 mode = env->localvar_getInt(runtime, pos++);

    glProvokingVertex((GLenum)mode);

    return 0;
}

int org_mini_gl_GL_glFenceSync(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 condition = env->localvar_getInt(runtime, pos++);
    s32 flags = env->localvar_getInt(runtime, pos++);

    s64 ret_value = (s64)(intptr_t)glFenceSync((GLenum)condition, (GLbitfield)flags);
    env->push_long(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glIsSync(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s64 sync = getParaLong(runtime, pos);pos += 2;

    s32 ret_value = (s32)glIsSync((GLsync)(intptr_t)sync);
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glDeleteSync(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s64 sync = getParaLong(runtime, pos);pos += 2;

    glDeleteSync((GLsync)(intptr_t)sync);

    return 0;
}

int org_mini_gl_GL_glClientWaitSync(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s64 sync = getParaLong(runtime, pos);pos += 2;
    s32 flags = env->localvar_getInt(runtime, pos++);
    s64 timeout = getParaLong(runtime, pos);pos += 2;

    s32 ret_value = (s32)glClientWaitSync((GLsync)(intptr_t)sync, (GLbitfield)flags, (GLuint64)timeout);
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glWaitSync(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s64 sync = getParaLong(runtime, pos);pos += 2;
    s32 flags = env->localvar_getInt(runtime, pos++);
    s64 timeout = getParaLong(runtime, pos);pos += 2;

    glWaitSync((GLsync)(intptr_t)sync, (GLbitfield)flags, (GLuint64)timeout);

    return 0;
}

int org_mini_gl_GL_glGetInteger64v(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *data = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(data){
        arr_offset1 *= env->data_type_bytes[data->mb.arr_type_index];
        ptr1 = data->arr_body + arr_offset1;
    }

    glGetInteger64v((GLenum)pname, (GLint64 *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glGetSynciv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s64 sync = getParaLong(runtime, pos);pos += 2;
    s32 pname = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *length = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(length){
        arr_offset3 *= env->data_type_bytes[length->mb.arr_type_index];
        ptr3 = length->arr_body + arr_offset3;
    }
    Instance *values = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(values){
        arr_offset5 *= env->data_type_bytes[values->mb.arr_type_index];
        ptr5 = values->arr_body + arr_offset5;
    }

    glGetSynciv((GLsync)(intptr_t)sync, (GLenum)pname, (GLsizei)bufSize, (GLsizei *)(ptr3), (GLint *)(ptr5));

    return 0;
}

int org_mini_gl_GL_glGetInteger64i_v(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *data = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(data){
        arr_offset2 *= env->data_type_bytes[data->mb.arr_type_index];
        ptr2 = data->arr_body + arr_offset2;
    }

    glGetInteger64i_v((GLenum)target, (GLuint)index, (GLint64 *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetBufferParameteri64v(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(params){
        arr_offset2 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr2 = params->arr_body + arr_offset2;
    }

    glGetBufferParameteri64v((GLenum)target, (GLenum)pname, (GLint64 *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glFramebufferTexture(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 attachment = env->localvar_getInt(runtime, pos++);
    s32 texture = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);

    glFramebufferTexture((GLenum)target, (GLenum)attachment, (GLuint)texture, (GLint)level);

    return 0;
}

int org_mini_gl_GL_glTexImage2DMultisample(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 samples = env->localvar_getInt(runtime, pos++);
    s32 internalformat = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);
    s32 fixedsamplelocations = env->localvar_getInt(runtime, pos++);

    glTexImage2DMultisample((GLenum)target, (GLsizei)samples, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLboolean)fixedsamplelocations);

    return 0;
}

int org_mini_gl_GL_glTexImage3DMultisample(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 samples = env->localvar_getInt(runtime, pos++);
    s32 internalformat = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);
    s32 depth = env->localvar_getInt(runtime, pos++);
    s32 fixedsamplelocations = env->localvar_getInt(runtime, pos++);

    glTexImage3DMultisample((GLenum)target, (GLsizei)samples, (GLenum)internalformat, (GLsizei)width, (GLsizei)height, (GLsizei)depth, (GLboolean)fixedsamplelocations);

    return 0;
}

int org_mini_gl_GL_glGetMultisamplefv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    s32 index = env->localvar_getInt(runtime, pos++);
    Instance *val = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(val){
        arr_offset2 *= env->data_type_bytes[val->mb.arr_type_index];
        ptr2 = val->arr_body + arr_offset2;
    }

    glGetMultisamplefv((GLenum)pname, (GLuint)index, (GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glSampleMaski(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 maskNumber = env->localvar_getInt(runtime, pos++);
    s32 mask = env->localvar_getInt(runtime, pos++);

    glSampleMaski((GLuint)maskNumber, (GLbitfield)mask);

    return 0;
}

int org_mini_gl_GL_glSampleCoverageARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Int2Float value;value.i = env->localvar_getInt(runtime, pos++);
    s32 invert = env->localvar_getInt(runtime, pos++);

    glSampleCoverageARB((GLfloat)value.f, (GLboolean)invert);

    return 0;
}

int org_mini_gl_GL_glGetGraphicsResetStatusARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;


    s32 ret_value = (s32)glGetGraphicsResetStatusARB();
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glGetnTexImageARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 level = env->localvar_getInt(runtime, pos++);
    s32 format = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *img = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(img){
        arr_offset5 *= env->data_type_bytes[img->mb.arr_type_index];
        ptr5 = img->arr_body + arr_offset5;
    }

    glGetnTexImageARB((GLenum)target, (GLint)level, (GLenum)format, (GLenum)type, (GLsizei)bufSize, (void *)(ptr5));

    return 0;
}

int org_mini_gl_GL_glReadnPixelsARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 x = env->localvar_getInt(runtime, pos++);
    s32 y = env->localvar_getInt(runtime, pos++);
    s32 width = env->localvar_getInt(runtime, pos++);
    s32 height = env->localvar_getInt(runtime, pos++);
    s32 format = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *data = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset7 = env->localvar_getInt(runtime, pos++);
    __refer ptr7 = NULL;
    if(data){
        arr_offset7 *= env->data_type_bytes[data->mb.arr_type_index];
        ptr7 = data->arr_body + arr_offset7;
    }

    glReadnPixelsARB((GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, (GLsizei)bufSize, (void *)(ptr7));

    return 0;
}

int org_mini_gl_GL_glGetnCompressedTexImageARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 lod = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *img = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(img){
        arr_offset3 *= env->data_type_bytes[img->mb.arr_type_index];
        ptr3 = img->arr_body + arr_offset3;
    }

    glGetnCompressedTexImageARB((GLenum)target, (GLint)lod, (GLsizei)bufSize, (void *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glGetnUniformfvARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 location = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(params){
        arr_offset3 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr3 = params->arr_body + arr_offset3;
    }

    glGetnUniformfvARB((GLuint)program, (GLint)location, (GLsizei)bufSize, (GLfloat *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glGetnUniformivARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 location = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(params){
        arr_offset3 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr3 = params->arr_body + arr_offset3;
    }

    glGetnUniformivARB((GLuint)program, (GLint)location, (GLsizei)bufSize, (GLint *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glGetnUniformuivARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 location = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(params){
        arr_offset3 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr3 = params->arr_body + arr_offset3;
    }

    glGetnUniformuivARB((GLuint)program, (GLint)location, (GLsizei)bufSize, (GLuint *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glGetnUniformdvARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 program = env->localvar_getInt(runtime, pos++);
    s32 location = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(params){
        arr_offset3 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr3 = params->arr_body + arr_offset3;
    }

    glGetnUniformdvARB((GLuint)program, (GLint)location, (GLsizei)bufSize, (GLdouble *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glGetnMapdvARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 query = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(v){
        arr_offset3 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr3 = v->arr_body + arr_offset3;
    }

    glGetnMapdvARB((GLenum)target, (GLenum)query, (GLsizei)bufSize, (GLdouble *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glGetnMapfvARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 query = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(v){
        arr_offset3 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr3 = v->arr_body + arr_offset3;
    }

    glGetnMapfvARB((GLenum)target, (GLenum)query, (GLsizei)bufSize, (GLfloat *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glGetnMapivARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 query = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *v = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(v){
        arr_offset3 *= env->data_type_bytes[v->mb.arr_type_index];
        ptr3 = v->arr_body + arr_offset3;
    }

    glGetnMapivARB((GLenum)target, (GLenum)query, (GLsizei)bufSize, (GLint *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glGetnPixelMapfvARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 map = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *values = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(values){
        arr_offset2 *= env->data_type_bytes[values->mb.arr_type_index];
        ptr2 = values->arr_body + arr_offset2;
    }

    glGetnPixelMapfvARB((GLenum)map, (GLsizei)bufSize, (GLfloat *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetnPixelMapuivARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 map = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *values = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(values){
        arr_offset2 *= env->data_type_bytes[values->mb.arr_type_index];
        ptr2 = values->arr_body + arr_offset2;
    }

    glGetnPixelMapuivARB((GLenum)map, (GLsizei)bufSize, (GLuint *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetnPixelMapusvARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 map = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *values = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(values){
        arr_offset2 *= env->data_type_bytes[values->mb.arr_type_index];
        ptr2 = values->arr_body + arr_offset2;
    }

    glGetnPixelMapusvARB((GLenum)map, (GLsizei)bufSize, (GLushort *)(ptr2));

    return 0;
}

int org_mini_gl_GL_glGetnPolygonStippleARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *pattern = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(pattern){
        arr_offset1 *= env->data_type_bytes[pattern->mb.arr_type_index];
        ptr1 = pattern->arr_body + arr_offset1;
    }

    glGetnPolygonStippleARB((GLsizei)bufSize, (GLubyte *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glGetnColorTableARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 format = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *table = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset4 = env->localvar_getInt(runtime, pos++);
    __refer ptr4 = NULL;
    if(table){
        arr_offset4 *= env->data_type_bytes[table->mb.arr_type_index];
        ptr4 = table->arr_body + arr_offset4;
    }

    glGetnColorTableARB((GLenum)target, (GLenum)format, (GLenum)type, (GLsizei)bufSize, (void *)(ptr4));

    return 0;
}

int org_mini_gl_GL_glGetnConvolutionFilterARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 format = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *image = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset4 = env->localvar_getInt(runtime, pos++);
    __refer ptr4 = NULL;
    if(image){
        arr_offset4 *= env->data_type_bytes[image->mb.arr_type_index];
        ptr4 = image->arr_body + arr_offset4;
    }

    glGetnConvolutionFilterARB((GLenum)target, (GLenum)format, (GLenum)type, (GLsizei)bufSize, (void *)(ptr4));

    return 0;
}

int org_mini_gl_GL_glGetnSeparableFilterARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 format = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    s32 rowBufSize = env->localvar_getInt(runtime, pos++);
    Instance *row = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset4 = env->localvar_getInt(runtime, pos++);
    __refer ptr4 = NULL;
    if(row){
        arr_offset4 *= env->data_type_bytes[row->mb.arr_type_index];
        ptr4 = row->arr_body + arr_offset4;
    }
    s32 columnBufSize = env->localvar_getInt(runtime, pos++);
    Instance *column = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset7 = env->localvar_getInt(runtime, pos++);
    __refer ptr7 = NULL;
    if(column){
        arr_offset7 *= env->data_type_bytes[column->mb.arr_type_index];
        ptr7 = column->arr_body + arr_offset7;
    }
    Instance *span = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset9 = env->localvar_getInt(runtime, pos++);
    __refer ptr9 = NULL;
    if(span){
        arr_offset9 *= env->data_type_bytes[span->mb.arr_type_index];
        ptr9 = span->arr_body + arr_offset9;
    }

    glGetnSeparableFilterARB((GLenum)target, (GLenum)format, (GLenum)type, (GLsizei)rowBufSize, (void *)(ptr4), (GLsizei)columnBufSize, (void *)(ptr7), (void *)(ptr9));

    return 0;
}

int org_mini_gl_GL_glGetnHistogramARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 reset = env->localvar_getInt(runtime, pos++);
    s32 format = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *values = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(values){
        arr_offset5 *= env->data_type_bytes[values->mb.arr_type_index];
        ptr5 = values->arr_body + arr_offset5;
    }

    glGetnHistogramARB((GLenum)target, (GLboolean)reset, (GLenum)format, (GLenum)type, (GLsizei)bufSize, (void *)(ptr5));

    return 0;
}

int org_mini_gl_GL_glGetnMinmaxARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 target = env->localvar_getInt(runtime, pos++);
    s32 reset = env->localvar_getInt(runtime, pos++);
    s32 format = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *values = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(values){
        arr_offset5 *= env->data_type_bytes[values->mb.arr_type_index];
        ptr5 = values->arr_body + arr_offset5;
    }

    glGetnMinmaxARB((GLenum)target, (GLboolean)reset, (GLenum)format, (GLenum)type, (GLsizei)bufSize, (void *)(ptr5));

    return 0;
}

int org_mini_gl_GL_glDebugMessageControl(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 source = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    s32 severity = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    Instance *ids = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset4 = env->localvar_getInt(runtime, pos++);
    __refer ptr4 = NULL;
    if(ids){
        arr_offset4 *= env->data_type_bytes[ids->mb.arr_type_index];
        ptr4 = ids->arr_body + arr_offset4;
    }
    s32 enabled = env->localvar_getInt(runtime, pos++);

    glDebugMessageControl((GLenum)source, (GLenum)type, (GLenum)severity, (GLsizei)count, (const GLuint *)(ptr4), (GLboolean)enabled);

    return 0;
}

int org_mini_gl_GL_glDebugMessageInsert(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 source = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    s32 id = env->localvar_getInt(runtime, pos++);
    s32 severity = env->localvar_getInt(runtime, pos++);
    s32 length = env->localvar_getInt(runtime, pos++);
    Instance *buf = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(buf){
        arr_offset5 *= env->data_type_bytes[buf->mb.arr_type_index];
        ptr5 = buf->arr_body + arr_offset5;
    }

    glDebugMessageInsert((GLenum)source, (GLenum)type, (GLuint)id, (GLenum)severity, (GLsizei)length, (const GLchar *)(ptr5));

    return 0;
}

int org_mini_gl_GL_glDebugMessageCallback(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s64 callback = getParaLong(runtime, pos);pos += 2;
    Instance *userParam = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(userParam){
        arr_offset1 *= env->data_type_bytes[userParam->mb.arr_type_index];
        ptr1 = userParam->arr_body + arr_offset1;
    }

    glDebugMessageCallback((GLDEBUGPROC)(intptr_t)callback, (const void *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glGetDebugMessageLog(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 count = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *sources = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(sources){
        arr_offset2 *= env->data_type_bytes[sources->mb.arr_type_index];
        ptr2 = sources->arr_body + arr_offset2;
    }
    Instance *types = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset4 = env->localvar_getInt(runtime, pos++);
    __refer ptr4 = NULL;
    if(types){
        arr_offset4 *= env->data_type_bytes[types->mb.arr_type_index];
        ptr4 = types->arr_body + arr_offset4;
    }
    Instance *ids = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset6 = env->localvar_getInt(runtime, pos++);
    __refer ptr6 = NULL;
    if(ids){
        arr_offset6 *= env->data_type_bytes[ids->mb.arr_type_index];
        ptr6 = ids->arr_body + arr_offset6;
    }
    Instance *severities = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset8 = env->localvar_getInt(runtime, pos++);
    __refer ptr8 = NULL;
    if(severities){
        arr_offset8 *= env->data_type_bytes[severities->mb.arr_type_index];
        ptr8 = severities->arr_body + arr_offset8;
    }
    Instance *lengths = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset10 = env->localvar_getInt(runtime, pos++);
    __refer ptr10 = NULL;
    if(lengths){
        arr_offset10 *= env->data_type_bytes[lengths->mb.arr_type_index];
        ptr10 = lengths->arr_body + arr_offset10;
    }
    Instance *messageLog = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset12 = env->localvar_getInt(runtime, pos++);
    __refer ptr12 = NULL;
    if(messageLog){
        arr_offset12 *= env->data_type_bytes[messageLog->mb.arr_type_index];
        ptr12 = messageLog->arr_body + arr_offset12;
    }

    s32 ret_value = (s32)glGetDebugMessageLog((GLuint)count, (GLsizei)bufSize, (GLenum *)(ptr2), (GLenum *)(ptr4), (GLuint *)(ptr6), (GLenum *)(ptr8), (GLsizei *)(ptr10), (GLchar *)(ptr12));
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glPushDebugGroup(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 source = env->localvar_getInt(runtime, pos++);
    s32 id = env->localvar_getInt(runtime, pos++);
    s32 length = env->localvar_getInt(runtime, pos++);
    Instance *message = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(message){
        arr_offset3 *= env->data_type_bytes[message->mb.arr_type_index];
        ptr3 = message->arr_body + arr_offset3;
    }

    glPushDebugGroup((GLenum)source, (GLuint)id, (GLsizei)length, (const GLchar *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glPopDebugGroup(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;


    glPopDebugGroup();

    return 0;
}

int org_mini_gl_GL_glObjectLabel(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 identifier = env->localvar_getInt(runtime, pos++);
    s32 name = env->localvar_getInt(runtime, pos++);
    s32 length = env->localvar_getInt(runtime, pos++);
    Instance *label = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(label){
        arr_offset3 *= env->data_type_bytes[label->mb.arr_type_index];
        ptr3 = label->arr_body + arr_offset3;
    }

    glObjectLabel((GLenum)identifier, (GLuint)name, (GLsizei)length, (const GLchar *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glGetObjectLabel(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 identifier = env->localvar_getInt(runtime, pos++);
    s32 name = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *length = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(length){
        arr_offset3 *= env->data_type_bytes[length->mb.arr_type_index];
        ptr3 = length->arr_body + arr_offset3;
    }
    Instance *label = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(label){
        arr_offset5 *= env->data_type_bytes[label->mb.arr_type_index];
        ptr5 = label->arr_body + arr_offset5;
    }

    glGetObjectLabel((GLenum)identifier, (GLuint)name, (GLsizei)bufSize, (GLsizei *)(ptr3), (GLchar *)(ptr5));

    return 0;
}

int org_mini_gl_GL_glObjectPtrLabel(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *ptr = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(ptr){
        arr_offset0 *= env->data_type_bytes[ptr->mb.arr_type_index];
        ptr0 = ptr->arr_body + arr_offset0;
    }
    s32 length = env->localvar_getInt(runtime, pos++);
    Instance *label = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(label){
        arr_offset3 *= env->data_type_bytes[label->mb.arr_type_index];
        ptr3 = label->arr_body + arr_offset3;
    }

    glObjectPtrLabel((const void *)(ptr0), (GLsizei)length, (const GLchar *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glGetObjectPtrLabel(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *ptr = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(ptr){
        arr_offset0 *= env->data_type_bytes[ptr->mb.arr_type_index];
        ptr0 = ptr->arr_body + arr_offset0;
    }
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *length = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(length){
        arr_offset3 *= env->data_type_bytes[length->mb.arr_type_index];
        ptr3 = length->arr_body + arr_offset3;
    }
    Instance *label = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(label){
        arr_offset5 *= env->data_type_bytes[label->mb.arr_type_index];
        ptr5 = label->arr_body + arr_offset5;
    }

    glGetObjectPtrLabel((const void *)(ptr0), (GLsizei)bufSize, (GLsizei *)(ptr3), (GLchar *)(ptr5));

    return 0;
}

int org_mini_gl_GL_glDebugMessageControlKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 source = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    s32 severity = env->localvar_getInt(runtime, pos++);
    s32 count = env->localvar_getInt(runtime, pos++);
    Instance *ids = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset4 = env->localvar_getInt(runtime, pos++);
    __refer ptr4 = NULL;
    if(ids){
        arr_offset4 *= env->data_type_bytes[ids->mb.arr_type_index];
        ptr4 = ids->arr_body + arr_offset4;
    }
    s32 enabled = env->localvar_getInt(runtime, pos++);

    glDebugMessageControlKHR((GLenum)source, (GLenum)type, (GLenum)severity, (GLsizei)count, (const GLuint *)(ptr4), (GLboolean)enabled);

    return 0;
}

int org_mini_gl_GL_glDebugMessageInsertKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 source = env->localvar_getInt(runtime, pos++);
    s32 type = env->localvar_getInt(runtime, pos++);
    s32 id = env->localvar_getInt(runtime, pos++);
    s32 severity = env->localvar_getInt(runtime, pos++);
    s32 length = env->localvar_getInt(runtime, pos++);
    Instance *buf = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(buf){
        arr_offset5 *= env->data_type_bytes[buf->mb.arr_type_index];
        ptr5 = buf->arr_body + arr_offset5;
    }

    glDebugMessageInsertKHR((GLenum)source, (GLenum)type, (GLuint)id, (GLenum)severity, (GLsizei)length, (const GLchar *)(ptr5));

    return 0;
}

int org_mini_gl_GL_glDebugMessageCallbackKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s64 callback = getParaLong(runtime, pos);pos += 2;
    Instance *userParam = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(userParam){
        arr_offset1 *= env->data_type_bytes[userParam->mb.arr_type_index];
        ptr1 = userParam->arr_body + arr_offset1;
    }

    glDebugMessageCallbackKHR((GLDEBUGPROCKHR)(intptr_t)callback, (const void *)(ptr1));

    return 0;
}

int org_mini_gl_GL_glGetDebugMessageLogKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 count = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *sources = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset2 = env->localvar_getInt(runtime, pos++);
    __refer ptr2 = NULL;
    if(sources){
        arr_offset2 *= env->data_type_bytes[sources->mb.arr_type_index];
        ptr2 = sources->arr_body + arr_offset2;
    }
    Instance *types = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset4 = env->localvar_getInt(runtime, pos++);
    __refer ptr4 = NULL;
    if(types){
        arr_offset4 *= env->data_type_bytes[types->mb.arr_type_index];
        ptr4 = types->arr_body + arr_offset4;
    }
    Instance *ids = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset6 = env->localvar_getInt(runtime, pos++);
    __refer ptr6 = NULL;
    if(ids){
        arr_offset6 *= env->data_type_bytes[ids->mb.arr_type_index];
        ptr6 = ids->arr_body + arr_offset6;
    }
    Instance *severities = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset8 = env->localvar_getInt(runtime, pos++);
    __refer ptr8 = NULL;
    if(severities){
        arr_offset8 *= env->data_type_bytes[severities->mb.arr_type_index];
        ptr8 = severities->arr_body + arr_offset8;
    }
    Instance *lengths = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset10 = env->localvar_getInt(runtime, pos++);
    __refer ptr10 = NULL;
    if(lengths){
        arr_offset10 *= env->data_type_bytes[lengths->mb.arr_type_index];
        ptr10 = lengths->arr_body + arr_offset10;
    }
    Instance *messageLog = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset12 = env->localvar_getInt(runtime, pos++);
    __refer ptr12 = NULL;
    if(messageLog){
        arr_offset12 *= env->data_type_bytes[messageLog->mb.arr_type_index];
        ptr12 = messageLog->arr_body + arr_offset12;
    }

    s32 ret_value = (s32)glGetDebugMessageLogKHR((GLuint)count, (GLsizei)bufSize, (GLenum *)(ptr2), (GLenum *)(ptr4), (GLuint *)(ptr6), (GLenum *)(ptr8), (GLsizei *)(ptr10), (GLchar *)(ptr12));
    env->push_int(runtime->stack, ret_value);
    return 0;
}

int org_mini_gl_GL_glPushDebugGroupKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 source = env->localvar_getInt(runtime, pos++);
    s32 id = env->localvar_getInt(runtime, pos++);
    s32 length = env->localvar_getInt(runtime, pos++);
    Instance *message = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(message){
        arr_offset3 *= env->data_type_bytes[message->mb.arr_type_index];
        ptr3 = message->arr_body + arr_offset3;
    }

    glPushDebugGroupKHR((GLenum)source, (GLuint)id, (GLsizei)length, (const GLchar *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glPopDebugGroupKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;


    glPopDebugGroupKHR();

    return 0;
}

int org_mini_gl_GL_glObjectLabelKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 identifier = env->localvar_getInt(runtime, pos++);
    s32 name = env->localvar_getInt(runtime, pos++);
    s32 length = env->localvar_getInt(runtime, pos++);
    Instance *label = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(label){
        arr_offset3 *= env->data_type_bytes[label->mb.arr_type_index];
        ptr3 = label->arr_body + arr_offset3;
    }

    glObjectLabelKHR((GLenum)identifier, (GLuint)name, (GLsizei)length, (const GLchar *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glGetObjectLabelKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 identifier = env->localvar_getInt(runtime, pos++);
    s32 name = env->localvar_getInt(runtime, pos++);
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *length = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(length){
        arr_offset3 *= env->data_type_bytes[length->mb.arr_type_index];
        ptr3 = length->arr_body + arr_offset3;
    }
    Instance *label = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(label){
        arr_offset5 *= env->data_type_bytes[label->mb.arr_type_index];
        ptr5 = label->arr_body + arr_offset5;
    }

    glGetObjectLabelKHR((GLenum)identifier, (GLuint)name, (GLsizei)bufSize, (GLsizei *)(ptr3), (GLchar *)(ptr5));

    return 0;
}

int org_mini_gl_GL_glObjectPtrLabelKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *ptr = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(ptr){
        arr_offset0 *= env->data_type_bytes[ptr->mb.arr_type_index];
        ptr0 = ptr->arr_body + arr_offset0;
    }
    s32 length = env->localvar_getInt(runtime, pos++);
    Instance *label = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(label){
        arr_offset3 *= env->data_type_bytes[label->mb.arr_type_index];
        ptr3 = label->arr_body + arr_offset3;
    }

    glObjectPtrLabelKHR((const void *)(ptr0), (GLsizei)length, (const GLchar *)(ptr3));

    return 0;
}

int org_mini_gl_GL_glGetObjectPtrLabelKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    Instance *ptr = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset0 = env->localvar_getInt(runtime, pos++);
    __refer ptr0 = NULL;
    if(ptr){
        arr_offset0 *= env->data_type_bytes[ptr->mb.arr_type_index];
        ptr0 = ptr->arr_body + arr_offset0;
    }
    s32 bufSize = env->localvar_getInt(runtime, pos++);
    Instance *length = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset3 = env->localvar_getInt(runtime, pos++);
    __refer ptr3 = NULL;
    if(length){
        arr_offset3 *= env->data_type_bytes[length->mb.arr_type_index];
        ptr3 = length->arr_body + arr_offset3;
    }
    Instance *label = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset5 = env->localvar_getInt(runtime, pos++);
    __refer ptr5 = NULL;
    if(label){
        arr_offset5 *= env->data_type_bytes[label->mb.arr_type_index];
        ptr5 = label->arr_body + arr_offset5;
    }

    glGetObjectPtrLabelKHR((const void *)(ptr0), (GLsizei)bufSize, (GLsizei *)(ptr3), (GLchar *)(ptr5));

    return 0;
}

int org_mini_gl_GL_glGetPointervKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;

    s32 pname = env->localvar_getInt(runtime, pos++);
    Instance *params = env->localvar_getRefer(runtime, pos++);
    s32 arr_offset1 = env->localvar_getInt(runtime, pos++);
    __refer ptr1 = NULL;
    if(params){
        arr_offset1 *= env->data_type_bytes[params->mb.arr_type_index];
        ptr1 = params->arr_body + arr_offset1;
    }

    glGetPointervKHR((GLenum)pname, (void **)(ptr1));

    return 0;
}




static java_native_method method_test2_table[] = {
        {"org/mini/glfw/utils/Gutil", "f2b",                                                      "([F[B)[B",                                                             org_mini_glfw_utils_Gutil_f2b},
        {"org/mini/glfw/utils/Gutil", "vec_add",                                                  "([F[F[F)[F",                                                           org_mini_glfw_utils_Gutil_vec_add},
        {"org/mini/glfw/utils/Gutil", "vec_sub",                                                  "([F[F[F)[F",                                                           org_mini_glfw_utils_Gutil_vec_sub},
        {"org/mini/glfw/utils/Gutil", "vec_scale",                                                "([F[FF)[F",                                                            org_mini_glfw_utils_Gutil_vec_scale},
        {"org/mini/glfw/utils/Gutil", "vec_mul_inner",                                            "([F[F)[F",                                                             org_mini_glfw_utils_Gutil_vec_mul_inner},
        {"org/mini/glfw/utils/Gutil", "vec_len",                                                  "([F)F",                                                                org_mini_glfw_utils_Gutil_vec_len},
        {"org/mini/glfw/utils/Gutil", "vec_normal",                                               "([F[F)[F",                                                             org_mini_glfw_utils_Gutil_vec_normal},
        {"org/mini/glfw/utils/Gutil", "vec_mul_cross",                                            "([F[F[F)[F",                                                           org_mini_glfw_utils_Gutil_vec_mul_cross},
        {"org/mini/glfw/utils/Gutil", "vec_reflect",                                              "([F[F[F)[F",                                                           org_mini_glfw_utils_Gutil_vec_reflect},
        {"org/mini/glfw/utils/Gutil", "mat4x4_identity",                                          "([F)[F",                                                               org_mini_glfw_utils_Gutil_mat4x4_identity},
        {"org/mini/glfw/utils/Gutil", "mat4x4_dup",                                               "([F[F)[F",                                                             org_mini_glfw_utils_Gutil_mat4x4_dup},
        {"org/mini/glfw/utils/Gutil", "mat4x4_row",                                               "([F[FI)[F",                                                            org_mini_glfw_utils_Gutil_mat4x4_row},
        {"org/mini/glfw/utils/Gutil", "mat4x4_col",                                               "([F[FI)[F",                                                            org_mini_glfw_utils_Gutil_mat4x4_col},
        {"org/mini/glfw/utils/Gutil", "mat4x4_transpose",                                         "([F[F)[F",                                                             org_mini_glfw_utils_Gutil_mat4x4_transpose},
        {"org/mini/glfw/utils/Gutil", "mat4x4_add",                                               "([F[F[F)[F",                                                           org_mini_glfw_utils_Gutil_mat4x4_add},
        {"org/mini/glfw/utils/Gutil", "mat4x4_sub",                                               "([F[F[F)[F",                                                           org_mini_glfw_utils_Gutil_mat4x4_sub},
        {"org/mini/glfw/utils/Gutil", "mat4x4_mul",                                               "([F[F[F)[F",                                                           org_mini_glfw_utils_Gutil_mat4x4_mul},
        {"org/mini/glfw/utils/Gutil", "mat4x4_mul_vec4",                                          "([F[F[F)[F",                                                           org_mini_glfw_utils_Gutil_mat4x4_mul_vec4},
        {"org/mini/glfw/utils/Gutil", "mat4x4_from_vec3_mul_outer",                               "([F[F[F)[F",                                                           org_mini_glfw_utils_Gutil_mat4x4_from_vec3_mul_outer},
        {"org/mini/glfw/utils/Gutil", "mat4x4_translate",                                         "([FFFF)[F",                                                            org_mini_glfw_utils_Gutil_mat4x4_translate},
        {"org/mini/glfw/utils/Gutil", "mat4x4_translate_in_place",                                "([FFFF)[F",                                                            org_mini_glfw_utils_Gutil_mat4x4_translate_in_place},
        {"org/mini/glfw/utils/Gutil", "mat4x4_scale",                                             "([F[FF)[F",                                                            org_mini_glfw_utils_Gutil_mat4x4_scale},
        {"org/mini/glfw/utils/Gutil", "mat4x4_scale_aniso",                                       "([F[FFFF)[F",                                                          org_mini_glfw_utils_Gutil_mat4x4_scale_aniso},
        {"org/mini/glfw/utils/Gutil", "mat4x4_rotate",                                            "([F[FFFFF)[F",                                                         org_mini_glfw_utils_Gutil_mat4x4_rotate},
        {"org/mini/glfw/utils/Gutil", "mat4x4_rotateX",                                           "([F[FF)[F",                                                            org_mini_glfw_utils_Gutil_mat4x4_rotateX},
        {"org/mini/glfw/utils/Gutil", "mat4x4_rotateY",                                           "([F[FF)[F",                                                            org_mini_glfw_utils_Gutil_mat4x4_rotateY},
        {"org/mini/glfw/utils/Gutil", "mat4x4_rotateZ",                                           "([F[FF)[F",                                                            org_mini_glfw_utils_Gutil_mat4x4_rotateZ},
        {"org/mini/glfw/utils/Gutil", "mat4x4_invert",                                            "([F[F)[F",                                                             org_mini_glfw_utils_Gutil_mat4x4_invert},
        {"org/mini/glfw/utils/Gutil", "mat4x4_orthonormalize",                                    "([F[F)[F",                                                             org_mini_glfw_utils_Gutil_mat4x4_orthonormalize},
        {"org/mini/glfw/utils/Gutil", "mat4x4_ortho",                                             "([FFFFFFF)[F",                                                         org_mini_glfw_utils_Gutil_mat4x4_ortho},
        {"org/mini/glfw/utils/Gutil", "mat4x4_frustum",                                           "([FFFFFFF)[F",                                                         org_mini_glfw_utils_Gutil_mat4x4_frustum},
        {"org/mini/glfw/utils/Gutil", "mat4x4_perspective",                                       "([FFFFF)[F",                                                           org_mini_glfw_utils_Gutil_mat4x4_perspective},
        {"org/mini/glfw/utils/Gutil", "mat4x4_look_at",                                           "([F[F[F[F)[F",                                                         org_mini_glfw_utils_Gutil_mat4x4_look_at},
        {"org/mini/glfw/Glfw",        "glfwGetTime",                                              "()D",                                                                  org_mini_glfw_Glfw_glfwGetTime},
        {"org/mini/glfw/Glfw",        "glfwSetTime",                                              "(D)V",                                                                 org_mini_glfw_Glfw_glfwSetTime},
        {"org/mini/glfw/Glfw",        "glfwCreateWindowJni",                                      "(II[BJJ)J",                                                            org_mini_glfw_Glfw_glfwCreateWindowJni},
        {"org/mini/glfw/Glfw",        "glfwDestroyWindow",                                        "(J)V",                                                                 org_mini_glfw_Glfw_glfwDestroyWindow},
        {"org/mini/glfw/Glfw",        "glfwWindowShouldClose",                                    "(J)Z",                                                                 org_mini_glfw_Glfw_glfwWindowShouldClose},
        {"org/mini/glfw/Glfw",        "glfwSetCallbackJni",                                       "(JLorg/mini/glfw/GlfwCallback;)V",                                     org_mini_glfw_Glfw_glfwSetCallbackJni},
        {"org/mini/glfw/Glfw",        "glfwInitJni",                                              "()Z",                                                                  org_mini_glfw_Glfw_glfwInitJni},
        {"org/mini/glfw/Glfw",        "glfwTerminate",                                            "()V",                                                                  org_mini_glfw_Glfw_glfwTerminate},
        {"org/mini/glfw/Glfw",        "glfwWindowHint",                                           "(II)V",                                                                org_mini_glfw_Glfw_glfwWindowHint},
        {"org/mini/glfw/Glfw",        "glfwPollEvents",                                           "()V",                                                                  org_mini_glfw_Glfw_glfwPollEvents},
        {"org/mini/glfw/Glfw",        "glfwSetWindowShouldClose",                                 "(JI)V",                                                                org_mini_glfw_Glfw_glfwSetWindowShouldClose},
        {"org/mini/glfw/Glfw",        "glfwMakeContextCurrentJni",                                "(J)V",                                                                 org_mini_glfw_Glfw_glfwMakeContextCurrentJni},
        {"org/mini/glfw/Glfw",        "glfwSwapInterval",                                         "(I)V",                                                                 org_mini_glfw_Glfw_glfwSwapInterval},
        {"org/mini/glfw/Glfw",        "glfwSwapBuffers",                                          "(J)V",                                                                 org_mini_glfw_Glfw_glfwSwapBuffers},
        {"org/mini/glfw/Glfw",        "glfwGetFramebufferSizeW",                                  "(J)I",                                                                 org_mini_glfw_Glfw_glfwGetFramebufferSizeW},
        {"org/mini/glfw/Glfw",        "glfwGetFramebufferSizeH",                                  "(J)I",                                                                 org_mini_glfw_Glfw_glfwGetFramebufferSizeH},
        {"org/mini/glfw/Glfw",        "glfwSetWindowAspectRatio",                                 "(JII)V",                                                               org_mini_glfw_Glfw_glfwSetWindowAspectRatio},
        {"org/mini/glfw/Glfw",        "glfwGetClipboardString",                                   "(J)Ljava/lang/String;",                                                org_mini_glfw_Glfw_glfwGetClipboardString},
//        {"org/mini/gl/GL",            "init",                       "()V",                              org_mini_gl_GL_init},
//        {"org/mini/gl/GL",            "glViewport",                 "(IIII)V",                          org_mini_gl_GL_glViewport},
//        {"org/mini/gl/GL",            "glMatrixMode",               "(I)V",                             org_mini_gl_GL_glMatrixMode},
//        {"org/mini/gl/GL",            "glLoadMatrixf",              "([FI)V",                           org_mini_gl_GL_glLoadMatrixf},
//        {"org/mini/gl/GL",            "glPushMatrix",               "()V",                              org_mini_gl_GL_glPushMatrix},
//        {"org/mini/gl/GL",            "glPopMatrix",                "()V",                              org_mini_gl_GL_glPopMatrix},
//        {"org/mini/gl/GL",            "glEnable",                   "(I)V",                             org_mini_gl_GL_glEnable},
//        {"org/mini/gl/GL",            "glDisable",                  "(I)V",                             org_mini_gl_GL_glDisable},
//        {"org/mini/gl/GL",            "glShadeModel",               "(I)V",                             org_mini_gl_GL_glShadeModel},
//        {"org/mini/gl/GL",            "glClear",                    "(I)V",                             org_mini_gl_GL_glClear},
//        {"org/mini/gl/GL",            "glHint",                     "(II)V",                            org_mini_gl_GL_glHint},
//        {"org/mini/gl/GL",            "glClearDepth",               "(D)V",                             org_mini_gl_GL_glClearDepth},
//        {"org/mini/gl/GL",            "glDepthRange",               "(DD)V",                            org_mini_gl_GL_glDepthRange},
//        {"org/mini/gl/GL",            "glDepthFunc",                "(I)V",                             org_mini_gl_GL_glDepthFunc},
//        {"org/mini/gl/GL",            "glDepthMask",                "(I)V",                             org_mini_gl_GL_glDepthMask},
//        {"org/mini/gl/GL",            "glDrawBuffer",               "(I)V",                             org_mini_gl_GL_glDrawBuffer},
//        {"org/mini/gl/GL",            "glReadBuffer",               "(I)V",                             org_mini_gl_GL_glReadBuffer},
//        {"org/mini/gl/GL",            "glTranslatef",               "(FFF)V",                           org_mini_gl_GL_glTranslatef},
//        {"org/mini/gl/GL",            "glTranslated",               "(DDD)V",                           org_mini_gl_GL_glTranslated},
//        {"org/mini/gl/GL",            "glScalef",                   "(FFF)V",                           org_mini_gl_GL_glScalef},
//        {"org/mini/gl/GL",            "glScaled",                   "(DDD)V",                           org_mini_gl_GL_glScaled},
//        {"org/mini/gl/GL",            "glRotatef",                  "(FFFF)V",                          org_mini_gl_GL_glRotatef},
//        {"org/mini/gl/GL",            "glRotated",                  "(DDDD)V",                          org_mini_gl_GL_glRotated},
//        {"org/mini/gl/GL",            "glCullFace",                 "(I)V",                             org_mini_gl_GL_glCullFace},
//        {"org/mini/gl/GL",            "glFrontFace",                "(I)V",                             org_mini_gl_GL_glFrontFace},
//        {"org/mini/gl/GL",            "glBlendFunc",                "(II)V",                            org_mini_gl_GL_glBlendFunc},
//        {"org/mini/gl/GL",            "glAlphaFunc",                "(IF)V",                            org_mini_gl_GL_glAlphaFunc},
//        {"org/mini/gl/GL",            "glIndexMask",                "(I)V",                             org_mini_gl_GL_glIndexMask},
//        {"org/mini/gl/GL",            "glLogicOp",                  "(I)V",                             org_mini_gl_GL_glLogicOp},
//        {"org/mini/gl/GL",            "glColorMask",                "(ZZZZ)V",                          org_mini_gl_GL_glColorMask},
//        {"org/mini/gl/GL",            "glBegin",                    "(I)V",                             org_mini_gl_GL_glBegin},
//        {"org/mini/gl/GL",            "glEnd",                      "()V",                              org_mini_gl_GL_glEnd},
//        {"org/mini/gl/GL",            "glFlush",                    "()V",                              org_mini_gl_GL_glFlush},
//        {"org/mini/gl/GL",            "glFinish",                   "()V",                              org_mini_gl_GL_glFinish},
//        {"org/mini/gl/GL",            "glColor3f",                  "(FFF)V",                           org_mini_gl_GL_glColor3f},
//        {"org/mini/gl/GL",            "glColor4f",                  "(FFFF)V",                          org_mini_gl_GL_glColor4f},
//        {"org/mini/gl/GL",            "glColor3d",                  "(DDD)V",                           org_mini_gl_GL_glColor3d},
//        {"org/mini/gl/GL",            "glColor4d",                  "(DDDD)V",                          org_mini_gl_GL_glColor4d},
//        {"org/mini/gl/GL",            "glColor3i",                  "(III)V",                           org_mini_gl_GL_glColor3i},
//        {"org/mini/gl/GL",            "glColor4i",                  "(IIII)V",                          org_mini_gl_GL_glColor4i},
//        {"org/mini/gl/GL",            "glColor3ui",                 "(III)V",                           org_mini_gl_GL_glColor3ui},
//        {"org/mini/gl/GL",            "glColor4ui",                 "(IIII)V",                          org_mini_gl_GL_glColor4ui},
//        {"org/mini/gl/GL",            "glColor3b",                  "(BBB)V",                           org_mini_gl_GL_glColor3b},
//        {"org/mini/gl/GL",            "glColor4b",                  "(BBBB)V",                          org_mini_gl_GL_glColor4b},
//        {"org/mini/gl/GL",            "glColor3ub",                 "(BBB)V",                           org_mini_gl_GL_glColor3ub},
//        {"org/mini/gl/GL",            "glColor4ub",                 "(BBBB)V",                          org_mini_gl_GL_glColor4ub},
//        {"org/mini/gl/GL",            "glColor3s",                  "(SSS)V",                           org_mini_gl_GL_glColor3s},
//        {"org/mini/gl/GL",            "glColor4s",                  "(SSSS)V",                          org_mini_gl_GL_glColor4s},
//        {"org/mini/gl/GL",            "glColor3us",                 "(SSS)V",                           org_mini_gl_GL_glColor3us},
//        {"org/mini/gl/GL",            "glColor4us",                 "(SSSS)V",                          org_mini_gl_GL_glColor4us},
//        {"org/mini/gl/GL",            "glColor3fv",                 "([FI)V",                           org_mini_gl_GL_glColor3fv},
//        {"org/mini/gl/GL",            "glColor3dv",                 "([DI)V",                           org_mini_gl_GL_glColor3dv},
//        {"org/mini/gl/GL",            "glColor3iv",                 "([II)V",                           org_mini_gl_GL_glColor3iv},
//        {"org/mini/gl/GL",            "glColor3bv",                 "([BI)V",                           org_mini_gl_GL_glColor3bv},
//        {"org/mini/gl/GL",            "glColor3sv",                 "([SI)V",                           org_mini_gl_GL_glColor3sv},
//        {"org/mini/gl/GL",            "glColor3uiv",                "([II)V",                           org_mini_gl_GL_glColor3uiv},
//        {"org/mini/gl/GL",            "glColor3ubv",                "([BI)V",                           org_mini_gl_GL_glColor3ubv},
//        {"org/mini/gl/GL",            "glColor3usv",                "([SI)V",                           org_mini_gl_GL_glColor3usv},
//        {"org/mini/gl/GL",            "glColor4fv",                 "([FI)V",                           org_mini_gl_GL_glColor4fv},
//        {"org/mini/gl/GL",            "glColor4dv",                 "([DI)V",                           org_mini_gl_GL_glColor4dv},
//        {"org/mini/gl/GL",            "glColor4iv",                 "([II)V",                           org_mini_gl_GL_glColor4iv},
//        {"org/mini/gl/GL",            "glColor4bv",                 "([BI)V",                           org_mini_gl_GL_glColor4bv},
//        {"org/mini/gl/GL",            "glColor4sv",                 "([SI)V",                           org_mini_gl_GL_glColor4sv},
//        {"org/mini/gl/GL",            "glColor4uiv",                "([II)V",                           org_mini_gl_GL_glColor4uiv},
//        {"org/mini/gl/GL",            "glColor4ubv",                "([BI)V",                           org_mini_gl_GL_glColor4ubv},
//        {"org/mini/gl/GL",            "glColor4usv",                "([SI)V",                           org_mini_gl_GL_glColor4usv},
//        {"org/mini/gl/GL",            "glClearColor",               "(FFFF)V",                          org_mini_gl_GL_glClearColor},
//        {"org/mini/gl/GL",            "glVertex2s",                 "(SS)V",                            org_mini_gl_GL_glVertex2s},
//        {"org/mini/gl/GL",            "glVertex3s",                 "(SSS)V",                           org_mini_gl_GL_glVertex3s},
//        {"org/mini/gl/GL",            "glVertex4s",                 "(SSSS)V",                          org_mini_gl_GL_glVertex4s},
//        {"org/mini/gl/GL",            "glVertex2i",                 "(II)V",                            org_mini_gl_GL_glVertex2i},
//        {"org/mini/gl/GL",            "glVertex3i",                 "(III)V",                           org_mini_gl_GL_glVertex3i},
//        {"org/mini/gl/GL",            "glVertex4i",                 "(IIII)V",                          org_mini_gl_GL_glVertex4i},
//        {"org/mini/gl/GL",            "glVertex2f",                 "(FF)V",                            org_mini_gl_GL_glVertex2f},
//        {"org/mini/gl/GL",            "glVertex3f",                 "(FFF)V",                           org_mini_gl_GL_glVertex3f},
//        {"org/mini/gl/GL",            "glVertex4f",                 "(FFFF)V",                          org_mini_gl_GL_glVertex4f},
//        {"org/mini/gl/GL",            "glVertex2d",                 "(DD)V",                            org_mini_gl_GL_glVertex2d},
//        {"org/mini/gl/GL",            "glVertex3d",                 "(DDD)V",                           org_mini_gl_GL_glVertex3d},
//        {"org/mini/gl/GL",            "glVertex4d",                 "(DDDD)V",                          org_mini_gl_GL_glVertex4d},
//        {"org/mini/gl/GL",            "glVertex2sv",                "([SI)V",                           org_mini_gl_GL_glVertex2sv},
//        {"org/mini/gl/GL",            "glVertex3sv",                "([SI)V",                           org_mini_gl_GL_glVertex3sv},
//        {"org/mini/gl/GL",            "glVertex4sv",                "([SI)V",                           org_mini_gl_GL_glVertex4sv},
//        {"org/mini/gl/GL",            "glVertex2iv",                "([II)V",                           org_mini_gl_GL_glVertex2iv},
//        {"org/mini/gl/GL",            "glVertex3iv",                "([II)V",                           org_mini_gl_GL_glVertex3iv},
//        {"org/mini/gl/GL",            "glVertex4iv",                "([II)V",                           org_mini_gl_GL_glVertex4iv},
//        {"org/mini/gl/GL",            "glVertex2fv",                "([FI)V",                           org_mini_gl_GL_glVertex2fv},
//        {"org/mini/gl/GL",            "glVertex3fv",                "([FI)V",                           org_mini_gl_GL_glVertex3fv},
//        {"org/mini/gl/GL",            "glVertex4fv",                "([FI)V",                           org_mini_gl_GL_glVertex4fv},
//        {"org/mini/gl/GL",            "glVertex2dv",                "([DI)V",                           org_mini_gl_GL_glVertex2dv},
//        {"org/mini/gl/GL",            "glVertex3dv",                "([DI)V",                           org_mini_gl_GL_glVertex3dv},
//        {"org/mini/gl/GL",            "glVertex4dv",                "([DI)V",                           org_mini_gl_GL_glVertex4dv},
//        {"org/mini/gl/GL",            "glNormal3b",                 "(BBB)V",                           org_mini_gl_GL_glNormal3b},
//        {"org/mini/gl/GL",            "glNormal3s",                 "(SSS)V",                           org_mini_gl_GL_glNormal3s},
//        {"org/mini/gl/GL",            "glNormal3i",                 "(III)V",                           org_mini_gl_GL_glNormal3i},
//        {"org/mini/gl/GL",            "glNormal3f",                 "(FFF)V",                           org_mini_gl_GL_glNormal3f},
//        {"org/mini/gl/GL",            "glNormal3d",                 "(DDD)V",                           org_mini_gl_GL_glNormal3d},
//        {"org/mini/gl/GL",            "glNormal3bv",                "([BI)V",                           org_mini_gl_GL_glNormal3bv},
//        {"org/mini/gl/GL",            "glNormal3sv",                "([SI)V",                           org_mini_gl_GL_glNormal3sv},
//        {"org/mini/gl/GL",            "glNormal3iv",                "([II)V",                           org_mini_gl_GL_glNormal3iv},
//        {"org/mini/gl/GL",            "glNormal3fv",                "([FI)V",                           org_mini_gl_GL_glNormal3fv},
//        {"org/mini/gl/GL",            "glNormal3dv",                "([DI)V",                           org_mini_gl_GL_glNormal3dv},
//        {"org/mini/gl/GL",            "glRectf",                    "(FFFF)V",                          org_mini_gl_GL_glRectf},
//        {"org/mini/gl/GL",            "glLightfv",                  "(II[FI)V",                         org_mini_gl_GL_glLightfv},
//        {"org/mini/gl/GL",            "glLightiv",                  "(II[II)V",                         org_mini_gl_GL_glLightiv},
//        {"org/mini/gl/GL",            "glMaterialf",                "(IIF)V",                           org_mini_gl_GL_glMaterialf},
//        {"org/mini/gl/GL",            "glMateriali",                "(III)V",                           org_mini_gl_GL_glMateriali},
//        {"org/mini/gl/GL",            "glMaterialfv",               "(II[FI)V",                         org_mini_gl_GL_glMaterialfv},
//        {"org/mini/gl/GL",            "glMaterialiv",               "(II[II)V",                         org_mini_gl_GL_glMaterialiv},
//        {"org/mini/gl/GL",            "glGetMaterialfv",            "(II[FI)V",                         org_mini_gl_GL_glGetMaterialfv},
//        {"org/mini/gl/GL",            "glGetMaterialiv",            "(II[II)V",                         org_mini_gl_GL_glGetMaterialiv},
//        {"org/mini/gl/GL",            "glColorMaterial",            "(II)V",                            org_mini_gl_GL_glColorMaterial},
//        {"org/mini/gl/GL",            "glNewList",                  "(II)V",                            org_mini_gl_GL_glNewList},
//        {"org/mini/gl/GL",            "glDeleteLists",              "(II)V",                            org_mini_gl_GL_glDeleteLists},
//        {"org/mini/gl/GL",            "glGenLists",                 "(I)I",                             org_mini_gl_GL_glGenLists},
//        {"org/mini/gl/GL",            "glIsList",                   "(I)Z",                             org_mini_gl_GL_glIsList},
//        {"org/mini/gl/GL",            "glListBase",                 "(I)V",                             org_mini_gl_GL_glListBase},
//        {"org/mini/gl/GL",            "glEndList",                  "()V",                              org_mini_gl_GL_glEndList},
//        {"org/mini/gl/GL",            "glCallList",                 "(I)V",                             org_mini_gl_GL_glCallList},
//        {"org/mini/gl/GL",            "glCallLists",                "(I)V",                             org_mini_gl_GL_glCallLists},
//        {"org/mini/gl/GL",            "glDrawArrays",               "(III)V",                           org_mini_gl_GL_glDrawArrays},
//        {"org/mini/gl/GL",            "glGenVertexArrays",          "(I[II)V",                          org_mini_gl_GL_glGenVertexArrays},
//        {"org/mini/gl/GL",            "glDeleteVertexArrays",       "(I[II)V",                          org_mini_gl_GL_glDeleteVertexArrays},
//        {"org/mini/gl/GL",            "glBindVertexArray",          "(I)V",                             org_mini_gl_GL_glBindVertexArray},
//        {"org/mini/gl/GL",            "glEnableVertexAttribArray",  "(I)V",                             org_mini_gl_GL_glEnableVertexAttribArray},
//        {"org/mini/gl/GL",            "glVertexAttribPointer",      "(IIIIII)V",                        org_mini_gl_GL_glVertexAttribPointer},
//        {"org/mini/gl/GL",            "glLoadIdentity",             "()V",                              org_mini_gl_GL_glLoadIdentity},
//        {"org/mini/gl/GL",            "glFrustum",                  "(DDDDDD)V",                        org_mini_gl_GL_glFrustum},
//        {"org/mini/gl/GL",            "glShaderSource",             "(I[B)V",                           org_mini_gl_GL_glShaderSource},
//        {"org/mini/gl/GL",            "glCompileShader",            "(I)V",                             org_mini_gl_GL_glCompileShader},
//        {"org/mini/gl/GL",            "glAttachShader",             "(II)V",                            org_mini_gl_GL_glAttachShader},
//        {"org/mini/gl/GL",            "glUseProgram",               "(I)V",                             org_mini_gl_GL_glUseProgram},
//        {"org/mini/gl/GL",            "glLinkProgram",              "(I)V",                             org_mini_gl_GL_glLinkProgram},
//        {"org/mini/gl/GL",            "glCreateShader",             "(I)I",                             org_mini_gl_GL_glCreateShader},
//        {"org/mini/gl/GL",            "glCreateProgram",            "()I",                              org_mini_gl_GL_glCreateProgram},
//        {"org/mini/gl/GL",            "glBufferData",               "(IJLjava/lang/Object;II)V",        org_mini_gl_GL_glBufferData},
//        {"org/mini/gl/GL",            "glGenBuffers",               "(I[II)V",                          org_mini_gl_GL_glGenBuffers},
//        {"org/mini/gl/GL",            "glBindBuffer",               "(II)V",                            org_mini_gl_GL_glBindBuffer},
//        {"org/mini/gl/GL",            "glPointSize",                "(F)V",                             org_mini_gl_GL_glPointSize},
//        {"org/mini/gl/GL",            "glLineWidth",                "(F)V",                             org_mini_gl_GL_glLineWidth},
//        {"org/mini/gl/GL",            "glPolygonStipple",           "([BI)V",                           org_mini_gl_GL_glPolygonStipple},
//        {"org/mini/gl/GL",            "glGetError",                 "()I",                              org_mini_gl_GL_glGetError},
//        {"org/mini/gl/GL",            "glGetString",                "(I)Ljava/lang/String;",            org_mini_gl_GL_glGetString},



        {"org/mini/gl/GL",  "init",  "()V",  org_mini_gl_GL_init},
        {"org/mini/gl/GL",  "glCullFace",  "(I)V",  org_mini_gl_GL_glCullFace},
        {"org/mini/gl/GL",  "glFrontFace",  "(I)V",  org_mini_gl_GL_glFrontFace},
        {"org/mini/gl/GL",  "glHint",  "(II)V",  org_mini_gl_GL_glHint},
        {"org/mini/gl/GL",  "glLineWidth",  "(F)V",  org_mini_gl_GL_glLineWidth},
        {"org/mini/gl/GL",  "glPointSize",  "(F)V",  org_mini_gl_GL_glPointSize},
        {"org/mini/gl/GL",  "glPolygonMode",  "(II)V",  org_mini_gl_GL_glPolygonMode},
        {"org/mini/gl/GL",  "glScissor",  "(IIII)V",  org_mini_gl_GL_glScissor},
        {"org/mini/gl/GL",  "glTexParameterf",  "(IIF)V",  org_mini_gl_GL_glTexParameterf},
        {"org/mini/gl/GL",  "glTexParameterfv",  "(II[FI)V",  org_mini_gl_GL_glTexParameterfv},
        {"org/mini/gl/GL",  "glTexParameteri",  "(III)V",  org_mini_gl_GL_glTexParameteri},
        {"org/mini/gl/GL",  "glTexParameteriv",  "(II[II)V",  org_mini_gl_GL_glTexParameteriv},
        {"org/mini/gl/GL",  "glTexImage1D",  "(IIIIIIILjava/lang/Object;I)V",  org_mini_gl_GL_glTexImage1D},
        {"org/mini/gl/GL",  "glTexImage2D",  "(IIIIIIIILjava/lang/Object;I)V",  org_mini_gl_GL_glTexImage2D},
        {"org/mini/gl/GL",  "glDrawBuffer",  "(I)V",  org_mini_gl_GL_glDrawBuffer},
        {"org/mini/gl/GL",  "glClear",  "(I)V",  org_mini_gl_GL_glClear},
        {"org/mini/gl/GL",  "glClearColor",  "(FFFF)V",  org_mini_gl_GL_glClearColor},
        {"org/mini/gl/GL",  "glClearStencil",  "(I)V",  org_mini_gl_GL_glClearStencil},
        {"org/mini/gl/GL",  "glClearDepth",  "(D)V",  org_mini_gl_GL_glClearDepth},
        {"org/mini/gl/GL",  "glStencilMask",  "(I)V",  org_mini_gl_GL_glStencilMask},
        {"org/mini/gl/GL",  "glColorMask",  "(IIII)V",  org_mini_gl_GL_glColorMask},
        {"org/mini/gl/GL",  "glDepthMask",  "(I)V",  org_mini_gl_GL_glDepthMask},
        {"org/mini/gl/GL",  "glDisable",  "(I)V",  org_mini_gl_GL_glDisable},
        {"org/mini/gl/GL",  "glEnable",  "(I)V",  org_mini_gl_GL_glEnable},
        {"org/mini/gl/GL",  "glFinish",  "()V",  org_mini_gl_GL_glFinish},
        {"org/mini/gl/GL",  "glFlush",  "()V",  org_mini_gl_GL_glFlush},
        {"org/mini/gl/GL",  "glBlendFunc",  "(II)V",  org_mini_gl_GL_glBlendFunc},
        {"org/mini/gl/GL",  "glLogicOp",  "(I)V",  org_mini_gl_GL_glLogicOp},
        {"org/mini/gl/GL",  "glStencilFunc",  "(III)V",  org_mini_gl_GL_glStencilFunc},
        {"org/mini/gl/GL",  "glStencilOp",  "(III)V",  org_mini_gl_GL_glStencilOp},
        {"org/mini/gl/GL",  "glDepthFunc",  "(I)V",  org_mini_gl_GL_glDepthFunc},
        {"org/mini/gl/GL",  "glPixelStoref",  "(IF)V",  org_mini_gl_GL_glPixelStoref},
        {"org/mini/gl/GL",  "glPixelStorei",  "(II)V",  org_mini_gl_GL_glPixelStorei},
        {"org/mini/gl/GL",  "glReadBuffer",  "(I)V",  org_mini_gl_GL_glReadBuffer},
        {"org/mini/gl/GL",  "glReadPixels",  "(IIIIIILjava/lang/Object;I)V",  org_mini_gl_GL_glReadPixels},
        {"org/mini/gl/GL",  "glGetBooleanv",  "(I[II)V",  org_mini_gl_GL_glGetBooleanv},
        {"org/mini/gl/GL",  "glGetDoublev",  "(I[DI)V",  org_mini_gl_GL_glGetDoublev},
        {"org/mini/gl/GL",  "glGetError",  "()I",  org_mini_gl_GL_glGetError},
        {"org/mini/gl/GL",  "glGetFloatv",  "(I[FI)V",  org_mini_gl_GL_glGetFloatv},
        {"org/mini/gl/GL",  "glGetIntegerv",  "(I[II)V",  org_mini_gl_GL_glGetIntegerv},
        {"org/mini/gl/GL",  "glGetString",  "(I)[B",  org_mini_gl_GL_glGetString},
        {"org/mini/gl/GL",  "glGetTexImage",  "(IIIILjava/lang/Object;I)V",  org_mini_gl_GL_glGetTexImage},
        {"org/mini/gl/GL",  "glGetTexParameterfv",  "(II[FI)V",  org_mini_gl_GL_glGetTexParameterfv},
        {"org/mini/gl/GL",  "glGetTexParameteriv",  "(II[II)V",  org_mini_gl_GL_glGetTexParameteriv},
        {"org/mini/gl/GL",  "glGetTexLevelParameterfv",  "(III[FI)V",  org_mini_gl_GL_glGetTexLevelParameterfv},
        {"org/mini/gl/GL",  "glGetTexLevelParameteriv",  "(III[II)V",  org_mini_gl_GL_glGetTexLevelParameteriv},
        {"org/mini/gl/GL",  "glIsEnabled",  "(I)I",  org_mini_gl_GL_glIsEnabled},
        {"org/mini/gl/GL",  "glDepthRange",  "(DD)V",  org_mini_gl_GL_glDepthRange},
        {"org/mini/gl/GL",  "glViewport",  "(IIII)V",  org_mini_gl_GL_glViewport},
        {"org/mini/gl/GL",  "glNewList",  "(II)V",  org_mini_gl_GL_glNewList},
        {"org/mini/gl/GL",  "glEndList",  "()V",  org_mini_gl_GL_glEndList},
        {"org/mini/gl/GL",  "glCallList",  "(I)V",  org_mini_gl_GL_glCallList},
        {"org/mini/gl/GL",  "glCallLists",  "(IILjava/lang/Object;I)V",  org_mini_gl_GL_glCallLists},
        {"org/mini/gl/GL",  "glDeleteLists",  "(II)V",  org_mini_gl_GL_glDeleteLists},
        {"org/mini/gl/GL",  "glGenLists",  "(I)I",  org_mini_gl_GL_glGenLists},
        {"org/mini/gl/GL",  "glListBase",  "(I)V",  org_mini_gl_GL_glListBase},
        {"org/mini/gl/GL",  "glBegin",  "(I)V",  org_mini_gl_GL_glBegin},
        {"org/mini/gl/GL",  "glBitmap",  "(IIFFFF[BI)V",  org_mini_gl_GL_glBitmap},
        {"org/mini/gl/GL",  "glColor3b",  "(BBB)V",  org_mini_gl_GL_glColor3b},
        {"org/mini/gl/GL",  "glColor3bv",  "([BI)V",  org_mini_gl_GL_glColor3bv},
        {"org/mini/gl/GL",  "glColor3d",  "(DDD)V",  org_mini_gl_GL_glColor3d},
        {"org/mini/gl/GL",  "glColor3dv",  "([DI)V",  org_mini_gl_GL_glColor3dv},
        {"org/mini/gl/GL",  "glColor3f",  "(FFF)V",  org_mini_gl_GL_glColor3f},
        {"org/mini/gl/GL",  "glColor3fv",  "([FI)V",  org_mini_gl_GL_glColor3fv},
        {"org/mini/gl/GL",  "glColor3i",  "(III)V",  org_mini_gl_GL_glColor3i},
        {"org/mini/gl/GL",  "glColor3iv",  "([II)V",  org_mini_gl_GL_glColor3iv},
        {"org/mini/gl/GL",  "glColor3s",  "(SSS)V",  org_mini_gl_GL_glColor3s},
        {"org/mini/gl/GL",  "glColor3sv",  "([SI)V",  org_mini_gl_GL_glColor3sv},
        {"org/mini/gl/GL",  "glColor3ub",  "(BBB)V",  org_mini_gl_GL_glColor3ub},
        {"org/mini/gl/GL",  "glColor3ubv",  "([BI)V",  org_mini_gl_GL_glColor3ubv},
        {"org/mini/gl/GL",  "glColor3ui",  "(III)V",  org_mini_gl_GL_glColor3ui},
        {"org/mini/gl/GL",  "glColor3uiv",  "([II)V",  org_mini_gl_GL_glColor3uiv},
        {"org/mini/gl/GL",  "glColor3us",  "(SSS)V",  org_mini_gl_GL_glColor3us},
        {"org/mini/gl/GL",  "glColor3usv",  "([SI)V",  org_mini_gl_GL_glColor3usv},
        {"org/mini/gl/GL",  "glColor4b",  "(BBBB)V",  org_mini_gl_GL_glColor4b},
        {"org/mini/gl/GL",  "glColor4bv",  "([BI)V",  org_mini_gl_GL_glColor4bv},
        {"org/mini/gl/GL",  "glColor4d",  "(DDDD)V",  org_mini_gl_GL_glColor4d},
        {"org/mini/gl/GL",  "glColor4dv",  "([DI)V",  org_mini_gl_GL_glColor4dv},
        {"org/mini/gl/GL",  "glColor4f",  "(FFFF)V",  org_mini_gl_GL_glColor4f},
        {"org/mini/gl/GL",  "glColor4fv",  "([FI)V",  org_mini_gl_GL_glColor4fv},
        {"org/mini/gl/GL",  "glColor4i",  "(IIII)V",  org_mini_gl_GL_glColor4i},
        {"org/mini/gl/GL",  "glColor4iv",  "([II)V",  org_mini_gl_GL_glColor4iv},
        {"org/mini/gl/GL",  "glColor4s",  "(SSSS)V",  org_mini_gl_GL_glColor4s},
        {"org/mini/gl/GL",  "glColor4sv",  "([SI)V",  org_mini_gl_GL_glColor4sv},
        {"org/mini/gl/GL",  "glColor4ub",  "(BBBB)V",  org_mini_gl_GL_glColor4ub},
        {"org/mini/gl/GL",  "glColor4ubv",  "([BI)V",  org_mini_gl_GL_glColor4ubv},
        {"org/mini/gl/GL",  "glColor4ui",  "(IIII)V",  org_mini_gl_GL_glColor4ui},
        {"org/mini/gl/GL",  "glColor4uiv",  "([II)V",  org_mini_gl_GL_glColor4uiv},
        {"org/mini/gl/GL",  "glColor4us",  "(SSSS)V",  org_mini_gl_GL_glColor4us},
        {"org/mini/gl/GL",  "glColor4usv",  "([SI)V",  org_mini_gl_GL_glColor4usv},
        {"org/mini/gl/GL",  "glEdgeFlag",  "(I)V",  org_mini_gl_GL_glEdgeFlag},
        {"org/mini/gl/GL",  "glEdgeFlagv",  "([II)V",  org_mini_gl_GL_glEdgeFlagv},
        {"org/mini/gl/GL",  "glEnd",  "()V",  org_mini_gl_GL_glEnd},
        {"org/mini/gl/GL",  "glIndexd",  "(D)V",  org_mini_gl_GL_glIndexd},
        {"org/mini/gl/GL",  "glIndexdv",  "([DI)V",  org_mini_gl_GL_glIndexdv},
        {"org/mini/gl/GL",  "glIndexf",  "(F)V",  org_mini_gl_GL_glIndexf},
        {"org/mini/gl/GL",  "glIndexfv",  "([FI)V",  org_mini_gl_GL_glIndexfv},
        {"org/mini/gl/GL",  "glIndexi",  "(I)V",  org_mini_gl_GL_glIndexi},
        {"org/mini/gl/GL",  "glIndexiv",  "([II)V",  org_mini_gl_GL_glIndexiv},
        {"org/mini/gl/GL",  "glIndexs",  "(S)V",  org_mini_gl_GL_glIndexs},
        {"org/mini/gl/GL",  "glIndexsv",  "([SI)V",  org_mini_gl_GL_glIndexsv},
        {"org/mini/gl/GL",  "glNormal3b",  "(BBB)V",  org_mini_gl_GL_glNormal3b},
        {"org/mini/gl/GL",  "glNormal3bv",  "([BI)V",  org_mini_gl_GL_glNormal3bv},
        {"org/mini/gl/GL",  "glNormal3d",  "(DDD)V",  org_mini_gl_GL_glNormal3d},
        {"org/mini/gl/GL",  "glNormal3dv",  "([DI)V",  org_mini_gl_GL_glNormal3dv},
        {"org/mini/gl/GL",  "glNormal3f",  "(FFF)V",  org_mini_gl_GL_glNormal3f},
        {"org/mini/gl/GL",  "glNormal3fv",  "([FI)V",  org_mini_gl_GL_glNormal3fv},
        {"org/mini/gl/GL",  "glNormal3i",  "(III)V",  org_mini_gl_GL_glNormal3i},
        {"org/mini/gl/GL",  "glNormal3iv",  "([II)V",  org_mini_gl_GL_glNormal3iv},
        {"org/mini/gl/GL",  "glNormal3s",  "(SSS)V",  org_mini_gl_GL_glNormal3s},
        {"org/mini/gl/GL",  "glNormal3sv",  "([SI)V",  org_mini_gl_GL_glNormal3sv},
        {"org/mini/gl/GL",  "glRasterPos2d",  "(DD)V",  org_mini_gl_GL_glRasterPos2d},
        {"org/mini/gl/GL",  "glRasterPos2dv",  "([DI)V",  org_mini_gl_GL_glRasterPos2dv},
        {"org/mini/gl/GL",  "glRasterPos2f",  "(FF)V",  org_mini_gl_GL_glRasterPos2f},
        {"org/mini/gl/GL",  "glRasterPos2fv",  "([FI)V",  org_mini_gl_GL_glRasterPos2fv},
        {"org/mini/gl/GL",  "glRasterPos2i",  "(II)V",  org_mini_gl_GL_glRasterPos2i},
        {"org/mini/gl/GL",  "glRasterPos2iv",  "([II)V",  org_mini_gl_GL_glRasterPos2iv},
        {"org/mini/gl/GL",  "glRasterPos2s",  "(SS)V",  org_mini_gl_GL_glRasterPos2s},
        {"org/mini/gl/GL",  "glRasterPos2sv",  "([SI)V",  org_mini_gl_GL_glRasterPos2sv},
        {"org/mini/gl/GL",  "glRasterPos3d",  "(DDD)V",  org_mini_gl_GL_glRasterPos3d},
        {"org/mini/gl/GL",  "glRasterPos3dv",  "([DI)V",  org_mini_gl_GL_glRasterPos3dv},
        {"org/mini/gl/GL",  "glRasterPos3f",  "(FFF)V",  org_mini_gl_GL_glRasterPos3f},
        {"org/mini/gl/GL",  "glRasterPos3fv",  "([FI)V",  org_mini_gl_GL_glRasterPos3fv},
        {"org/mini/gl/GL",  "glRasterPos3i",  "(III)V",  org_mini_gl_GL_glRasterPos3i},
        {"org/mini/gl/GL",  "glRasterPos3iv",  "([II)V",  org_mini_gl_GL_glRasterPos3iv},
        {"org/mini/gl/GL",  "glRasterPos3s",  "(SSS)V",  org_mini_gl_GL_glRasterPos3s},
        {"org/mini/gl/GL",  "glRasterPos3sv",  "([SI)V",  org_mini_gl_GL_glRasterPos3sv},
        {"org/mini/gl/GL",  "glRasterPos4d",  "(DDDD)V",  org_mini_gl_GL_glRasterPos4d},
        {"org/mini/gl/GL",  "glRasterPos4dv",  "([DI)V",  org_mini_gl_GL_glRasterPos4dv},
        {"org/mini/gl/GL",  "glRasterPos4f",  "(FFFF)V",  org_mini_gl_GL_glRasterPos4f},
        {"org/mini/gl/GL",  "glRasterPos4fv",  "([FI)V",  org_mini_gl_GL_glRasterPos4fv},
        {"org/mini/gl/GL",  "glRasterPos4i",  "(IIII)V",  org_mini_gl_GL_glRasterPos4i},
        {"org/mini/gl/GL",  "glRasterPos4iv",  "([II)V",  org_mini_gl_GL_glRasterPos4iv},
        {"org/mini/gl/GL",  "glRasterPos4s",  "(SSSS)V",  org_mini_gl_GL_glRasterPos4s},
        {"org/mini/gl/GL",  "glRasterPos4sv",  "([SI)V",  org_mini_gl_GL_glRasterPos4sv},
        {"org/mini/gl/GL",  "glRectd",  "(DDDD)V",  org_mini_gl_GL_glRectd},
        {"org/mini/gl/GL",  "glRectdv",  "([DI[DI)V",  org_mini_gl_GL_glRectdv},
        {"org/mini/gl/GL",  "glRectf",  "(FFFF)V",  org_mini_gl_GL_glRectf},
        {"org/mini/gl/GL",  "glRectfv",  "([FI[FI)V",  org_mini_gl_GL_glRectfv},
        {"org/mini/gl/GL",  "glRecti",  "(IIII)V",  org_mini_gl_GL_glRecti},
        {"org/mini/gl/GL",  "glRectiv",  "([II[II)V",  org_mini_gl_GL_glRectiv},
        {"org/mini/gl/GL",  "glRects",  "(SSSS)V",  org_mini_gl_GL_glRects},
        {"org/mini/gl/GL",  "glRectsv",  "([SI[SI)V",  org_mini_gl_GL_glRectsv},
        {"org/mini/gl/GL",  "glTexCoord1d",  "(D)V",  org_mini_gl_GL_glTexCoord1d},
        {"org/mini/gl/GL",  "glTexCoord1dv",  "([DI)V",  org_mini_gl_GL_glTexCoord1dv},
        {"org/mini/gl/GL",  "glTexCoord1f",  "(F)V",  org_mini_gl_GL_glTexCoord1f},
        {"org/mini/gl/GL",  "glTexCoord1fv",  "([FI)V",  org_mini_gl_GL_glTexCoord1fv},
        {"org/mini/gl/GL",  "glTexCoord1i",  "(I)V",  org_mini_gl_GL_glTexCoord1i},
        {"org/mini/gl/GL",  "glTexCoord1iv",  "([II)V",  org_mini_gl_GL_glTexCoord1iv},
        {"org/mini/gl/GL",  "glTexCoord1s",  "(S)V",  org_mini_gl_GL_glTexCoord1s},
        {"org/mini/gl/GL",  "glTexCoord1sv",  "([SI)V",  org_mini_gl_GL_glTexCoord1sv},
        {"org/mini/gl/GL",  "glTexCoord2d",  "(DD)V",  org_mini_gl_GL_glTexCoord2d},
        {"org/mini/gl/GL",  "glTexCoord2dv",  "([DI)V",  org_mini_gl_GL_glTexCoord2dv},
        {"org/mini/gl/GL",  "glTexCoord2f",  "(FF)V",  org_mini_gl_GL_glTexCoord2f},
        {"org/mini/gl/GL",  "glTexCoord2fv",  "([FI)V",  org_mini_gl_GL_glTexCoord2fv},
        {"org/mini/gl/GL",  "glTexCoord2i",  "(II)V",  org_mini_gl_GL_glTexCoord2i},
        {"org/mini/gl/GL",  "glTexCoord2iv",  "([II)V",  org_mini_gl_GL_glTexCoord2iv},
        {"org/mini/gl/GL",  "glTexCoord2s",  "(SS)V",  org_mini_gl_GL_glTexCoord2s},
        {"org/mini/gl/GL",  "glTexCoord2sv",  "([SI)V",  org_mini_gl_GL_glTexCoord2sv},
        {"org/mini/gl/GL",  "glTexCoord3d",  "(DDD)V",  org_mini_gl_GL_glTexCoord3d},
        {"org/mini/gl/GL",  "glTexCoord3dv",  "([DI)V",  org_mini_gl_GL_glTexCoord3dv},
        {"org/mini/gl/GL",  "glTexCoord3f",  "(FFF)V",  org_mini_gl_GL_glTexCoord3f},
        {"org/mini/gl/GL",  "glTexCoord3fv",  "([FI)V",  org_mini_gl_GL_glTexCoord3fv},
        {"org/mini/gl/GL",  "glTexCoord3i",  "(III)V",  org_mini_gl_GL_glTexCoord3i},
        {"org/mini/gl/GL",  "glTexCoord3iv",  "([II)V",  org_mini_gl_GL_glTexCoord3iv},
        {"org/mini/gl/GL",  "glTexCoord3s",  "(SSS)V",  org_mini_gl_GL_glTexCoord3s},
        {"org/mini/gl/GL",  "glTexCoord3sv",  "([SI)V",  org_mini_gl_GL_glTexCoord3sv},
        {"org/mini/gl/GL",  "glTexCoord4d",  "(DDDD)V",  org_mini_gl_GL_glTexCoord4d},
        {"org/mini/gl/GL",  "glTexCoord4dv",  "([DI)V",  org_mini_gl_GL_glTexCoord4dv},
        {"org/mini/gl/GL",  "glTexCoord4f",  "(FFFF)V",  org_mini_gl_GL_glTexCoord4f},
        {"org/mini/gl/GL",  "glTexCoord4fv",  "([FI)V",  org_mini_gl_GL_glTexCoord4fv},
        {"org/mini/gl/GL",  "glTexCoord4i",  "(IIII)V",  org_mini_gl_GL_glTexCoord4i},
        {"org/mini/gl/GL",  "glTexCoord4iv",  "([II)V",  org_mini_gl_GL_glTexCoord4iv},
        {"org/mini/gl/GL",  "glTexCoord4s",  "(SSSS)V",  org_mini_gl_GL_glTexCoord4s},
        {"org/mini/gl/GL",  "glTexCoord4sv",  "([SI)V",  org_mini_gl_GL_glTexCoord4sv},
        {"org/mini/gl/GL",  "glVertex2d",  "(DD)V",  org_mini_gl_GL_glVertex2d},
        {"org/mini/gl/GL",  "glVertex2dv",  "([DI)V",  org_mini_gl_GL_glVertex2dv},
        {"org/mini/gl/GL",  "glVertex2f",  "(FF)V",  org_mini_gl_GL_glVertex2f},
        {"org/mini/gl/GL",  "glVertex2fv",  "([FI)V",  org_mini_gl_GL_glVertex2fv},
        {"org/mini/gl/GL",  "glVertex2i",  "(II)V",  org_mini_gl_GL_glVertex2i},
        {"org/mini/gl/GL",  "glVertex2iv",  "([II)V",  org_mini_gl_GL_glVertex2iv},
        {"org/mini/gl/GL",  "glVertex2s",  "(SS)V",  org_mini_gl_GL_glVertex2s},
        {"org/mini/gl/GL",  "glVertex2sv",  "([SI)V",  org_mini_gl_GL_glVertex2sv},
        {"org/mini/gl/GL",  "glVertex3d",  "(DDD)V",  org_mini_gl_GL_glVertex3d},
        {"org/mini/gl/GL",  "glVertex3dv",  "([DI)V",  org_mini_gl_GL_glVertex3dv},
        {"org/mini/gl/GL",  "glVertex3f",  "(FFF)V",  org_mini_gl_GL_glVertex3f},
        {"org/mini/gl/GL",  "glVertex3fv",  "([FI)V",  org_mini_gl_GL_glVertex3fv},
        {"org/mini/gl/GL",  "glVertex3i",  "(III)V",  org_mini_gl_GL_glVertex3i},
        {"org/mini/gl/GL",  "glVertex3iv",  "([II)V",  org_mini_gl_GL_glVertex3iv},
        {"org/mini/gl/GL",  "glVertex3s",  "(SSS)V",  org_mini_gl_GL_glVertex3s},
        {"org/mini/gl/GL",  "glVertex3sv",  "([SI)V",  org_mini_gl_GL_glVertex3sv},
        {"org/mini/gl/GL",  "glVertex4d",  "(DDDD)V",  org_mini_gl_GL_glVertex4d},
        {"org/mini/gl/GL",  "glVertex4dv",  "([DI)V",  org_mini_gl_GL_glVertex4dv},
        {"org/mini/gl/GL",  "glVertex4f",  "(FFFF)V",  org_mini_gl_GL_glVertex4f},
        {"org/mini/gl/GL",  "glVertex4fv",  "([FI)V",  org_mini_gl_GL_glVertex4fv},
        {"org/mini/gl/GL",  "glVertex4i",  "(IIII)V",  org_mini_gl_GL_glVertex4i},
        {"org/mini/gl/GL",  "glVertex4iv",  "([II)V",  org_mini_gl_GL_glVertex4iv},
        {"org/mini/gl/GL",  "glVertex4s",  "(SSSS)V",  org_mini_gl_GL_glVertex4s},
        {"org/mini/gl/GL",  "glVertex4sv",  "([SI)V",  org_mini_gl_GL_glVertex4sv},
        {"org/mini/gl/GL",  "glClipPlane",  "(I[DI)V",  org_mini_gl_GL_glClipPlane},
        {"org/mini/gl/GL",  "glColorMaterial",  "(II)V",  org_mini_gl_GL_glColorMaterial},
        {"org/mini/gl/GL",  "glFogf",  "(IF)V",  org_mini_gl_GL_glFogf},
        {"org/mini/gl/GL",  "glFogfv",  "(I[FI)V",  org_mini_gl_GL_glFogfv},
        {"org/mini/gl/GL",  "glFogi",  "(II)V",  org_mini_gl_GL_glFogi},
        {"org/mini/gl/GL",  "glFogiv",  "(I[II)V",  org_mini_gl_GL_glFogiv},
        {"org/mini/gl/GL",  "glLightf",  "(IIF)V",  org_mini_gl_GL_glLightf},
        {"org/mini/gl/GL",  "glLightfv",  "(II[FI)V",  org_mini_gl_GL_glLightfv},
        {"org/mini/gl/GL",  "glLighti",  "(III)V",  org_mini_gl_GL_glLighti},
        {"org/mini/gl/GL",  "glLightiv",  "(II[II)V",  org_mini_gl_GL_glLightiv},
        {"org/mini/gl/GL",  "glLightModelf",  "(IF)V",  org_mini_gl_GL_glLightModelf},
        {"org/mini/gl/GL",  "glLightModelfv",  "(I[FI)V",  org_mini_gl_GL_glLightModelfv},
        {"org/mini/gl/GL",  "glLightModeli",  "(II)V",  org_mini_gl_GL_glLightModeli},
        {"org/mini/gl/GL",  "glLightModeliv",  "(I[II)V",  org_mini_gl_GL_glLightModeliv},
        {"org/mini/gl/GL",  "glLineStipple",  "(IS)V",  org_mini_gl_GL_glLineStipple},
        {"org/mini/gl/GL",  "glMaterialf",  "(IIF)V",  org_mini_gl_GL_glMaterialf},
        {"org/mini/gl/GL",  "glMaterialfv",  "(II[FI)V",  org_mini_gl_GL_glMaterialfv},
        {"org/mini/gl/GL",  "glMateriali",  "(III)V",  org_mini_gl_GL_glMateriali},
        {"org/mini/gl/GL",  "glMaterialiv",  "(II[II)V",  org_mini_gl_GL_glMaterialiv},
        {"org/mini/gl/GL",  "glPolygonStipple",  "([BI)V",  org_mini_gl_GL_glPolygonStipple},
        {"org/mini/gl/GL",  "glShadeModel",  "(I)V",  org_mini_gl_GL_glShadeModel},
        {"org/mini/gl/GL",  "glTexEnvf",  "(IIF)V",  org_mini_gl_GL_glTexEnvf},
        {"org/mini/gl/GL",  "glTexEnvfv",  "(II[FI)V",  org_mini_gl_GL_glTexEnvfv},
        {"org/mini/gl/GL",  "glTexEnvi",  "(III)V",  org_mini_gl_GL_glTexEnvi},
        {"org/mini/gl/GL",  "glTexEnviv",  "(II[II)V",  org_mini_gl_GL_glTexEnviv},
        {"org/mini/gl/GL",  "glTexGend",  "(IID)V",  org_mini_gl_GL_glTexGend},
        {"org/mini/gl/GL",  "glTexGendv",  "(II[DI)V",  org_mini_gl_GL_glTexGendv},
        {"org/mini/gl/GL",  "glTexGenf",  "(IIF)V",  org_mini_gl_GL_glTexGenf},
        {"org/mini/gl/GL",  "glTexGenfv",  "(II[FI)V",  org_mini_gl_GL_glTexGenfv},
        {"org/mini/gl/GL",  "glTexGeni",  "(III)V",  org_mini_gl_GL_glTexGeni},
        {"org/mini/gl/GL",  "glTexGeniv",  "(II[II)V",  org_mini_gl_GL_glTexGeniv},
        {"org/mini/gl/GL",  "glFeedbackBuffer",  "(II[FI)V",  org_mini_gl_GL_glFeedbackBuffer},
        {"org/mini/gl/GL",  "glSelectBuffer",  "(I[II)V",  org_mini_gl_GL_glSelectBuffer},
        {"org/mini/gl/GL",  "glRenderMode",  "(I)I",  org_mini_gl_GL_glRenderMode},
        {"org/mini/gl/GL",  "glInitNames",  "()V",  org_mini_gl_GL_glInitNames},
        {"org/mini/gl/GL",  "glLoadName",  "(I)V",  org_mini_gl_GL_glLoadName},
        {"org/mini/gl/GL",  "glPassThrough",  "(F)V",  org_mini_gl_GL_glPassThrough},
        {"org/mini/gl/GL",  "glPopName",  "()V",  org_mini_gl_GL_glPopName},
        {"org/mini/gl/GL",  "glPushName",  "(I)V",  org_mini_gl_GL_glPushName},
        {"org/mini/gl/GL",  "glClearAccum",  "(FFFF)V",  org_mini_gl_GL_glClearAccum},
        {"org/mini/gl/GL",  "glClearIndex",  "(F)V",  org_mini_gl_GL_glClearIndex},
        {"org/mini/gl/GL",  "glIndexMask",  "(I)V",  org_mini_gl_GL_glIndexMask},
        {"org/mini/gl/GL",  "glAccum",  "(IF)V",  org_mini_gl_GL_glAccum},
        {"org/mini/gl/GL",  "glPopAttrib",  "()V",  org_mini_gl_GL_glPopAttrib},
        {"org/mini/gl/GL",  "glPushAttrib",  "(I)V",  org_mini_gl_GL_glPushAttrib},
        {"org/mini/gl/GL",  "glMap1d",  "(IDDII[DI)V",  org_mini_gl_GL_glMap1d},
        {"org/mini/gl/GL",  "glMap1f",  "(IFFII[FI)V",  org_mini_gl_GL_glMap1f},
        {"org/mini/gl/GL",  "glMap2d",  "(IDDIIDDII[DI)V",  org_mini_gl_GL_glMap2d},
        {"org/mini/gl/GL",  "glMap2f",  "(IFFIIFFII[FI)V",  org_mini_gl_GL_glMap2f},
        {"org/mini/gl/GL",  "glMapGrid1d",  "(IDD)V",  org_mini_gl_GL_glMapGrid1d},
        {"org/mini/gl/GL",  "glMapGrid1f",  "(IFF)V",  org_mini_gl_GL_glMapGrid1f},
        {"org/mini/gl/GL",  "glMapGrid2d",  "(IDDIDD)V",  org_mini_gl_GL_glMapGrid2d},
        {"org/mini/gl/GL",  "glMapGrid2f",  "(IFFIFF)V",  org_mini_gl_GL_glMapGrid2f},
        {"org/mini/gl/GL",  "glEvalCoord1d",  "(D)V",  org_mini_gl_GL_glEvalCoord1d},
        {"org/mini/gl/GL",  "glEvalCoord1dv",  "([DI)V",  org_mini_gl_GL_glEvalCoord1dv},
        {"org/mini/gl/GL",  "glEvalCoord1f",  "(F)V",  org_mini_gl_GL_glEvalCoord1f},
        {"org/mini/gl/GL",  "glEvalCoord1fv",  "([FI)V",  org_mini_gl_GL_glEvalCoord1fv},
        {"org/mini/gl/GL",  "glEvalCoord2d",  "(DD)V",  org_mini_gl_GL_glEvalCoord2d},
        {"org/mini/gl/GL",  "glEvalCoord2dv",  "([DI)V",  org_mini_gl_GL_glEvalCoord2dv},
        {"org/mini/gl/GL",  "glEvalCoord2f",  "(FF)V",  org_mini_gl_GL_glEvalCoord2f},
        {"org/mini/gl/GL",  "glEvalCoord2fv",  "([FI)V",  org_mini_gl_GL_glEvalCoord2fv},
        {"org/mini/gl/GL",  "glEvalMesh1",  "(III)V",  org_mini_gl_GL_glEvalMesh1},
        {"org/mini/gl/GL",  "glEvalPoint1",  "(I)V",  org_mini_gl_GL_glEvalPoint1},
        {"org/mini/gl/GL",  "glEvalMesh2",  "(IIIII)V",  org_mini_gl_GL_glEvalMesh2},
        {"org/mini/gl/GL",  "glEvalPoint2",  "(II)V",  org_mini_gl_GL_glEvalPoint2},
        {"org/mini/gl/GL",  "glAlphaFunc",  "(IF)V",  org_mini_gl_GL_glAlphaFunc},
        {"org/mini/gl/GL",  "glPixelZoom",  "(FF)V",  org_mini_gl_GL_glPixelZoom},
        {"org/mini/gl/GL",  "glPixelTransferf",  "(IF)V",  org_mini_gl_GL_glPixelTransferf},
        {"org/mini/gl/GL",  "glPixelTransferi",  "(II)V",  org_mini_gl_GL_glPixelTransferi},
        {"org/mini/gl/GL",  "glPixelMapfv",  "(II[FI)V",  org_mini_gl_GL_glPixelMapfv},
        {"org/mini/gl/GL",  "glPixelMapuiv",  "(II[II)V",  org_mini_gl_GL_glPixelMapuiv},
        {"org/mini/gl/GL",  "glPixelMapusv",  "(II[SI)V",  org_mini_gl_GL_glPixelMapusv},
        {"org/mini/gl/GL",  "glCopyPixels",  "(IIIII)V",  org_mini_gl_GL_glCopyPixels},
        {"org/mini/gl/GL",  "glDrawPixels",  "(IIIILjava/lang/Object;I)V",  org_mini_gl_GL_glDrawPixels},
        {"org/mini/gl/GL",  "glGetClipPlane",  "(I[DI)V",  org_mini_gl_GL_glGetClipPlane},
        {"org/mini/gl/GL",  "glGetLightfv",  "(II[FI)V",  org_mini_gl_GL_glGetLightfv},
        {"org/mini/gl/GL",  "glGetLightiv",  "(II[II)V",  org_mini_gl_GL_glGetLightiv},
        {"org/mini/gl/GL",  "glGetMapdv",  "(II[DI)V",  org_mini_gl_GL_glGetMapdv},
        {"org/mini/gl/GL",  "glGetMapfv",  "(II[FI)V",  org_mini_gl_GL_glGetMapfv},
        {"org/mini/gl/GL",  "glGetMapiv",  "(II[II)V",  org_mini_gl_GL_glGetMapiv},
        {"org/mini/gl/GL",  "glGetMaterialfv",  "(II[FI)V",  org_mini_gl_GL_glGetMaterialfv},
        {"org/mini/gl/GL",  "glGetMaterialiv",  "(II[II)V",  org_mini_gl_GL_glGetMaterialiv},
        {"org/mini/gl/GL",  "glGetPixelMapfv",  "(I[FI)V",  org_mini_gl_GL_glGetPixelMapfv},
        {"org/mini/gl/GL",  "glGetPixelMapuiv",  "(I[II)V",  org_mini_gl_GL_glGetPixelMapuiv},
        {"org/mini/gl/GL",  "glGetPixelMapusv",  "(I[SI)V",  org_mini_gl_GL_glGetPixelMapusv},
        {"org/mini/gl/GL",  "glGetPolygonStipple",  "([BI)V",  org_mini_gl_GL_glGetPolygonStipple},
        {"org/mini/gl/GL",  "glGetTexEnvfv",  "(II[FI)V",  org_mini_gl_GL_glGetTexEnvfv},
        {"org/mini/gl/GL",  "glGetTexEnviv",  "(II[II)V",  org_mini_gl_GL_glGetTexEnviv},
        {"org/mini/gl/GL",  "glGetTexGendv",  "(II[DI)V",  org_mini_gl_GL_glGetTexGendv},
        {"org/mini/gl/GL",  "glGetTexGenfv",  "(II[FI)V",  org_mini_gl_GL_glGetTexGenfv},
        {"org/mini/gl/GL",  "glGetTexGeniv",  "(II[II)V",  org_mini_gl_GL_glGetTexGeniv},
        {"org/mini/gl/GL",  "glIsList",  "(I)I",  org_mini_gl_GL_glIsList},
        {"org/mini/gl/GL",  "glFrustum",  "(DDDDDD)V",  org_mini_gl_GL_glFrustum},
        {"org/mini/gl/GL",  "glLoadIdentity",  "()V",  org_mini_gl_GL_glLoadIdentity},
        {"org/mini/gl/GL",  "glLoadMatrixf",  "([FI)V",  org_mini_gl_GL_glLoadMatrixf},
        {"org/mini/gl/GL",  "glLoadMatrixd",  "([DI)V",  org_mini_gl_GL_glLoadMatrixd},
        {"org/mini/gl/GL",  "glMatrixMode",  "(I)V",  org_mini_gl_GL_glMatrixMode},
        {"org/mini/gl/GL",  "glMultMatrixf",  "([FI)V",  org_mini_gl_GL_glMultMatrixf},
        {"org/mini/gl/GL",  "glMultMatrixd",  "([DI)V",  org_mini_gl_GL_glMultMatrixd},
        {"org/mini/gl/GL",  "glOrtho",  "(DDDDDD)V",  org_mini_gl_GL_glOrtho},
        {"org/mini/gl/GL",  "glPopMatrix",  "()V",  org_mini_gl_GL_glPopMatrix},
        {"org/mini/gl/GL",  "glPushMatrix",  "()V",  org_mini_gl_GL_glPushMatrix},
        {"org/mini/gl/GL",  "glRotated",  "(DDDD)V",  org_mini_gl_GL_glRotated},
        {"org/mini/gl/GL",  "glRotatef",  "(FFFF)V",  org_mini_gl_GL_glRotatef},
        {"org/mini/gl/GL",  "glScaled",  "(DDD)V",  org_mini_gl_GL_glScaled},
        {"org/mini/gl/GL",  "glScalef",  "(FFF)V",  org_mini_gl_GL_glScalef},
        {"org/mini/gl/GL",  "glTranslated",  "(DDD)V",  org_mini_gl_GL_glTranslated},
        {"org/mini/gl/GL",  "glTranslatef",  "(FFF)V",  org_mini_gl_GL_glTranslatef},
        {"org/mini/gl/GL",  "glDrawArrays",  "(III)V",  org_mini_gl_GL_glDrawArrays},
        {"org/mini/gl/GL",  "glDrawElements",  "(IIILjava/lang/Object;I)V",  org_mini_gl_GL_glDrawElements},
        {"org/mini/gl/GL",  "glGetPointerv",  "(ILjava/lang/Object;I)V",  org_mini_gl_GL_glGetPointerv},
        {"org/mini/gl/GL",  "glPolygonOffset",  "(FF)V",  org_mini_gl_GL_glPolygonOffset},
        {"org/mini/gl/GL",  "glCopyTexImage1D",  "(IIIIIII)V",  org_mini_gl_GL_glCopyTexImage1D},
        {"org/mini/gl/GL",  "glCopyTexImage2D",  "(IIIIIIII)V",  org_mini_gl_GL_glCopyTexImage2D},
        {"org/mini/gl/GL",  "glCopyTexSubImage1D",  "(IIIIII)V",  org_mini_gl_GL_glCopyTexSubImage1D},
        {"org/mini/gl/GL",  "glCopyTexSubImage2D",  "(IIIIIIII)V",  org_mini_gl_GL_glCopyTexSubImage2D},
        {"org/mini/gl/GL",  "glTexSubImage1D",  "(IIIIIILjava/lang/Object;I)V",  org_mini_gl_GL_glTexSubImage1D},
        {"org/mini/gl/GL",  "glTexSubImage2D",  "(IIIIIIIILjava/lang/Object;I)V",  org_mini_gl_GL_glTexSubImage2D},
        {"org/mini/gl/GL",  "glBindTexture",  "(II)V",  org_mini_gl_GL_glBindTexture},
        {"org/mini/gl/GL",  "glDeleteTextures",  "(I[II)V",  org_mini_gl_GL_glDeleteTextures},
        {"org/mini/gl/GL",  "glGenTextures",  "(I[II)V",  org_mini_gl_GL_glGenTextures},
        {"org/mini/gl/GL",  "glIsTexture",  "(I)I",  org_mini_gl_GL_glIsTexture},
        {"org/mini/gl/GL",  "glArrayElement",  "(I)V",  org_mini_gl_GL_glArrayElement},
        {"org/mini/gl/GL",  "glColorPointer",  "(IIILjava/lang/Object;I)V",  org_mini_gl_GL_glColorPointer},
        {"org/mini/gl/GL",  "glDisableClientState",  "(I)V",  org_mini_gl_GL_glDisableClientState},
        {"org/mini/gl/GL",  "glEdgeFlagPointer",  "(ILjava/lang/Object;I)V",  org_mini_gl_GL_glEdgeFlagPointer},
        {"org/mini/gl/GL",  "glEnableClientState",  "(I)V",  org_mini_gl_GL_glEnableClientState},
        {"org/mini/gl/GL",  "glIndexPointer",  "(IILjava/lang/Object;I)V",  org_mini_gl_GL_glIndexPointer},
        {"org/mini/gl/GL",  "glInterleavedArrays",  "(IILjava/lang/Object;I)V",  org_mini_gl_GL_glInterleavedArrays},
        {"org/mini/gl/GL",  "glNormalPointer",  "(IILjava/lang/Object;I)V",  org_mini_gl_GL_glNormalPointer},
        {"org/mini/gl/GL",  "glTexCoordPointer",  "(IIILjava/lang/Object;I)V",  org_mini_gl_GL_glTexCoordPointer},
        {"org/mini/gl/GL",  "glVertexPointer",  "(IIILjava/lang/Object;I)V",  org_mini_gl_GL_glVertexPointer},
        {"org/mini/gl/GL",  "glAreTexturesResident",  "(I[II[II)I",  org_mini_gl_GL_glAreTexturesResident},
        {"org/mini/gl/GL",  "glPrioritizeTextures",  "(I[II[FI)V",  org_mini_gl_GL_glPrioritizeTextures},
        {"org/mini/gl/GL",  "glIndexub",  "(B)V",  org_mini_gl_GL_glIndexub},
        {"org/mini/gl/GL",  "glIndexubv",  "([BI)V",  org_mini_gl_GL_glIndexubv},
        {"org/mini/gl/GL",  "glPopClientAttrib",  "()V",  org_mini_gl_GL_glPopClientAttrib},
        {"org/mini/gl/GL",  "glPushClientAttrib",  "(I)V",  org_mini_gl_GL_glPushClientAttrib},
        {"org/mini/gl/GL",  "glDrawRangeElements",  "(IIIIILjava/lang/Object;I)V",  org_mini_gl_GL_glDrawRangeElements},
        {"org/mini/gl/GL",  "glTexImage3D",  "(IIIIIIIIILjava/lang/Object;I)V",  org_mini_gl_GL_glTexImage3D},
        {"org/mini/gl/GL",  "glTexSubImage3D",  "(IIIIIIIIIILjava/lang/Object;I)V",  org_mini_gl_GL_glTexSubImage3D},
        {"org/mini/gl/GL",  "glCopyTexSubImage3D",  "(IIIIIIIII)V",  org_mini_gl_GL_glCopyTexSubImage3D},
        {"org/mini/gl/GL",  "glActiveTexture",  "(I)V",  org_mini_gl_GL_glActiveTexture},
        {"org/mini/gl/GL",  "glSampleCoverage",  "(FI)V",  org_mini_gl_GL_glSampleCoverage},
        {"org/mini/gl/GL",  "glCompressedTexImage3D",  "(IIIIIIIILjava/lang/Object;I)V",  org_mini_gl_GL_glCompressedTexImage3D},
        {"org/mini/gl/GL",  "glCompressedTexImage2D",  "(IIIIIIILjava/lang/Object;I)V",  org_mini_gl_GL_glCompressedTexImage2D},
        {"org/mini/gl/GL",  "glCompressedTexImage1D",  "(IIIIIILjava/lang/Object;I)V",  org_mini_gl_GL_glCompressedTexImage1D},
        {"org/mini/gl/GL",  "glCompressedTexSubImage3D",  "(IIIIIIIIIILjava/lang/Object;I)V",  org_mini_gl_GL_glCompressedTexSubImage3D},
        {"org/mini/gl/GL",  "glCompressedTexSubImage2D",  "(IIIIIIIILjava/lang/Object;I)V",  org_mini_gl_GL_glCompressedTexSubImage2D},
        {"org/mini/gl/GL",  "glCompressedTexSubImage1D",  "(IIIIIILjava/lang/Object;I)V",  org_mini_gl_GL_glCompressedTexSubImage1D},
        {"org/mini/gl/GL",  "glGetCompressedTexImage",  "(IILjava/lang/Object;I)V",  org_mini_gl_GL_glGetCompressedTexImage},
        {"org/mini/gl/GL",  "glClientActiveTexture",  "(I)V",  org_mini_gl_GL_glClientActiveTexture},
        {"org/mini/gl/GL",  "glMultiTexCoord1d",  "(ID)V",  org_mini_gl_GL_glMultiTexCoord1d},
        {"org/mini/gl/GL",  "glMultiTexCoord1dv",  "(I[DI)V",  org_mini_gl_GL_glMultiTexCoord1dv},
        {"org/mini/gl/GL",  "glMultiTexCoord1f",  "(IF)V",  org_mini_gl_GL_glMultiTexCoord1f},
        {"org/mini/gl/GL",  "glMultiTexCoord1fv",  "(I[FI)V",  org_mini_gl_GL_glMultiTexCoord1fv},
        {"org/mini/gl/GL",  "glMultiTexCoord1i",  "(II)V",  org_mini_gl_GL_glMultiTexCoord1i},
        {"org/mini/gl/GL",  "glMultiTexCoord1iv",  "(I[II)V",  org_mini_gl_GL_glMultiTexCoord1iv},
        {"org/mini/gl/GL",  "glMultiTexCoord1s",  "(IS)V",  org_mini_gl_GL_glMultiTexCoord1s},
        {"org/mini/gl/GL",  "glMultiTexCoord1sv",  "(I[SI)V",  org_mini_gl_GL_glMultiTexCoord1sv},
        {"org/mini/gl/GL",  "glMultiTexCoord2d",  "(IDD)V",  org_mini_gl_GL_glMultiTexCoord2d},
        {"org/mini/gl/GL",  "glMultiTexCoord2dv",  "(I[DI)V",  org_mini_gl_GL_glMultiTexCoord2dv},
        {"org/mini/gl/GL",  "glMultiTexCoord2f",  "(IFF)V",  org_mini_gl_GL_glMultiTexCoord2f},
        {"org/mini/gl/GL",  "glMultiTexCoord2fv",  "(I[FI)V",  org_mini_gl_GL_glMultiTexCoord2fv},
        {"org/mini/gl/GL",  "glMultiTexCoord2i",  "(III)V",  org_mini_gl_GL_glMultiTexCoord2i},
        {"org/mini/gl/GL",  "glMultiTexCoord2iv",  "(I[II)V",  org_mini_gl_GL_glMultiTexCoord2iv},
        {"org/mini/gl/GL",  "glMultiTexCoord2s",  "(ISS)V",  org_mini_gl_GL_glMultiTexCoord2s},
        {"org/mini/gl/GL",  "glMultiTexCoord2sv",  "(I[SI)V",  org_mini_gl_GL_glMultiTexCoord2sv},
        {"org/mini/gl/GL",  "glMultiTexCoord3d",  "(IDDD)V",  org_mini_gl_GL_glMultiTexCoord3d},
        {"org/mini/gl/GL",  "glMultiTexCoord3dv",  "(I[DI)V",  org_mini_gl_GL_glMultiTexCoord3dv},
        {"org/mini/gl/GL",  "glMultiTexCoord3f",  "(IFFF)V",  org_mini_gl_GL_glMultiTexCoord3f},
        {"org/mini/gl/GL",  "glMultiTexCoord3fv",  "(I[FI)V",  org_mini_gl_GL_glMultiTexCoord3fv},
        {"org/mini/gl/GL",  "glMultiTexCoord3i",  "(IIII)V",  org_mini_gl_GL_glMultiTexCoord3i},
        {"org/mini/gl/GL",  "glMultiTexCoord3iv",  "(I[II)V",  org_mini_gl_GL_glMultiTexCoord3iv},
        {"org/mini/gl/GL",  "glMultiTexCoord3s",  "(ISSS)V",  org_mini_gl_GL_glMultiTexCoord3s},
        {"org/mini/gl/GL",  "glMultiTexCoord3sv",  "(I[SI)V",  org_mini_gl_GL_glMultiTexCoord3sv},
        {"org/mini/gl/GL",  "glMultiTexCoord4d",  "(IDDDD)V",  org_mini_gl_GL_glMultiTexCoord4d},
        {"org/mini/gl/GL",  "glMultiTexCoord4dv",  "(I[DI)V",  org_mini_gl_GL_glMultiTexCoord4dv},
        {"org/mini/gl/GL",  "glMultiTexCoord4f",  "(IFFFF)V",  org_mini_gl_GL_glMultiTexCoord4f},
        {"org/mini/gl/GL",  "glMultiTexCoord4fv",  "(I[FI)V",  org_mini_gl_GL_glMultiTexCoord4fv},
        {"org/mini/gl/GL",  "glMultiTexCoord4i",  "(IIIII)V",  org_mini_gl_GL_glMultiTexCoord4i},
        {"org/mini/gl/GL",  "glMultiTexCoord4iv",  "(I[II)V",  org_mini_gl_GL_glMultiTexCoord4iv},
        {"org/mini/gl/GL",  "glMultiTexCoord4s",  "(ISSSS)V",  org_mini_gl_GL_glMultiTexCoord4s},
        {"org/mini/gl/GL",  "glMultiTexCoord4sv",  "(I[SI)V",  org_mini_gl_GL_glMultiTexCoord4sv},
        {"org/mini/gl/GL",  "glLoadTransposeMatrixf",  "([FI)V",  org_mini_gl_GL_glLoadTransposeMatrixf},
        {"org/mini/gl/GL",  "glLoadTransposeMatrixd",  "([DI)V",  org_mini_gl_GL_glLoadTransposeMatrixd},
        {"org/mini/gl/GL",  "glMultTransposeMatrixf",  "([FI)V",  org_mini_gl_GL_glMultTransposeMatrixf},
        {"org/mini/gl/GL",  "glMultTransposeMatrixd",  "([DI)V",  org_mini_gl_GL_glMultTransposeMatrixd},
        {"org/mini/gl/GL",  "glBlendFuncSeparate",  "(IIII)V",  org_mini_gl_GL_glBlendFuncSeparate},
        {"org/mini/gl/GL",  "glMultiDrawArrays",  "(I[II[III)V",  org_mini_gl_GL_glMultiDrawArrays},
        {"org/mini/gl/GL",  "glMultiDrawElements",  "(I[IIILjava/lang/Object;II)V",  org_mini_gl_GL_glMultiDrawElements},
        {"org/mini/gl/GL",  "glPointParameterf",  "(IF)V",  org_mini_gl_GL_glPointParameterf},
        {"org/mini/gl/GL",  "glPointParameterfv",  "(I[FI)V",  org_mini_gl_GL_glPointParameterfv},
        {"org/mini/gl/GL",  "glPointParameteri",  "(II)V",  org_mini_gl_GL_glPointParameteri},
        {"org/mini/gl/GL",  "glPointParameteriv",  "(I[II)V",  org_mini_gl_GL_glPointParameteriv},
        {"org/mini/gl/GL",  "glFogCoordf",  "(F)V",  org_mini_gl_GL_glFogCoordf},
        {"org/mini/gl/GL",  "glFogCoordfv",  "([FI)V",  org_mini_gl_GL_glFogCoordfv},
        {"org/mini/gl/GL",  "glFogCoordd",  "(D)V",  org_mini_gl_GL_glFogCoordd},
        {"org/mini/gl/GL",  "glFogCoorddv",  "([DI)V",  org_mini_gl_GL_glFogCoorddv},
        {"org/mini/gl/GL",  "glFogCoordPointer",  "(IILjava/lang/Object;I)V",  org_mini_gl_GL_glFogCoordPointer},
        {"org/mini/gl/GL",  "glSecondaryColor3b",  "(BBB)V",  org_mini_gl_GL_glSecondaryColor3b},
        {"org/mini/gl/GL",  "glSecondaryColor3bv",  "([BI)V",  org_mini_gl_GL_glSecondaryColor3bv},
        {"org/mini/gl/GL",  "glSecondaryColor3d",  "(DDD)V",  org_mini_gl_GL_glSecondaryColor3d},
        {"org/mini/gl/GL",  "glSecondaryColor3dv",  "([DI)V",  org_mini_gl_GL_glSecondaryColor3dv},
        {"org/mini/gl/GL",  "glSecondaryColor3f",  "(FFF)V",  org_mini_gl_GL_glSecondaryColor3f},
        {"org/mini/gl/GL",  "glSecondaryColor3fv",  "([FI)V",  org_mini_gl_GL_glSecondaryColor3fv},
        {"org/mini/gl/GL",  "glSecondaryColor3i",  "(III)V",  org_mini_gl_GL_glSecondaryColor3i},
        {"org/mini/gl/GL",  "glSecondaryColor3iv",  "([II)V",  org_mini_gl_GL_glSecondaryColor3iv},
        {"org/mini/gl/GL",  "glSecondaryColor3s",  "(SSS)V",  org_mini_gl_GL_glSecondaryColor3s},
        {"org/mini/gl/GL",  "glSecondaryColor3sv",  "([SI)V",  org_mini_gl_GL_glSecondaryColor3sv},
        {"org/mini/gl/GL",  "glSecondaryColor3ub",  "(BBB)V",  org_mini_gl_GL_glSecondaryColor3ub},
        {"org/mini/gl/GL",  "glSecondaryColor3ubv",  "([BI)V",  org_mini_gl_GL_glSecondaryColor3ubv},
        {"org/mini/gl/GL",  "glSecondaryColor3ui",  "(III)V",  org_mini_gl_GL_glSecondaryColor3ui},
        {"org/mini/gl/GL",  "glSecondaryColor3uiv",  "([II)V",  org_mini_gl_GL_glSecondaryColor3uiv},
        {"org/mini/gl/GL",  "glSecondaryColor3us",  "(SSS)V",  org_mini_gl_GL_glSecondaryColor3us},
        {"org/mini/gl/GL",  "glSecondaryColor3usv",  "([SI)V",  org_mini_gl_GL_glSecondaryColor3usv},
        {"org/mini/gl/GL",  "glSecondaryColorPointer",  "(IIILjava/lang/Object;I)V",  org_mini_gl_GL_glSecondaryColorPointer},
        {"org/mini/gl/GL",  "glWindowPos2d",  "(DD)V",  org_mini_gl_GL_glWindowPos2d},
        {"org/mini/gl/GL",  "glWindowPos2dv",  "([DI)V",  org_mini_gl_GL_glWindowPos2dv},
        {"org/mini/gl/GL",  "glWindowPos2f",  "(FF)V",  org_mini_gl_GL_glWindowPos2f},
        {"org/mini/gl/GL",  "glWindowPos2fv",  "([FI)V",  org_mini_gl_GL_glWindowPos2fv},
        {"org/mini/gl/GL",  "glWindowPos2i",  "(II)V",  org_mini_gl_GL_glWindowPos2i},
        {"org/mini/gl/GL",  "glWindowPos2iv",  "([II)V",  org_mini_gl_GL_glWindowPos2iv},
        {"org/mini/gl/GL",  "glWindowPos2s",  "(SS)V",  org_mini_gl_GL_glWindowPos2s},
        {"org/mini/gl/GL",  "glWindowPos2sv",  "([SI)V",  org_mini_gl_GL_glWindowPos2sv},
        {"org/mini/gl/GL",  "glWindowPos3d",  "(DDD)V",  org_mini_gl_GL_glWindowPos3d},
        {"org/mini/gl/GL",  "glWindowPos3dv",  "([DI)V",  org_mini_gl_GL_glWindowPos3dv},
        {"org/mini/gl/GL",  "glWindowPos3f",  "(FFF)V",  org_mini_gl_GL_glWindowPos3f},
        {"org/mini/gl/GL",  "glWindowPos3fv",  "([FI)V",  org_mini_gl_GL_glWindowPos3fv},
        {"org/mini/gl/GL",  "glWindowPos3i",  "(III)V",  org_mini_gl_GL_glWindowPos3i},
        {"org/mini/gl/GL",  "glWindowPos3iv",  "([II)V",  org_mini_gl_GL_glWindowPos3iv},
        {"org/mini/gl/GL",  "glWindowPos3s",  "(SSS)V",  org_mini_gl_GL_glWindowPos3s},
        {"org/mini/gl/GL",  "glWindowPos3sv",  "([SI)V",  org_mini_gl_GL_glWindowPos3sv},
        {"org/mini/gl/GL",  "glBlendColor",  "(FFFF)V",  org_mini_gl_GL_glBlendColor},
        {"org/mini/gl/GL",  "glBlendEquation",  "(I)V",  org_mini_gl_GL_glBlendEquation},
        {"org/mini/gl/GL",  "glGenQueries",  "(I[II)V",  org_mini_gl_GL_glGenQueries},
        {"org/mini/gl/GL",  "glDeleteQueries",  "(I[II)V",  org_mini_gl_GL_glDeleteQueries},
        {"org/mini/gl/GL",  "glIsQuery",  "(I)I",  org_mini_gl_GL_glIsQuery},
        {"org/mini/gl/GL",  "glBeginQuery",  "(II)V",  org_mini_gl_GL_glBeginQuery},
        {"org/mini/gl/GL",  "glEndQuery",  "(I)V",  org_mini_gl_GL_glEndQuery},
        {"org/mini/gl/GL",  "glGetQueryiv",  "(II[II)V",  org_mini_gl_GL_glGetQueryiv},
        {"org/mini/gl/GL",  "glGetQueryObjectiv",  "(II[II)V",  org_mini_gl_GL_glGetQueryObjectiv},
        {"org/mini/gl/GL",  "glGetQueryObjectuiv",  "(II[II)V",  org_mini_gl_GL_glGetQueryObjectuiv},
        {"org/mini/gl/GL",  "glBindBuffer",  "(II)V",  org_mini_gl_GL_glBindBuffer},
        {"org/mini/gl/GL",  "glDeleteBuffers",  "(I[II)V",  org_mini_gl_GL_glDeleteBuffers},
        {"org/mini/gl/GL",  "glGenBuffers",  "(I[II)V",  org_mini_gl_GL_glGenBuffers},
        {"org/mini/gl/GL",  "glIsBuffer",  "(I)I",  org_mini_gl_GL_glIsBuffer},
        {"org/mini/gl/GL",  "glBufferData",  "(IJLjava/lang/Object;II)V",  org_mini_gl_GL_glBufferData},
        {"org/mini/gl/GL",  "glBufferSubData",  "(IJJLjava/lang/Object;I)V",  org_mini_gl_GL_glBufferSubData},
        {"org/mini/gl/GL",  "glGetBufferSubData",  "(IJJLjava/lang/Object;I)V",  org_mini_gl_GL_glGetBufferSubData},
        {"org/mini/gl/GL",  "glMapBuffer",  "(II)Ljava/lang/Object;",  org_mini_gl_GL_glMapBuffer},
        {"org/mini/gl/GL",  "glUnmapBuffer",  "(I)I",  org_mini_gl_GL_glUnmapBuffer},
        {"org/mini/gl/GL",  "glGetBufferParameteriv",  "(II[II)V",  org_mini_gl_GL_glGetBufferParameteriv},
        {"org/mini/gl/GL",  "glGetBufferPointerv",  "(IILjava/lang/Object;I)V",  org_mini_gl_GL_glGetBufferPointerv},
        {"org/mini/gl/GL",  "glBlendEquationSeparate",  "(II)V",  org_mini_gl_GL_glBlendEquationSeparate},
        {"org/mini/gl/GL",  "glDrawBuffers",  "(I[II)V",  org_mini_gl_GL_glDrawBuffers},
        {"org/mini/gl/GL",  "glStencilOpSeparate",  "(IIII)V",  org_mini_gl_GL_glStencilOpSeparate},
        {"org/mini/gl/GL",  "glStencilFuncSeparate",  "(IIII)V",  org_mini_gl_GL_glStencilFuncSeparate},
        {"org/mini/gl/GL",  "glStencilMaskSeparate",  "(II)V",  org_mini_gl_GL_glStencilMaskSeparate},
        {"org/mini/gl/GL",  "glAttachShader",  "(II)V",  org_mini_gl_GL_glAttachShader},
        {"org/mini/gl/GL",  "glBindAttribLocation",  "(II[BI)V",  org_mini_gl_GL_glBindAttribLocation},
        {"org/mini/gl/GL",  "glCompileShader",  "(I)V",  org_mini_gl_GL_glCompileShader},
        {"org/mini/gl/GL",  "glCreateProgram",  "()I",  org_mini_gl_GL_glCreateProgram},
        {"org/mini/gl/GL",  "glCreateShader",  "(I)I",  org_mini_gl_GL_glCreateShader},
        {"org/mini/gl/GL",  "glDeleteProgram",  "(I)V",  org_mini_gl_GL_glDeleteProgram},
        {"org/mini/gl/GL",  "glDeleteShader",  "(I)V",  org_mini_gl_GL_glDeleteShader},
        {"org/mini/gl/GL",  "glDetachShader",  "(II)V",  org_mini_gl_GL_glDetachShader},
        {"org/mini/gl/GL",  "glDisableVertexAttribArray",  "(I)V",  org_mini_gl_GL_glDisableVertexAttribArray},
        {"org/mini/gl/GL",  "glEnableVertexAttribArray",  "(I)V",  org_mini_gl_GL_glEnableVertexAttribArray},
        {"org/mini/gl/GL",  "glGetActiveAttrib",  "(III[II[II[II[BI)V",  org_mini_gl_GL_glGetActiveAttrib},
        {"org/mini/gl/GL",  "glGetActiveUniform",  "(III[II[II[II[BI)V",  org_mini_gl_GL_glGetActiveUniform},
        {"org/mini/gl/GL",  "glGetAttachedShaders",  "(II[II[II)V",  org_mini_gl_GL_glGetAttachedShaders},
        {"org/mini/gl/GL",  "glGetAttribLocation",  "(I[BI)I",  org_mini_gl_GL_glGetAttribLocation},
        {"org/mini/gl/GL",  "glGetProgramiv",  "(II[II)V",  org_mini_gl_GL_glGetProgramiv},
        {"org/mini/gl/GL",  "glGetProgramInfoLog",  "(II[II[BI)V",  org_mini_gl_GL_glGetProgramInfoLog},
        {"org/mini/gl/GL",  "glGetShaderiv",  "(II[II)V",  org_mini_gl_GL_glGetShaderiv},
        {"org/mini/gl/GL",  "glGetShaderInfoLog",  "(II[II[BI)V",  org_mini_gl_GL_glGetShaderInfoLog},
        {"org/mini/gl/GL",  "glGetShaderSource",  "(II[II[BI)V",  org_mini_gl_GL_glGetShaderSource},
        {"org/mini/gl/GL",  "glGetUniformLocation",  "(I[BI)I",  org_mini_gl_GL_glGetUniformLocation},
        {"org/mini/gl/GL",  "glGetUniformfv",  "(II[FI)V",  org_mini_gl_GL_glGetUniformfv},
        {"org/mini/gl/GL",  "glGetUniformiv",  "(II[II)V",  org_mini_gl_GL_glGetUniformiv},
        {"org/mini/gl/GL",  "glGetVertexAttribdv",  "(II[DI)V",  org_mini_gl_GL_glGetVertexAttribdv},
        {"org/mini/gl/GL",  "glGetVertexAttribfv",  "(II[FI)V",  org_mini_gl_GL_glGetVertexAttribfv},
        {"org/mini/gl/GL",  "glGetVertexAttribiv",  "(II[II)V",  org_mini_gl_GL_glGetVertexAttribiv},
        {"org/mini/gl/GL",  "glGetVertexAttribPointerv",  "(IILjava/lang/Object;I)V",  org_mini_gl_GL_glGetVertexAttribPointerv},
        {"org/mini/gl/GL",  "glIsProgram",  "(I)I",  org_mini_gl_GL_glIsProgram},
        {"org/mini/gl/GL",  "glIsShader",  "(I)I",  org_mini_gl_GL_glIsShader},
        {"org/mini/gl/GL",  "glLinkProgram",  "(I)V",  org_mini_gl_GL_glLinkProgram},
        {"org/mini/gl/GL",  "glShaderSource",  "(II[BI[II)V",  org_mini_gl_GL_glShaderSource},
        {"org/mini/gl/GL",  "glUseProgram",  "(I)V",  org_mini_gl_GL_glUseProgram},
        {"org/mini/gl/GL",  "glUniform1f",  "(IF)V",  org_mini_gl_GL_glUniform1f},
        {"org/mini/gl/GL",  "glUniform2f",  "(IFF)V",  org_mini_gl_GL_glUniform2f},
        {"org/mini/gl/GL",  "glUniform3f",  "(IFFF)V",  org_mini_gl_GL_glUniform3f},
        {"org/mini/gl/GL",  "glUniform4f",  "(IFFFF)V",  org_mini_gl_GL_glUniform4f},
        {"org/mini/gl/GL",  "glUniform1i",  "(II)V",  org_mini_gl_GL_glUniform1i},
        {"org/mini/gl/GL",  "glUniform2i",  "(III)V",  org_mini_gl_GL_glUniform2i},
        {"org/mini/gl/GL",  "glUniform3i",  "(IIII)V",  org_mini_gl_GL_glUniform3i},
        {"org/mini/gl/GL",  "glUniform4i",  "(IIIII)V",  org_mini_gl_GL_glUniform4i},
        {"org/mini/gl/GL",  "glUniform1fv",  "(II[FI)V",  org_mini_gl_GL_glUniform1fv},
        {"org/mini/gl/GL",  "glUniform2fv",  "(II[FI)V",  org_mini_gl_GL_glUniform2fv},
        {"org/mini/gl/GL",  "glUniform3fv",  "(II[FI)V",  org_mini_gl_GL_glUniform3fv},
        {"org/mini/gl/GL",  "glUniform4fv",  "(II[FI)V",  org_mini_gl_GL_glUniform4fv},
        {"org/mini/gl/GL",  "glUniform1iv",  "(II[II)V",  org_mini_gl_GL_glUniform1iv},
        {"org/mini/gl/GL",  "glUniform2iv",  "(II[II)V",  org_mini_gl_GL_glUniform2iv},
        {"org/mini/gl/GL",  "glUniform3iv",  "(II[II)V",  org_mini_gl_GL_glUniform3iv},
        {"org/mini/gl/GL",  "glUniform4iv",  "(II[II)V",  org_mini_gl_GL_glUniform4iv},
        {"org/mini/gl/GL",  "glUniformMatrix2fv",  "(III[FI)V",  org_mini_gl_GL_glUniformMatrix2fv},
        {"org/mini/gl/GL",  "glUniformMatrix3fv",  "(III[FI)V",  org_mini_gl_GL_glUniformMatrix3fv},
        {"org/mini/gl/GL",  "glUniformMatrix4fv",  "(III[FI)V",  org_mini_gl_GL_glUniformMatrix4fv},
        {"org/mini/gl/GL",  "glValidateProgram",  "(I)V",  org_mini_gl_GL_glValidateProgram},
        {"org/mini/gl/GL",  "glVertexAttrib1d",  "(ID)V",  org_mini_gl_GL_glVertexAttrib1d},
        {"org/mini/gl/GL",  "glVertexAttrib1dv",  "(I[DI)V",  org_mini_gl_GL_glVertexAttrib1dv},
        {"org/mini/gl/GL",  "glVertexAttrib1f",  "(IF)V",  org_mini_gl_GL_glVertexAttrib1f},
        {"org/mini/gl/GL",  "glVertexAttrib1fv",  "(I[FI)V",  org_mini_gl_GL_glVertexAttrib1fv},
        {"org/mini/gl/GL",  "glVertexAttrib1s",  "(IS)V",  org_mini_gl_GL_glVertexAttrib1s},
        {"org/mini/gl/GL",  "glVertexAttrib1sv",  "(I[SI)V",  org_mini_gl_GL_glVertexAttrib1sv},
        {"org/mini/gl/GL",  "glVertexAttrib2d",  "(IDD)V",  org_mini_gl_GL_glVertexAttrib2d},
        {"org/mini/gl/GL",  "glVertexAttrib2dv",  "(I[DI)V",  org_mini_gl_GL_glVertexAttrib2dv},
        {"org/mini/gl/GL",  "glVertexAttrib2f",  "(IFF)V",  org_mini_gl_GL_glVertexAttrib2f},
        {"org/mini/gl/GL",  "glVertexAttrib2fv",  "(I[FI)V",  org_mini_gl_GL_glVertexAttrib2fv},
        {"org/mini/gl/GL",  "glVertexAttrib2s",  "(ISS)V",  org_mini_gl_GL_glVertexAttrib2s},
        {"org/mini/gl/GL",  "glVertexAttrib2sv",  "(I[SI)V",  org_mini_gl_GL_glVertexAttrib2sv},
        {"org/mini/gl/GL",  "glVertexAttrib3d",  "(IDDD)V",  org_mini_gl_GL_glVertexAttrib3d},
        {"org/mini/gl/GL",  "glVertexAttrib3dv",  "(I[DI)V",  org_mini_gl_GL_glVertexAttrib3dv},
        {"org/mini/gl/GL",  "glVertexAttrib3f",  "(IFFF)V",  org_mini_gl_GL_glVertexAttrib3f},
        {"org/mini/gl/GL",  "glVertexAttrib3fv",  "(I[FI)V",  org_mini_gl_GL_glVertexAttrib3fv},
        {"org/mini/gl/GL",  "glVertexAttrib3s",  "(ISSS)V",  org_mini_gl_GL_glVertexAttrib3s},
        {"org/mini/gl/GL",  "glVertexAttrib3sv",  "(I[SI)V",  org_mini_gl_GL_glVertexAttrib3sv},
        {"org/mini/gl/GL",  "glVertexAttrib4Nbv",  "(I[BI)V",  org_mini_gl_GL_glVertexAttrib4Nbv},
        {"org/mini/gl/GL",  "glVertexAttrib4Niv",  "(I[II)V",  org_mini_gl_GL_glVertexAttrib4Niv},
        {"org/mini/gl/GL",  "glVertexAttrib4Nsv",  "(I[SI)V",  org_mini_gl_GL_glVertexAttrib4Nsv},
        {"org/mini/gl/GL",  "glVertexAttrib4Nub",  "(IBBBB)V",  org_mini_gl_GL_glVertexAttrib4Nub},
        {"org/mini/gl/GL",  "glVertexAttrib4Nubv",  "(I[BI)V",  org_mini_gl_GL_glVertexAttrib4Nubv},
        {"org/mini/gl/GL",  "glVertexAttrib4Nuiv",  "(I[II)V",  org_mini_gl_GL_glVertexAttrib4Nuiv},
        {"org/mini/gl/GL",  "glVertexAttrib4Nusv",  "(I[SI)V",  org_mini_gl_GL_glVertexAttrib4Nusv},
        {"org/mini/gl/GL",  "glVertexAttrib4bv",  "(I[BI)V",  org_mini_gl_GL_glVertexAttrib4bv},
        {"org/mini/gl/GL",  "glVertexAttrib4d",  "(IDDDD)V",  org_mini_gl_GL_glVertexAttrib4d},
        {"org/mini/gl/GL",  "glVertexAttrib4dv",  "(I[DI)V",  org_mini_gl_GL_glVertexAttrib4dv},
        {"org/mini/gl/GL",  "glVertexAttrib4f",  "(IFFFF)V",  org_mini_gl_GL_glVertexAttrib4f},
        {"org/mini/gl/GL",  "glVertexAttrib4fv",  "(I[FI)V",  org_mini_gl_GL_glVertexAttrib4fv},
        {"org/mini/gl/GL",  "glVertexAttrib4iv",  "(I[II)V",  org_mini_gl_GL_glVertexAttrib4iv},
        {"org/mini/gl/GL",  "glVertexAttrib4s",  "(ISSSS)V",  org_mini_gl_GL_glVertexAttrib4s},
        {"org/mini/gl/GL",  "glVertexAttrib4sv",  "(I[SI)V",  org_mini_gl_GL_glVertexAttrib4sv},
        {"org/mini/gl/GL",  "glVertexAttrib4ubv",  "(I[BI)V",  org_mini_gl_GL_glVertexAttrib4ubv},
        {"org/mini/gl/GL",  "glVertexAttrib4uiv",  "(I[II)V",  org_mini_gl_GL_glVertexAttrib4uiv},
        {"org/mini/gl/GL",  "glVertexAttrib4usv",  "(I[SI)V",  org_mini_gl_GL_glVertexAttrib4usv},
        {"org/mini/gl/GL",  "glVertexAttribPointer",  "(IIIIILjava/lang/Object;I)V",  org_mini_gl_GL_glVertexAttribPointer},
        {"org/mini/gl/GL",  "glUniformMatrix2x3fv",  "(III[FI)V",  org_mini_gl_GL_glUniformMatrix2x3fv},
        {"org/mini/gl/GL",  "glUniformMatrix3x2fv",  "(III[FI)V",  org_mini_gl_GL_glUniformMatrix3x2fv},
        {"org/mini/gl/GL",  "glUniformMatrix2x4fv",  "(III[FI)V",  org_mini_gl_GL_glUniformMatrix2x4fv},
        {"org/mini/gl/GL",  "glUniformMatrix4x2fv",  "(III[FI)V",  org_mini_gl_GL_glUniformMatrix4x2fv},
        {"org/mini/gl/GL",  "glUniformMatrix3x4fv",  "(III[FI)V",  org_mini_gl_GL_glUniformMatrix3x4fv},
        {"org/mini/gl/GL",  "glUniformMatrix4x3fv",  "(III[FI)V",  org_mini_gl_GL_glUniformMatrix4x3fv},
        {"org/mini/gl/GL",  "glColorMaski",  "(IIIII)V",  org_mini_gl_GL_glColorMaski},
        {"org/mini/gl/GL",  "glGetBooleani_v",  "(II[II)V",  org_mini_gl_GL_glGetBooleani_v},
        {"org/mini/gl/GL",  "glGetIntegeri_v",  "(II[II)V",  org_mini_gl_GL_glGetIntegeri_v},
        {"org/mini/gl/GL",  "glEnablei",  "(II)V",  org_mini_gl_GL_glEnablei},
        {"org/mini/gl/GL",  "glDisablei",  "(II)V",  org_mini_gl_GL_glDisablei},
        {"org/mini/gl/GL",  "glIsEnabledi",  "(II)I",  org_mini_gl_GL_glIsEnabledi},
        {"org/mini/gl/GL",  "glBeginTransformFeedback",  "(I)V",  org_mini_gl_GL_glBeginTransformFeedback},
        {"org/mini/gl/GL",  "glEndTransformFeedback",  "()V",  org_mini_gl_GL_glEndTransformFeedback},
        {"org/mini/gl/GL",  "glBindBufferRange",  "(IIIJJ)V",  org_mini_gl_GL_glBindBufferRange},
        {"org/mini/gl/GL",  "glBindBufferBase",  "(III)V",  org_mini_gl_GL_glBindBufferBase},
        {"org/mini/gl/GL",  "glTransformFeedbackVaryings",  "(II[BII)V",  org_mini_gl_GL_glTransformFeedbackVaryings},
        {"org/mini/gl/GL",  "glGetTransformFeedbackVarying",  "(III[II[II[II[BI)V",  org_mini_gl_GL_glGetTransformFeedbackVarying},
        {"org/mini/gl/GL",  "glClampColor",  "(II)V",  org_mini_gl_GL_glClampColor},
        {"org/mini/gl/GL",  "glBeginConditionalRender",  "(II)V",  org_mini_gl_GL_glBeginConditionalRender},
        {"org/mini/gl/GL",  "glEndConditionalRender",  "()V",  org_mini_gl_GL_glEndConditionalRender},
        {"org/mini/gl/GL",  "glVertexAttribIPointer",  "(IIIILjava/lang/Object;I)V",  org_mini_gl_GL_glVertexAttribIPointer},
        {"org/mini/gl/GL",  "glGetVertexAttribIiv",  "(II[II)V",  org_mini_gl_GL_glGetVertexAttribIiv},
        {"org/mini/gl/GL",  "glGetVertexAttribIuiv",  "(II[II)V",  org_mini_gl_GL_glGetVertexAttribIuiv},
        {"org/mini/gl/GL",  "glVertexAttribI1i",  "(II)V",  org_mini_gl_GL_glVertexAttribI1i},
        {"org/mini/gl/GL",  "glVertexAttribI2i",  "(III)V",  org_mini_gl_GL_glVertexAttribI2i},
        {"org/mini/gl/GL",  "glVertexAttribI3i",  "(IIII)V",  org_mini_gl_GL_glVertexAttribI3i},
        {"org/mini/gl/GL",  "glVertexAttribI4i",  "(IIIII)V",  org_mini_gl_GL_glVertexAttribI4i},
        {"org/mini/gl/GL",  "glVertexAttribI1ui",  "(II)V",  org_mini_gl_GL_glVertexAttribI1ui},
        {"org/mini/gl/GL",  "glVertexAttribI2ui",  "(III)V",  org_mini_gl_GL_glVertexAttribI2ui},
        {"org/mini/gl/GL",  "glVertexAttribI3ui",  "(IIII)V",  org_mini_gl_GL_glVertexAttribI3ui},
        {"org/mini/gl/GL",  "glVertexAttribI4ui",  "(IIIII)V",  org_mini_gl_GL_glVertexAttribI4ui},
        {"org/mini/gl/GL",  "glVertexAttribI1iv",  "(I[II)V",  org_mini_gl_GL_glVertexAttribI1iv},
        {"org/mini/gl/GL",  "glVertexAttribI2iv",  "(I[II)V",  org_mini_gl_GL_glVertexAttribI2iv},
        {"org/mini/gl/GL",  "glVertexAttribI3iv",  "(I[II)V",  org_mini_gl_GL_glVertexAttribI3iv},
        {"org/mini/gl/GL",  "glVertexAttribI4iv",  "(I[II)V",  org_mini_gl_GL_glVertexAttribI4iv},
        {"org/mini/gl/GL",  "glVertexAttribI1uiv",  "(I[II)V",  org_mini_gl_GL_glVertexAttribI1uiv},
        {"org/mini/gl/GL",  "glVertexAttribI2uiv",  "(I[II)V",  org_mini_gl_GL_glVertexAttribI2uiv},
        {"org/mini/gl/GL",  "glVertexAttribI3uiv",  "(I[II)V",  org_mini_gl_GL_glVertexAttribI3uiv},
        {"org/mini/gl/GL",  "glVertexAttribI4uiv",  "(I[II)V",  org_mini_gl_GL_glVertexAttribI4uiv},
        {"org/mini/gl/GL",  "glVertexAttribI4bv",  "(I[BI)V",  org_mini_gl_GL_glVertexAttribI4bv},
        {"org/mini/gl/GL",  "glVertexAttribI4sv",  "(I[SI)V",  org_mini_gl_GL_glVertexAttribI4sv},
        {"org/mini/gl/GL",  "glVertexAttribI4ubv",  "(I[BI)V",  org_mini_gl_GL_glVertexAttribI4ubv},
        {"org/mini/gl/GL",  "glVertexAttribI4usv",  "(I[SI)V",  org_mini_gl_GL_glVertexAttribI4usv},
        {"org/mini/gl/GL",  "glGetUniformuiv",  "(II[II)V",  org_mini_gl_GL_glGetUniformuiv},
        {"org/mini/gl/GL",  "glBindFragDataLocation",  "(II[BI)V",  org_mini_gl_GL_glBindFragDataLocation},
        {"org/mini/gl/GL",  "glGetFragDataLocation",  "(I[BI)I",  org_mini_gl_GL_glGetFragDataLocation},
        {"org/mini/gl/GL",  "glUniform1ui",  "(II)V",  org_mini_gl_GL_glUniform1ui},
        {"org/mini/gl/GL",  "glUniform2ui",  "(III)V",  org_mini_gl_GL_glUniform2ui},
        {"org/mini/gl/GL",  "glUniform3ui",  "(IIII)V",  org_mini_gl_GL_glUniform3ui},
        {"org/mini/gl/GL",  "glUniform4ui",  "(IIIII)V",  org_mini_gl_GL_glUniform4ui},
        {"org/mini/gl/GL",  "glUniform1uiv",  "(II[II)V",  org_mini_gl_GL_glUniform1uiv},
        {"org/mini/gl/GL",  "glUniform2uiv",  "(II[II)V",  org_mini_gl_GL_glUniform2uiv},
        {"org/mini/gl/GL",  "glUniform3uiv",  "(II[II)V",  org_mini_gl_GL_glUniform3uiv},
        {"org/mini/gl/GL",  "glUniform4uiv",  "(II[II)V",  org_mini_gl_GL_glUniform4uiv},
        {"org/mini/gl/GL",  "glTexParameterIiv",  "(II[II)V",  org_mini_gl_GL_glTexParameterIiv},
        {"org/mini/gl/GL",  "glTexParameterIuiv",  "(II[II)V",  org_mini_gl_GL_glTexParameterIuiv},
        {"org/mini/gl/GL",  "glGetTexParameterIiv",  "(II[II)V",  org_mini_gl_GL_glGetTexParameterIiv},
        {"org/mini/gl/GL",  "glGetTexParameterIuiv",  "(II[II)V",  org_mini_gl_GL_glGetTexParameterIuiv},
        {"org/mini/gl/GL",  "glClearBufferiv",  "(II[II)V",  org_mini_gl_GL_glClearBufferiv},
        {"org/mini/gl/GL",  "glClearBufferuiv",  "(II[II)V",  org_mini_gl_GL_glClearBufferuiv},
        {"org/mini/gl/GL",  "glClearBufferfv",  "(II[FI)V",  org_mini_gl_GL_glClearBufferfv},
        {"org/mini/gl/GL",  "glClearBufferfi",  "(IIFI)V",  org_mini_gl_GL_glClearBufferfi},
        {"org/mini/gl/GL",  "glGetStringi",  "(II)[B",  org_mini_gl_GL_glGetStringi},
        {"org/mini/gl/GL",  "glIsRenderbuffer",  "(I)I",  org_mini_gl_GL_glIsRenderbuffer},
        {"org/mini/gl/GL",  "glBindRenderbuffer",  "(II)V",  org_mini_gl_GL_glBindRenderbuffer},
        {"org/mini/gl/GL",  "glDeleteRenderbuffers",  "(I[II)V",  org_mini_gl_GL_glDeleteRenderbuffers},
        {"org/mini/gl/GL",  "glGenRenderbuffers",  "(I[II)V",  org_mini_gl_GL_glGenRenderbuffers},
        {"org/mini/gl/GL",  "glRenderbufferStorage",  "(IIII)V",  org_mini_gl_GL_glRenderbufferStorage},
        {"org/mini/gl/GL",  "glGetRenderbufferParameteriv",  "(II[II)V",  org_mini_gl_GL_glGetRenderbufferParameteriv},
        {"org/mini/gl/GL",  "glIsFramebuffer",  "(I)I",  org_mini_gl_GL_glIsFramebuffer},
        {"org/mini/gl/GL",  "glBindFramebuffer",  "(II)V",  org_mini_gl_GL_glBindFramebuffer},
        {"org/mini/gl/GL",  "glDeleteFramebuffers",  "(I[II)V",  org_mini_gl_GL_glDeleteFramebuffers},
        {"org/mini/gl/GL",  "glGenFramebuffers",  "(I[II)V",  org_mini_gl_GL_glGenFramebuffers},
        {"org/mini/gl/GL",  "glCheckFramebufferStatus",  "(I)I",  org_mini_gl_GL_glCheckFramebufferStatus},
        {"org/mini/gl/GL",  "glFramebufferTexture1D",  "(IIIII)V",  org_mini_gl_GL_glFramebufferTexture1D},
        {"org/mini/gl/GL",  "glFramebufferTexture2D",  "(IIIII)V",  org_mini_gl_GL_glFramebufferTexture2D},
        {"org/mini/gl/GL",  "glFramebufferTexture3D",  "(IIIIII)V",  org_mini_gl_GL_glFramebufferTexture3D},
        {"org/mini/gl/GL",  "glFramebufferRenderbuffer",  "(IIII)V",  org_mini_gl_GL_glFramebufferRenderbuffer},
        {"org/mini/gl/GL",  "glGetFramebufferAttachmentParameteriv",  "(III[II)V",  org_mini_gl_GL_glGetFramebufferAttachmentParameteriv},
        {"org/mini/gl/GL",  "glGenerateMipmap",  "(I)V",  org_mini_gl_GL_glGenerateMipmap},
        {"org/mini/gl/GL",  "glBlitFramebuffer",  "(IIIIIIIIII)V",  org_mini_gl_GL_glBlitFramebuffer},
        {"org/mini/gl/GL",  "glRenderbufferStorageMultisample",  "(IIIII)V",  org_mini_gl_GL_glRenderbufferStorageMultisample},
        {"org/mini/gl/GL",  "glFramebufferTextureLayer",  "(IIIII)V",  org_mini_gl_GL_glFramebufferTextureLayer},
        {"org/mini/gl/GL",  "glMapBufferRange",  "(IJJI)Ljava/lang/Object;",  org_mini_gl_GL_glMapBufferRange},
        {"org/mini/gl/GL",  "glFlushMappedBufferRange",  "(IJJ)V",  org_mini_gl_GL_glFlushMappedBufferRange},
        {"org/mini/gl/GL",  "glBindVertexArray",  "(I)V",  org_mini_gl_GL_glBindVertexArray},
        {"org/mini/gl/GL",  "glDeleteVertexArrays",  "(I[II)V",  org_mini_gl_GL_glDeleteVertexArrays},
        {"org/mini/gl/GL",  "glGenVertexArrays",  "(I[II)V",  org_mini_gl_GL_glGenVertexArrays},
        {"org/mini/gl/GL",  "glIsVertexArray",  "(I)I",  org_mini_gl_GL_glIsVertexArray},
        {"org/mini/gl/GL",  "glDrawArraysInstanced",  "(IIII)V",  org_mini_gl_GL_glDrawArraysInstanced},
        {"org/mini/gl/GL",  "glDrawElementsInstanced",  "(IIILjava/lang/Object;II)V",  org_mini_gl_GL_glDrawElementsInstanced},
        {"org/mini/gl/GL",  "glTexBuffer",  "(III)V",  org_mini_gl_GL_glTexBuffer},
        {"org/mini/gl/GL",  "glPrimitiveRestartIndex",  "(I)V",  org_mini_gl_GL_glPrimitiveRestartIndex},
        {"org/mini/gl/GL",  "glCopyBufferSubData",  "(IIJJJ)V",  org_mini_gl_GL_glCopyBufferSubData},
        {"org/mini/gl/GL",  "glGetUniformIndices",  "(II[BI[II)V",  org_mini_gl_GL_glGetUniformIndices},
        {"org/mini/gl/GL",  "glGetActiveUniformsiv",  "(II[III[II)V",  org_mini_gl_GL_glGetActiveUniformsiv},
        {"org/mini/gl/GL",  "glGetActiveUniformName",  "(III[II[BI)V",  org_mini_gl_GL_glGetActiveUniformName},
        {"org/mini/gl/GL",  "glGetUniformBlockIndex",  "(I[BI)I",  org_mini_gl_GL_glGetUniformBlockIndex},
        {"org/mini/gl/GL",  "glGetActiveUniformBlockiv",  "(III[II)V",  org_mini_gl_GL_glGetActiveUniformBlockiv},
        {"org/mini/gl/GL",  "glGetActiveUniformBlockName",  "(III[II[BI)V",  org_mini_gl_GL_glGetActiveUniformBlockName},
        {"org/mini/gl/GL",  "glUniformBlockBinding",  "(III)V",  org_mini_gl_GL_glUniformBlockBinding},
        {"org/mini/gl/GL",  "glDrawElementsBaseVertex",  "(IIILjava/lang/Object;II)V",  org_mini_gl_GL_glDrawElementsBaseVertex},
        {"org/mini/gl/GL",  "glDrawRangeElementsBaseVertex",  "(IIIIILjava/lang/Object;II)V",  org_mini_gl_GL_glDrawRangeElementsBaseVertex},
        {"org/mini/gl/GL",  "glDrawElementsInstancedBaseVertex",  "(IIILjava/lang/Object;III)V",  org_mini_gl_GL_glDrawElementsInstancedBaseVertex},
        {"org/mini/gl/GL",  "glMultiDrawElementsBaseVertex",  "(I[IIILjava/lang/Object;II[II)V",  org_mini_gl_GL_glMultiDrawElementsBaseVertex},
        {"org/mini/gl/GL",  "glProvokingVertex",  "(I)V",  org_mini_gl_GL_glProvokingVertex},
        {"org/mini/gl/GL",  "glFenceSync",  "(II)J",  org_mini_gl_GL_glFenceSync},
        {"org/mini/gl/GL",  "glIsSync",  "(J)I",  org_mini_gl_GL_glIsSync},
        {"org/mini/gl/GL",  "glDeleteSync",  "(J)V",  org_mini_gl_GL_glDeleteSync},
        {"org/mini/gl/GL",  "glClientWaitSync",  "(JIJ)I",  org_mini_gl_GL_glClientWaitSync},
        {"org/mini/gl/GL",  "glWaitSync",  "(JIJ)V",  org_mini_gl_GL_glWaitSync},
        {"org/mini/gl/GL",  "glGetInteger64v",  "(I[JI)V",  org_mini_gl_GL_glGetInteger64v},
        {"org/mini/gl/GL",  "glGetSynciv",  "(JII[II[II)V",  org_mini_gl_GL_glGetSynciv},
        {"org/mini/gl/GL",  "glGetInteger64i_v",  "(II[JI)V",  org_mini_gl_GL_glGetInteger64i_v},
        {"org/mini/gl/GL",  "glGetBufferParameteri64v",  "(II[JI)V",  org_mini_gl_GL_glGetBufferParameteri64v},
        {"org/mini/gl/GL",  "glFramebufferTexture",  "(IIII)V",  org_mini_gl_GL_glFramebufferTexture},
        {"org/mini/gl/GL",  "glTexImage2DMultisample",  "(IIIIII)V",  org_mini_gl_GL_glTexImage2DMultisample},
        {"org/mini/gl/GL",  "glTexImage3DMultisample",  "(IIIIIII)V",  org_mini_gl_GL_glTexImage3DMultisample},
        {"org/mini/gl/GL",  "glGetMultisamplefv",  "(II[FI)V",  org_mini_gl_GL_glGetMultisamplefv},
        {"org/mini/gl/GL",  "glSampleMaski",  "(II)V",  org_mini_gl_GL_glSampleMaski},
        {"org/mini/gl/GL",  "glSampleCoverageARB",  "(FI)V",  org_mini_gl_GL_glSampleCoverageARB},
        {"org/mini/gl/GL",  "glGetGraphicsResetStatusARB",  "()I",  org_mini_gl_GL_glGetGraphicsResetStatusARB},
        {"org/mini/gl/GL",  "glGetnTexImageARB",  "(IIIIILjava/lang/Object;I)V",  org_mini_gl_GL_glGetnTexImageARB},
        {"org/mini/gl/GL",  "glReadnPixelsARB",  "(IIIIIIILjava/lang/Object;I)V",  org_mini_gl_GL_glReadnPixelsARB},
        {"org/mini/gl/GL",  "glGetnCompressedTexImageARB",  "(IIILjava/lang/Object;I)V",  org_mini_gl_GL_glGetnCompressedTexImageARB},
        {"org/mini/gl/GL",  "glGetnUniformfvARB",  "(III[FI)V",  org_mini_gl_GL_glGetnUniformfvARB},
        {"org/mini/gl/GL",  "glGetnUniformivARB",  "(III[II)V",  org_mini_gl_GL_glGetnUniformivARB},
        {"org/mini/gl/GL",  "glGetnUniformuivARB",  "(III[II)V",  org_mini_gl_GL_glGetnUniformuivARB},
        {"org/mini/gl/GL",  "glGetnUniformdvARB",  "(III[DI)V",  org_mini_gl_GL_glGetnUniformdvARB},
        {"org/mini/gl/GL",  "glGetnMapdvARB",  "(III[DI)V",  org_mini_gl_GL_glGetnMapdvARB},
        {"org/mini/gl/GL",  "glGetnMapfvARB",  "(III[FI)V",  org_mini_gl_GL_glGetnMapfvARB},
        {"org/mini/gl/GL",  "glGetnMapivARB",  "(III[II)V",  org_mini_gl_GL_glGetnMapivARB},
        {"org/mini/gl/GL",  "glGetnPixelMapfvARB",  "(II[FI)V",  org_mini_gl_GL_glGetnPixelMapfvARB},
        {"org/mini/gl/GL",  "glGetnPixelMapuivARB",  "(II[II)V",  org_mini_gl_GL_glGetnPixelMapuivARB},
        {"org/mini/gl/GL",  "glGetnPixelMapusvARB",  "(II[SI)V",  org_mini_gl_GL_glGetnPixelMapusvARB},
        {"org/mini/gl/GL",  "glGetnPolygonStippleARB",  "(I[BI)V",  org_mini_gl_GL_glGetnPolygonStippleARB},
        {"org/mini/gl/GL",  "glGetnColorTableARB",  "(IIIILjava/lang/Object;I)V",  org_mini_gl_GL_glGetnColorTableARB},
        {"org/mini/gl/GL",  "glGetnConvolutionFilterARB",  "(IIIILjava/lang/Object;I)V",  org_mini_gl_GL_glGetnConvolutionFilterARB},
        {"org/mini/gl/GL",  "glGetnSeparableFilterARB",  "(IIIILjava/lang/Object;IILjava/lang/Object;ILjava/lang/Object;I)V",  org_mini_gl_GL_glGetnSeparableFilterARB},
        {"org/mini/gl/GL",  "glGetnHistogramARB",  "(IIIIILjava/lang/Object;I)V",  org_mini_gl_GL_glGetnHistogramARB},
        {"org/mini/gl/GL",  "glGetnMinmaxARB",  "(IIIIILjava/lang/Object;I)V",  org_mini_gl_GL_glGetnMinmaxARB},
        {"org/mini/gl/GL",  "glDebugMessageControl",  "(IIII[III)V",  org_mini_gl_GL_glDebugMessageControl},
        {"org/mini/gl/GL",  "glDebugMessageInsert",  "(IIIII[BI)V",  org_mini_gl_GL_glDebugMessageInsert},
        {"org/mini/gl/GL",  "glDebugMessageCallback",  "(JLjava/lang/Object;I)V",  org_mini_gl_GL_glDebugMessageCallback},
        {"org/mini/gl/GL",  "glGetDebugMessageLog",  "(II[II[II[II[II[II[BI)I",  org_mini_gl_GL_glGetDebugMessageLog},
        {"org/mini/gl/GL",  "glPushDebugGroup",  "(III[BI)V",  org_mini_gl_GL_glPushDebugGroup},
        {"org/mini/gl/GL",  "glPopDebugGroup",  "()V",  org_mini_gl_GL_glPopDebugGroup},
        {"org/mini/gl/GL",  "glObjectLabel",  "(III[BI)V",  org_mini_gl_GL_glObjectLabel},
        {"org/mini/gl/GL",  "glGetObjectLabel",  "(III[II[BI)V",  org_mini_gl_GL_glGetObjectLabel},
        {"org/mini/gl/GL",  "glObjectPtrLabel",  "(Ljava/lang/Object;II[BI)V",  org_mini_gl_GL_glObjectPtrLabel},
        {"org/mini/gl/GL",  "glGetObjectPtrLabel",  "(Ljava/lang/Object;II[II[BI)V",  org_mini_gl_GL_glGetObjectPtrLabel},
        {"org/mini/gl/GL",  "glDebugMessageControlKHR",  "(IIII[III)V",  org_mini_gl_GL_glDebugMessageControlKHR},
        {"org/mini/gl/GL",  "glDebugMessageInsertKHR",  "(IIIII[BI)V",  org_mini_gl_GL_glDebugMessageInsertKHR},
        {"org/mini/gl/GL",  "glDebugMessageCallbackKHR",  "(JLjava/lang/Object;I)V",  org_mini_gl_GL_glDebugMessageCallbackKHR},
        {"org/mini/gl/GL",  "glGetDebugMessageLogKHR",  "(II[II[II[II[II[II[BI)I",  org_mini_gl_GL_glGetDebugMessageLogKHR},
        {"org/mini/gl/GL",  "glPushDebugGroupKHR",  "(III[BI)V",  org_mini_gl_GL_glPushDebugGroupKHR},
        {"org/mini/gl/GL",  "glPopDebugGroupKHR",  "()V",  org_mini_gl_GL_glPopDebugGroupKHR},
        {"org/mini/gl/GL",  "glObjectLabelKHR",  "(III[BI)V",  org_mini_gl_GL_glObjectLabelKHR},
        {"org/mini/gl/GL",  "glGetObjectLabelKHR",  "(III[II[BI)V",  org_mini_gl_GL_glGetObjectLabelKHR},
        {"org/mini/gl/GL",  "glObjectPtrLabelKHR",  "(Ljava/lang/Object;II[BI)V",  org_mini_gl_GL_glObjectPtrLabelKHR},
        {"org/mini/gl/GL",  "glGetObjectPtrLabelKHR",  "(Ljava/lang/Object;II[II[BI)V",  org_mini_gl_GL_glGetObjectPtrLabelKHR},
        {"org/mini/gl/GL",  "glGetPointervKHR",  "(ILjava/lang/Object;I)V",  org_mini_gl_GL_glGetPointervKHR},
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
