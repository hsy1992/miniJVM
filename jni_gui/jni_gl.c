#include <stdio.h>
#include <string.h>
#include "deps/include/glad/glad.h"
#include "deps/include/GLFW/glfw3.h"
#include "deps/include/linmath.h"

#include "../mini_jvm/jvm/jvm.h"
#include "jni_gui.h"

int org_mini_gl_GL_init(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    

    init();
    
    
    return 0;
}

int org_mini_gl_GL_glCullFace(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);

    glCullFace((GLenum)pmode);
    
    
    return 0;
}

int org_mini_gl_GL_glFrontFace(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);

    glFrontFace((GLenum)pmode);
    
    
    return 0;
}

int org_mini_gl_GL_glHint(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pmode = env->localvar_getInt(runtime, pos++);

    glHint((GLenum)ptarget, (GLenum)pmode);
    
    
    return 0;
}

int org_mini_gl_GL_glLineWidth(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float pwidth;pwidth.i = env->localvar_getInt(runtime, pos++);

    glLineWidth((GLfloat)pwidth.f);
    
    
    return 0;
}

int org_mini_gl_GL_glPointSize(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float psize;psize.i = env->localvar_getInt(runtime, pos++);

    glPointSize((GLfloat)psize.f);
    
    
    return 0;
}

int org_mini_gl_GL_glPolygonMode(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pface = env->localvar_getInt(runtime, pos++);
    s32 pmode = env->localvar_getInt(runtime, pos++);

    glPolygonMode((GLenum)pface, (GLenum)pmode);
    
    
    return 0;
}

int org_mini_gl_GL_glScissor(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);

    glScissor((GLint)px, (GLint)py, (GLsizei)pwidth, (GLsizei)pheight);
    
    
    return 0;
}

int org_mini_gl_GL_glTexParameterf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Int2Float pparam;pparam.i = env->localvar_getInt(runtime, pos++);

    glTexParameterf((GLenum)ptarget, (GLenum)ppname, (GLfloat)pparam.f);
    
    
    return 0;
}

int org_mini_gl_GL_glTexParameterfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glTexParameterfv((GLenum)ptarget, (GLenum)ppname, (const GLfloat*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glTexParameteri(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    s32 pparam = env->localvar_getInt(runtime, pos++);

    glTexParameteri((GLenum)ptarget, (GLenum)ppname, (GLint)pparam);
    
    
    return 0;
}

int org_mini_gl_GL_glTexParameteriv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glTexParameteriv((GLenum)ptarget, (GLenum)ppname, (const GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glTexImage1D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pinternalformat = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pborder = env->localvar_getInt(runtime, pos++);
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    Instance *ppixels = env->localvar_getRefer(runtime, pos++);
    int offset_ppixels = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppixels = NULL;
    if(ppixels){
        ptr_ppixels = ppixels->arr_body + offset_ppixels;
    }

    glTexImage1D((GLenum)ptarget, (GLint)plevel, (GLint)pinternalformat, (GLsizei)pwidth, (GLint)pborder, (GLenum)pformat, (GLenum)ptype, (const void*)(ptr_ppixels));
    
    
    return 0;
}

int org_mini_gl_GL_glTexImage2D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pinternalformat = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);
    s32 pborder = env->localvar_getInt(runtime, pos++);
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    Instance *ppixels = env->localvar_getRefer(runtime, pos++);
    int offset_ppixels = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppixels = NULL;
    if(ppixels){
        ptr_ppixels = ppixels->arr_body + offset_ppixels;
    }

    glTexImage2D((GLenum)ptarget, (GLint)plevel, (GLint)pinternalformat, (GLsizei)pwidth, (GLsizei)pheight, (GLint)pborder, (GLenum)pformat, (GLenum)ptype, (const void*)(ptr_ppixels));
    
    
    return 0;
}

int org_mini_gl_GL_glDrawBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pbuf = env->localvar_getInt(runtime, pos++);

    glDrawBuffer((GLenum)pbuf);
    
    
    return 0;
}

int org_mini_gl_GL_glClear(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmask = env->localvar_getInt(runtime, pos++);

    glClear((GLbitfield)pmask);
    
    
    return 0;
}

int org_mini_gl_GL_glClearColor(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float pred;pred.i = env->localvar_getInt(runtime, pos++);
    Int2Float pgreen;pgreen.i = env->localvar_getInt(runtime, pos++);
    Int2Float pblue;pblue.i = env->localvar_getInt(runtime, pos++);
    Int2Float palpha;palpha.i = env->localvar_getInt(runtime, pos++);

    glClearColor((GLfloat)pred.f, (GLfloat)pgreen.f, (GLfloat)pblue.f, (GLfloat)palpha.f);
    
    
    return 0;
}

int org_mini_gl_GL_glClearStencil(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ps = env->localvar_getInt(runtime, pos++);

    glClearStencil((GLint)ps);
    
    
    return 0;
}

int org_mini_gl_GL_glClearDepth(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double pdepth;pdepth.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glClearDepth((GLdouble)pdepth.d);
    
    
    return 0;
}

int org_mini_gl_GL_glStencilMask(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmask = env->localvar_getInt(runtime, pos++);

    glStencilMask((GLuint)pmask);
    
    
    return 0;
}

int org_mini_gl_GL_glColorMask(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pred = env->localvar_getInt(runtime, pos++);
    s32 pgreen = env->localvar_getInt(runtime, pos++);
    s32 pblue = env->localvar_getInt(runtime, pos++);
    s32 palpha = env->localvar_getInt(runtime, pos++);

    glColorMask((GLboolean)pred, (GLboolean)pgreen, (GLboolean)pblue, (GLboolean)palpha);
    
    
    return 0;
}

int org_mini_gl_GL_glDepthMask(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pflag = env->localvar_getInt(runtime, pos++);

    glDepthMask((GLboolean)pflag);
    
    
    return 0;
}

int org_mini_gl_GL_glDisable(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pcap = env->localvar_getInt(runtime, pos++);

    glDisable((GLenum)pcap);
    
    
    return 0;
}

int org_mini_gl_GL_glEnable(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pcap = env->localvar_getInt(runtime, pos++);

    glEnable((GLenum)pcap);
    
    
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
    
    s32 psfactor = env->localvar_getInt(runtime, pos++);
    s32 pdfactor = env->localvar_getInt(runtime, pos++);

    glBlendFunc((GLenum)psfactor, (GLenum)pdfactor);
    
    
    return 0;
}

int org_mini_gl_GL_glLogicOp(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 popcode = env->localvar_getInt(runtime, pos++);

    glLogicOp((GLenum)popcode);
    
    
    return 0;
}

int org_mini_gl_GL_glStencilFunc(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pfunc = env->localvar_getInt(runtime, pos++);
    s32 pref = env->localvar_getInt(runtime, pos++);
    s32 pmask = env->localvar_getInt(runtime, pos++);

    glStencilFunc((GLenum)pfunc, (GLint)pref, (GLuint)pmask);
    
    
    return 0;
}

int org_mini_gl_GL_glStencilOp(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pfail = env->localvar_getInt(runtime, pos++);
    s32 pzfail = env->localvar_getInt(runtime, pos++);
    s32 pzpass = env->localvar_getInt(runtime, pos++);

    glStencilOp((GLenum)pfail, (GLenum)pzfail, (GLenum)pzpass);
    
    
    return 0;
}

int org_mini_gl_GL_glDepthFunc(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pfunc = env->localvar_getInt(runtime, pos++);

    glDepthFunc((GLenum)pfunc);
    
    
    return 0;
}

int org_mini_gl_GL_glPixelStoref(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Int2Float pparam;pparam.i = env->localvar_getInt(runtime, pos++);

    glPixelStoref((GLenum)ppname, (GLfloat)pparam.f);
    
    
    return 0;
}

int org_mini_gl_GL_glPixelStorei(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    s32 pparam = env->localvar_getInt(runtime, pos++);

    glPixelStorei((GLenum)ppname, (GLint)pparam);
    
    
    return 0;
}

int org_mini_gl_GL_glReadBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 psrc = env->localvar_getInt(runtime, pos++);

    glReadBuffer((GLenum)psrc);
    
    
    return 0;
}

int org_mini_gl_GL_glReadPixels(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    Instance *ppixels = env->localvar_getRefer(runtime, pos++);
    int offset_ppixels = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppixels = NULL;
    if(ppixels){
        ptr_ppixels = ppixels->arr_body + offset_ppixels;
    }

    glReadPixels((GLint)px, (GLint)py, (GLsizei)pwidth, (GLsizei)pheight, (GLenum)pformat, (GLenum)ptype, (void*)(ptr_ppixels));
    
    
    return 0;
}

int org_mini_gl_GL_glGetBooleanv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pdata = env->localvar_getRefer(runtime, pos++);
    int offset_pdata = env->localvar_getInt(runtime, pos++);
    __refer ptr_pdata = NULL;
    if(pdata){
        ptr_pdata = pdata->arr_body + offset_pdata;
    }

    glGetBooleanv((GLenum)ppname, (GLboolean*)(ptr_pdata));
    
    
    return 0;
}

int org_mini_gl_GL_glGetDoublev(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pdata = env->localvar_getRefer(runtime, pos++);
    int offset_pdata = env->localvar_getInt(runtime, pos++);
    __refer ptr_pdata = NULL;
    if(pdata){
        ptr_pdata = pdata->arr_body + offset_pdata;
    }

    glGetDoublev((GLenum)ppname, (GLdouble*)(ptr_pdata));
    
    
    return 0;
}

int org_mini_gl_GL_glGetError(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    

    GLenum _re_val = glGetError();
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glGetFloatv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pdata = env->localvar_getRefer(runtime, pos++);
    int offset_pdata = env->localvar_getInt(runtime, pos++);
    __refer ptr_pdata = NULL;
    if(pdata){
        ptr_pdata = pdata->arr_body + offset_pdata;
    }

    glGetFloatv((GLenum)ppname, (GLfloat*)(ptr_pdata));
    
    
    return 0;
}

int org_mini_gl_GL_glGetIntegerv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pdata = env->localvar_getRefer(runtime, pos++);
    int offset_pdata = env->localvar_getInt(runtime, pos++);
    __refer ptr_pdata = NULL;
    if(pdata){
        ptr_pdata = pdata->arr_body + offset_pdata;
    }

    glGetIntegerv((GLenum)ppname, (GLint*)(ptr_pdata));
    
    
    return 0;
}

int org_mini_gl_GL_glGetString(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pname = env->localvar_getInt(runtime, pos++);

    const GLubyte* _re_val = glGetString((GLenum)pname);
    c8* _ptr_re_val = (c8*)&_re_val;
    if (_ptr_re_val) {
        s32 bytes = sizeof(const GLubyte*);
        s32 j_t_bytes = sizeof(c8);
        Instance *_arr = env->jarray_create(bytes / j_t_bytes, DATATYPE_BYTE, NULL);
        memcpy(_arr->arr_body, _ptr_re_val,bytes);
        env->push_ref(runtime->stack, _arr);
    } else {
        env->push_ref(runtime->stack, NULL);
    }
    
    return 0;
}

int org_mini_gl_GL_glGetTexImage(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    Instance *ppixels = env->localvar_getRefer(runtime, pos++);
    int offset_ppixels = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppixels = NULL;
    if(ppixels){
        ptr_ppixels = ppixels->arr_body + offset_ppixels;
    }

    glGetTexImage((GLenum)ptarget, (GLint)plevel, (GLenum)pformat, (GLenum)ptype, (void*)(ptr_ppixels));
    
    
    return 0;
}

int org_mini_gl_GL_glGetTexParameterfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetTexParameterfv((GLenum)ptarget, (GLenum)ppname, (GLfloat*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetTexParameteriv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetTexParameteriv((GLenum)ptarget, (GLenum)ppname, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetTexLevelParameterfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetTexLevelParameterfv((GLenum)ptarget, (GLint)plevel, (GLenum)ppname, (GLfloat*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetTexLevelParameteriv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetTexLevelParameteriv((GLenum)ptarget, (GLint)plevel, (GLenum)ppname, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glIsEnabled(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pcap = env->localvar_getInt(runtime, pos++);

    GLboolean _re_val = glIsEnabled((GLenum)pcap);
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glDepthRange(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double pnear;pnear.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pfar;pfar.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glDepthRange((GLdouble)pnear.d, (GLdouble)pfar.d);
    
    
    return 0;
}

int org_mini_gl_GL_glViewport(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);

    glViewport((GLint)px, (GLint)py, (GLsizei)pwidth, (GLsizei)pheight);
    
    
    return 0;
}

int org_mini_gl_GL_glNewList(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plist = env->localvar_getInt(runtime, pos++);
    s32 pmode = env->localvar_getInt(runtime, pos++);

    glNewList((GLuint)plist, (GLenum)pmode);
    
    
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
    
    s32 plist = env->localvar_getInt(runtime, pos++);

    glCallList((GLuint)plist);
    
    
    return 0;
}

int org_mini_gl_GL_glCallLists(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pn = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    Instance *plists = env->localvar_getRefer(runtime, pos++);
    int offset_plists = env->localvar_getInt(runtime, pos++);
    __refer ptr_plists = NULL;
    if(plists){
        ptr_plists = plists->arr_body + offset_plists;
    }

    glCallLists((GLsizei)pn, (GLenum)ptype, (const void*)(ptr_plists));
    
    
    return 0;
}

int org_mini_gl_GL_glDeleteLists(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plist = env->localvar_getInt(runtime, pos++);
    s32 prange = env->localvar_getInt(runtime, pos++);

    glDeleteLists((GLuint)plist, (GLsizei)prange);
    
    
    return 0;
}

int org_mini_gl_GL_glGenLists(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 prange = env->localvar_getInt(runtime, pos++);

    GLuint _re_val = glGenLists((GLsizei)prange);
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glListBase(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pbase = env->localvar_getInt(runtime, pos++);

    glListBase((GLuint)pbase);
    
    
    return 0;
}

int org_mini_gl_GL_glBegin(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);

    glBegin((GLenum)pmode);
    
    
    return 0;
}

int org_mini_gl_GL_glBitmap(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);
    Int2Float pxorig;pxorig.i = env->localvar_getInt(runtime, pos++);
    Int2Float pyorig;pyorig.i = env->localvar_getInt(runtime, pos++);
    Int2Float pxmove;pxmove.i = env->localvar_getInt(runtime, pos++);
    Int2Float pymove;pymove.i = env->localvar_getInt(runtime, pos++);
    Instance *pbitmap = env->localvar_getRefer(runtime, pos++);
    int offset_pbitmap = env->localvar_getInt(runtime, pos++);
    __refer ptr_pbitmap = NULL;
    if(pbitmap){
        ptr_pbitmap = pbitmap->arr_body + offset_pbitmap;
    }

    glBitmap((GLsizei)pwidth, (GLsizei)pheight, (GLfloat)pxorig.f, (GLfloat)pyorig.f, (GLfloat)pxmove.f, (GLfloat)pymove.f, (const GLubyte*)(ptr_pbitmap));
    
    
    return 0;
}

int org_mini_gl_GL_glColor3b(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pred = env->localvar_getInt(runtime, pos++);
    s32 pgreen = env->localvar_getInt(runtime, pos++);
    s32 pblue = env->localvar_getInt(runtime, pos++);

    glColor3b((GLbyte)pred, (GLbyte)pgreen, (GLbyte)pblue);
    
    
    return 0;
}

int org_mini_gl_GL_glColor3bv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glColor3bv((const GLbyte*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glColor3d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double pred;pred.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pgreen;pgreen.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pblue;pblue.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glColor3d((GLdouble)pred.d, (GLdouble)pgreen.d, (GLdouble)pblue.d);
    
    
    return 0;
}

int org_mini_gl_GL_glColor3dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glColor3dv((const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glColor3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float pred;pred.i = env->localvar_getInt(runtime, pos++);
    Int2Float pgreen;pgreen.i = env->localvar_getInt(runtime, pos++);
    Int2Float pblue;pblue.i = env->localvar_getInt(runtime, pos++);

    glColor3f((GLfloat)pred.f, (GLfloat)pgreen.f, (GLfloat)pblue.f);
    
    
    return 0;
}

int org_mini_gl_GL_glColor3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glColor3fv((const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glColor3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pred = env->localvar_getInt(runtime, pos++);
    s32 pgreen = env->localvar_getInt(runtime, pos++);
    s32 pblue = env->localvar_getInt(runtime, pos++);

    glColor3i((GLint)pred, (GLint)pgreen, (GLint)pblue);
    
    
    return 0;
}

int org_mini_gl_GL_glColor3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glColor3iv((const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glColor3s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pred = env->localvar_getInt(runtime, pos++);
    s32 pgreen = env->localvar_getInt(runtime, pos++);
    s32 pblue = env->localvar_getInt(runtime, pos++);

    glColor3s((GLshort)pred, (GLshort)pgreen, (GLshort)pblue);
    
    
    return 0;
}

int org_mini_gl_GL_glColor3sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glColor3sv((const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glColor3ub(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pred = env->localvar_getInt(runtime, pos++);
    s32 pgreen = env->localvar_getInt(runtime, pos++);
    s32 pblue = env->localvar_getInt(runtime, pos++);

    glColor3ub((GLubyte)pred, (GLubyte)pgreen, (GLubyte)pblue);
    
    
    return 0;
}

int org_mini_gl_GL_glColor3ubv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glColor3ubv((const GLubyte*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glColor3ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pred = env->localvar_getInt(runtime, pos++);
    s32 pgreen = env->localvar_getInt(runtime, pos++);
    s32 pblue = env->localvar_getInt(runtime, pos++);

    glColor3ui((GLuint)pred, (GLuint)pgreen, (GLuint)pblue);
    
    
    return 0;
}

int org_mini_gl_GL_glColor3uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glColor3uiv((const GLuint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glColor3us(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pred = env->localvar_getInt(runtime, pos++);
    s32 pgreen = env->localvar_getInt(runtime, pos++);
    s32 pblue = env->localvar_getInt(runtime, pos++);

    glColor3us((GLushort)pred, (GLushort)pgreen, (GLushort)pblue);
    
    
    return 0;
}

int org_mini_gl_GL_glColor3usv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glColor3usv((const GLushort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glColor4b(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pred = env->localvar_getInt(runtime, pos++);
    s32 pgreen = env->localvar_getInt(runtime, pos++);
    s32 pblue = env->localvar_getInt(runtime, pos++);
    s32 palpha = env->localvar_getInt(runtime, pos++);

    glColor4b((GLbyte)pred, (GLbyte)pgreen, (GLbyte)pblue, (GLbyte)palpha);
    
    
    return 0;
}

int org_mini_gl_GL_glColor4bv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glColor4bv((const GLbyte*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glColor4d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double pred;pred.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pgreen;pgreen.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pblue;pblue.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double palpha;palpha.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glColor4d((GLdouble)pred.d, (GLdouble)pgreen.d, (GLdouble)pblue.d, (GLdouble)palpha.d);
    
    
    return 0;
}

int org_mini_gl_GL_glColor4dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glColor4dv((const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glColor4f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float pred;pred.i = env->localvar_getInt(runtime, pos++);
    Int2Float pgreen;pgreen.i = env->localvar_getInt(runtime, pos++);
    Int2Float pblue;pblue.i = env->localvar_getInt(runtime, pos++);
    Int2Float palpha;palpha.i = env->localvar_getInt(runtime, pos++);

    glColor4f((GLfloat)pred.f, (GLfloat)pgreen.f, (GLfloat)pblue.f, (GLfloat)palpha.f);
    
    
    return 0;
}

int org_mini_gl_GL_glColor4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glColor4fv((const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glColor4i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pred = env->localvar_getInt(runtime, pos++);
    s32 pgreen = env->localvar_getInt(runtime, pos++);
    s32 pblue = env->localvar_getInt(runtime, pos++);
    s32 palpha = env->localvar_getInt(runtime, pos++);

    glColor4i((GLint)pred, (GLint)pgreen, (GLint)pblue, (GLint)palpha);
    
    
    return 0;
}

int org_mini_gl_GL_glColor4iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glColor4iv((const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glColor4s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pred = env->localvar_getInt(runtime, pos++);
    s32 pgreen = env->localvar_getInt(runtime, pos++);
    s32 pblue = env->localvar_getInt(runtime, pos++);
    s32 palpha = env->localvar_getInt(runtime, pos++);

    glColor4s((GLshort)pred, (GLshort)pgreen, (GLshort)pblue, (GLshort)palpha);
    
    
    return 0;
}

int org_mini_gl_GL_glColor4sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glColor4sv((const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glColor4ub(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pred = env->localvar_getInt(runtime, pos++);
    s32 pgreen = env->localvar_getInt(runtime, pos++);
    s32 pblue = env->localvar_getInt(runtime, pos++);
    s32 palpha = env->localvar_getInt(runtime, pos++);

    glColor4ub((GLubyte)pred, (GLubyte)pgreen, (GLubyte)pblue, (GLubyte)palpha);
    
    
    return 0;
}

int org_mini_gl_GL_glColor4ubv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glColor4ubv((const GLubyte*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glColor4ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pred = env->localvar_getInt(runtime, pos++);
    s32 pgreen = env->localvar_getInt(runtime, pos++);
    s32 pblue = env->localvar_getInt(runtime, pos++);
    s32 palpha = env->localvar_getInt(runtime, pos++);

    glColor4ui((GLuint)pred, (GLuint)pgreen, (GLuint)pblue, (GLuint)palpha);
    
    
    return 0;
}

int org_mini_gl_GL_glColor4uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glColor4uiv((const GLuint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glColor4us(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pred = env->localvar_getInt(runtime, pos++);
    s32 pgreen = env->localvar_getInt(runtime, pos++);
    s32 pblue = env->localvar_getInt(runtime, pos++);
    s32 palpha = env->localvar_getInt(runtime, pos++);

    glColor4us((GLushort)pred, (GLushort)pgreen, (GLushort)pblue, (GLushort)palpha);
    
    
    return 0;
}

int org_mini_gl_GL_glColor4usv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glColor4usv((const GLushort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glEdgeFlag(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pflag = env->localvar_getInt(runtime, pos++);

    glEdgeFlag((GLboolean)pflag);
    
    
    return 0;
}

int org_mini_gl_GL_glEdgeFlagv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pflag = env->localvar_getRefer(runtime, pos++);
    int offset_pflag = env->localvar_getInt(runtime, pos++);
    __refer ptr_pflag = NULL;
    if(pflag){
        ptr_pflag = pflag->arr_body + offset_pflag;
    }

    glEdgeFlagv((const GLboolean*)(ptr_pflag));
    
    
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
    
    Long2Double pc;pc.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glIndexd((GLdouble)pc.d);
    
    
    return 0;
}

int org_mini_gl_GL_glIndexdv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pc = env->localvar_getRefer(runtime, pos++);
    int offset_pc = env->localvar_getInt(runtime, pos++);
    __refer ptr_pc = NULL;
    if(pc){
        ptr_pc = pc->arr_body + offset_pc;
    }

    glIndexdv((const GLdouble*)(ptr_pc));
    
    
    return 0;
}

int org_mini_gl_GL_glIndexf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float pc;pc.i = env->localvar_getInt(runtime, pos++);

    glIndexf((GLfloat)pc.f);
    
    
    return 0;
}

int org_mini_gl_GL_glIndexfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pc = env->localvar_getRefer(runtime, pos++);
    int offset_pc = env->localvar_getInt(runtime, pos++);
    __refer ptr_pc = NULL;
    if(pc){
        ptr_pc = pc->arr_body + offset_pc;
    }

    glIndexfv((const GLfloat*)(ptr_pc));
    
    
    return 0;
}

int org_mini_gl_GL_glIndexi(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pc = env->localvar_getInt(runtime, pos++);

    glIndexi((GLint)pc);
    
    
    return 0;
}

int org_mini_gl_GL_glIndexiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pc = env->localvar_getRefer(runtime, pos++);
    int offset_pc = env->localvar_getInt(runtime, pos++);
    __refer ptr_pc = NULL;
    if(pc){
        ptr_pc = pc->arr_body + offset_pc;
    }

    glIndexiv((const GLint*)(ptr_pc));
    
    
    return 0;
}

int org_mini_gl_GL_glIndexs(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pc = env->localvar_getInt(runtime, pos++);

    glIndexs((GLshort)pc);
    
    
    return 0;
}

int org_mini_gl_GL_glIndexsv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pc = env->localvar_getRefer(runtime, pos++);
    int offset_pc = env->localvar_getInt(runtime, pos++);
    __refer ptr_pc = NULL;
    if(pc){
        ptr_pc = pc->arr_body + offset_pc;
    }

    glIndexsv((const GLshort*)(ptr_pc));
    
    
    return 0;
}

int org_mini_gl_GL_glNormal3b(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pnx = env->localvar_getInt(runtime, pos++);
    s32 pny = env->localvar_getInt(runtime, pos++);
    s32 pnz = env->localvar_getInt(runtime, pos++);

    glNormal3b((GLbyte)pnx, (GLbyte)pny, (GLbyte)pnz);
    
    
    return 0;
}

int org_mini_gl_GL_glNormal3bv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glNormal3bv((const GLbyte*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glNormal3d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double pnx;pnx.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pny;pny.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pnz;pnz.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glNormal3d((GLdouble)pnx.d, (GLdouble)pny.d, (GLdouble)pnz.d);
    
    
    return 0;
}

int org_mini_gl_GL_glNormal3dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glNormal3dv((const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glNormal3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float pnx;pnx.i = env->localvar_getInt(runtime, pos++);
    Int2Float pny;pny.i = env->localvar_getInt(runtime, pos++);
    Int2Float pnz;pnz.i = env->localvar_getInt(runtime, pos++);

    glNormal3f((GLfloat)pnx.f, (GLfloat)pny.f, (GLfloat)pnz.f);
    
    
    return 0;
}

int org_mini_gl_GL_glNormal3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glNormal3fv((const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glNormal3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pnx = env->localvar_getInt(runtime, pos++);
    s32 pny = env->localvar_getInt(runtime, pos++);
    s32 pnz = env->localvar_getInt(runtime, pos++);

    glNormal3i((GLint)pnx, (GLint)pny, (GLint)pnz);
    
    
    return 0;
}

int org_mini_gl_GL_glNormal3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glNormal3iv((const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glNormal3s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pnx = env->localvar_getInt(runtime, pos++);
    s32 pny = env->localvar_getInt(runtime, pos++);
    s32 pnz = env->localvar_getInt(runtime, pos++);

    glNormal3s((GLshort)pnx, (GLshort)pny, (GLshort)pnz);
    
    
    return 0;
}

int org_mini_gl_GL_glNormal3sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glNormal3sv((const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos2d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double px;px.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double py;py.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glRasterPos2d((GLdouble)px.d, (GLdouble)py.d);
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos2dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glRasterPos2dv((const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float px;px.i = env->localvar_getInt(runtime, pos++);
    Int2Float py;py.i = env->localvar_getInt(runtime, pos++);

    glRasterPos2f((GLfloat)px.f, (GLfloat)py.f);
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glRasterPos2fv((const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos2i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);

    glRasterPos2i((GLint)px, (GLint)py);
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos2iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glRasterPos2iv((const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos2s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);

    glRasterPos2s((GLshort)px, (GLshort)py);
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos2sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glRasterPos2sv((const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos3d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double px;px.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double py;py.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pz;pz.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glRasterPos3d((GLdouble)px.d, (GLdouble)py.d, (GLdouble)pz.d);
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos3dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glRasterPos3dv((const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float px;px.i = env->localvar_getInt(runtime, pos++);
    Int2Float py;py.i = env->localvar_getInt(runtime, pos++);
    Int2Float pz;pz.i = env->localvar_getInt(runtime, pos++);

    glRasterPos3f((GLfloat)px.f, (GLfloat)py.f, (GLfloat)pz.f);
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glRasterPos3fv((const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pz = env->localvar_getInt(runtime, pos++);

    glRasterPos3i((GLint)px, (GLint)py, (GLint)pz);
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glRasterPos3iv((const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos3s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pz = env->localvar_getInt(runtime, pos++);

    glRasterPos3s((GLshort)px, (GLshort)py, (GLshort)pz);
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos3sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glRasterPos3sv((const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos4d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double px;px.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double py;py.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pz;pz.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pw;pw.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glRasterPos4d((GLdouble)px.d, (GLdouble)py.d, (GLdouble)pz.d, (GLdouble)pw.d);
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos4dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glRasterPos4dv((const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos4f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float px;px.i = env->localvar_getInt(runtime, pos++);
    Int2Float py;py.i = env->localvar_getInt(runtime, pos++);
    Int2Float pz;pz.i = env->localvar_getInt(runtime, pos++);
    Int2Float pw;pw.i = env->localvar_getInt(runtime, pos++);

    glRasterPos4f((GLfloat)px.f, (GLfloat)py.f, (GLfloat)pz.f, (GLfloat)pw.f);
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glRasterPos4fv((const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos4i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pz = env->localvar_getInt(runtime, pos++);
    s32 pw = env->localvar_getInt(runtime, pos++);

    glRasterPos4i((GLint)px, (GLint)py, (GLint)pz, (GLint)pw);
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos4iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glRasterPos4iv((const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos4s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pz = env->localvar_getInt(runtime, pos++);
    s32 pw = env->localvar_getInt(runtime, pos++);

    glRasterPos4s((GLshort)px, (GLshort)py, (GLshort)pz, (GLshort)pw);
    
    
    return 0;
}

int org_mini_gl_GL_glRasterPos4sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glRasterPos4sv((const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glRectd(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double px1;px1.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double py1;py1.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double px2;px2.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double py2;py2.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glRectd((GLdouble)px1.d, (GLdouble)py1.d, (GLdouble)px2.d, (GLdouble)py2.d);
    
    
    return 0;
}

int org_mini_gl_GL_glRectdv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv1 = env->localvar_getRefer(runtime, pos++);
    int offset_pv1 = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv1 = NULL;
    if(pv1){
        ptr_pv1 = pv1->arr_body + offset_pv1;
    }
    Instance *pv2 = env->localvar_getRefer(runtime, pos++);
    int offset_pv2 = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv2 = NULL;
    if(pv2){
        ptr_pv2 = pv2->arr_body + offset_pv2;
    }

    glRectdv((const GLdouble*)(ptr_pv1), (const GLdouble*)(ptr_pv2));
    
    
    return 0;
}

int org_mini_gl_GL_glRectf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float px1;px1.i = env->localvar_getInt(runtime, pos++);
    Int2Float py1;py1.i = env->localvar_getInt(runtime, pos++);
    Int2Float px2;px2.i = env->localvar_getInt(runtime, pos++);
    Int2Float py2;py2.i = env->localvar_getInt(runtime, pos++);

    glRectf((GLfloat)px1.f, (GLfloat)py1.f, (GLfloat)px2.f, (GLfloat)py2.f);
    
    
    return 0;
}

int org_mini_gl_GL_glRectfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv1 = env->localvar_getRefer(runtime, pos++);
    int offset_pv1 = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv1 = NULL;
    if(pv1){
        ptr_pv1 = pv1->arr_body + offset_pv1;
    }
    Instance *pv2 = env->localvar_getRefer(runtime, pos++);
    int offset_pv2 = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv2 = NULL;
    if(pv2){
        ptr_pv2 = pv2->arr_body + offset_pv2;
    }

    glRectfv((const GLfloat*)(ptr_pv1), (const GLfloat*)(ptr_pv2));
    
    
    return 0;
}

int org_mini_gl_GL_glRecti(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px1 = env->localvar_getInt(runtime, pos++);
    s32 py1 = env->localvar_getInt(runtime, pos++);
    s32 px2 = env->localvar_getInt(runtime, pos++);
    s32 py2 = env->localvar_getInt(runtime, pos++);

    glRecti((GLint)px1, (GLint)py1, (GLint)px2, (GLint)py2);
    
    
    return 0;
}

int org_mini_gl_GL_glRectiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv1 = env->localvar_getRefer(runtime, pos++);
    int offset_pv1 = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv1 = NULL;
    if(pv1){
        ptr_pv1 = pv1->arr_body + offset_pv1;
    }
    Instance *pv2 = env->localvar_getRefer(runtime, pos++);
    int offset_pv2 = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv2 = NULL;
    if(pv2){
        ptr_pv2 = pv2->arr_body + offset_pv2;
    }

    glRectiv((const GLint*)(ptr_pv1), (const GLint*)(ptr_pv2));
    
    
    return 0;
}

int org_mini_gl_GL_glRects(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px1 = env->localvar_getInt(runtime, pos++);
    s32 py1 = env->localvar_getInt(runtime, pos++);
    s32 px2 = env->localvar_getInt(runtime, pos++);
    s32 py2 = env->localvar_getInt(runtime, pos++);

    glRects((GLshort)px1, (GLshort)py1, (GLshort)px2, (GLshort)py2);
    
    
    return 0;
}

int org_mini_gl_GL_glRectsv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv1 = env->localvar_getRefer(runtime, pos++);
    int offset_pv1 = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv1 = NULL;
    if(pv1){
        ptr_pv1 = pv1->arr_body + offset_pv1;
    }
    Instance *pv2 = env->localvar_getRefer(runtime, pos++);
    int offset_pv2 = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv2 = NULL;
    if(pv2){
        ptr_pv2 = pv2->arr_body + offset_pv2;
    }

    glRectsv((const GLshort*)(ptr_pv1), (const GLshort*)(ptr_pv2));
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord1d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double ps;ps.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glTexCoord1d((GLdouble)ps.d);
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord1dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glTexCoord1dv((const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord1f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float ps;ps.i = env->localvar_getInt(runtime, pos++);

    glTexCoord1f((GLfloat)ps.f);
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord1fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glTexCoord1fv((const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord1i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ps = env->localvar_getInt(runtime, pos++);

    glTexCoord1i((GLint)ps);
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord1iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glTexCoord1iv((const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord1s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ps = env->localvar_getInt(runtime, pos++);

    glTexCoord1s((GLshort)ps);
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord1sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glTexCoord1sv((const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord2d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double ps;ps.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pt;pt.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glTexCoord2d((GLdouble)ps.d, (GLdouble)pt.d);
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord2dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glTexCoord2dv((const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float ps;ps.i = env->localvar_getInt(runtime, pos++);
    Int2Float pt;pt.i = env->localvar_getInt(runtime, pos++);

    glTexCoord2f((GLfloat)ps.f, (GLfloat)pt.f);
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glTexCoord2fv((const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord2i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ps = env->localvar_getInt(runtime, pos++);
    s32 pt = env->localvar_getInt(runtime, pos++);

    glTexCoord2i((GLint)ps, (GLint)pt);
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord2iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glTexCoord2iv((const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord2s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ps = env->localvar_getInt(runtime, pos++);
    s32 pt = env->localvar_getInt(runtime, pos++);

    glTexCoord2s((GLshort)ps, (GLshort)pt);
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord2sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glTexCoord2sv((const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord3d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double ps;ps.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pt;pt.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pr;pr.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glTexCoord3d((GLdouble)ps.d, (GLdouble)pt.d, (GLdouble)pr.d);
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord3dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glTexCoord3dv((const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float ps;ps.i = env->localvar_getInt(runtime, pos++);
    Int2Float pt;pt.i = env->localvar_getInt(runtime, pos++);
    Int2Float pr;pr.i = env->localvar_getInt(runtime, pos++);

    glTexCoord3f((GLfloat)ps.f, (GLfloat)pt.f, (GLfloat)pr.f);
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glTexCoord3fv((const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ps = env->localvar_getInt(runtime, pos++);
    s32 pt = env->localvar_getInt(runtime, pos++);
    s32 pr = env->localvar_getInt(runtime, pos++);

    glTexCoord3i((GLint)ps, (GLint)pt, (GLint)pr);
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glTexCoord3iv((const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord3s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ps = env->localvar_getInt(runtime, pos++);
    s32 pt = env->localvar_getInt(runtime, pos++);
    s32 pr = env->localvar_getInt(runtime, pos++);

    glTexCoord3s((GLshort)ps, (GLshort)pt, (GLshort)pr);
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord3sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glTexCoord3sv((const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord4d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double ps;ps.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pt;pt.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pr;pr.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pq;pq.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glTexCoord4d((GLdouble)ps.d, (GLdouble)pt.d, (GLdouble)pr.d, (GLdouble)pq.d);
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord4dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glTexCoord4dv((const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord4f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float ps;ps.i = env->localvar_getInt(runtime, pos++);
    Int2Float pt;pt.i = env->localvar_getInt(runtime, pos++);
    Int2Float pr;pr.i = env->localvar_getInt(runtime, pos++);
    Int2Float pq;pq.i = env->localvar_getInt(runtime, pos++);

    glTexCoord4f((GLfloat)ps.f, (GLfloat)pt.f, (GLfloat)pr.f, (GLfloat)pq.f);
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glTexCoord4fv((const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord4i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ps = env->localvar_getInt(runtime, pos++);
    s32 pt = env->localvar_getInt(runtime, pos++);
    s32 pr = env->localvar_getInt(runtime, pos++);
    s32 pq = env->localvar_getInt(runtime, pos++);

    glTexCoord4i((GLint)ps, (GLint)pt, (GLint)pr, (GLint)pq);
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord4iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glTexCoord4iv((const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord4s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ps = env->localvar_getInt(runtime, pos++);
    s32 pt = env->localvar_getInt(runtime, pos++);
    s32 pr = env->localvar_getInt(runtime, pos++);
    s32 pq = env->localvar_getInt(runtime, pos++);

    glTexCoord4s((GLshort)ps, (GLshort)pt, (GLshort)pr, (GLshort)pq);
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoord4sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glTexCoord4sv((const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertex2d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double px;px.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double py;py.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glVertex2d((GLdouble)px.d, (GLdouble)py.d);
    
    
    return 0;
}

int org_mini_gl_GL_glVertex2dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertex2dv((const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertex2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float px;px.i = env->localvar_getInt(runtime, pos++);
    Int2Float py;py.i = env->localvar_getInt(runtime, pos++);

    glVertex2f((GLfloat)px.f, (GLfloat)py.f);
    
    
    return 0;
}

int org_mini_gl_GL_glVertex2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertex2fv((const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertex2i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);

    glVertex2i((GLint)px, (GLint)py);
    
    
    return 0;
}

int org_mini_gl_GL_glVertex2iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertex2iv((const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertex2s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);

    glVertex2s((GLshort)px, (GLshort)py);
    
    
    return 0;
}

int org_mini_gl_GL_glVertex2sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertex2sv((const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertex3d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double px;px.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double py;py.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pz;pz.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glVertex3d((GLdouble)px.d, (GLdouble)py.d, (GLdouble)pz.d);
    
    
    return 0;
}

int org_mini_gl_GL_glVertex3dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertex3dv((const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertex3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float px;px.i = env->localvar_getInt(runtime, pos++);
    Int2Float py;py.i = env->localvar_getInt(runtime, pos++);
    Int2Float pz;pz.i = env->localvar_getInt(runtime, pos++);

    glVertex3f((GLfloat)px.f, (GLfloat)py.f, (GLfloat)pz.f);
    
    
    return 0;
}

int org_mini_gl_GL_glVertex3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertex3fv((const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertex3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pz = env->localvar_getInt(runtime, pos++);

    glVertex3i((GLint)px, (GLint)py, (GLint)pz);
    
    
    return 0;
}

int org_mini_gl_GL_glVertex3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertex3iv((const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertex3s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pz = env->localvar_getInt(runtime, pos++);

    glVertex3s((GLshort)px, (GLshort)py, (GLshort)pz);
    
    
    return 0;
}

int org_mini_gl_GL_glVertex3sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertex3sv((const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertex4d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double px;px.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double py;py.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pz;pz.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pw;pw.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glVertex4d((GLdouble)px.d, (GLdouble)py.d, (GLdouble)pz.d, (GLdouble)pw.d);
    
    
    return 0;
}

int org_mini_gl_GL_glVertex4dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertex4dv((const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertex4f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float px;px.i = env->localvar_getInt(runtime, pos++);
    Int2Float py;py.i = env->localvar_getInt(runtime, pos++);
    Int2Float pz;pz.i = env->localvar_getInt(runtime, pos++);
    Int2Float pw;pw.i = env->localvar_getInt(runtime, pos++);

    glVertex4f((GLfloat)px.f, (GLfloat)py.f, (GLfloat)pz.f, (GLfloat)pw.f);
    
    
    return 0;
}

int org_mini_gl_GL_glVertex4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertex4fv((const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertex4i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pz = env->localvar_getInt(runtime, pos++);
    s32 pw = env->localvar_getInt(runtime, pos++);

    glVertex4i((GLint)px, (GLint)py, (GLint)pz, (GLint)pw);
    
    
    return 0;
}

int org_mini_gl_GL_glVertex4iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertex4iv((const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertex4s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pz = env->localvar_getInt(runtime, pos++);
    s32 pw = env->localvar_getInt(runtime, pos++);

    glVertex4s((GLshort)px, (GLshort)py, (GLshort)pz, (GLshort)pw);
    
    
    return 0;
}

int org_mini_gl_GL_glVertex4sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertex4sv((const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glClipPlane(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pplane = env->localvar_getInt(runtime, pos++);
    Instance *pequation = env->localvar_getRefer(runtime, pos++);
    int offset_pequation = env->localvar_getInt(runtime, pos++);
    __refer ptr_pequation = NULL;
    if(pequation){
        ptr_pequation = pequation->arr_body + offset_pequation;
    }

    glClipPlane((GLenum)pplane, (const GLdouble*)(ptr_pequation));
    
    
    return 0;
}

int org_mini_gl_GL_glColorMaterial(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pface = env->localvar_getInt(runtime, pos++);
    s32 pmode = env->localvar_getInt(runtime, pos++);

    glColorMaterial((GLenum)pface, (GLenum)pmode);
    
    
    return 0;
}

int org_mini_gl_GL_glFogf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Int2Float pparam;pparam.i = env->localvar_getInt(runtime, pos++);

    glFogf((GLenum)ppname, (GLfloat)pparam.f);
    
    
    return 0;
}

int org_mini_gl_GL_glFogfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glFogfv((GLenum)ppname, (const GLfloat*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glFogi(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    s32 pparam = env->localvar_getInt(runtime, pos++);

    glFogi((GLenum)ppname, (GLint)pparam);
    
    
    return 0;
}

int org_mini_gl_GL_glFogiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glFogiv((GLenum)ppname, (const GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glLightf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plight = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Int2Float pparam;pparam.i = env->localvar_getInt(runtime, pos++);

    glLightf((GLenum)plight, (GLenum)ppname, (GLfloat)pparam.f);
    
    
    return 0;
}

int org_mini_gl_GL_glLightfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plight = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glLightfv((GLenum)plight, (GLenum)ppname, (const GLfloat*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glLighti(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plight = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    s32 pparam = env->localvar_getInt(runtime, pos++);

    glLighti((GLenum)plight, (GLenum)ppname, (GLint)pparam);
    
    
    return 0;
}

int org_mini_gl_GL_glLightiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plight = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glLightiv((GLenum)plight, (GLenum)ppname, (const GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glLightModelf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Int2Float pparam;pparam.i = env->localvar_getInt(runtime, pos++);

    glLightModelf((GLenum)ppname, (GLfloat)pparam.f);
    
    
    return 0;
}

int org_mini_gl_GL_glLightModelfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glLightModelfv((GLenum)ppname, (const GLfloat*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glLightModeli(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    s32 pparam = env->localvar_getInt(runtime, pos++);

    glLightModeli((GLenum)ppname, (GLint)pparam);
    
    
    return 0;
}

int org_mini_gl_GL_glLightModeliv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glLightModeliv((GLenum)ppname, (const GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glLineStipple(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pfactor = env->localvar_getInt(runtime, pos++);
    s32 ppattern = env->localvar_getInt(runtime, pos++);

    glLineStipple((GLint)pfactor, (GLushort)ppattern);
    
    
    return 0;
}

int org_mini_gl_GL_glMaterialf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pface = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Int2Float pparam;pparam.i = env->localvar_getInt(runtime, pos++);

    glMaterialf((GLenum)pface, (GLenum)ppname, (GLfloat)pparam.f);
    
    
    return 0;
}

int org_mini_gl_GL_glMaterialfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pface = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glMaterialfv((GLenum)pface, (GLenum)ppname, (const GLfloat*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glMateriali(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pface = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    s32 pparam = env->localvar_getInt(runtime, pos++);

    glMateriali((GLenum)pface, (GLenum)ppname, (GLint)pparam);
    
    
    return 0;
}

int org_mini_gl_GL_glMaterialiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pface = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glMaterialiv((GLenum)pface, (GLenum)ppname, (const GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glPolygonStipple(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pmask = env->localvar_getRefer(runtime, pos++);
    int offset_pmask = env->localvar_getInt(runtime, pos++);
    __refer ptr_pmask = NULL;
    if(pmask){
        ptr_pmask = pmask->arr_body + offset_pmask;
    }

    glPolygonStipple((const GLubyte*)(ptr_pmask));
    
    
    return 0;
}

int org_mini_gl_GL_glShadeModel(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);

    glShadeModel((GLenum)pmode);
    
    
    return 0;
}

int org_mini_gl_GL_glTexEnvf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Int2Float pparam;pparam.i = env->localvar_getInt(runtime, pos++);

    glTexEnvf((GLenum)ptarget, (GLenum)ppname, (GLfloat)pparam.f);
    
    
    return 0;
}

int org_mini_gl_GL_glTexEnvfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glTexEnvfv((GLenum)ptarget, (GLenum)ppname, (const GLfloat*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glTexEnvi(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    s32 pparam = env->localvar_getInt(runtime, pos++);

    glTexEnvi((GLenum)ptarget, (GLenum)ppname, (GLint)pparam);
    
    
    return 0;
}

int org_mini_gl_GL_glTexEnviv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glTexEnviv((GLenum)ptarget, (GLenum)ppname, (const GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glTexGend(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pcoord = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Long2Double pparam;pparam.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glTexGend((GLenum)pcoord, (GLenum)ppname, (GLdouble)pparam.d);
    
    
    return 0;
}

int org_mini_gl_GL_glTexGendv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pcoord = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glTexGendv((GLenum)pcoord, (GLenum)ppname, (const GLdouble*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glTexGenf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pcoord = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Int2Float pparam;pparam.i = env->localvar_getInt(runtime, pos++);

    glTexGenf((GLenum)pcoord, (GLenum)ppname, (GLfloat)pparam.f);
    
    
    return 0;
}

int org_mini_gl_GL_glTexGenfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pcoord = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glTexGenfv((GLenum)pcoord, (GLenum)ppname, (const GLfloat*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glTexGeni(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pcoord = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    s32 pparam = env->localvar_getInt(runtime, pos++);

    glTexGeni((GLenum)pcoord, (GLenum)ppname, (GLint)pparam);
    
    
    return 0;
}

int org_mini_gl_GL_glTexGeniv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pcoord = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glTexGeniv((GLenum)pcoord, (GLenum)ppname, (const GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glFeedbackBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 psize = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    Instance *pbuffer = env->localvar_getRefer(runtime, pos++);
    int offset_pbuffer = env->localvar_getInt(runtime, pos++);
    __refer ptr_pbuffer = NULL;
    if(pbuffer){
        ptr_pbuffer = pbuffer->arr_body + offset_pbuffer;
    }

    glFeedbackBuffer((GLsizei)psize, (GLenum)ptype, (GLfloat*)(ptr_pbuffer));
    
    
    return 0;
}

int org_mini_gl_GL_glSelectBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 psize = env->localvar_getInt(runtime, pos++);
    Instance *pbuffer = env->localvar_getRefer(runtime, pos++);
    int offset_pbuffer = env->localvar_getInt(runtime, pos++);
    __refer ptr_pbuffer = NULL;
    if(pbuffer){
        ptr_pbuffer = pbuffer->arr_body + offset_pbuffer;
    }

    glSelectBuffer((GLsizei)psize, (GLuint*)(ptr_pbuffer));
    
    
    return 0;
}

int org_mini_gl_GL_glRenderMode(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);

    GLint _re_val = glRenderMode((GLenum)pmode);
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
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
    
    s32 pname = env->localvar_getInt(runtime, pos++);

    glLoadName((GLuint)pname);
    
    
    return 0;
}

int org_mini_gl_GL_glPassThrough(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float ptoken;ptoken.i = env->localvar_getInt(runtime, pos++);

    glPassThrough((GLfloat)ptoken.f);
    
    
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
    
    s32 pname = env->localvar_getInt(runtime, pos++);

    glPushName((GLuint)pname);
    
    
    return 0;
}

int org_mini_gl_GL_glClearAccum(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float pred;pred.i = env->localvar_getInt(runtime, pos++);
    Int2Float pgreen;pgreen.i = env->localvar_getInt(runtime, pos++);
    Int2Float pblue;pblue.i = env->localvar_getInt(runtime, pos++);
    Int2Float palpha;palpha.i = env->localvar_getInt(runtime, pos++);

    glClearAccum((GLfloat)pred.f, (GLfloat)pgreen.f, (GLfloat)pblue.f, (GLfloat)palpha.f);
    
    
    return 0;
}

int org_mini_gl_GL_glClearIndex(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float pc;pc.i = env->localvar_getInt(runtime, pos++);

    glClearIndex((GLfloat)pc.f);
    
    
    return 0;
}

int org_mini_gl_GL_glIndexMask(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmask = env->localvar_getInt(runtime, pos++);

    glIndexMask((GLuint)pmask);
    
    
    return 0;
}

int org_mini_gl_GL_glAccum(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pop = env->localvar_getInt(runtime, pos++);
    Int2Float pvalue;pvalue.i = env->localvar_getInt(runtime, pos++);

    glAccum((GLenum)pop, (GLfloat)pvalue.f);
    
    
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
    
    s32 pmask = env->localvar_getInt(runtime, pos++);

    glPushAttrib((GLbitfield)pmask);
    
    
    return 0;
}

int org_mini_gl_GL_glMap1d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Long2Double pu1;pu1.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pu2;pu2.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    s32 pstride = env->localvar_getInt(runtime, pos++);
    s32 porder = env->localvar_getInt(runtime, pos++);
    Instance *ppoints = env->localvar_getRefer(runtime, pos++);
    int offset_ppoints = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppoints = NULL;
    if(ppoints){
        ptr_ppoints = ppoints->arr_body + offset_ppoints;
    }

    glMap1d((GLenum)ptarget, (GLdouble)pu1.d, (GLdouble)pu2.d, (GLint)pstride, (GLint)porder, (const GLdouble*)(ptr_ppoints));
    
    
    return 0;
}

int org_mini_gl_GL_glMap1f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Int2Float pu1;pu1.i = env->localvar_getInt(runtime, pos++);
    Int2Float pu2;pu2.i = env->localvar_getInt(runtime, pos++);
    s32 pstride = env->localvar_getInt(runtime, pos++);
    s32 porder = env->localvar_getInt(runtime, pos++);
    Instance *ppoints = env->localvar_getRefer(runtime, pos++);
    int offset_ppoints = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppoints = NULL;
    if(ppoints){
        ptr_ppoints = ppoints->arr_body + offset_ppoints;
    }

    glMap1f((GLenum)ptarget, (GLfloat)pu1.f, (GLfloat)pu2.f, (GLint)pstride, (GLint)porder, (const GLfloat*)(ptr_ppoints));
    
    
    return 0;
}

int org_mini_gl_GL_glMap2d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Long2Double pu1;pu1.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pu2;pu2.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    s32 pustride = env->localvar_getInt(runtime, pos++);
    s32 puorder = env->localvar_getInt(runtime, pos++);
    Long2Double pv1;pv1.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pv2;pv2.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    s32 pvstride = env->localvar_getInt(runtime, pos++);
    s32 pvorder = env->localvar_getInt(runtime, pos++);
    Instance *ppoints = env->localvar_getRefer(runtime, pos++);
    int offset_ppoints = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppoints = NULL;
    if(ppoints){
        ptr_ppoints = ppoints->arr_body + offset_ppoints;
    }

    glMap2d((GLenum)ptarget, (GLdouble)pu1.d, (GLdouble)pu2.d, (GLint)pustride, (GLint)puorder, (GLdouble)pv1.d, (GLdouble)pv2.d, (GLint)pvstride, (GLint)pvorder, (const GLdouble*)(ptr_ppoints));
    
    
    return 0;
}

int org_mini_gl_GL_glMap2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Int2Float pu1;pu1.i = env->localvar_getInt(runtime, pos++);
    Int2Float pu2;pu2.i = env->localvar_getInt(runtime, pos++);
    s32 pustride = env->localvar_getInt(runtime, pos++);
    s32 puorder = env->localvar_getInt(runtime, pos++);
    Int2Float pv1;pv1.i = env->localvar_getInt(runtime, pos++);
    Int2Float pv2;pv2.i = env->localvar_getInt(runtime, pos++);
    s32 pvstride = env->localvar_getInt(runtime, pos++);
    s32 pvorder = env->localvar_getInt(runtime, pos++);
    Instance *ppoints = env->localvar_getRefer(runtime, pos++);
    int offset_ppoints = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppoints = NULL;
    if(ppoints){
        ptr_ppoints = ppoints->arr_body + offset_ppoints;
    }

    glMap2f((GLenum)ptarget, (GLfloat)pu1.f, (GLfloat)pu2.f, (GLint)pustride, (GLint)puorder, (GLfloat)pv1.f, (GLfloat)pv2.f, (GLint)pvstride, (GLint)pvorder, (const GLfloat*)(ptr_ppoints));
    
    
    return 0;
}

int org_mini_gl_GL_glMapGrid1d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pun = env->localvar_getInt(runtime, pos++);
    Long2Double pu1;pu1.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pu2;pu2.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glMapGrid1d((GLint)pun, (GLdouble)pu1.d, (GLdouble)pu2.d);
    
    
    return 0;
}

int org_mini_gl_GL_glMapGrid1f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pun = env->localvar_getInt(runtime, pos++);
    Int2Float pu1;pu1.i = env->localvar_getInt(runtime, pos++);
    Int2Float pu2;pu2.i = env->localvar_getInt(runtime, pos++);

    glMapGrid1f((GLint)pun, (GLfloat)pu1.f, (GLfloat)pu2.f);
    
    
    return 0;
}

int org_mini_gl_GL_glMapGrid2d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pun = env->localvar_getInt(runtime, pos++);
    Long2Double pu1;pu1.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pu2;pu2.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    s32 pvn = env->localvar_getInt(runtime, pos++);
    Long2Double pv1;pv1.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pv2;pv2.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glMapGrid2d((GLint)pun, (GLdouble)pu1.d, (GLdouble)pu2.d, (GLint)pvn, (GLdouble)pv1.d, (GLdouble)pv2.d);
    
    
    return 0;
}

int org_mini_gl_GL_glMapGrid2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pun = env->localvar_getInt(runtime, pos++);
    Int2Float pu1;pu1.i = env->localvar_getInt(runtime, pos++);
    Int2Float pu2;pu2.i = env->localvar_getInt(runtime, pos++);
    s32 pvn = env->localvar_getInt(runtime, pos++);
    Int2Float pv1;pv1.i = env->localvar_getInt(runtime, pos++);
    Int2Float pv2;pv2.i = env->localvar_getInt(runtime, pos++);

    glMapGrid2f((GLint)pun, (GLfloat)pu1.f, (GLfloat)pu2.f, (GLint)pvn, (GLfloat)pv1.f, (GLfloat)pv2.f);
    
    
    return 0;
}

int org_mini_gl_GL_glEvalCoord1d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double pu;pu.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glEvalCoord1d((GLdouble)pu.d);
    
    
    return 0;
}

int org_mini_gl_GL_glEvalCoord1dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pu = env->localvar_getRefer(runtime, pos++);
    int offset_pu = env->localvar_getInt(runtime, pos++);
    __refer ptr_pu = NULL;
    if(pu){
        ptr_pu = pu->arr_body + offset_pu;
    }

    glEvalCoord1dv((const GLdouble*)(ptr_pu));
    
    
    return 0;
}

int org_mini_gl_GL_glEvalCoord1f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float pu;pu.i = env->localvar_getInt(runtime, pos++);

    glEvalCoord1f((GLfloat)pu.f);
    
    
    return 0;
}

int org_mini_gl_GL_glEvalCoord1fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pu = env->localvar_getRefer(runtime, pos++);
    int offset_pu = env->localvar_getInt(runtime, pos++);
    __refer ptr_pu = NULL;
    if(pu){
        ptr_pu = pu->arr_body + offset_pu;
    }

    glEvalCoord1fv((const GLfloat*)(ptr_pu));
    
    
    return 0;
}

int org_mini_gl_GL_glEvalCoord2d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double pu;pu.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pv;pv.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glEvalCoord2d((GLdouble)pu.d, (GLdouble)pv.d);
    
    
    return 0;
}

int org_mini_gl_GL_glEvalCoord2dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pu = env->localvar_getRefer(runtime, pos++);
    int offset_pu = env->localvar_getInt(runtime, pos++);
    __refer ptr_pu = NULL;
    if(pu){
        ptr_pu = pu->arr_body + offset_pu;
    }

    glEvalCoord2dv((const GLdouble*)(ptr_pu));
    
    
    return 0;
}

int org_mini_gl_GL_glEvalCoord2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float pu;pu.i = env->localvar_getInt(runtime, pos++);
    Int2Float pv;pv.i = env->localvar_getInt(runtime, pos++);

    glEvalCoord2f((GLfloat)pu.f, (GLfloat)pv.f);
    
    
    return 0;
}

int org_mini_gl_GL_glEvalCoord2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pu = env->localvar_getRefer(runtime, pos++);
    int offset_pu = env->localvar_getInt(runtime, pos++);
    __refer ptr_pu = NULL;
    if(pu){
        ptr_pu = pu->arr_body + offset_pu;
    }

    glEvalCoord2fv((const GLfloat*)(ptr_pu));
    
    
    return 0;
}

int org_mini_gl_GL_glEvalMesh1(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);
    s32 pi1 = env->localvar_getInt(runtime, pos++);
    s32 pi2 = env->localvar_getInt(runtime, pos++);

    glEvalMesh1((GLenum)pmode, (GLint)pi1, (GLint)pi2);
    
    
    return 0;
}

int org_mini_gl_GL_glEvalPoint1(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pi = env->localvar_getInt(runtime, pos++);

    glEvalPoint1((GLint)pi);
    
    
    return 0;
}

int org_mini_gl_GL_glEvalMesh2(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);
    s32 pi1 = env->localvar_getInt(runtime, pos++);
    s32 pi2 = env->localvar_getInt(runtime, pos++);
    s32 pj1 = env->localvar_getInt(runtime, pos++);
    s32 pj2 = env->localvar_getInt(runtime, pos++);

    glEvalMesh2((GLenum)pmode, (GLint)pi1, (GLint)pi2, (GLint)pj1, (GLint)pj2);
    
    
    return 0;
}

int org_mini_gl_GL_glEvalPoint2(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pi = env->localvar_getInt(runtime, pos++);
    s32 pj = env->localvar_getInt(runtime, pos++);

    glEvalPoint2((GLint)pi, (GLint)pj);
    
    
    return 0;
}

int org_mini_gl_GL_glAlphaFunc(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pfunc = env->localvar_getInt(runtime, pos++);
    Int2Float pref;pref.i = env->localvar_getInt(runtime, pos++);

    glAlphaFunc((GLenum)pfunc, (GLfloat)pref.f);
    
    
    return 0;
}

int org_mini_gl_GL_glPixelZoom(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float pxfactor;pxfactor.i = env->localvar_getInt(runtime, pos++);
    Int2Float pyfactor;pyfactor.i = env->localvar_getInt(runtime, pos++);

    glPixelZoom((GLfloat)pxfactor.f, (GLfloat)pyfactor.f);
    
    
    return 0;
}

int org_mini_gl_GL_glPixelTransferf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Int2Float pparam;pparam.i = env->localvar_getInt(runtime, pos++);

    glPixelTransferf((GLenum)ppname, (GLfloat)pparam.f);
    
    
    return 0;
}

int org_mini_gl_GL_glPixelTransferi(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    s32 pparam = env->localvar_getInt(runtime, pos++);

    glPixelTransferi((GLenum)ppname, (GLint)pparam);
    
    
    return 0;
}

int org_mini_gl_GL_glPixelMapfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmap = env->localvar_getInt(runtime, pos++);
    s32 pmapsize = env->localvar_getInt(runtime, pos++);
    Instance *pvalues = env->localvar_getRefer(runtime, pos++);
    int offset_pvalues = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalues = NULL;
    if(pvalues){
        ptr_pvalues = pvalues->arr_body + offset_pvalues;
    }

    glPixelMapfv((GLenum)pmap, (GLsizei)pmapsize, (const GLfloat*)(ptr_pvalues));
    
    
    return 0;
}

int org_mini_gl_GL_glPixelMapuiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmap = env->localvar_getInt(runtime, pos++);
    s32 pmapsize = env->localvar_getInt(runtime, pos++);
    Instance *pvalues = env->localvar_getRefer(runtime, pos++);
    int offset_pvalues = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalues = NULL;
    if(pvalues){
        ptr_pvalues = pvalues->arr_body + offset_pvalues;
    }

    glPixelMapuiv((GLenum)pmap, (GLsizei)pmapsize, (const GLuint*)(ptr_pvalues));
    
    
    return 0;
}

int org_mini_gl_GL_glPixelMapusv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmap = env->localvar_getInt(runtime, pos++);
    s32 pmapsize = env->localvar_getInt(runtime, pos++);
    Instance *pvalues = env->localvar_getRefer(runtime, pos++);
    int offset_pvalues = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalues = NULL;
    if(pvalues){
        ptr_pvalues = pvalues->arr_body + offset_pvalues;
    }

    glPixelMapusv((GLenum)pmap, (GLsizei)pmapsize, (const GLushort*)(ptr_pvalues));
    
    
    return 0;
}

int org_mini_gl_GL_glCopyPixels(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);

    glCopyPixels((GLint)px, (GLint)py, (GLsizei)pwidth, (GLsizei)pheight, (GLenum)ptype);
    
    
    return 0;
}

int org_mini_gl_GL_glDrawPixels(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    Instance *ppixels = env->localvar_getRefer(runtime, pos++);
    int offset_ppixels = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppixels = NULL;
    if(ppixels){
        ptr_ppixels = ppixels->arr_body + offset_ppixels;
    }

    glDrawPixels((GLsizei)pwidth, (GLsizei)pheight, (GLenum)pformat, (GLenum)ptype, (const void*)(ptr_ppixels));
    
    
    return 0;
}

int org_mini_gl_GL_glGetClipPlane(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pplane = env->localvar_getInt(runtime, pos++);
    Instance *pequation = env->localvar_getRefer(runtime, pos++);
    int offset_pequation = env->localvar_getInt(runtime, pos++);
    __refer ptr_pequation = NULL;
    if(pequation){
        ptr_pequation = pequation->arr_body + offset_pequation;
    }

    glGetClipPlane((GLenum)pplane, (GLdouble*)(ptr_pequation));
    
    
    return 0;
}

int org_mini_gl_GL_glGetLightfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plight = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetLightfv((GLenum)plight, (GLenum)ppname, (GLfloat*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetLightiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plight = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetLightiv((GLenum)plight, (GLenum)ppname, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetMapdv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pquery = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glGetMapdv((GLenum)ptarget, (GLenum)pquery, (GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glGetMapfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pquery = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glGetMapfv((GLenum)ptarget, (GLenum)pquery, (GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glGetMapiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pquery = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glGetMapiv((GLenum)ptarget, (GLenum)pquery, (GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glGetMaterialfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pface = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetMaterialfv((GLenum)pface, (GLenum)ppname, (GLfloat*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetMaterialiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pface = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetMaterialiv((GLenum)pface, (GLenum)ppname, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetPixelMapfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmap = env->localvar_getInt(runtime, pos++);
    Instance *pvalues = env->localvar_getRefer(runtime, pos++);
    int offset_pvalues = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalues = NULL;
    if(pvalues){
        ptr_pvalues = pvalues->arr_body + offset_pvalues;
    }

    glGetPixelMapfv((GLenum)pmap, (GLfloat*)(ptr_pvalues));
    
    
    return 0;
}

int org_mini_gl_GL_glGetPixelMapuiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmap = env->localvar_getInt(runtime, pos++);
    Instance *pvalues = env->localvar_getRefer(runtime, pos++);
    int offset_pvalues = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalues = NULL;
    if(pvalues){
        ptr_pvalues = pvalues->arr_body + offset_pvalues;
    }

    glGetPixelMapuiv((GLenum)pmap, (GLuint*)(ptr_pvalues));
    
    
    return 0;
}

int org_mini_gl_GL_glGetPixelMapusv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmap = env->localvar_getInt(runtime, pos++);
    Instance *pvalues = env->localvar_getRefer(runtime, pos++);
    int offset_pvalues = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalues = NULL;
    if(pvalues){
        ptr_pvalues = pvalues->arr_body + offset_pvalues;
    }

    glGetPixelMapusv((GLenum)pmap, (GLushort*)(ptr_pvalues));
    
    
    return 0;
}

int org_mini_gl_GL_glGetPolygonStipple(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pmask = env->localvar_getRefer(runtime, pos++);
    int offset_pmask = env->localvar_getInt(runtime, pos++);
    __refer ptr_pmask = NULL;
    if(pmask){
        ptr_pmask = pmask->arr_body + offset_pmask;
    }

    glGetPolygonStipple((GLubyte*)(ptr_pmask));
    
    
    return 0;
}

int org_mini_gl_GL_glGetTexEnvfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetTexEnvfv((GLenum)ptarget, (GLenum)ppname, (GLfloat*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetTexEnviv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetTexEnviv((GLenum)ptarget, (GLenum)ppname, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetTexGendv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pcoord = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetTexGendv((GLenum)pcoord, (GLenum)ppname, (GLdouble*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetTexGenfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pcoord = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetTexGenfv((GLenum)pcoord, (GLenum)ppname, (GLfloat*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetTexGeniv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pcoord = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetTexGeniv((GLenum)pcoord, (GLenum)ppname, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glIsList(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plist = env->localvar_getInt(runtime, pos++);

    GLboolean _re_val = glIsList((GLuint)plist);
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glFrustum(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double pleft;pleft.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pright;pright.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pbottom;pbottom.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double ptop;ptop.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pzNear;pzNear.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pzFar;pzFar.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glFrustum((GLdouble)pleft.d, (GLdouble)pright.d, (GLdouble)pbottom.d, (GLdouble)ptop.d, (GLdouble)pzNear.d, (GLdouble)pzFar.d);
    
    
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
    
    Instance *pm = env->localvar_getRefer(runtime, pos++);
    int offset_pm = env->localvar_getInt(runtime, pos++);
    __refer ptr_pm = NULL;
    if(pm){
        ptr_pm = pm->arr_body + offset_pm;
    }

    glLoadMatrixf((const GLfloat*)(ptr_pm));
    
    
    return 0;
}

int org_mini_gl_GL_glLoadMatrixd(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pm = env->localvar_getRefer(runtime, pos++);
    int offset_pm = env->localvar_getInt(runtime, pos++);
    __refer ptr_pm = NULL;
    if(pm){
        ptr_pm = pm->arr_body + offset_pm;
    }

    glLoadMatrixd((const GLdouble*)(ptr_pm));
    
    
    return 0;
}

int org_mini_gl_GL_glMatrixMode(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);

    glMatrixMode((GLenum)pmode);
    
    
    return 0;
}

int org_mini_gl_GL_glMultMatrixf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pm = env->localvar_getRefer(runtime, pos++);
    int offset_pm = env->localvar_getInt(runtime, pos++);
    __refer ptr_pm = NULL;
    if(pm){
        ptr_pm = pm->arr_body + offset_pm;
    }

    glMultMatrixf((const GLfloat*)(ptr_pm));
    
    
    return 0;
}

int org_mini_gl_GL_glMultMatrixd(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pm = env->localvar_getRefer(runtime, pos++);
    int offset_pm = env->localvar_getInt(runtime, pos++);
    __refer ptr_pm = NULL;
    if(pm){
        ptr_pm = pm->arr_body + offset_pm;
    }

    glMultMatrixd((const GLdouble*)(ptr_pm));
    
    
    return 0;
}

int org_mini_gl_GL_glOrtho(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double pleft;pleft.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pright;pright.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pbottom;pbottom.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double ptop;ptop.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pzNear;pzNear.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pzFar;pzFar.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glOrtho((GLdouble)pleft.d, (GLdouble)pright.d, (GLdouble)pbottom.d, (GLdouble)ptop.d, (GLdouble)pzNear.d, (GLdouble)pzFar.d);
    
    
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
    
    Long2Double pangle;pangle.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double px;px.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double py;py.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pz;pz.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glRotated((GLdouble)pangle.d, (GLdouble)px.d, (GLdouble)py.d, (GLdouble)pz.d);
    
    
    return 0;
}

int org_mini_gl_GL_glRotatef(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float pangle;pangle.i = env->localvar_getInt(runtime, pos++);
    Int2Float px;px.i = env->localvar_getInt(runtime, pos++);
    Int2Float py;py.i = env->localvar_getInt(runtime, pos++);
    Int2Float pz;pz.i = env->localvar_getInt(runtime, pos++);

    glRotatef((GLfloat)pangle.f, (GLfloat)px.f, (GLfloat)py.f, (GLfloat)pz.f);
    
    
    return 0;
}

int org_mini_gl_GL_glScaled(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double px;px.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double py;py.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pz;pz.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glScaled((GLdouble)px.d, (GLdouble)py.d, (GLdouble)pz.d);
    
    
    return 0;
}

int org_mini_gl_GL_glScalef(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float px;px.i = env->localvar_getInt(runtime, pos++);
    Int2Float py;py.i = env->localvar_getInt(runtime, pos++);
    Int2Float pz;pz.i = env->localvar_getInt(runtime, pos++);

    glScalef((GLfloat)px.f, (GLfloat)py.f, (GLfloat)pz.f);
    
    
    return 0;
}

int org_mini_gl_GL_glTranslated(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double px;px.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double py;py.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pz;pz.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glTranslated((GLdouble)px.d, (GLdouble)py.d, (GLdouble)pz.d);
    
    
    return 0;
}

int org_mini_gl_GL_glTranslatef(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float px;px.i = env->localvar_getInt(runtime, pos++);
    Int2Float py;py.i = env->localvar_getInt(runtime, pos++);
    Int2Float pz;pz.i = env->localvar_getInt(runtime, pos++);

    glTranslatef((GLfloat)px.f, (GLfloat)py.f, (GLfloat)pz.f);
    
    
    return 0;
}

int org_mini_gl_GL_glDrawArrays(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);
    s32 pfirst = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);

    glDrawArrays((GLenum)pmode, (GLint)pfirst, (GLsizei)pcount);
    
    
    return 0;
}

int org_mini_gl_GL_glDrawElements(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    Instance *pindices = env->localvar_getRefer(runtime, pos++);
    int offset_pindices = env->localvar_getInt(runtime, pos++);
    __refer ptr_pindices = NULL;
    if(pindices){
        ptr_pindices = pindices->arr_body + offset_pindices;
    }

    glDrawElements((GLenum)pmode, (GLsizei)pcount, (GLenum)ptype, (const void*)(ptr_pindices));
    
    
    return 0;
}

int org_mini_gl_GL_glGetPointerv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    ReferArr *ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = env->referarr_create(pparams);
    }

    glGetPointerv((GLenum)ppname, (void**)(ptr_pparams->arr_body));
    
    env->referarr_destory(ptr_pparams);
    return 0;
}

int org_mini_gl_GL_glPolygonOffset(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float pfactor;pfactor.i = env->localvar_getInt(runtime, pos++);
    Int2Float punits;punits.i = env->localvar_getInt(runtime, pos++);

    glPolygonOffset((GLfloat)pfactor.f, (GLfloat)punits.f);
    
    
    return 0;
}

int org_mini_gl_GL_glCopyTexImage1D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pinternalformat = env->localvar_getInt(runtime, pos++);
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pborder = env->localvar_getInt(runtime, pos++);

    glCopyTexImage1D((GLenum)ptarget, (GLint)plevel, (GLenum)pinternalformat, (GLint)px, (GLint)py, (GLsizei)pwidth, (GLint)pborder);
    
    
    return 0;
}

int org_mini_gl_GL_glCopyTexImage2D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pinternalformat = env->localvar_getInt(runtime, pos++);
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);
    s32 pborder = env->localvar_getInt(runtime, pos++);

    glCopyTexImage2D((GLenum)ptarget, (GLint)plevel, (GLenum)pinternalformat, (GLint)px, (GLint)py, (GLsizei)pwidth, (GLsizei)pheight, (GLint)pborder);
    
    
    return 0;
}

int org_mini_gl_GL_glCopyTexSubImage1D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pxoffset = env->localvar_getInt(runtime, pos++);
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);

    glCopyTexSubImage1D((GLenum)ptarget, (GLint)plevel, (GLint)pxoffset, (GLint)px, (GLint)py, (GLsizei)pwidth);
    
    
    return 0;
}

int org_mini_gl_GL_glCopyTexSubImage2D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pxoffset = env->localvar_getInt(runtime, pos++);
    s32 pyoffset = env->localvar_getInt(runtime, pos++);
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);

    glCopyTexSubImage2D((GLenum)ptarget, (GLint)plevel, (GLint)pxoffset, (GLint)pyoffset, (GLint)px, (GLint)py, (GLsizei)pwidth, (GLsizei)pheight);
    
    
    return 0;
}

int org_mini_gl_GL_glTexSubImage1D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pxoffset = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    Instance *ppixels = env->localvar_getRefer(runtime, pos++);
    int offset_ppixels = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppixels = NULL;
    if(ppixels){
        ptr_ppixels = ppixels->arr_body + offset_ppixels;
    }

    glTexSubImage1D((GLenum)ptarget, (GLint)plevel, (GLint)pxoffset, (GLsizei)pwidth, (GLenum)pformat, (GLenum)ptype, (const void*)(ptr_ppixels));
    
    
    return 0;
}

int org_mini_gl_GL_glTexSubImage2D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pxoffset = env->localvar_getInt(runtime, pos++);
    s32 pyoffset = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    Instance *ppixels = env->localvar_getRefer(runtime, pos++);
    int offset_ppixels = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppixels = NULL;
    if(ppixels){
        ptr_ppixels = ppixels->arr_body + offset_ppixels;
    }

    glTexSubImage2D((GLenum)ptarget, (GLint)plevel, (GLint)pxoffset, (GLint)pyoffset, (GLsizei)pwidth, (GLsizei)pheight, (GLenum)pformat, (GLenum)ptype, (const void*)(ptr_ppixels));
    
    
    return 0;
}

int org_mini_gl_GL_glBindTexture(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ptexture = env->localvar_getInt(runtime, pos++);

    glBindTexture((GLenum)ptarget, (GLuint)ptexture);
    
    
    return 0;
}

int org_mini_gl_GL_glDeleteTextures(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pn = env->localvar_getInt(runtime, pos++);
    Instance *ptextures = env->localvar_getRefer(runtime, pos++);
    int offset_ptextures = env->localvar_getInt(runtime, pos++);
    __refer ptr_ptextures = NULL;
    if(ptextures){
        ptr_ptextures = ptextures->arr_body + offset_ptextures;
    }

    glDeleteTextures((GLsizei)pn, (const GLuint*)(ptr_ptextures));
    
    
    return 0;
}

int org_mini_gl_GL_glGenTextures(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pn = env->localvar_getInt(runtime, pos++);
    Instance *ptextures = env->localvar_getRefer(runtime, pos++);
    int offset_ptextures = env->localvar_getInt(runtime, pos++);
    __refer ptr_ptextures = NULL;
    if(ptextures){
        ptr_ptextures = ptextures->arr_body + offset_ptextures;
    }

    glGenTextures((GLsizei)pn, (GLuint*)(ptr_ptextures));
    
    
    return 0;
}

int org_mini_gl_GL_glIsTexture(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptexture = env->localvar_getInt(runtime, pos++);

    GLboolean _re_val = glIsTexture((GLuint)ptexture);
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glArrayElement(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pi = env->localvar_getInt(runtime, pos++);

    glArrayElement((GLint)pi);
    
    
    return 0;
}

int org_mini_gl_GL_glColorPointer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 psize = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 pstride = env->localvar_getInt(runtime, pos++);
    Instance *ppointer = env->localvar_getRefer(runtime, pos++);
    int offset_ppointer = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppointer = NULL;
    if(ppointer){
        ptr_ppointer = ppointer->arr_body + offset_ppointer;
    }

    glColorPointer((GLint)psize, (GLenum)ptype, (GLsizei)pstride, (const void*)(ptr_ppointer));
    
    
    return 0;
}

int org_mini_gl_GL_glDisableClientState(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 parray = env->localvar_getInt(runtime, pos++);

    glDisableClientState((GLenum)parray);
    
    
    return 0;
}

int org_mini_gl_GL_glEdgeFlagPointer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pstride = env->localvar_getInt(runtime, pos++);
    Instance *ppointer = env->localvar_getRefer(runtime, pos++);
    int offset_ppointer = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppointer = NULL;
    if(ppointer){
        ptr_ppointer = ppointer->arr_body + offset_ppointer;
    }

    glEdgeFlagPointer((GLsizei)pstride, (const void*)(ptr_ppointer));
    
    
    return 0;
}

int org_mini_gl_GL_glEnableClientState(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 parray = env->localvar_getInt(runtime, pos++);

    glEnableClientState((GLenum)parray);
    
    
    return 0;
}

int org_mini_gl_GL_glIndexPointer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 pstride = env->localvar_getInt(runtime, pos++);
    Instance *ppointer = env->localvar_getRefer(runtime, pos++);
    int offset_ppointer = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppointer = NULL;
    if(ppointer){
        ptr_ppointer = ppointer->arr_body + offset_ppointer;
    }

    glIndexPointer((GLenum)ptype, (GLsizei)pstride, (const void*)(ptr_ppointer));
    
    
    return 0;
}

int org_mini_gl_GL_glInterleavedArrays(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 pstride = env->localvar_getInt(runtime, pos++);
    Instance *ppointer = env->localvar_getRefer(runtime, pos++);
    int offset_ppointer = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppointer = NULL;
    if(ppointer){
        ptr_ppointer = ppointer->arr_body + offset_ppointer;
    }

    glInterleavedArrays((GLenum)pformat, (GLsizei)pstride, (const void*)(ptr_ppointer));
    
    
    return 0;
}

int org_mini_gl_GL_glNormalPointer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 pstride = env->localvar_getInt(runtime, pos++);
    Instance *ppointer = env->localvar_getRefer(runtime, pos++);
    int offset_ppointer = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppointer = NULL;
    if(ppointer){
        ptr_ppointer = ppointer->arr_body + offset_ppointer;
    }

    glNormalPointer((GLenum)ptype, (GLsizei)pstride, (const void*)(ptr_ppointer));
    
    
    return 0;
}

int org_mini_gl_GL_glTexCoordPointer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 psize = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 pstride = env->localvar_getInt(runtime, pos++);
    Instance *ppointer = env->localvar_getRefer(runtime, pos++);
    int offset_ppointer = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppointer = NULL;
    if(ppointer){
        ptr_ppointer = ppointer->arr_body + offset_ppointer;
    }

    glTexCoordPointer((GLint)psize, (GLenum)ptype, (GLsizei)pstride, (const void*)(ptr_ppointer));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexPointer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 psize = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 pstride = env->localvar_getInt(runtime, pos++);
    Instance *ppointer = env->localvar_getRefer(runtime, pos++);
    int offset_ppointer = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppointer = NULL;
    if(ppointer){
        ptr_ppointer = ppointer->arr_body + offset_ppointer;
    }

    glVertexPointer((GLint)psize, (GLenum)ptype, (GLsizei)pstride, (const void*)(ptr_ppointer));
    
    
    return 0;
}

int org_mini_gl_GL_glAreTexturesResident(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pn = env->localvar_getInt(runtime, pos++);
    Instance *ptextures = env->localvar_getRefer(runtime, pos++);
    int offset_ptextures = env->localvar_getInt(runtime, pos++);
    __refer ptr_ptextures = NULL;
    if(ptextures){
        ptr_ptextures = ptextures->arr_body + offset_ptextures;
    }
    Instance *presidences = env->localvar_getRefer(runtime, pos++);
    int offset_presidences = env->localvar_getInt(runtime, pos++);
    __refer ptr_presidences = NULL;
    if(presidences){
        ptr_presidences = presidences->arr_body + offset_presidences;
    }

    GLboolean _re_val = glAreTexturesResident((GLsizei)pn, (const GLuint*)(ptr_ptextures), (GLboolean*)(ptr_presidences));
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glPrioritizeTextures(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pn = env->localvar_getInt(runtime, pos++);
    Instance *ptextures = env->localvar_getRefer(runtime, pos++);
    int offset_ptextures = env->localvar_getInt(runtime, pos++);
    __refer ptr_ptextures = NULL;
    if(ptextures){
        ptr_ptextures = ptextures->arr_body + offset_ptextures;
    }
    Instance *ppriorities = env->localvar_getRefer(runtime, pos++);
    int offset_ppriorities = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppriorities = NULL;
    if(ppriorities){
        ptr_ppriorities = ppriorities->arr_body + offset_ppriorities;
    }

    glPrioritizeTextures((GLsizei)pn, (const GLuint*)(ptr_ptextures), (const GLfloat*)(ptr_ppriorities));
    
    
    return 0;
}

int org_mini_gl_GL_glIndexub(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pc = env->localvar_getInt(runtime, pos++);

    glIndexub((GLubyte)pc);
    
    
    return 0;
}

int org_mini_gl_GL_glIndexubv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pc = env->localvar_getRefer(runtime, pos++);
    int offset_pc = env->localvar_getInt(runtime, pos++);
    __refer ptr_pc = NULL;
    if(pc){
        ptr_pc = pc->arr_body + offset_pc;
    }

    glIndexubv((const GLubyte*)(ptr_pc));
    
    
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
    
    s32 pmask = env->localvar_getInt(runtime, pos++);

    glPushClientAttrib((GLbitfield)pmask);
    
    
    return 0;
}

int org_mini_gl_GL_glDrawRangeElements(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);
    s32 pstart = env->localvar_getInt(runtime, pos++);
    s32 pend = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    Instance *pindices = env->localvar_getRefer(runtime, pos++);
    int offset_pindices = env->localvar_getInt(runtime, pos++);
    __refer ptr_pindices = NULL;
    if(pindices){
        ptr_pindices = pindices->arr_body + offset_pindices;
    }

    glDrawRangeElements((GLenum)pmode, (GLuint)pstart, (GLuint)pend, (GLsizei)pcount, (GLenum)ptype, (const void*)(ptr_pindices));
    
    
    return 0;
}

int org_mini_gl_GL_glTexImage3D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pinternalformat = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);
    s32 pdepth = env->localvar_getInt(runtime, pos++);
    s32 pborder = env->localvar_getInt(runtime, pos++);
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    Instance *ppixels = env->localvar_getRefer(runtime, pos++);
    int offset_ppixels = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppixels = NULL;
    if(ppixels){
        ptr_ppixels = ppixels->arr_body + offset_ppixels;
    }

    glTexImage3D((GLenum)ptarget, (GLint)plevel, (GLint)pinternalformat, (GLsizei)pwidth, (GLsizei)pheight, (GLsizei)pdepth, (GLint)pborder, (GLenum)pformat, (GLenum)ptype, (const void*)(ptr_ppixels));
    
    
    return 0;
}

int org_mini_gl_GL_glTexSubImage3D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pxoffset = env->localvar_getInt(runtime, pos++);
    s32 pyoffset = env->localvar_getInt(runtime, pos++);
    s32 pzoffset = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);
    s32 pdepth = env->localvar_getInt(runtime, pos++);
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    Instance *ppixels = env->localvar_getRefer(runtime, pos++);
    int offset_ppixels = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppixels = NULL;
    if(ppixels){
        ptr_ppixels = ppixels->arr_body + offset_ppixels;
    }

    glTexSubImage3D((GLenum)ptarget, (GLint)plevel, (GLint)pxoffset, (GLint)pyoffset, (GLint)pzoffset, (GLsizei)pwidth, (GLsizei)pheight, (GLsizei)pdepth, (GLenum)pformat, (GLenum)ptype, (const void*)(ptr_ppixels));
    
    
    return 0;
}

int org_mini_gl_GL_glCopyTexSubImage3D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pxoffset = env->localvar_getInt(runtime, pos++);
    s32 pyoffset = env->localvar_getInt(runtime, pos++);
    s32 pzoffset = env->localvar_getInt(runtime, pos++);
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);

    glCopyTexSubImage3D((GLenum)ptarget, (GLint)plevel, (GLint)pxoffset, (GLint)pyoffset, (GLint)pzoffset, (GLint)px, (GLint)py, (GLsizei)pwidth, (GLsizei)pheight);
    
    
    return 0;
}

int org_mini_gl_GL_glActiveTexture(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptexture = env->localvar_getInt(runtime, pos++);

    glActiveTexture((GLenum)ptexture);
    
    
    return 0;
}

int org_mini_gl_GL_glSampleCoverage(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float pvalue;pvalue.i = env->localvar_getInt(runtime, pos++);
    s32 pinvert = env->localvar_getInt(runtime, pos++);

    glSampleCoverage((GLfloat)pvalue.f, (GLboolean)pinvert);
    
    
    return 0;
}

int org_mini_gl_GL_glCompressedTexImage3D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pinternalformat = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);
    s32 pdepth = env->localvar_getInt(runtime, pos++);
    s32 pborder = env->localvar_getInt(runtime, pos++);
    s32 pimageSize = env->localvar_getInt(runtime, pos++);
    Instance *pdata = env->localvar_getRefer(runtime, pos++);
    int offset_pdata = env->localvar_getInt(runtime, pos++);
    __refer ptr_pdata = NULL;
    if(pdata){
        ptr_pdata = pdata->arr_body + offset_pdata;
    }

    glCompressedTexImage3D((GLenum)ptarget, (GLint)plevel, (GLenum)pinternalformat, (GLsizei)pwidth, (GLsizei)pheight, (GLsizei)pdepth, (GLint)pborder, (GLsizei)pimageSize, (const void*)(ptr_pdata));
    
    
    return 0;
}

int org_mini_gl_GL_glCompressedTexImage2D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pinternalformat = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);
    s32 pborder = env->localvar_getInt(runtime, pos++);
    s32 pimageSize = env->localvar_getInt(runtime, pos++);
    Instance *pdata = env->localvar_getRefer(runtime, pos++);
    int offset_pdata = env->localvar_getInt(runtime, pos++);
    __refer ptr_pdata = NULL;
    if(pdata){
        ptr_pdata = pdata->arr_body + offset_pdata;
    }

    glCompressedTexImage2D((GLenum)ptarget, (GLint)plevel, (GLenum)pinternalformat, (GLsizei)pwidth, (GLsizei)pheight, (GLint)pborder, (GLsizei)pimageSize, (const void*)(ptr_pdata));
    
    
    return 0;
}

int org_mini_gl_GL_glCompressedTexImage1D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pinternalformat = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pborder = env->localvar_getInt(runtime, pos++);
    s32 pimageSize = env->localvar_getInt(runtime, pos++);
    Instance *pdata = env->localvar_getRefer(runtime, pos++);
    int offset_pdata = env->localvar_getInt(runtime, pos++);
    __refer ptr_pdata = NULL;
    if(pdata){
        ptr_pdata = pdata->arr_body + offset_pdata;
    }

    glCompressedTexImage1D((GLenum)ptarget, (GLint)plevel, (GLenum)pinternalformat, (GLsizei)pwidth, (GLint)pborder, (GLsizei)pimageSize, (const void*)(ptr_pdata));
    
    
    return 0;
}

int org_mini_gl_GL_glCompressedTexSubImage3D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pxoffset = env->localvar_getInt(runtime, pos++);
    s32 pyoffset = env->localvar_getInt(runtime, pos++);
    s32 pzoffset = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);
    s32 pdepth = env->localvar_getInt(runtime, pos++);
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 pimageSize = env->localvar_getInt(runtime, pos++);
    Instance *pdata = env->localvar_getRefer(runtime, pos++);
    int offset_pdata = env->localvar_getInt(runtime, pos++);
    __refer ptr_pdata = NULL;
    if(pdata){
        ptr_pdata = pdata->arr_body + offset_pdata;
    }

    glCompressedTexSubImage3D((GLenum)ptarget, (GLint)plevel, (GLint)pxoffset, (GLint)pyoffset, (GLint)pzoffset, (GLsizei)pwidth, (GLsizei)pheight, (GLsizei)pdepth, (GLenum)pformat, (GLsizei)pimageSize, (const void*)(ptr_pdata));
    
    
    return 0;
}

int org_mini_gl_GL_glCompressedTexSubImage2D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pxoffset = env->localvar_getInt(runtime, pos++);
    s32 pyoffset = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 pimageSize = env->localvar_getInt(runtime, pos++);
    Instance *pdata = env->localvar_getRefer(runtime, pos++);
    int offset_pdata = env->localvar_getInt(runtime, pos++);
    __refer ptr_pdata = NULL;
    if(pdata){
        ptr_pdata = pdata->arr_body + offset_pdata;
    }

    glCompressedTexSubImage2D((GLenum)ptarget, (GLint)plevel, (GLint)pxoffset, (GLint)pyoffset, (GLsizei)pwidth, (GLsizei)pheight, (GLenum)pformat, (GLsizei)pimageSize, (const void*)(ptr_pdata));
    
    
    return 0;
}

int org_mini_gl_GL_glCompressedTexSubImage1D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pxoffset = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 pimageSize = env->localvar_getInt(runtime, pos++);
    Instance *pdata = env->localvar_getRefer(runtime, pos++);
    int offset_pdata = env->localvar_getInt(runtime, pos++);
    __refer ptr_pdata = NULL;
    if(pdata){
        ptr_pdata = pdata->arr_body + offset_pdata;
    }

    glCompressedTexSubImage1D((GLenum)ptarget, (GLint)plevel, (GLint)pxoffset, (GLsizei)pwidth, (GLenum)pformat, (GLsizei)pimageSize, (const void*)(ptr_pdata));
    
    
    return 0;
}

int org_mini_gl_GL_glGetCompressedTexImage(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    Instance *pimg = env->localvar_getRefer(runtime, pos++);
    int offset_pimg = env->localvar_getInt(runtime, pos++);
    __refer ptr_pimg = NULL;
    if(pimg){
        ptr_pimg = pimg->arr_body + offset_pimg;
    }

    glGetCompressedTexImage((GLenum)ptarget, (GLint)plevel, (void*)(ptr_pimg));
    
    
    return 0;
}

int org_mini_gl_GL_glClientActiveTexture(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptexture = env->localvar_getInt(runtime, pos++);

    glClientActiveTexture((GLenum)ptexture);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord1d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Long2Double ps;ps.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glMultiTexCoord1d((GLenum)ptarget, (GLdouble)ps.d);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord1dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glMultiTexCoord1dv((GLenum)ptarget, (const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord1f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Int2Float ps;ps.i = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord1f((GLenum)ptarget, (GLfloat)ps.f);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord1fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glMultiTexCoord1fv((GLenum)ptarget, (const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord1i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ps = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord1i((GLenum)ptarget, (GLint)ps);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord1iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glMultiTexCoord1iv((GLenum)ptarget, (const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord1s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ps = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord1s((GLenum)ptarget, (GLshort)ps);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord1sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glMultiTexCoord1sv((GLenum)ptarget, (const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord2d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Long2Double ps;ps.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pt;pt.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glMultiTexCoord2d((GLenum)ptarget, (GLdouble)ps.d, (GLdouble)pt.d);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord2dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glMultiTexCoord2dv((GLenum)ptarget, (const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Int2Float ps;ps.i = env->localvar_getInt(runtime, pos++);
    Int2Float pt;pt.i = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord2f((GLenum)ptarget, (GLfloat)ps.f, (GLfloat)pt.f);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glMultiTexCoord2fv((GLenum)ptarget, (const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord2i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ps = env->localvar_getInt(runtime, pos++);
    s32 pt = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord2i((GLenum)ptarget, (GLint)ps, (GLint)pt);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord2iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glMultiTexCoord2iv((GLenum)ptarget, (const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord2s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ps = env->localvar_getInt(runtime, pos++);
    s32 pt = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord2s((GLenum)ptarget, (GLshort)ps, (GLshort)pt);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord2sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glMultiTexCoord2sv((GLenum)ptarget, (const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord3d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Long2Double ps;ps.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pt;pt.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pr;pr.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glMultiTexCoord3d((GLenum)ptarget, (GLdouble)ps.d, (GLdouble)pt.d, (GLdouble)pr.d);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord3dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glMultiTexCoord3dv((GLenum)ptarget, (const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Int2Float ps;ps.i = env->localvar_getInt(runtime, pos++);
    Int2Float pt;pt.i = env->localvar_getInt(runtime, pos++);
    Int2Float pr;pr.i = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord3f((GLenum)ptarget, (GLfloat)ps.f, (GLfloat)pt.f, (GLfloat)pr.f);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glMultiTexCoord3fv((GLenum)ptarget, (const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ps = env->localvar_getInt(runtime, pos++);
    s32 pt = env->localvar_getInt(runtime, pos++);
    s32 pr = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord3i((GLenum)ptarget, (GLint)ps, (GLint)pt, (GLint)pr);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glMultiTexCoord3iv((GLenum)ptarget, (const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord3s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ps = env->localvar_getInt(runtime, pos++);
    s32 pt = env->localvar_getInt(runtime, pos++);
    s32 pr = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord3s((GLenum)ptarget, (GLshort)ps, (GLshort)pt, (GLshort)pr);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord3sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glMultiTexCoord3sv((GLenum)ptarget, (const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord4d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Long2Double ps;ps.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pt;pt.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pr;pr.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pq;pq.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glMultiTexCoord4d((GLenum)ptarget, (GLdouble)ps.d, (GLdouble)pt.d, (GLdouble)pr.d, (GLdouble)pq.d);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord4dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glMultiTexCoord4dv((GLenum)ptarget, (const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord4f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Int2Float ps;ps.i = env->localvar_getInt(runtime, pos++);
    Int2Float pt;pt.i = env->localvar_getInt(runtime, pos++);
    Int2Float pr;pr.i = env->localvar_getInt(runtime, pos++);
    Int2Float pq;pq.i = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord4f((GLenum)ptarget, (GLfloat)ps.f, (GLfloat)pt.f, (GLfloat)pr.f, (GLfloat)pq.f);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glMultiTexCoord4fv((GLenum)ptarget, (const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord4i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ps = env->localvar_getInt(runtime, pos++);
    s32 pt = env->localvar_getInt(runtime, pos++);
    s32 pr = env->localvar_getInt(runtime, pos++);
    s32 pq = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord4i((GLenum)ptarget, (GLint)ps, (GLint)pt, (GLint)pr, (GLint)pq);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord4iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glMultiTexCoord4iv((GLenum)ptarget, (const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord4s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ps = env->localvar_getInt(runtime, pos++);
    s32 pt = env->localvar_getInt(runtime, pos++);
    s32 pr = env->localvar_getInt(runtime, pos++);
    s32 pq = env->localvar_getInt(runtime, pos++);

    glMultiTexCoord4s((GLenum)ptarget, (GLshort)ps, (GLshort)pt, (GLshort)pr, (GLshort)pq);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiTexCoord4sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glMultiTexCoord4sv((GLenum)ptarget, (const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glLoadTransposeMatrixf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pm = env->localvar_getRefer(runtime, pos++);
    int offset_pm = env->localvar_getInt(runtime, pos++);
    __refer ptr_pm = NULL;
    if(pm){
        ptr_pm = pm->arr_body + offset_pm;
    }

    glLoadTransposeMatrixf((const GLfloat*)(ptr_pm));
    
    
    return 0;
}

int org_mini_gl_GL_glLoadTransposeMatrixd(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pm = env->localvar_getRefer(runtime, pos++);
    int offset_pm = env->localvar_getInt(runtime, pos++);
    __refer ptr_pm = NULL;
    if(pm){
        ptr_pm = pm->arr_body + offset_pm;
    }

    glLoadTransposeMatrixd((const GLdouble*)(ptr_pm));
    
    
    return 0;
}

int org_mini_gl_GL_glMultTransposeMatrixf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pm = env->localvar_getRefer(runtime, pos++);
    int offset_pm = env->localvar_getInt(runtime, pos++);
    __refer ptr_pm = NULL;
    if(pm){
        ptr_pm = pm->arr_body + offset_pm;
    }

    glMultTransposeMatrixf((const GLfloat*)(ptr_pm));
    
    
    return 0;
}

int org_mini_gl_GL_glMultTransposeMatrixd(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pm = env->localvar_getRefer(runtime, pos++);
    int offset_pm = env->localvar_getInt(runtime, pos++);
    __refer ptr_pm = NULL;
    if(pm){
        ptr_pm = pm->arr_body + offset_pm;
    }

    glMultTransposeMatrixd((const GLdouble*)(ptr_pm));
    
    
    return 0;
}

int org_mini_gl_GL_glBlendFuncSeparate(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 psfactorRGB = env->localvar_getInt(runtime, pos++);
    s32 pdfactorRGB = env->localvar_getInt(runtime, pos++);
    s32 psfactorAlpha = env->localvar_getInt(runtime, pos++);
    s32 pdfactorAlpha = env->localvar_getInt(runtime, pos++);

    glBlendFuncSeparate((GLenum)psfactorRGB, (GLenum)pdfactorRGB, (GLenum)psfactorAlpha, (GLenum)pdfactorAlpha);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiDrawArrays(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);
    Instance *pfirst = env->localvar_getRefer(runtime, pos++);
    int offset_pfirst = env->localvar_getInt(runtime, pos++);
    __refer ptr_pfirst = NULL;
    if(pfirst){
        ptr_pfirst = pfirst->arr_body + offset_pfirst;
    }
    Instance *pcount = env->localvar_getRefer(runtime, pos++);
    int offset_pcount = env->localvar_getInt(runtime, pos++);
    __refer ptr_pcount = NULL;
    if(pcount){
        ptr_pcount = pcount->arr_body + offset_pcount;
    }
    s32 pdrawcount = env->localvar_getInt(runtime, pos++);

    glMultiDrawArrays((GLenum)pmode, (const GLint*)(ptr_pfirst), (const GLsizei*)(ptr_pcount), (GLsizei)pdrawcount);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiDrawElements(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);
    Instance *pcount = env->localvar_getRefer(runtime, pos++);
    int offset_pcount = env->localvar_getInt(runtime, pos++);
    __refer ptr_pcount = NULL;
    if(pcount){
        ptr_pcount = pcount->arr_body + offset_pcount;
    }
    s32 ptype = env->localvar_getInt(runtime, pos++);
    Instance *pindices = env->localvar_getRefer(runtime, pos++);
    ReferArr *ptr_pindices = NULL;
    if(pindices){
        ptr_pindices = env->referarr_create(pindices);
    }
    s32 pdrawcount = env->localvar_getInt(runtime, pos++);

    glMultiDrawElements((GLenum)pmode, (const GLsizei*)(ptr_pcount), (GLenum)ptype, (const void*const*)(ptr_pindices->arr_body), (GLsizei)pdrawcount);
    
    env->referarr_destory(ptr_pindices);
    return 0;
}

int org_mini_gl_GL_glPointParameterf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Int2Float pparam;pparam.i = env->localvar_getInt(runtime, pos++);

    glPointParameterf((GLenum)ppname, (GLfloat)pparam.f);
    
    
    return 0;
}

int org_mini_gl_GL_glPointParameterfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glPointParameterfv((GLenum)ppname, (const GLfloat*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glPointParameteri(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    s32 pparam = env->localvar_getInt(runtime, pos++);

    glPointParameteri((GLenum)ppname, (GLint)pparam);
    
    
    return 0;
}

int org_mini_gl_GL_glPointParameteriv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glPointParameteriv((GLenum)ppname, (const GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glFogCoordf(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float pcoord;pcoord.i = env->localvar_getInt(runtime, pos++);

    glFogCoordf((GLfloat)pcoord.f);
    
    
    return 0;
}

int org_mini_gl_GL_glFogCoordfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pcoord = env->localvar_getRefer(runtime, pos++);
    int offset_pcoord = env->localvar_getInt(runtime, pos++);
    __refer ptr_pcoord = NULL;
    if(pcoord){
        ptr_pcoord = pcoord->arr_body + offset_pcoord;
    }

    glFogCoordfv((const GLfloat*)(ptr_pcoord));
    
    
    return 0;
}

int org_mini_gl_GL_glFogCoordd(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double pcoord;pcoord.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glFogCoordd((GLdouble)pcoord.d);
    
    
    return 0;
}

int org_mini_gl_GL_glFogCoorddv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pcoord = env->localvar_getRefer(runtime, pos++);
    int offset_pcoord = env->localvar_getInt(runtime, pos++);
    __refer ptr_pcoord = NULL;
    if(pcoord){
        ptr_pcoord = pcoord->arr_body + offset_pcoord;
    }

    glFogCoorddv((const GLdouble*)(ptr_pcoord));
    
    
    return 0;
}

int org_mini_gl_GL_glFogCoordPointer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 pstride = env->localvar_getInt(runtime, pos++);
    Instance *ppointer = env->localvar_getRefer(runtime, pos++);
    int offset_ppointer = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppointer = NULL;
    if(ppointer){
        ptr_ppointer = ppointer->arr_body + offset_ppointer;
    }

    glFogCoordPointer((GLenum)ptype, (GLsizei)pstride, (const void*)(ptr_ppointer));
    
    
    return 0;
}

int org_mini_gl_GL_glSecondaryColor3b(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pred = env->localvar_getInt(runtime, pos++);
    s32 pgreen = env->localvar_getInt(runtime, pos++);
    s32 pblue = env->localvar_getInt(runtime, pos++);

    glSecondaryColor3b((GLbyte)pred, (GLbyte)pgreen, (GLbyte)pblue);
    
    
    return 0;
}

int org_mini_gl_GL_glSecondaryColor3bv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glSecondaryColor3bv((const GLbyte*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glSecondaryColor3d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double pred;pred.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pgreen;pgreen.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pblue;pblue.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glSecondaryColor3d((GLdouble)pred.d, (GLdouble)pgreen.d, (GLdouble)pblue.d);
    
    
    return 0;
}

int org_mini_gl_GL_glSecondaryColor3dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glSecondaryColor3dv((const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glSecondaryColor3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float pred;pred.i = env->localvar_getInt(runtime, pos++);
    Int2Float pgreen;pgreen.i = env->localvar_getInt(runtime, pos++);
    Int2Float pblue;pblue.i = env->localvar_getInt(runtime, pos++);

    glSecondaryColor3f((GLfloat)pred.f, (GLfloat)pgreen.f, (GLfloat)pblue.f);
    
    
    return 0;
}

int org_mini_gl_GL_glSecondaryColor3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glSecondaryColor3fv((const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glSecondaryColor3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pred = env->localvar_getInt(runtime, pos++);
    s32 pgreen = env->localvar_getInt(runtime, pos++);
    s32 pblue = env->localvar_getInt(runtime, pos++);

    glSecondaryColor3i((GLint)pred, (GLint)pgreen, (GLint)pblue);
    
    
    return 0;
}

int org_mini_gl_GL_glSecondaryColor3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glSecondaryColor3iv((const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glSecondaryColor3s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pred = env->localvar_getInt(runtime, pos++);
    s32 pgreen = env->localvar_getInt(runtime, pos++);
    s32 pblue = env->localvar_getInt(runtime, pos++);

    glSecondaryColor3s((GLshort)pred, (GLshort)pgreen, (GLshort)pblue);
    
    
    return 0;
}

int org_mini_gl_GL_glSecondaryColor3sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glSecondaryColor3sv((const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glSecondaryColor3ub(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pred = env->localvar_getInt(runtime, pos++);
    s32 pgreen = env->localvar_getInt(runtime, pos++);
    s32 pblue = env->localvar_getInt(runtime, pos++);

    glSecondaryColor3ub((GLubyte)pred, (GLubyte)pgreen, (GLubyte)pblue);
    
    
    return 0;
}

int org_mini_gl_GL_glSecondaryColor3ubv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glSecondaryColor3ubv((const GLubyte*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glSecondaryColor3ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pred = env->localvar_getInt(runtime, pos++);
    s32 pgreen = env->localvar_getInt(runtime, pos++);
    s32 pblue = env->localvar_getInt(runtime, pos++);

    glSecondaryColor3ui((GLuint)pred, (GLuint)pgreen, (GLuint)pblue);
    
    
    return 0;
}

int org_mini_gl_GL_glSecondaryColor3uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glSecondaryColor3uiv((const GLuint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glSecondaryColor3us(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pred = env->localvar_getInt(runtime, pos++);
    s32 pgreen = env->localvar_getInt(runtime, pos++);
    s32 pblue = env->localvar_getInt(runtime, pos++);

    glSecondaryColor3us((GLushort)pred, (GLushort)pgreen, (GLushort)pblue);
    
    
    return 0;
}

int org_mini_gl_GL_glSecondaryColor3usv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glSecondaryColor3usv((const GLushort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glSecondaryColorPointer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 psize = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 pstride = env->localvar_getInt(runtime, pos++);
    Instance *ppointer = env->localvar_getRefer(runtime, pos++);
    int offset_ppointer = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppointer = NULL;
    if(ppointer){
        ptr_ppointer = ppointer->arr_body + offset_ppointer;
    }

    glSecondaryColorPointer((GLint)psize, (GLenum)ptype, (GLsizei)pstride, (const void*)(ptr_ppointer));
    
    
    return 0;
}

int org_mini_gl_GL_glWindowPos2d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double px;px.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double py;py.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glWindowPos2d((GLdouble)px.d, (GLdouble)py.d);
    
    
    return 0;
}

int org_mini_gl_GL_glWindowPos2dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glWindowPos2dv((const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glWindowPos2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float px;px.i = env->localvar_getInt(runtime, pos++);
    Int2Float py;py.i = env->localvar_getInt(runtime, pos++);

    glWindowPos2f((GLfloat)px.f, (GLfloat)py.f);
    
    
    return 0;
}

int org_mini_gl_GL_glWindowPos2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glWindowPos2fv((const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glWindowPos2i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);

    glWindowPos2i((GLint)px, (GLint)py);
    
    
    return 0;
}

int org_mini_gl_GL_glWindowPos2iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glWindowPos2iv((const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glWindowPos2s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);

    glWindowPos2s((GLshort)px, (GLshort)py);
    
    
    return 0;
}

int org_mini_gl_GL_glWindowPos2sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glWindowPos2sv((const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glWindowPos3d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Long2Double px;px.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double py;py.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pz;pz.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glWindowPos3d((GLdouble)px.d, (GLdouble)py.d, (GLdouble)pz.d);
    
    
    return 0;
}

int org_mini_gl_GL_glWindowPos3dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glWindowPos3dv((const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glWindowPos3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float px;px.i = env->localvar_getInt(runtime, pos++);
    Int2Float py;py.i = env->localvar_getInt(runtime, pos++);
    Int2Float pz;pz.i = env->localvar_getInt(runtime, pos++);

    glWindowPos3f((GLfloat)px.f, (GLfloat)py.f, (GLfloat)pz.f);
    
    
    return 0;
}

int org_mini_gl_GL_glWindowPos3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glWindowPos3fv((const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glWindowPos3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pz = env->localvar_getInt(runtime, pos++);

    glWindowPos3i((GLint)px, (GLint)py, (GLint)pz);
    
    
    return 0;
}

int org_mini_gl_GL_glWindowPos3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glWindowPos3iv((const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glWindowPos3s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pz = env->localvar_getInt(runtime, pos++);

    glWindowPos3s((GLshort)px, (GLshort)py, (GLshort)pz);
    
    
    return 0;
}

int org_mini_gl_GL_glWindowPos3sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glWindowPos3sv((const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glBlendColor(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float pred;pred.i = env->localvar_getInt(runtime, pos++);
    Int2Float pgreen;pgreen.i = env->localvar_getInt(runtime, pos++);
    Int2Float pblue;pblue.i = env->localvar_getInt(runtime, pos++);
    Int2Float palpha;palpha.i = env->localvar_getInt(runtime, pos++);

    glBlendColor((GLfloat)pred.f, (GLfloat)pgreen.f, (GLfloat)pblue.f, (GLfloat)palpha.f);
    
    
    return 0;
}

int org_mini_gl_GL_glBlendEquation(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);

    glBlendEquation((GLenum)pmode);
    
    
    return 0;
}

int org_mini_gl_GL_glGenQueries(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pn = env->localvar_getInt(runtime, pos++);
    Instance *pids = env->localvar_getRefer(runtime, pos++);
    int offset_pids = env->localvar_getInt(runtime, pos++);
    __refer ptr_pids = NULL;
    if(pids){
        ptr_pids = pids->arr_body + offset_pids;
    }

    glGenQueries((GLsizei)pn, (GLuint*)(ptr_pids));
    
    
    return 0;
}

int org_mini_gl_GL_glDeleteQueries(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pn = env->localvar_getInt(runtime, pos++);
    Instance *pids = env->localvar_getRefer(runtime, pos++);
    int offset_pids = env->localvar_getInt(runtime, pos++);
    __refer ptr_pids = NULL;
    if(pids){
        ptr_pids = pids->arr_body + offset_pids;
    }

    glDeleteQueries((GLsizei)pn, (const GLuint*)(ptr_pids));
    
    
    return 0;
}

int org_mini_gl_GL_glIsQuery(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pid = env->localvar_getInt(runtime, pos++);

    GLboolean _re_val = glIsQuery((GLuint)pid);
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glBeginQuery(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pid = env->localvar_getInt(runtime, pos++);

    glBeginQuery((GLenum)ptarget, (GLuint)pid);
    
    
    return 0;
}

int org_mini_gl_GL_glEndQuery(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);

    glEndQuery((GLenum)ptarget);
    
    
    return 0;
}

int org_mini_gl_GL_glGetQueryiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetQueryiv((GLenum)ptarget, (GLenum)ppname, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetQueryObjectiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pid = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetQueryObjectiv((GLuint)pid, (GLenum)ppname, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetQueryObjectuiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pid = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetQueryObjectuiv((GLuint)pid, (GLenum)ppname, (GLuint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glBindBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pbuffer = env->localvar_getInt(runtime, pos++);

    glBindBuffer((GLenum)ptarget, (GLuint)pbuffer);
    
    
    return 0;
}

int org_mini_gl_GL_glDeleteBuffers(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pn = env->localvar_getInt(runtime, pos++);
    Instance *pbuffers = env->localvar_getRefer(runtime, pos++);
    int offset_pbuffers = env->localvar_getInt(runtime, pos++);
    __refer ptr_pbuffers = NULL;
    if(pbuffers){
        ptr_pbuffers = pbuffers->arr_body + offset_pbuffers;
    }

    glDeleteBuffers((GLsizei)pn, (const GLuint*)(ptr_pbuffers));
    
    
    return 0;
}

int org_mini_gl_GL_glGenBuffers(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pn = env->localvar_getInt(runtime, pos++);
    Instance *pbuffers = env->localvar_getRefer(runtime, pos++);
    int offset_pbuffers = env->localvar_getInt(runtime, pos++);
    __refer ptr_pbuffers = NULL;
    if(pbuffers){
        ptr_pbuffers = pbuffers->arr_body + offset_pbuffers;
    }

    glGenBuffers((GLsizei)pn, (GLuint*)(ptr_pbuffers));
    
    
    return 0;
}

int org_mini_gl_GL_glIsBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pbuffer = env->localvar_getInt(runtime, pos++);

    GLboolean _re_val = glIsBuffer((GLuint)pbuffer);
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glBufferData(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    intptr_t psize = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Instance *pdata = env->localvar_getRefer(runtime, pos++);
    int offset_pdata = env->localvar_getInt(runtime, pos++);
    __refer ptr_pdata = NULL;
    if(pdata){
        ptr_pdata = pdata->arr_body + offset_pdata;
    }
    s32 pusage = env->localvar_getInt(runtime, pos++);

    glBufferData((GLenum)ptarget, (GLsizeiptr)psize, (const void*)(ptr_pdata), (GLenum)pusage);
    
    
    return 0;
}

int org_mini_gl_GL_glBufferSubData(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    intptr_t poffset = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    intptr_t psize = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Instance *pdata = env->localvar_getRefer(runtime, pos++);
    int offset_pdata = env->localvar_getInt(runtime, pos++);
    __refer ptr_pdata = NULL;
    if(pdata){
        ptr_pdata = pdata->arr_body + offset_pdata;
    }

    glBufferSubData((GLenum)ptarget, (GLintptr)poffset, (GLsizeiptr)psize, (const void*)(ptr_pdata));
    
    
    return 0;
}

int org_mini_gl_GL_glGetBufferSubData(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    intptr_t poffset = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    intptr_t psize = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Instance *pdata = env->localvar_getRefer(runtime, pos++);
    int offset_pdata = env->localvar_getInt(runtime, pos++);
    __refer ptr_pdata = NULL;
    if(pdata){
        ptr_pdata = pdata->arr_body + offset_pdata;
    }

    glGetBufferSubData((GLenum)ptarget, (GLintptr)poffset, (GLsizeiptr)psize, (void*)(ptr_pdata));
    
    
    return 0;
}

int org_mini_gl_GL_glMapBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 paccess = env->localvar_getInt(runtime, pos++);

    void* _re_val = glMapBuffer((GLenum)ptarget, (GLenum)paccess);
    s64 ret_value = *((s64*)&_re_val);env->push_long(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glUnmapBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);

    GLboolean _re_val = glUnmapBuffer((GLenum)ptarget);
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glGetBufferParameteriv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetBufferParameteriv((GLenum)ptarget, (GLenum)ppname, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetBufferPointerv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    ReferArr *ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = env->referarr_create(pparams);
    }

    glGetBufferPointerv((GLenum)ptarget, (GLenum)ppname, (void**)(ptr_pparams->arr_body));
    
    env->referarr_destory(ptr_pparams);
    return 0;
}

int org_mini_gl_GL_glBlendEquationSeparate(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmodeRGB = env->localvar_getInt(runtime, pos++);
    s32 pmodeAlpha = env->localvar_getInt(runtime, pos++);

    glBlendEquationSeparate((GLenum)pmodeRGB, (GLenum)pmodeAlpha);
    
    
    return 0;
}

int org_mini_gl_GL_glDrawBuffers(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pn = env->localvar_getInt(runtime, pos++);
    Instance *pbufs = env->localvar_getRefer(runtime, pos++);
    int offset_pbufs = env->localvar_getInt(runtime, pos++);
    __refer ptr_pbufs = NULL;
    if(pbufs){
        ptr_pbufs = pbufs->arr_body + offset_pbufs;
    }

    glDrawBuffers((GLsizei)pn, (const GLenum*)(ptr_pbufs));
    
    
    return 0;
}

int org_mini_gl_GL_glStencilOpSeparate(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pface = env->localvar_getInt(runtime, pos++);
    s32 psfail = env->localvar_getInt(runtime, pos++);
    s32 pdpfail = env->localvar_getInt(runtime, pos++);
    s32 pdppass = env->localvar_getInt(runtime, pos++);

    glStencilOpSeparate((GLenum)pface, (GLenum)psfail, (GLenum)pdpfail, (GLenum)pdppass);
    
    
    return 0;
}

int org_mini_gl_GL_glStencilFuncSeparate(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pface = env->localvar_getInt(runtime, pos++);
    s32 pfunc = env->localvar_getInt(runtime, pos++);
    s32 pref = env->localvar_getInt(runtime, pos++);
    s32 pmask = env->localvar_getInt(runtime, pos++);

    glStencilFuncSeparate((GLenum)pface, (GLenum)pfunc, (GLint)pref, (GLuint)pmask);
    
    
    return 0;
}

int org_mini_gl_GL_glStencilMaskSeparate(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pface = env->localvar_getInt(runtime, pos++);
    s32 pmask = env->localvar_getInt(runtime, pos++);

    glStencilMaskSeparate((GLenum)pface, (GLuint)pmask);
    
    
    return 0;
}

int org_mini_gl_GL_glAttachShader(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 pshader = env->localvar_getInt(runtime, pos++);

    glAttachShader((GLuint)pprogram, (GLuint)pshader);
    
    
    return 0;
}

int org_mini_gl_GL_glBindAttribLocation(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pname = env->localvar_getRefer(runtime, pos++);
    __refer ptr_pname = NULL;
    Utf8String *u_pname;
    if(pname){
        u_pname = env->utf8_create();
        env->jstring_2_utf8(pname, u_pname);
        ptr_pname = env->utf8_cstr(u_pname);
    }

    glBindAttribLocation((GLuint)pprogram, (GLuint)pindex, (const GLchar*)(ptr_pname));
    
    env->utf8_destory(u_pname);
    return 0;
}

int org_mini_gl_GL_glCompileShader(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pshader = env->localvar_getInt(runtime, pos++);

    glCompileShader((GLuint)pshader);
    
    
    return 0;
}

int org_mini_gl_GL_glCreateProgram(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    

    GLuint _re_val = glCreateProgram();
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glCreateShader(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptype = env->localvar_getInt(runtime, pos++);

    GLuint _re_val = glCreateShader((GLenum)ptype);
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glDeleteProgram(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);

    glDeleteProgram((GLuint)pprogram);
    
    
    return 0;
}

int org_mini_gl_GL_glDeleteShader(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pshader = env->localvar_getInt(runtime, pos++);

    glDeleteShader((GLuint)pshader);
    
    
    return 0;
}

int org_mini_gl_GL_glDetachShader(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 pshader = env->localvar_getInt(runtime, pos++);

    glDetachShader((GLuint)pprogram, (GLuint)pshader);
    
    
    return 0;
}

int org_mini_gl_GL_glDisableVertexAttribArray(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);

    glDisableVertexAttribArray((GLuint)pindex);
    
    
    return 0;
}

int org_mini_gl_GL_glEnableVertexAttribArray(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);

    glEnableVertexAttribArray((GLuint)pindex);
    
    
    return 0;
}

int org_mini_gl_GL_glGetActiveAttrib(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *plength = env->localvar_getRefer(runtime, pos++);
    int offset_plength = env->localvar_getInt(runtime, pos++);
    __refer ptr_plength = NULL;
    if(plength){
        ptr_plength = plength->arr_body + offset_plength;
    }
    Instance *psize = env->localvar_getRefer(runtime, pos++);
    int offset_psize = env->localvar_getInt(runtime, pos++);
    __refer ptr_psize = NULL;
    if(psize){
        ptr_psize = psize->arr_body + offset_psize;
    }
    Instance *ptype = env->localvar_getRefer(runtime, pos++);
    int offset_ptype = env->localvar_getInt(runtime, pos++);
    __refer ptr_ptype = NULL;
    if(ptype){
        ptr_ptype = ptype->arr_body + offset_ptype;
    }
    Instance *pname = env->localvar_getRefer(runtime, pos++);
    __refer ptr_pname = NULL;
    Utf8String *u_pname;
    if(pname){
        u_pname = env->utf8_create();
        env->jstring_2_utf8(pname, u_pname);
        ptr_pname = env->utf8_cstr(u_pname);
    }

    glGetActiveAttrib((GLuint)pprogram, (GLuint)pindex, (GLsizei)pbufSize, (GLsizei*)(ptr_plength), (GLint*)(ptr_psize), (GLenum*)(ptr_ptype), (GLchar*)(ptr_pname));
    
    env->utf8_destory(u_pname);
    return 0;
}

int org_mini_gl_GL_glGetActiveUniform(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *plength = env->localvar_getRefer(runtime, pos++);
    int offset_plength = env->localvar_getInt(runtime, pos++);
    __refer ptr_plength = NULL;
    if(plength){
        ptr_plength = plength->arr_body + offset_plength;
    }
    Instance *psize = env->localvar_getRefer(runtime, pos++);
    int offset_psize = env->localvar_getInt(runtime, pos++);
    __refer ptr_psize = NULL;
    if(psize){
        ptr_psize = psize->arr_body + offset_psize;
    }
    Instance *ptype = env->localvar_getRefer(runtime, pos++);
    int offset_ptype = env->localvar_getInt(runtime, pos++);
    __refer ptr_ptype = NULL;
    if(ptype){
        ptr_ptype = ptype->arr_body + offset_ptype;
    }
    Instance *pname = env->localvar_getRefer(runtime, pos++);
    __refer ptr_pname = NULL;
    Utf8String *u_pname;
    if(pname){
        u_pname = env->utf8_create();
        env->jstring_2_utf8(pname, u_pname);
        ptr_pname = env->utf8_cstr(u_pname);
    }

    glGetActiveUniform((GLuint)pprogram, (GLuint)pindex, (GLsizei)pbufSize, (GLsizei*)(ptr_plength), (GLint*)(ptr_psize), (GLenum*)(ptr_ptype), (GLchar*)(ptr_pname));
    
    env->utf8_destory(u_pname);
    return 0;
}

int org_mini_gl_GL_glGetAttachedShaders(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 pmaxCount = env->localvar_getInt(runtime, pos++);
    Instance *pcount = env->localvar_getRefer(runtime, pos++);
    int offset_pcount = env->localvar_getInt(runtime, pos++);
    __refer ptr_pcount = NULL;
    if(pcount){
        ptr_pcount = pcount->arr_body + offset_pcount;
    }
    Instance *pshaders = env->localvar_getRefer(runtime, pos++);
    int offset_pshaders = env->localvar_getInt(runtime, pos++);
    __refer ptr_pshaders = NULL;
    if(pshaders){
        ptr_pshaders = pshaders->arr_body + offset_pshaders;
    }

    glGetAttachedShaders((GLuint)pprogram, (GLsizei)pmaxCount, (GLsizei*)(ptr_pcount), (GLuint*)(ptr_pshaders));
    
    
    return 0;
}

int org_mini_gl_GL_glGetAttribLocation(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    Instance *pname = env->localvar_getRefer(runtime, pos++);
    __refer ptr_pname = NULL;
    Utf8String *u_pname;
    if(pname){
        u_pname = env->utf8_create();
        env->jstring_2_utf8(pname, u_pname);
        ptr_pname = env->utf8_cstr(u_pname);
    }

    GLint _re_val = glGetAttribLocation((GLuint)pprogram, (const GLchar*)(ptr_pname));
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    env->utf8_destory(u_pname);
    return 0;
}

int org_mini_gl_GL_glGetProgramiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetProgramiv((GLuint)pprogram, (GLenum)ppname, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetProgramInfoLog(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *plength = env->localvar_getRefer(runtime, pos++);
    int offset_plength = env->localvar_getInt(runtime, pos++);
    __refer ptr_plength = NULL;
    if(plength){
        ptr_plength = plength->arr_body + offset_plength;
    }
    Instance *pinfoLog = env->localvar_getRefer(runtime, pos++);
    __refer ptr_pinfoLog = NULL;
    Utf8String *u_pinfoLog;
    if(pinfoLog){
        u_pinfoLog = env->utf8_create();
        env->jstring_2_utf8(pinfoLog, u_pinfoLog);
        ptr_pinfoLog = env->utf8_cstr(u_pinfoLog);
    }

    glGetProgramInfoLog((GLuint)pprogram, (GLsizei)pbufSize, (GLsizei*)(ptr_plength), (GLchar*)(ptr_pinfoLog));
    
    env->utf8_destory(u_pinfoLog);
    return 0;
}

int org_mini_gl_GL_glGetShaderiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pshader = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetShaderiv((GLuint)pshader, (GLenum)ppname, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetShaderInfoLog(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pshader = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *plength = env->localvar_getRefer(runtime, pos++);
    int offset_plength = env->localvar_getInt(runtime, pos++);
    __refer ptr_plength = NULL;
    if(plength){
        ptr_plength = plength->arr_body + offset_plength;
    }
    Instance *pinfoLog = env->localvar_getRefer(runtime, pos++);
    __refer ptr_pinfoLog = NULL;
    Utf8String *u_pinfoLog;
    if(pinfoLog){
        u_pinfoLog = env->utf8_create();
        env->jstring_2_utf8(pinfoLog, u_pinfoLog);
        ptr_pinfoLog = env->utf8_cstr(u_pinfoLog);
    }

    glGetShaderInfoLog((GLuint)pshader, (GLsizei)pbufSize, (GLsizei*)(ptr_plength), (GLchar*)(ptr_pinfoLog));
    
    env->utf8_destory(u_pinfoLog);
    return 0;
}

int org_mini_gl_GL_glGetShaderSource(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pshader = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *plength = env->localvar_getRefer(runtime, pos++);
    int offset_plength = env->localvar_getInt(runtime, pos++);
    __refer ptr_plength = NULL;
    if(plength){
        ptr_plength = plength->arr_body + offset_plength;
    }
    Instance *psource = env->localvar_getRefer(runtime, pos++);
    __refer ptr_psource = NULL;
    Utf8String *u_psource;
    if(psource){
        u_psource = env->utf8_create();
        env->jstring_2_utf8(psource, u_psource);
        ptr_psource = env->utf8_cstr(u_psource);
    }

    glGetShaderSource((GLuint)pshader, (GLsizei)pbufSize, (GLsizei*)(ptr_plength), (GLchar*)(ptr_psource));
    
    env->utf8_destory(u_psource);
    return 0;
}

int org_mini_gl_GL_glGetUniformLocation(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    Instance *pname = env->localvar_getRefer(runtime, pos++);
    __refer ptr_pname = NULL;
    Utf8String *u_pname;
    if(pname){
        u_pname = env->utf8_create();
        env->jstring_2_utf8(pname, u_pname);
        ptr_pname = env->utf8_cstr(u_pname);
    }

    GLint _re_val = glGetUniformLocation((GLuint)pprogram, (const GLchar*)(ptr_pname));
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    env->utf8_destory(u_pname);
    return 0;
}

int org_mini_gl_GL_glGetUniformfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 plocation = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetUniformfv((GLuint)pprogram, (GLint)plocation, (GLfloat*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetUniformiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 plocation = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetUniformiv((GLuint)pprogram, (GLint)plocation, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetVertexAttribdv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetVertexAttribdv((GLuint)pindex, (GLenum)ppname, (GLdouble*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetVertexAttribfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetVertexAttribfv((GLuint)pindex, (GLenum)ppname, (GLfloat*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetVertexAttribiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetVertexAttribiv((GLuint)pindex, (GLenum)ppname, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetVertexAttribPointerv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *ppointer = env->localvar_getRefer(runtime, pos++);
    ReferArr *ptr_ppointer = NULL;
    if(ppointer){
        ptr_ppointer = env->referarr_create(ppointer);
    }

    glGetVertexAttribPointerv((GLuint)pindex, (GLenum)ppname, (void**)(ptr_ppointer->arr_body));
    
    env->referarr_destory(ptr_ppointer);
    return 0;
}

int org_mini_gl_GL_glIsProgram(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);

    GLboolean _re_val = glIsProgram((GLuint)pprogram);
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glIsShader(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pshader = env->localvar_getInt(runtime, pos++);

    GLboolean _re_val = glIsShader((GLuint)pshader);
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glLinkProgram(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);

    glLinkProgram((GLuint)pprogram);
    
    
    return 0;
}

int org_mini_gl_GL_glShaderSource(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pshader = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    Instance *pstring = env->localvar_getRefer(runtime, pos++);
    CStringArr *ptr_pstring = NULL;
    if(pstring){
        ptr_pstring = env->cstringarr_create(pstring);
    }
    Instance *plength = env->localvar_getRefer(runtime, pos++);
    int offset_plength = env->localvar_getInt(runtime, pos++);
    __refer ptr_plength = NULL;
    if(plength){
        ptr_plength = plength->arr_body + offset_plength;
    }

    glShaderSource((GLuint)pshader, (GLsizei)pcount, (const GLchar*const*)(ptr_pstring->arr_body), (const GLint*)(ptr_plength));
    
    env->cstringarr_destory(ptr_pstring);
    return 0;
}

int org_mini_gl_GL_glUseProgram(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);

    glUseProgram((GLuint)pprogram);
    
    
    return 0;
}

int org_mini_gl_GL_glUniform1f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    Int2Float pv0;pv0.i = env->localvar_getInt(runtime, pos++);

    glUniform1f((GLint)plocation, (GLfloat)pv0.f);
    
    
    return 0;
}

int org_mini_gl_GL_glUniform2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    Int2Float pv0;pv0.i = env->localvar_getInt(runtime, pos++);
    Int2Float pv1;pv1.i = env->localvar_getInt(runtime, pos++);

    glUniform2f((GLint)plocation, (GLfloat)pv0.f, (GLfloat)pv1.f);
    
    
    return 0;
}

int org_mini_gl_GL_glUniform3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    Int2Float pv0;pv0.i = env->localvar_getInt(runtime, pos++);
    Int2Float pv1;pv1.i = env->localvar_getInt(runtime, pos++);
    Int2Float pv2;pv2.i = env->localvar_getInt(runtime, pos++);

    glUniform3f((GLint)plocation, (GLfloat)pv0.f, (GLfloat)pv1.f, (GLfloat)pv2.f);
    
    
    return 0;
}

int org_mini_gl_GL_glUniform4f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    Int2Float pv0;pv0.i = env->localvar_getInt(runtime, pos++);
    Int2Float pv1;pv1.i = env->localvar_getInt(runtime, pos++);
    Int2Float pv2;pv2.i = env->localvar_getInt(runtime, pos++);
    Int2Float pv3;pv3.i = env->localvar_getInt(runtime, pos++);

    glUniform4f((GLint)plocation, (GLfloat)pv0.f, (GLfloat)pv1.f, (GLfloat)pv2.f, (GLfloat)pv3.f);
    
    
    return 0;
}

int org_mini_gl_GL_glUniform1i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pv0 = env->localvar_getInt(runtime, pos++);

    glUniform1i((GLint)plocation, (GLint)pv0);
    
    
    return 0;
}

int org_mini_gl_GL_glUniform2i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pv0 = env->localvar_getInt(runtime, pos++);
    s32 pv1 = env->localvar_getInt(runtime, pos++);

    glUniform2i((GLint)plocation, (GLint)pv0, (GLint)pv1);
    
    
    return 0;
}

int org_mini_gl_GL_glUniform3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pv0 = env->localvar_getInt(runtime, pos++);
    s32 pv1 = env->localvar_getInt(runtime, pos++);
    s32 pv2 = env->localvar_getInt(runtime, pos++);

    glUniform3i((GLint)plocation, (GLint)pv0, (GLint)pv1, (GLint)pv2);
    
    
    return 0;
}

int org_mini_gl_GL_glUniform4i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pv0 = env->localvar_getInt(runtime, pos++);
    s32 pv1 = env->localvar_getInt(runtime, pos++);
    s32 pv2 = env->localvar_getInt(runtime, pos++);
    s32 pv3 = env->localvar_getInt(runtime, pos++);

    glUniform4i((GLint)plocation, (GLint)pv0, (GLint)pv1, (GLint)pv2, (GLint)pv3);
    
    
    return 0;
}

int org_mini_gl_GL_glUniform1fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniform1fv((GLint)plocation, (GLsizei)pcount, (const GLfloat*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glUniform2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniform2fv((GLint)plocation, (GLsizei)pcount, (const GLfloat*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glUniform3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniform3fv((GLint)plocation, (GLsizei)pcount, (const GLfloat*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glUniform4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniform4fv((GLint)plocation, (GLsizei)pcount, (const GLfloat*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glUniform1iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniform1iv((GLint)plocation, (GLsizei)pcount, (const GLint*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glUniform2iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniform2iv((GLint)plocation, (GLsizei)pcount, (const GLint*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glUniform3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniform3iv((GLint)plocation, (GLsizei)pcount, (const GLint*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glUniform4iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniform4iv((GLint)plocation, (GLsizei)pcount, (const GLint*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glUniformMatrix2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    s32 ptranspose = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniformMatrix2fv((GLint)plocation, (GLsizei)pcount, (GLboolean)ptranspose, (const GLfloat*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glUniformMatrix3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    s32 ptranspose = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniformMatrix3fv((GLint)plocation, (GLsizei)pcount, (GLboolean)ptranspose, (const GLfloat*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glUniformMatrix4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    s32 ptranspose = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniformMatrix4fv((GLint)plocation, (GLsizei)pcount, (GLboolean)ptranspose, (const GLfloat*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glValidateProgram(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);

    glValidateProgram((GLuint)pprogram);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib1d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Long2Double px;px.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glVertexAttrib1d((GLuint)pindex, (GLdouble)px.d);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib1dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib1dv((GLuint)pindex, (const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib1f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Int2Float px;px.i = env->localvar_getInt(runtime, pos++);

    glVertexAttrib1f((GLuint)pindex, (GLfloat)px.f);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib1fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib1fv((GLuint)pindex, (const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib1s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 px = env->localvar_getInt(runtime, pos++);

    glVertexAttrib1s((GLuint)pindex, (GLshort)px);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib1sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib1sv((GLuint)pindex, (const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib2d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Long2Double px;px.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double py;py.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glVertexAttrib2d((GLuint)pindex, (GLdouble)px.d, (GLdouble)py.d);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib2dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib2dv((GLuint)pindex, (const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib2f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Int2Float px;px.i = env->localvar_getInt(runtime, pos++);
    Int2Float py;py.i = env->localvar_getInt(runtime, pos++);

    glVertexAttrib2f((GLuint)pindex, (GLfloat)px.f, (GLfloat)py.f);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib2fv((GLuint)pindex, (const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib2s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);

    glVertexAttrib2s((GLuint)pindex, (GLshort)px, (GLshort)py);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib2sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib2sv((GLuint)pindex, (const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib3d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Long2Double px;px.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double py;py.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pz;pz.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glVertexAttrib3d((GLuint)pindex, (GLdouble)px.d, (GLdouble)py.d, (GLdouble)pz.d);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib3dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib3dv((GLuint)pindex, (const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib3f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Int2Float px;px.i = env->localvar_getInt(runtime, pos++);
    Int2Float py;py.i = env->localvar_getInt(runtime, pos++);
    Int2Float pz;pz.i = env->localvar_getInt(runtime, pos++);

    glVertexAttrib3f((GLuint)pindex, (GLfloat)px.f, (GLfloat)py.f, (GLfloat)pz.f);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib3fv((GLuint)pindex, (const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib3s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pz = env->localvar_getInt(runtime, pos++);

    glVertexAttrib3s((GLuint)pindex, (GLshort)px, (GLshort)py, (GLshort)pz);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib3sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib3sv((GLuint)pindex, (const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib4Nbv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib4Nbv((GLuint)pindex, (const GLbyte*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib4Niv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib4Niv((GLuint)pindex, (const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib4Nsv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib4Nsv((GLuint)pindex, (const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib4Nub(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pz = env->localvar_getInt(runtime, pos++);
    s32 pw = env->localvar_getInt(runtime, pos++);

    glVertexAttrib4Nub((GLuint)pindex, (GLubyte)px, (GLubyte)py, (GLubyte)pz, (GLubyte)pw);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib4Nubv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib4Nubv((GLuint)pindex, (const GLubyte*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib4Nuiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib4Nuiv((GLuint)pindex, (const GLuint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib4Nusv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib4Nusv((GLuint)pindex, (const GLushort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib4bv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib4bv((GLuint)pindex, (const GLbyte*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib4d(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Long2Double px;px.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double py;py.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pz;pz.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Long2Double pw;pw.l = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glVertexAttrib4d((GLuint)pindex, (GLdouble)px.d, (GLdouble)py.d, (GLdouble)pz.d, (GLdouble)pw.d);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib4dv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib4dv((GLuint)pindex, (const GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib4f(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Int2Float px;px.i = env->localvar_getInt(runtime, pos++);
    Int2Float py;py.i = env->localvar_getInt(runtime, pos++);
    Int2Float pz;pz.i = env->localvar_getInt(runtime, pos++);
    Int2Float pw;pw.i = env->localvar_getInt(runtime, pos++);

    glVertexAttrib4f((GLuint)pindex, (GLfloat)px.f, (GLfloat)py.f, (GLfloat)pz.f, (GLfloat)pw.f);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib4fv((GLuint)pindex, (const GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib4iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib4iv((GLuint)pindex, (const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib4s(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pz = env->localvar_getInt(runtime, pos++);
    s32 pw = env->localvar_getInt(runtime, pos++);

    glVertexAttrib4s((GLuint)pindex, (GLshort)px, (GLshort)py, (GLshort)pz, (GLshort)pw);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib4sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib4sv((GLuint)pindex, (const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib4ubv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib4ubv((GLuint)pindex, (const GLubyte*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib4uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib4uiv((GLuint)pindex, (const GLuint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttrib4usv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttrib4usv((GLuint)pindex, (const GLushort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribPointer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 psize = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 pnormalized = env->localvar_getInt(runtime, pos++);
    s32 pstride = env->localvar_getInt(runtime, pos++);
    Instance *ppointer = env->localvar_getRefer(runtime, pos++);
    int offset_ppointer = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppointer = NULL;
    if(ppointer){
        ptr_ppointer = ppointer->arr_body + offset_ppointer;
    }

    glVertexAttribPointer((GLuint)pindex, (GLint)psize, (GLenum)ptype, (GLboolean)pnormalized, (GLsizei)pstride, (const void*)(ptr_ppointer));
    
    
    return 0;
}

int org_mini_gl_GL_glUniformMatrix2x3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    s32 ptranspose = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniformMatrix2x3fv((GLint)plocation, (GLsizei)pcount, (GLboolean)ptranspose, (const GLfloat*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glUniformMatrix3x2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    s32 ptranspose = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniformMatrix3x2fv((GLint)plocation, (GLsizei)pcount, (GLboolean)ptranspose, (const GLfloat*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glUniformMatrix2x4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    s32 ptranspose = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniformMatrix2x4fv((GLint)plocation, (GLsizei)pcount, (GLboolean)ptranspose, (const GLfloat*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glUniformMatrix4x2fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    s32 ptranspose = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniformMatrix4x2fv((GLint)plocation, (GLsizei)pcount, (GLboolean)ptranspose, (const GLfloat*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glUniformMatrix3x4fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    s32 ptranspose = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniformMatrix3x4fv((GLint)plocation, (GLsizei)pcount, (GLboolean)ptranspose, (const GLfloat*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glUniformMatrix4x3fv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    s32 ptranspose = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniformMatrix4x3fv((GLint)plocation, (GLsizei)pcount, (GLboolean)ptranspose, (const GLfloat*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glColorMaski(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 pr = env->localvar_getInt(runtime, pos++);
    s32 pg = env->localvar_getInt(runtime, pos++);
    s32 pb = env->localvar_getInt(runtime, pos++);
    s32 pa = env->localvar_getInt(runtime, pos++);

    glColorMaski((GLuint)pindex, (GLboolean)pr, (GLboolean)pg, (GLboolean)pb, (GLboolean)pa);
    
    
    return 0;
}

int org_mini_gl_GL_glGetBooleani_v(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pdata = env->localvar_getRefer(runtime, pos++);
    int offset_pdata = env->localvar_getInt(runtime, pos++);
    __refer ptr_pdata = NULL;
    if(pdata){
        ptr_pdata = pdata->arr_body + offset_pdata;
    }

    glGetBooleani_v((GLenum)ptarget, (GLuint)pindex, (GLboolean*)(ptr_pdata));
    
    
    return 0;
}

int org_mini_gl_GL_glGetIntegeri_v(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pdata = env->localvar_getRefer(runtime, pos++);
    int offset_pdata = env->localvar_getInt(runtime, pos++);
    __refer ptr_pdata = NULL;
    if(pdata){
        ptr_pdata = pdata->arr_body + offset_pdata;
    }

    glGetIntegeri_v((GLenum)ptarget, (GLuint)pindex, (GLint*)(ptr_pdata));
    
    
    return 0;
}

int org_mini_gl_GL_glEnablei(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pindex = env->localvar_getInt(runtime, pos++);

    glEnablei((GLenum)ptarget, (GLuint)pindex);
    
    
    return 0;
}

int org_mini_gl_GL_glDisablei(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pindex = env->localvar_getInt(runtime, pos++);

    glDisablei((GLenum)ptarget, (GLuint)pindex);
    
    
    return 0;
}

int org_mini_gl_GL_glIsEnabledi(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pindex = env->localvar_getInt(runtime, pos++);

    GLboolean _re_val = glIsEnabledi((GLenum)ptarget, (GLuint)pindex);
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glBeginTransformFeedback(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprimitiveMode = env->localvar_getInt(runtime, pos++);

    glBeginTransformFeedback((GLenum)pprimitiveMode);
    
    
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
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 pbuffer = env->localvar_getInt(runtime, pos++);
    intptr_t poffset = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    intptr_t psize = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glBindBufferRange((GLenum)ptarget, (GLuint)pindex, (GLuint)pbuffer, (GLintptr)poffset, (GLsizeiptr)psize);
    
    
    return 0;
}

int org_mini_gl_GL_glBindBufferBase(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 pbuffer = env->localvar_getInt(runtime, pos++);

    glBindBufferBase((GLenum)ptarget, (GLuint)pindex, (GLuint)pbuffer);
    
    
    return 0;
}

int org_mini_gl_GL_glTransformFeedbackVaryings(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    Instance *pvaryings = env->localvar_getRefer(runtime, pos++);
    CStringArr *ptr_pvaryings = NULL;
    if(pvaryings){
        ptr_pvaryings = env->cstringarr_create(pvaryings);
    }
    s32 pbufferMode = env->localvar_getInt(runtime, pos++);

    glTransformFeedbackVaryings((GLuint)pprogram, (GLsizei)pcount, (const GLchar*const*)(ptr_pvaryings->arr_body), (GLenum)pbufferMode);
    
    env->cstringarr_destory(ptr_pvaryings);
    return 0;
}

int org_mini_gl_GL_glGetTransformFeedbackVarying(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *plength = env->localvar_getRefer(runtime, pos++);
    int offset_plength = env->localvar_getInt(runtime, pos++);
    __refer ptr_plength = NULL;
    if(plength){
        ptr_plength = plength->arr_body + offset_plength;
    }
    Instance *psize = env->localvar_getRefer(runtime, pos++);
    int offset_psize = env->localvar_getInt(runtime, pos++);
    __refer ptr_psize = NULL;
    if(psize){
        ptr_psize = psize->arr_body + offset_psize;
    }
    Instance *ptype = env->localvar_getRefer(runtime, pos++);
    int offset_ptype = env->localvar_getInt(runtime, pos++);
    __refer ptr_ptype = NULL;
    if(ptype){
        ptr_ptype = ptype->arr_body + offset_ptype;
    }
    Instance *pname = env->localvar_getRefer(runtime, pos++);
    __refer ptr_pname = NULL;
    Utf8String *u_pname;
    if(pname){
        u_pname = env->utf8_create();
        env->jstring_2_utf8(pname, u_pname);
        ptr_pname = env->utf8_cstr(u_pname);
    }

    glGetTransformFeedbackVarying((GLuint)pprogram, (GLuint)pindex, (GLsizei)pbufSize, (GLsizei*)(ptr_plength), (GLsizei*)(ptr_psize), (GLenum*)(ptr_ptype), (GLchar*)(ptr_pname));
    
    env->utf8_destory(u_pname);
    return 0;
}

int org_mini_gl_GL_glClampColor(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pclamp = env->localvar_getInt(runtime, pos++);

    glClampColor((GLenum)ptarget, (GLenum)pclamp);
    
    
    return 0;
}

int org_mini_gl_GL_glBeginConditionalRender(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pid = env->localvar_getInt(runtime, pos++);
    s32 pmode = env->localvar_getInt(runtime, pos++);

    glBeginConditionalRender((GLuint)pid, (GLenum)pmode);
    
    
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
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 psize = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 pstride = env->localvar_getInt(runtime, pos++);
    Instance *ppointer = env->localvar_getRefer(runtime, pos++);
    int offset_ppointer = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppointer = NULL;
    if(ppointer){
        ptr_ppointer = ppointer->arr_body + offset_ppointer;
    }

    glVertexAttribIPointer((GLuint)pindex, (GLint)psize, (GLenum)ptype, (GLsizei)pstride, (const void*)(ptr_ppointer));
    
    
    return 0;
}

int org_mini_gl_GL_glGetVertexAttribIiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetVertexAttribIiv((GLuint)pindex, (GLenum)ppname, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetVertexAttribIuiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetVertexAttribIuiv((GLuint)pindex, (GLenum)ppname, (GLuint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI1i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 px = env->localvar_getInt(runtime, pos++);

    glVertexAttribI1i((GLuint)pindex, (GLint)px);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI2i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);

    glVertexAttribI2i((GLuint)pindex, (GLint)px, (GLint)py);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI3i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pz = env->localvar_getInt(runtime, pos++);

    glVertexAttribI3i((GLuint)pindex, (GLint)px, (GLint)py, (GLint)pz);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI4i(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pz = env->localvar_getInt(runtime, pos++);
    s32 pw = env->localvar_getInt(runtime, pos++);

    glVertexAttribI4i((GLuint)pindex, (GLint)px, (GLint)py, (GLint)pz, (GLint)pw);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI1ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 px = env->localvar_getInt(runtime, pos++);

    glVertexAttribI1ui((GLuint)pindex, (GLuint)px);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI2ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);

    glVertexAttribI2ui((GLuint)pindex, (GLuint)px, (GLuint)py);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI3ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pz = env->localvar_getInt(runtime, pos++);

    glVertexAttribI3ui((GLuint)pindex, (GLuint)px, (GLuint)py, (GLuint)pz);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI4ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pz = env->localvar_getInt(runtime, pos++);
    s32 pw = env->localvar_getInt(runtime, pos++);

    glVertexAttribI4ui((GLuint)pindex, (GLuint)px, (GLuint)py, (GLuint)pz, (GLuint)pw);
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI1iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttribI1iv((GLuint)pindex, (const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI2iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttribI2iv((GLuint)pindex, (const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI3iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttribI3iv((GLuint)pindex, (const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI4iv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttribI4iv((GLuint)pindex, (const GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI1uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttribI1uiv((GLuint)pindex, (const GLuint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI2uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttribI2uiv((GLuint)pindex, (const GLuint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI3uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttribI3uiv((GLuint)pindex, (const GLuint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI4uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttribI4uiv((GLuint)pindex, (const GLuint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI4bv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttribI4bv((GLuint)pindex, (const GLbyte*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI4sv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttribI4sv((GLuint)pindex, (const GLshort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI4ubv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttribI4ubv((GLuint)pindex, (const GLubyte*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glVertexAttribI4usv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glVertexAttribI4usv((GLuint)pindex, (const GLushort*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glGetUniformuiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 plocation = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetUniformuiv((GLuint)pprogram, (GLint)plocation, (GLuint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glBindFragDataLocation(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 pcolor = env->localvar_getInt(runtime, pos++);
    Instance *pname = env->localvar_getRefer(runtime, pos++);
    __refer ptr_pname = NULL;
    Utf8String *u_pname;
    if(pname){
        u_pname = env->utf8_create();
        env->jstring_2_utf8(pname, u_pname);
        ptr_pname = env->utf8_cstr(u_pname);
    }

    glBindFragDataLocation((GLuint)pprogram, (GLuint)pcolor, (const GLchar*)(ptr_pname));
    
    env->utf8_destory(u_pname);
    return 0;
}

int org_mini_gl_GL_glGetFragDataLocation(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    Instance *pname = env->localvar_getRefer(runtime, pos++);
    __refer ptr_pname = NULL;
    Utf8String *u_pname;
    if(pname){
        u_pname = env->utf8_create();
        env->jstring_2_utf8(pname, u_pname);
        ptr_pname = env->utf8_cstr(u_pname);
    }

    GLint _re_val = glGetFragDataLocation((GLuint)pprogram, (const GLchar*)(ptr_pname));
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    env->utf8_destory(u_pname);
    return 0;
}

int org_mini_gl_GL_glUniform1ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pv0 = env->localvar_getInt(runtime, pos++);

    glUniform1ui((GLint)plocation, (GLuint)pv0);
    
    
    return 0;
}

int org_mini_gl_GL_glUniform2ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pv0 = env->localvar_getInt(runtime, pos++);
    s32 pv1 = env->localvar_getInt(runtime, pos++);

    glUniform2ui((GLint)plocation, (GLuint)pv0, (GLuint)pv1);
    
    
    return 0;
}

int org_mini_gl_GL_glUniform3ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pv0 = env->localvar_getInt(runtime, pos++);
    s32 pv1 = env->localvar_getInt(runtime, pos++);
    s32 pv2 = env->localvar_getInt(runtime, pos++);

    glUniform3ui((GLint)plocation, (GLuint)pv0, (GLuint)pv1, (GLuint)pv2);
    
    
    return 0;
}

int org_mini_gl_GL_glUniform4ui(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pv0 = env->localvar_getInt(runtime, pos++);
    s32 pv1 = env->localvar_getInt(runtime, pos++);
    s32 pv2 = env->localvar_getInt(runtime, pos++);
    s32 pv3 = env->localvar_getInt(runtime, pos++);

    glUniform4ui((GLint)plocation, (GLuint)pv0, (GLuint)pv1, (GLuint)pv2, (GLuint)pv3);
    
    
    return 0;
}

int org_mini_gl_GL_glUniform1uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniform1uiv((GLint)plocation, (GLsizei)pcount, (const GLuint*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glUniform2uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniform2uiv((GLint)plocation, (GLsizei)pcount, (const GLuint*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glUniform3uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniform3uiv((GLint)plocation, (GLsizei)pcount, (const GLuint*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glUniform4uiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glUniform4uiv((GLint)plocation, (GLsizei)pcount, (const GLuint*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glTexParameterIiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glTexParameterIiv((GLenum)ptarget, (GLenum)ppname, (const GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glTexParameterIuiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glTexParameterIuiv((GLenum)ptarget, (GLenum)ppname, (const GLuint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetTexParameterIiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetTexParameterIiv((GLenum)ptarget, (GLenum)ppname, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetTexParameterIuiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetTexParameterIuiv((GLenum)ptarget, (GLenum)ppname, (GLuint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glClearBufferiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pbuffer = env->localvar_getInt(runtime, pos++);
    s32 pdrawbuffer = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glClearBufferiv((GLenum)pbuffer, (GLint)pdrawbuffer, (const GLint*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glClearBufferuiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pbuffer = env->localvar_getInt(runtime, pos++);
    s32 pdrawbuffer = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glClearBufferuiv((GLenum)pbuffer, (GLint)pdrawbuffer, (const GLuint*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glClearBufferfv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pbuffer = env->localvar_getInt(runtime, pos++);
    s32 pdrawbuffer = env->localvar_getInt(runtime, pos++);
    Instance *pvalue = env->localvar_getRefer(runtime, pos++);
    int offset_pvalue = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalue = NULL;
    if(pvalue){
        ptr_pvalue = pvalue->arr_body + offset_pvalue;
    }

    glClearBufferfv((GLenum)pbuffer, (GLint)pdrawbuffer, (const GLfloat*)(ptr_pvalue));
    
    
    return 0;
}

int org_mini_gl_GL_glClearBufferfi(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pbuffer = env->localvar_getInt(runtime, pos++);
    s32 pdrawbuffer = env->localvar_getInt(runtime, pos++);
    Int2Float pdepth;pdepth.i = env->localvar_getInt(runtime, pos++);
    s32 pstencil = env->localvar_getInt(runtime, pos++);

    glClearBufferfi((GLenum)pbuffer, (GLint)pdrawbuffer, (GLfloat)pdepth.f, (GLint)pstencil);
    
    
    return 0;
}

int org_mini_gl_GL_glGetStringi(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pname = env->localvar_getInt(runtime, pos++);
    s32 pindex = env->localvar_getInt(runtime, pos++);

    const GLubyte* _re_val = glGetStringi((GLenum)pname, (GLuint)pindex);
    c8* _ptr_re_val = (c8*)&_re_val;
    if (_ptr_re_val) {
        s32 bytes = sizeof(const GLubyte*);
        s32 j_t_bytes = sizeof(c8);
        Instance *_arr = env->jarray_create(bytes / j_t_bytes, DATATYPE_BYTE, NULL);
        memcpy(_arr->arr_body, _ptr_re_val,bytes);
        env->push_ref(runtime->stack, _arr);
    } else {
        env->push_ref(runtime->stack, NULL);
    }
    
    return 0;
}

int org_mini_gl_GL_glIsRenderbuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 prenderbuffer = env->localvar_getInt(runtime, pos++);

    GLboolean _re_val = glIsRenderbuffer((GLuint)prenderbuffer);
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glBindRenderbuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 prenderbuffer = env->localvar_getInt(runtime, pos++);

    glBindRenderbuffer((GLenum)ptarget, (GLuint)prenderbuffer);
    
    
    return 0;
}

int org_mini_gl_GL_glDeleteRenderbuffers(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pn = env->localvar_getInt(runtime, pos++);
    Instance *prenderbuffers = env->localvar_getRefer(runtime, pos++);
    int offset_prenderbuffers = env->localvar_getInt(runtime, pos++);
    __refer ptr_prenderbuffers = NULL;
    if(prenderbuffers){
        ptr_prenderbuffers = prenderbuffers->arr_body + offset_prenderbuffers;
    }

    glDeleteRenderbuffers((GLsizei)pn, (const GLuint*)(ptr_prenderbuffers));
    
    
    return 0;
}

int org_mini_gl_GL_glGenRenderbuffers(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pn = env->localvar_getInt(runtime, pos++);
    Instance *prenderbuffers = env->localvar_getRefer(runtime, pos++);
    int offset_prenderbuffers = env->localvar_getInt(runtime, pos++);
    __refer ptr_prenderbuffers = NULL;
    if(prenderbuffers){
        ptr_prenderbuffers = prenderbuffers->arr_body + offset_prenderbuffers;
    }

    glGenRenderbuffers((GLsizei)pn, (GLuint*)(ptr_prenderbuffers));
    
    
    return 0;
}

int org_mini_gl_GL_glRenderbufferStorage(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pinternalformat = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);

    glRenderbufferStorage((GLenum)ptarget, (GLenum)pinternalformat, (GLsizei)pwidth, (GLsizei)pheight);
    
    
    return 0;
}

int org_mini_gl_GL_glGetRenderbufferParameteriv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetRenderbufferParameteriv((GLenum)ptarget, (GLenum)ppname, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glIsFramebuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pframebuffer = env->localvar_getInt(runtime, pos++);

    GLboolean _re_val = glIsFramebuffer((GLuint)pframebuffer);
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glBindFramebuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pframebuffer = env->localvar_getInt(runtime, pos++);

    glBindFramebuffer((GLenum)ptarget, (GLuint)pframebuffer);
    
    
    return 0;
}

int org_mini_gl_GL_glDeleteFramebuffers(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pn = env->localvar_getInt(runtime, pos++);
    Instance *pframebuffers = env->localvar_getRefer(runtime, pos++);
    int offset_pframebuffers = env->localvar_getInt(runtime, pos++);
    __refer ptr_pframebuffers = NULL;
    if(pframebuffers){
        ptr_pframebuffers = pframebuffers->arr_body + offset_pframebuffers;
    }

    glDeleteFramebuffers((GLsizei)pn, (const GLuint*)(ptr_pframebuffers));
    
    
    return 0;
}

int org_mini_gl_GL_glGenFramebuffers(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pn = env->localvar_getInt(runtime, pos++);
    Instance *pframebuffers = env->localvar_getRefer(runtime, pos++);
    int offset_pframebuffers = env->localvar_getInt(runtime, pos++);
    __refer ptr_pframebuffers = NULL;
    if(pframebuffers){
        ptr_pframebuffers = pframebuffers->arr_body + offset_pframebuffers;
    }

    glGenFramebuffers((GLsizei)pn, (GLuint*)(ptr_pframebuffers));
    
    
    return 0;
}

int org_mini_gl_GL_glCheckFramebufferStatus(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);

    GLenum _re_val = glCheckFramebufferStatus((GLenum)ptarget);
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glFramebufferTexture1D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pattachment = env->localvar_getInt(runtime, pos++);
    s32 ptextarget = env->localvar_getInt(runtime, pos++);
    s32 ptexture = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);

    glFramebufferTexture1D((GLenum)ptarget, (GLenum)pattachment, (GLenum)ptextarget, (GLuint)ptexture, (GLint)plevel);
    
    
    return 0;
}

int org_mini_gl_GL_glFramebufferTexture2D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pattachment = env->localvar_getInt(runtime, pos++);
    s32 ptextarget = env->localvar_getInt(runtime, pos++);
    s32 ptexture = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);

    glFramebufferTexture2D((GLenum)ptarget, (GLenum)pattachment, (GLenum)ptextarget, (GLuint)ptexture, (GLint)plevel);
    
    
    return 0;
}

int org_mini_gl_GL_glFramebufferTexture3D(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pattachment = env->localvar_getInt(runtime, pos++);
    s32 ptextarget = env->localvar_getInt(runtime, pos++);
    s32 ptexture = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pzoffset = env->localvar_getInt(runtime, pos++);

    glFramebufferTexture3D((GLenum)ptarget, (GLenum)pattachment, (GLenum)ptextarget, (GLuint)ptexture, (GLint)plevel, (GLint)pzoffset);
    
    
    return 0;
}

int org_mini_gl_GL_glFramebufferRenderbuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pattachment = env->localvar_getInt(runtime, pos++);
    s32 prenderbuffertarget = env->localvar_getInt(runtime, pos++);
    s32 prenderbuffer = env->localvar_getInt(runtime, pos++);

    glFramebufferRenderbuffer((GLenum)ptarget, (GLenum)pattachment, (GLenum)prenderbuffertarget, (GLuint)prenderbuffer);
    
    
    return 0;
}

int org_mini_gl_GL_glGetFramebufferAttachmentParameteriv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pattachment = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetFramebufferAttachmentParameteriv((GLenum)ptarget, (GLenum)pattachment, (GLenum)ppname, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGenerateMipmap(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);

    glGenerateMipmap((GLenum)ptarget);
    
    
    return 0;
}

int org_mini_gl_GL_glBlitFramebuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 psrcX0 = env->localvar_getInt(runtime, pos++);
    s32 psrcY0 = env->localvar_getInt(runtime, pos++);
    s32 psrcX1 = env->localvar_getInt(runtime, pos++);
    s32 psrcY1 = env->localvar_getInt(runtime, pos++);
    s32 pdstX0 = env->localvar_getInt(runtime, pos++);
    s32 pdstY0 = env->localvar_getInt(runtime, pos++);
    s32 pdstX1 = env->localvar_getInt(runtime, pos++);
    s32 pdstY1 = env->localvar_getInt(runtime, pos++);
    s32 pmask = env->localvar_getInt(runtime, pos++);
    s32 pfilter = env->localvar_getInt(runtime, pos++);

    glBlitFramebuffer((GLint)psrcX0, (GLint)psrcY0, (GLint)psrcX1, (GLint)psrcY1, (GLint)pdstX0, (GLint)pdstY0, (GLint)pdstX1, (GLint)pdstY1, (GLbitfield)pmask, (GLenum)pfilter);
    
    
    return 0;
}

int org_mini_gl_GL_glRenderbufferStorageMultisample(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 psamples = env->localvar_getInt(runtime, pos++);
    s32 pinternalformat = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);

    glRenderbufferStorageMultisample((GLenum)ptarget, (GLsizei)psamples, (GLenum)pinternalformat, (GLsizei)pwidth, (GLsizei)pheight);
    
    
    return 0;
}

int org_mini_gl_GL_glFramebufferTextureLayer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pattachment = env->localvar_getInt(runtime, pos++);
    s32 ptexture = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 player = env->localvar_getInt(runtime, pos++);

    glFramebufferTextureLayer((GLenum)ptarget, (GLenum)pattachment, (GLuint)ptexture, (GLint)plevel, (GLint)player);
    
    
    return 0;
}

int org_mini_gl_GL_glMapBufferRange(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    intptr_t poffset = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    intptr_t plength = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    s32 paccess = env->localvar_getInt(runtime, pos++);

    void* _re_val = glMapBufferRange((GLenum)ptarget, (GLintptr)poffset, (GLsizeiptr)plength, (GLbitfield)paccess);
    s64 ret_value = *((s64*)&_re_val);env->push_long(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glFlushMappedBufferRange(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    intptr_t poffset = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    intptr_t plength = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glFlushMappedBufferRange((GLenum)ptarget, (GLintptr)poffset, (GLsizeiptr)plength);
    
    
    return 0;
}

int org_mini_gl_GL_glBindVertexArray(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 parray = env->localvar_getInt(runtime, pos++);

    glBindVertexArray((GLuint)parray);
    
    
    return 0;
}

int org_mini_gl_GL_glDeleteVertexArrays(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pn = env->localvar_getInt(runtime, pos++);
    Instance *parrays = env->localvar_getRefer(runtime, pos++);
    int offset_parrays = env->localvar_getInt(runtime, pos++);
    __refer ptr_parrays = NULL;
    if(parrays){
        ptr_parrays = parrays->arr_body + offset_parrays;
    }

    glDeleteVertexArrays((GLsizei)pn, (const GLuint*)(ptr_parrays));
    
    
    return 0;
}

int org_mini_gl_GL_glGenVertexArrays(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pn = env->localvar_getInt(runtime, pos++);
    Instance *parrays = env->localvar_getRefer(runtime, pos++);
    int offset_parrays = env->localvar_getInt(runtime, pos++);
    __refer ptr_parrays = NULL;
    if(parrays){
        ptr_parrays = parrays->arr_body + offset_parrays;
    }

    glGenVertexArrays((GLsizei)pn, (GLuint*)(ptr_parrays));
    
    
    return 0;
}

int org_mini_gl_GL_glIsVertexArray(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 parray = env->localvar_getInt(runtime, pos++);

    GLboolean _re_val = glIsVertexArray((GLuint)parray);
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glDrawArraysInstanced(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);
    s32 pfirst = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    s32 pinstancecount = env->localvar_getInt(runtime, pos++);

    glDrawArraysInstanced((GLenum)pmode, (GLint)pfirst, (GLsizei)pcount, (GLsizei)pinstancecount);
    
    
    return 0;
}

int org_mini_gl_GL_glDrawElementsInstanced(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    Instance *pindices = env->localvar_getRefer(runtime, pos++);
    int offset_pindices = env->localvar_getInt(runtime, pos++);
    __refer ptr_pindices = NULL;
    if(pindices){
        ptr_pindices = pindices->arr_body + offset_pindices;
    }
    s32 pinstancecount = env->localvar_getInt(runtime, pos++);

    glDrawElementsInstanced((GLenum)pmode, (GLsizei)pcount, (GLenum)ptype, (const void*)(ptr_pindices), (GLsizei)pinstancecount);
    
    
    return 0;
}

int org_mini_gl_GL_glTexBuffer(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pinternalformat = env->localvar_getInt(runtime, pos++);
    s32 pbuffer = env->localvar_getInt(runtime, pos++);

    glTexBuffer((GLenum)ptarget, (GLenum)pinternalformat, (GLuint)pbuffer);
    
    
    return 0;
}

int org_mini_gl_GL_glPrimitiveRestartIndex(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pindex = env->localvar_getInt(runtime, pos++);

    glPrimitiveRestartIndex((GLuint)pindex);
    
    
    return 0;
}

int org_mini_gl_GL_glCopyBufferSubData(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 preadTarget = env->localvar_getInt(runtime, pos++);
    s32 pwriteTarget = env->localvar_getInt(runtime, pos++);
    intptr_t preadOffset = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    intptr_t pwriteOffset = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    intptr_t psize = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glCopyBufferSubData((GLenum)preadTarget, (GLenum)pwriteTarget, (GLintptr)preadOffset, (GLintptr)pwriteOffset, (GLsizeiptr)psize);
    
    
    return 0;
}

int org_mini_gl_GL_glGetUniformIndices(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 puniformCount = env->localvar_getInt(runtime, pos++);
    Instance *puniformNames = env->localvar_getRefer(runtime, pos++);
    CStringArr *ptr_puniformNames = NULL;
    if(puniformNames){
        ptr_puniformNames = env->cstringarr_create(puniformNames);
    }
    Instance *puniformIndices = env->localvar_getRefer(runtime, pos++);
    int offset_puniformIndices = env->localvar_getInt(runtime, pos++);
    __refer ptr_puniformIndices = NULL;
    if(puniformIndices){
        ptr_puniformIndices = puniformIndices->arr_body + offset_puniformIndices;
    }

    glGetUniformIndices((GLuint)pprogram, (GLsizei)puniformCount, (const GLchar*const*)(ptr_puniformNames->arr_body), (GLuint*)(ptr_puniformIndices));
    
    env->cstringarr_destory(ptr_puniformNames);
    return 0;
}

int org_mini_gl_GL_glGetActiveUniformsiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 puniformCount = env->localvar_getInt(runtime, pos++);
    Instance *puniformIndices = env->localvar_getRefer(runtime, pos++);
    int offset_puniformIndices = env->localvar_getInt(runtime, pos++);
    __refer ptr_puniformIndices = NULL;
    if(puniformIndices){
        ptr_puniformIndices = puniformIndices->arr_body + offset_puniformIndices;
    }
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetActiveUniformsiv((GLuint)pprogram, (GLsizei)puniformCount, (const GLuint*)(ptr_puniformIndices), (GLenum)ppname, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetActiveUniformName(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 puniformIndex = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *plength = env->localvar_getRefer(runtime, pos++);
    int offset_plength = env->localvar_getInt(runtime, pos++);
    __refer ptr_plength = NULL;
    if(plength){
        ptr_plength = plength->arr_body + offset_plength;
    }
    Instance *puniformName = env->localvar_getRefer(runtime, pos++);
    __refer ptr_puniformName = NULL;
    Utf8String *u_puniformName;
    if(puniformName){
        u_puniformName = env->utf8_create();
        env->jstring_2_utf8(puniformName, u_puniformName);
        ptr_puniformName = env->utf8_cstr(u_puniformName);
    }

    glGetActiveUniformName((GLuint)pprogram, (GLuint)puniformIndex, (GLsizei)pbufSize, (GLsizei*)(ptr_plength), (GLchar*)(ptr_puniformName));
    
    env->utf8_destory(u_puniformName);
    return 0;
}

int org_mini_gl_GL_glGetUniformBlockIndex(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    Instance *puniformBlockName = env->localvar_getRefer(runtime, pos++);
    __refer ptr_puniformBlockName = NULL;
    Utf8String *u_puniformBlockName;
    if(puniformBlockName){
        u_puniformBlockName = env->utf8_create();
        env->jstring_2_utf8(puniformBlockName, u_puniformBlockName);
        ptr_puniformBlockName = env->utf8_cstr(u_puniformBlockName);
    }

    GLuint _re_val = glGetUniformBlockIndex((GLuint)pprogram, (const GLchar*)(ptr_puniformBlockName));
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    env->utf8_destory(u_puniformBlockName);
    return 0;
}

int org_mini_gl_GL_glGetActiveUniformBlockiv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 puniformBlockIndex = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetActiveUniformBlockiv((GLuint)pprogram, (GLuint)puniformBlockIndex, (GLenum)ppname, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetActiveUniformBlockName(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 puniformBlockIndex = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *plength = env->localvar_getRefer(runtime, pos++);
    int offset_plength = env->localvar_getInt(runtime, pos++);
    __refer ptr_plength = NULL;
    if(plength){
        ptr_plength = plength->arr_body + offset_plength;
    }
    Instance *puniformBlockName = env->localvar_getRefer(runtime, pos++);
    __refer ptr_puniformBlockName = NULL;
    Utf8String *u_puniformBlockName;
    if(puniformBlockName){
        u_puniformBlockName = env->utf8_create();
        env->jstring_2_utf8(puniformBlockName, u_puniformBlockName);
        ptr_puniformBlockName = env->utf8_cstr(u_puniformBlockName);
    }

    glGetActiveUniformBlockName((GLuint)pprogram, (GLuint)puniformBlockIndex, (GLsizei)pbufSize, (GLsizei*)(ptr_plength), (GLchar*)(ptr_puniformBlockName));
    
    env->utf8_destory(u_puniformBlockName);
    return 0;
}

int org_mini_gl_GL_glUniformBlockBinding(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 puniformBlockIndex = env->localvar_getInt(runtime, pos++);
    s32 puniformBlockBinding = env->localvar_getInt(runtime, pos++);

    glUniformBlockBinding((GLuint)pprogram, (GLuint)puniformBlockIndex, (GLuint)puniformBlockBinding);
    
    
    return 0;
}

int org_mini_gl_GL_glDrawElementsBaseVertex(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    Instance *pindices = env->localvar_getRefer(runtime, pos++);
    int offset_pindices = env->localvar_getInt(runtime, pos++);
    __refer ptr_pindices = NULL;
    if(pindices){
        ptr_pindices = pindices->arr_body + offset_pindices;
    }
    s32 pbasevertex = env->localvar_getInt(runtime, pos++);

    glDrawElementsBaseVertex((GLenum)pmode, (GLsizei)pcount, (GLenum)ptype, (const void*)(ptr_pindices), (GLint)pbasevertex);
    
    
    return 0;
}

int org_mini_gl_GL_glDrawRangeElementsBaseVertex(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);
    s32 pstart = env->localvar_getInt(runtime, pos++);
    s32 pend = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    Instance *pindices = env->localvar_getRefer(runtime, pos++);
    int offset_pindices = env->localvar_getInt(runtime, pos++);
    __refer ptr_pindices = NULL;
    if(pindices){
        ptr_pindices = pindices->arr_body + offset_pindices;
    }
    s32 pbasevertex = env->localvar_getInt(runtime, pos++);

    glDrawRangeElementsBaseVertex((GLenum)pmode, (GLuint)pstart, (GLuint)pend, (GLsizei)pcount, (GLenum)ptype, (const void*)(ptr_pindices), (GLint)pbasevertex);
    
    
    return 0;
}

int org_mini_gl_GL_glDrawElementsInstancedBaseVertex(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    Instance *pindices = env->localvar_getRefer(runtime, pos++);
    int offset_pindices = env->localvar_getInt(runtime, pos++);
    __refer ptr_pindices = NULL;
    if(pindices){
        ptr_pindices = pindices->arr_body + offset_pindices;
    }
    s32 pinstancecount = env->localvar_getInt(runtime, pos++);
    s32 pbasevertex = env->localvar_getInt(runtime, pos++);

    glDrawElementsInstancedBaseVertex((GLenum)pmode, (GLsizei)pcount, (GLenum)ptype, (const void*)(ptr_pindices), (GLsizei)pinstancecount, (GLint)pbasevertex);
    
    
    return 0;
}

int org_mini_gl_GL_glMultiDrawElementsBaseVertex(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);
    Instance *pcount = env->localvar_getRefer(runtime, pos++);
    int offset_pcount = env->localvar_getInt(runtime, pos++);
    __refer ptr_pcount = NULL;
    if(pcount){
        ptr_pcount = pcount->arr_body + offset_pcount;
    }
    s32 ptype = env->localvar_getInt(runtime, pos++);
    Instance *pindices = env->localvar_getRefer(runtime, pos++);
    ReferArr *ptr_pindices = NULL;
    if(pindices){
        ptr_pindices = env->referarr_create(pindices);
    }
    s32 pdrawcount = env->localvar_getInt(runtime, pos++);
    Instance *pbasevertex = env->localvar_getRefer(runtime, pos++);
    int offset_pbasevertex = env->localvar_getInt(runtime, pos++);
    __refer ptr_pbasevertex = NULL;
    if(pbasevertex){
        ptr_pbasevertex = pbasevertex->arr_body + offset_pbasevertex;
    }

    glMultiDrawElementsBaseVertex((GLenum)pmode, (const GLsizei*)(ptr_pcount), (GLenum)ptype, (const void*const*)(ptr_pindices->arr_body), (GLsizei)pdrawcount, (const GLint*)(ptr_pbasevertex));
    
    env->referarr_destory(ptr_pindices);
    return 0;
}

int org_mini_gl_GL_glProvokingVertex(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmode = env->localvar_getInt(runtime, pos++);

    glProvokingVertex((GLenum)pmode);
    
    
    return 0;
}

int org_mini_gl_GL_glFenceSync(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pcondition = env->localvar_getInt(runtime, pos++);
    s32 pflags = env->localvar_getInt(runtime, pos++);

    GLsync _re_val = glFenceSync((GLenum)pcondition, (GLbitfield)pflags);
    s64 ret_value = *((s64*)&_re_val);env->push_long(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glIsSync(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    intptr_t psync = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    GLboolean _re_val = glIsSync((GLsync)psync);
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glDeleteSync(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    intptr_t psync = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glDeleteSync((GLsync)psync);
    
    
    return 0;
}

int org_mini_gl_GL_glClientWaitSync(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    intptr_t psync = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    s32 pflags = env->localvar_getInt(runtime, pos++);
    intptr_t ptimeout = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    GLenum _re_val = glClientWaitSync((GLsync)psync, (GLbitfield)pflags, (GLuint64)ptimeout);
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glWaitSync(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    intptr_t psync = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    s32 pflags = env->localvar_getInt(runtime, pos++);
    intptr_t ptimeout = env->localvar_getLong_2slot(runtime, pos);pos += 2;

    glWaitSync((GLsync)psync, (GLbitfield)pflags, (GLuint64)ptimeout);
    
    
    return 0;
}

int org_mini_gl_GL_glGetInteger64v(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pdata = env->localvar_getRefer(runtime, pos++);
    int offset_pdata = env->localvar_getInt(runtime, pos++);
    __refer ptr_pdata = NULL;
    if(pdata){
        ptr_pdata = pdata->arr_body + offset_pdata;
    }

    glGetInteger64v((GLenum)ppname, (GLint64*)(ptr_pdata));
    
    
    return 0;
}

int org_mini_gl_GL_glGetSynciv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    intptr_t psync = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    s32 ppname = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *plength = env->localvar_getRefer(runtime, pos++);
    int offset_plength = env->localvar_getInt(runtime, pos++);
    __refer ptr_plength = NULL;
    if(plength){
        ptr_plength = plength->arr_body + offset_plength;
    }
    Instance *pvalues = env->localvar_getRefer(runtime, pos++);
    int offset_pvalues = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalues = NULL;
    if(pvalues){
        ptr_pvalues = pvalues->arr_body + offset_pvalues;
    }

    glGetSynciv((GLsync)psync, (GLenum)ppname, (GLsizei)pbufSize, (GLsizei*)(ptr_plength), (GLint*)(ptr_pvalues));
    
    
    return 0;
}

int org_mini_gl_GL_glGetInteger64i_v(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pdata = env->localvar_getRefer(runtime, pos++);
    int offset_pdata = env->localvar_getInt(runtime, pos++);
    __refer ptr_pdata = NULL;
    if(pdata){
        ptr_pdata = pdata->arr_body + offset_pdata;
    }

    glGetInteger64i_v((GLenum)ptarget, (GLuint)pindex, (GLint64*)(ptr_pdata));
    
    
    return 0;
}

int org_mini_gl_GL_glGetBufferParameteri64v(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetBufferParameteri64v((GLenum)ptarget, (GLenum)ppname, (GLint64*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glFramebufferTexture(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pattachment = env->localvar_getInt(runtime, pos++);
    s32 ptexture = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);

    glFramebufferTexture((GLenum)ptarget, (GLenum)pattachment, (GLuint)ptexture, (GLint)plevel);
    
    
    return 0;
}

int org_mini_gl_GL_glTexImage2DMultisample(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 psamples = env->localvar_getInt(runtime, pos++);
    s32 pinternalformat = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);
    s32 pfixedsamplelocations = env->localvar_getInt(runtime, pos++);

    glTexImage2DMultisample((GLenum)ptarget, (GLsizei)psamples, (GLenum)pinternalformat, (GLsizei)pwidth, (GLsizei)pheight, (GLboolean)pfixedsamplelocations);
    
    
    return 0;
}

int org_mini_gl_GL_glTexImage3DMultisample(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 psamples = env->localvar_getInt(runtime, pos++);
    s32 pinternalformat = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);
    s32 pdepth = env->localvar_getInt(runtime, pos++);
    s32 pfixedsamplelocations = env->localvar_getInt(runtime, pos++);

    glTexImage3DMultisample((GLenum)ptarget, (GLsizei)psamples, (GLenum)pinternalformat, (GLsizei)pwidth, (GLsizei)pheight, (GLsizei)pdepth, (GLboolean)pfixedsamplelocations);
    
    
    return 0;
}

int org_mini_gl_GL_glGetMultisamplefv(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    s32 pindex = env->localvar_getInt(runtime, pos++);
    Instance *pval = env->localvar_getRefer(runtime, pos++);
    int offset_pval = env->localvar_getInt(runtime, pos++);
    __refer ptr_pval = NULL;
    if(pval){
        ptr_pval = pval->arr_body + offset_pval;
    }

    glGetMultisamplefv((GLenum)ppname, (GLuint)pindex, (GLfloat*)(ptr_pval));
    
    
    return 0;
}

int org_mini_gl_GL_glSampleMaski(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmaskNumber = env->localvar_getInt(runtime, pos++);
    s32 pmask = env->localvar_getInt(runtime, pos++);

    glSampleMaski((GLuint)pmaskNumber, (GLbitfield)pmask);
    
    
    return 0;
}

int org_mini_gl_GL_glSampleCoverageARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Int2Float pvalue;pvalue.i = env->localvar_getInt(runtime, pos++);
    s32 pinvert = env->localvar_getInt(runtime, pos++);

    glSampleCoverageARB((GLfloat)pvalue.f, (GLboolean)pinvert);
    
    
    return 0;
}

int org_mini_gl_GL_glGetGraphicsResetStatusARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    

    GLenum _re_val = glGetGraphicsResetStatusARB();
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    
    return 0;
}

int org_mini_gl_GL_glGetnTexImageARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plevel = env->localvar_getInt(runtime, pos++);
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *pimg = env->localvar_getRefer(runtime, pos++);
    int offset_pimg = env->localvar_getInt(runtime, pos++);
    __refer ptr_pimg = NULL;
    if(pimg){
        ptr_pimg = pimg->arr_body + offset_pimg;
    }

    glGetnTexImageARB((GLenum)ptarget, (GLint)plevel, (GLenum)pformat, (GLenum)ptype, (GLsizei)pbufSize, (void*)(ptr_pimg));
    
    
    return 0;
}

int org_mini_gl_GL_glReadnPixelsARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 px = env->localvar_getInt(runtime, pos++);
    s32 py = env->localvar_getInt(runtime, pos++);
    s32 pwidth = env->localvar_getInt(runtime, pos++);
    s32 pheight = env->localvar_getInt(runtime, pos++);
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *pdata = env->localvar_getRefer(runtime, pos++);
    int offset_pdata = env->localvar_getInt(runtime, pos++);
    __refer ptr_pdata = NULL;
    if(pdata){
        ptr_pdata = pdata->arr_body + offset_pdata;
    }

    glReadnPixelsARB((GLint)px, (GLint)py, (GLsizei)pwidth, (GLsizei)pheight, (GLenum)pformat, (GLenum)ptype, (GLsizei)pbufSize, (void*)(ptr_pdata));
    
    
    return 0;
}

int org_mini_gl_GL_glGetnCompressedTexImageARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 plod = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *pimg = env->localvar_getRefer(runtime, pos++);
    int offset_pimg = env->localvar_getInt(runtime, pos++);
    __refer ptr_pimg = NULL;
    if(pimg){
        ptr_pimg = pimg->arr_body + offset_pimg;
    }

    glGetnCompressedTexImageARB((GLenum)ptarget, (GLint)plod, (GLsizei)pbufSize, (void*)(ptr_pimg));
    
    
    return 0;
}

int org_mini_gl_GL_glGetnUniformfvARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetnUniformfvARB((GLuint)pprogram, (GLint)plocation, (GLsizei)pbufSize, (GLfloat*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetnUniformivARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetnUniformivARB((GLuint)pprogram, (GLint)plocation, (GLsizei)pbufSize, (GLint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetnUniformuivARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetnUniformuivARB((GLuint)pprogram, (GLint)plocation, (GLsizei)pbufSize, (GLuint*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetnUniformdvARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pprogram = env->localvar_getInt(runtime, pos++);
    s32 plocation = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    int offset_pparams = env->localvar_getInt(runtime, pos++);
    __refer ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = pparams->arr_body + offset_pparams;
    }

    glGetnUniformdvARB((GLuint)pprogram, (GLint)plocation, (GLsizei)pbufSize, (GLdouble*)(ptr_pparams));
    
    
    return 0;
}

int org_mini_gl_GL_glGetnMapdvARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pquery = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glGetnMapdvARB((GLenum)ptarget, (GLenum)pquery, (GLsizei)pbufSize, (GLdouble*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glGetnMapfvARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pquery = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glGetnMapfvARB((GLenum)ptarget, (GLenum)pquery, (GLsizei)pbufSize, (GLfloat*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glGetnMapivARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pquery = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *pv = env->localvar_getRefer(runtime, pos++);
    int offset_pv = env->localvar_getInt(runtime, pos++);
    __refer ptr_pv = NULL;
    if(pv){
        ptr_pv = pv->arr_body + offset_pv;
    }

    glGetnMapivARB((GLenum)ptarget, (GLenum)pquery, (GLsizei)pbufSize, (GLint*)(ptr_pv));
    
    
    return 0;
}

int org_mini_gl_GL_glGetnPixelMapfvARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmap = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *pvalues = env->localvar_getRefer(runtime, pos++);
    int offset_pvalues = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalues = NULL;
    if(pvalues){
        ptr_pvalues = pvalues->arr_body + offset_pvalues;
    }

    glGetnPixelMapfvARB((GLenum)pmap, (GLsizei)pbufSize, (GLfloat*)(ptr_pvalues));
    
    
    return 0;
}

int org_mini_gl_GL_glGetnPixelMapuivARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmap = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *pvalues = env->localvar_getRefer(runtime, pos++);
    int offset_pvalues = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalues = NULL;
    if(pvalues){
        ptr_pvalues = pvalues->arr_body + offset_pvalues;
    }

    glGetnPixelMapuivARB((GLenum)pmap, (GLsizei)pbufSize, (GLuint*)(ptr_pvalues));
    
    
    return 0;
}

int org_mini_gl_GL_glGetnPixelMapusvARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pmap = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *pvalues = env->localvar_getRefer(runtime, pos++);
    int offset_pvalues = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalues = NULL;
    if(pvalues){
        ptr_pvalues = pvalues->arr_body + offset_pvalues;
    }

    glGetnPixelMapusvARB((GLenum)pmap, (GLsizei)pbufSize, (GLushort*)(ptr_pvalues));
    
    
    return 0;
}

int org_mini_gl_GL_glGetnPolygonStippleARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *ppattern = env->localvar_getRefer(runtime, pos++);
    int offset_ppattern = env->localvar_getInt(runtime, pos++);
    __refer ptr_ppattern = NULL;
    if(ppattern){
        ptr_ppattern = ppattern->arr_body + offset_ppattern;
    }

    glGetnPolygonStippleARB((GLsizei)pbufSize, (GLubyte*)(ptr_ppattern));
    
    
    return 0;
}

int org_mini_gl_GL_glGetnColorTableARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *ptable = env->localvar_getRefer(runtime, pos++);
    int offset_ptable = env->localvar_getInt(runtime, pos++);
    __refer ptr_ptable = NULL;
    if(ptable){
        ptr_ptable = ptable->arr_body + offset_ptable;
    }

    glGetnColorTableARB((GLenum)ptarget, (GLenum)pformat, (GLenum)ptype, (GLsizei)pbufSize, (void*)(ptr_ptable));
    
    
    return 0;
}

int org_mini_gl_GL_glGetnConvolutionFilterARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *pimage = env->localvar_getRefer(runtime, pos++);
    int offset_pimage = env->localvar_getInt(runtime, pos++);
    __refer ptr_pimage = NULL;
    if(pimage){
        ptr_pimage = pimage->arr_body + offset_pimage;
    }

    glGetnConvolutionFilterARB((GLenum)ptarget, (GLenum)pformat, (GLenum)ptype, (GLsizei)pbufSize, (void*)(ptr_pimage));
    
    
    return 0;
}

int org_mini_gl_GL_glGetnSeparableFilterARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 prowBufSize = env->localvar_getInt(runtime, pos++);
    Instance *prow = env->localvar_getRefer(runtime, pos++);
    int offset_prow = env->localvar_getInt(runtime, pos++);
    __refer ptr_prow = NULL;
    if(prow){
        ptr_prow = prow->arr_body + offset_prow;
    }
    s32 pcolumnBufSize = env->localvar_getInt(runtime, pos++);
    Instance *pcolumn = env->localvar_getRefer(runtime, pos++);
    int offset_pcolumn = env->localvar_getInt(runtime, pos++);
    __refer ptr_pcolumn = NULL;
    if(pcolumn){
        ptr_pcolumn = pcolumn->arr_body + offset_pcolumn;
    }
    Instance *pspan = env->localvar_getRefer(runtime, pos++);
    int offset_pspan = env->localvar_getInt(runtime, pos++);
    __refer ptr_pspan = NULL;
    if(pspan){
        ptr_pspan = pspan->arr_body + offset_pspan;
    }

    glGetnSeparableFilterARB((GLenum)ptarget, (GLenum)pformat, (GLenum)ptype, (GLsizei)prowBufSize, (void*)(ptr_prow), (GLsizei)pcolumnBufSize, (void*)(ptr_pcolumn), (void*)(ptr_pspan));
    
    
    return 0;
}

int org_mini_gl_GL_glGetnHistogramARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 preset = env->localvar_getInt(runtime, pos++);
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *pvalues = env->localvar_getRefer(runtime, pos++);
    int offset_pvalues = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalues = NULL;
    if(pvalues){
        ptr_pvalues = pvalues->arr_body + offset_pvalues;
    }

    glGetnHistogramARB((GLenum)ptarget, (GLboolean)preset, (GLenum)pformat, (GLenum)ptype, (GLsizei)pbufSize, (void*)(ptr_pvalues));
    
    
    return 0;
}

int org_mini_gl_GL_glGetnMinmaxARB(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ptarget = env->localvar_getInt(runtime, pos++);
    s32 preset = env->localvar_getInt(runtime, pos++);
    s32 pformat = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *pvalues = env->localvar_getRefer(runtime, pos++);
    int offset_pvalues = env->localvar_getInt(runtime, pos++);
    __refer ptr_pvalues = NULL;
    if(pvalues){
        ptr_pvalues = pvalues->arr_body + offset_pvalues;
    }

    glGetnMinmaxARB((GLenum)ptarget, (GLboolean)preset, (GLenum)pformat, (GLenum)ptype, (GLsizei)pbufSize, (void*)(ptr_pvalues));
    
    
    return 0;
}

int org_mini_gl_GL_glDebugMessageControl(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 psource = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 pseverity = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    Instance *pids = env->localvar_getRefer(runtime, pos++);
    int offset_pids = env->localvar_getInt(runtime, pos++);
    __refer ptr_pids = NULL;
    if(pids){
        ptr_pids = pids->arr_body + offset_pids;
    }
    s32 penabled = env->localvar_getInt(runtime, pos++);

    glDebugMessageControl((GLenum)psource, (GLenum)ptype, (GLenum)pseverity, (GLsizei)pcount, (const GLuint*)(ptr_pids), (GLboolean)penabled);
    
    
    return 0;
}

int org_mini_gl_GL_glDebugMessageInsert(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 psource = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 pid = env->localvar_getInt(runtime, pos++);
    s32 pseverity = env->localvar_getInt(runtime, pos++);
    s32 plength = env->localvar_getInt(runtime, pos++);
    Instance *pbuf = env->localvar_getRefer(runtime, pos++);
    __refer ptr_pbuf = NULL;
    Utf8String *u_pbuf;
    if(pbuf){
        u_pbuf = env->utf8_create();
        env->jstring_2_utf8(pbuf, u_pbuf);
        ptr_pbuf = env->utf8_cstr(u_pbuf);
    }

    glDebugMessageInsert((GLenum)psource, (GLenum)ptype, (GLuint)pid, (GLenum)pseverity, (GLsizei)plength, (const GLchar*)(ptr_pbuf));
    
    env->utf8_destory(u_pbuf);
    return 0;
}

int org_mini_gl_GL_glDebugMessageCallback(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    intptr_t pcallback = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Instance *puserParam = env->localvar_getRefer(runtime, pos++);
    int offset_puserParam = env->localvar_getInt(runtime, pos++);
    __refer ptr_puserParam = NULL;
    if(puserParam){
        ptr_puserParam = puserParam->arr_body + offset_puserParam;
    }

    glDebugMessageCallback((GLDEBUGPROC)pcallback, (const void*)(ptr_puserParam));
    
    
    return 0;
}

int org_mini_gl_GL_glGetDebugMessageLog(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pcount = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *psources = env->localvar_getRefer(runtime, pos++);
    int offset_psources = env->localvar_getInt(runtime, pos++);
    __refer ptr_psources = NULL;
    if(psources){
        ptr_psources = psources->arr_body + offset_psources;
    }
    Instance *ptypes = env->localvar_getRefer(runtime, pos++);
    int offset_ptypes = env->localvar_getInt(runtime, pos++);
    __refer ptr_ptypes = NULL;
    if(ptypes){
        ptr_ptypes = ptypes->arr_body + offset_ptypes;
    }
    Instance *pids = env->localvar_getRefer(runtime, pos++);
    int offset_pids = env->localvar_getInt(runtime, pos++);
    __refer ptr_pids = NULL;
    if(pids){
        ptr_pids = pids->arr_body + offset_pids;
    }
    Instance *pseverities = env->localvar_getRefer(runtime, pos++);
    int offset_pseverities = env->localvar_getInt(runtime, pos++);
    __refer ptr_pseverities = NULL;
    if(pseverities){
        ptr_pseverities = pseverities->arr_body + offset_pseverities;
    }
    Instance *plengths = env->localvar_getRefer(runtime, pos++);
    int offset_plengths = env->localvar_getInt(runtime, pos++);
    __refer ptr_plengths = NULL;
    if(plengths){
        ptr_plengths = plengths->arr_body + offset_plengths;
    }
    Instance *pmessageLog = env->localvar_getRefer(runtime, pos++);
    __refer ptr_pmessageLog = NULL;
    Utf8String *u_pmessageLog;
    if(pmessageLog){
        u_pmessageLog = env->utf8_create();
        env->jstring_2_utf8(pmessageLog, u_pmessageLog);
        ptr_pmessageLog = env->utf8_cstr(u_pmessageLog);
    }

    GLuint _re_val = glGetDebugMessageLog((GLuint)pcount, (GLsizei)pbufSize, (GLenum*)(ptr_psources), (GLenum*)(ptr_ptypes), (GLuint*)(ptr_pids), (GLenum*)(ptr_pseverities), (GLsizei*)(ptr_plengths), (GLchar*)(ptr_pmessageLog));
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    env->utf8_destory(u_pmessageLog);
    return 0;
}

int org_mini_gl_GL_glPushDebugGroup(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 psource = env->localvar_getInt(runtime, pos++);
    s32 pid = env->localvar_getInt(runtime, pos++);
    s32 plength = env->localvar_getInt(runtime, pos++);
    Instance *pmessage = env->localvar_getRefer(runtime, pos++);
    __refer ptr_pmessage = NULL;
    Utf8String *u_pmessage;
    if(pmessage){
        u_pmessage = env->utf8_create();
        env->jstring_2_utf8(pmessage, u_pmessage);
        ptr_pmessage = env->utf8_cstr(u_pmessage);
    }

    glPushDebugGroup((GLenum)psource, (GLuint)pid, (GLsizei)plength, (const GLchar*)(ptr_pmessage));
    
    env->utf8_destory(u_pmessage);
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
    
    s32 pidentifier = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    s32 plength = env->localvar_getInt(runtime, pos++);
    Instance *plabel = env->localvar_getRefer(runtime, pos++);
    __refer ptr_plabel = NULL;
    Utf8String *u_plabel;
    if(plabel){
        u_plabel = env->utf8_create();
        env->jstring_2_utf8(plabel, u_plabel);
        ptr_plabel = env->utf8_cstr(u_plabel);
    }

    glObjectLabel((GLenum)pidentifier, (GLuint)pname, (GLsizei)plength, (const GLchar*)(ptr_plabel));
    
    env->utf8_destory(u_plabel);
    return 0;
}

int org_mini_gl_GL_glGetObjectLabel(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pidentifier = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *plength = env->localvar_getRefer(runtime, pos++);
    int offset_plength = env->localvar_getInt(runtime, pos++);
    __refer ptr_plength = NULL;
    if(plength){
        ptr_plength = plength->arr_body + offset_plength;
    }
    Instance *plabel = env->localvar_getRefer(runtime, pos++);
    __refer ptr_plabel = NULL;
    Utf8String *u_plabel;
    if(plabel){
        u_plabel = env->utf8_create();
        env->jstring_2_utf8(plabel, u_plabel);
        ptr_plabel = env->utf8_cstr(u_plabel);
    }

    glGetObjectLabel((GLenum)pidentifier, (GLuint)pname, (GLsizei)pbufSize, (GLsizei*)(ptr_plength), (GLchar*)(ptr_plabel));
    
    env->utf8_destory(u_plabel);
    return 0;
}

int org_mini_gl_GL_glObjectPtrLabel(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pptr = env->localvar_getRefer(runtime, pos++);
    int offset_pptr = env->localvar_getInt(runtime, pos++);
    __refer ptr_pptr = NULL;
    if(pptr){
        ptr_pptr = pptr->arr_body + offset_pptr;
    }
    s32 plength = env->localvar_getInt(runtime, pos++);
    Instance *plabel = env->localvar_getRefer(runtime, pos++);
    __refer ptr_plabel = NULL;
    Utf8String *u_plabel;
    if(plabel){
        u_plabel = env->utf8_create();
        env->jstring_2_utf8(plabel, u_plabel);
        ptr_plabel = env->utf8_cstr(u_plabel);
    }

    glObjectPtrLabel((const void*)(ptr_pptr), (GLsizei)plength, (const GLchar*)(ptr_plabel));
    
    env->utf8_destory(u_plabel);
    return 0;
}

int org_mini_gl_GL_glGetObjectPtrLabel(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pptr = env->localvar_getRefer(runtime, pos++);
    int offset_pptr = env->localvar_getInt(runtime, pos++);
    __refer ptr_pptr = NULL;
    if(pptr){
        ptr_pptr = pptr->arr_body + offset_pptr;
    }
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *plength = env->localvar_getRefer(runtime, pos++);
    int offset_plength = env->localvar_getInt(runtime, pos++);
    __refer ptr_plength = NULL;
    if(plength){
        ptr_plength = plength->arr_body + offset_plength;
    }
    Instance *plabel = env->localvar_getRefer(runtime, pos++);
    __refer ptr_plabel = NULL;
    Utf8String *u_plabel;
    if(plabel){
        u_plabel = env->utf8_create();
        env->jstring_2_utf8(plabel, u_plabel);
        ptr_plabel = env->utf8_cstr(u_plabel);
    }

    glGetObjectPtrLabel((const void*)(ptr_pptr), (GLsizei)pbufSize, (GLsizei*)(ptr_plength), (GLchar*)(ptr_plabel));
    
    env->utf8_destory(u_plabel);
    return 0;
}

int org_mini_gl_GL_glDebugMessageControlKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 psource = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 pseverity = env->localvar_getInt(runtime, pos++);
    s32 pcount = env->localvar_getInt(runtime, pos++);
    Instance *pids = env->localvar_getRefer(runtime, pos++);
    int offset_pids = env->localvar_getInt(runtime, pos++);
    __refer ptr_pids = NULL;
    if(pids){
        ptr_pids = pids->arr_body + offset_pids;
    }
    s32 penabled = env->localvar_getInt(runtime, pos++);

    glDebugMessageControlKHR((GLenum)psource, (GLenum)ptype, (GLenum)pseverity, (GLsizei)pcount, (const GLuint*)(ptr_pids), (GLboolean)penabled);
    
    
    return 0;
}

int org_mini_gl_GL_glDebugMessageInsertKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 psource = env->localvar_getInt(runtime, pos++);
    s32 ptype = env->localvar_getInt(runtime, pos++);
    s32 pid = env->localvar_getInt(runtime, pos++);
    s32 pseverity = env->localvar_getInt(runtime, pos++);
    s32 plength = env->localvar_getInt(runtime, pos++);
    Instance *pbuf = env->localvar_getRefer(runtime, pos++);
    __refer ptr_pbuf = NULL;
    Utf8String *u_pbuf;
    if(pbuf){
        u_pbuf = env->utf8_create();
        env->jstring_2_utf8(pbuf, u_pbuf);
        ptr_pbuf = env->utf8_cstr(u_pbuf);
    }

    glDebugMessageInsertKHR((GLenum)psource, (GLenum)ptype, (GLuint)pid, (GLenum)pseverity, (GLsizei)plength, (const GLchar*)(ptr_pbuf));
    
    env->utf8_destory(u_pbuf);
    return 0;
}

int org_mini_gl_GL_glDebugMessageCallbackKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    intptr_t pcallback = env->localvar_getLong_2slot(runtime, pos);pos += 2;
    Instance *puserParam = env->localvar_getRefer(runtime, pos++);
    int offset_puserParam = env->localvar_getInt(runtime, pos++);
    __refer ptr_puserParam = NULL;
    if(puserParam){
        ptr_puserParam = puserParam->arr_body + offset_puserParam;
    }

    glDebugMessageCallbackKHR((GLDEBUGPROCKHR)pcallback, (const void*)(ptr_puserParam));
    
    
    return 0;
}

int org_mini_gl_GL_glGetDebugMessageLogKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pcount = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *psources = env->localvar_getRefer(runtime, pos++);
    int offset_psources = env->localvar_getInt(runtime, pos++);
    __refer ptr_psources = NULL;
    if(psources){
        ptr_psources = psources->arr_body + offset_psources;
    }
    Instance *ptypes = env->localvar_getRefer(runtime, pos++);
    int offset_ptypes = env->localvar_getInt(runtime, pos++);
    __refer ptr_ptypes = NULL;
    if(ptypes){
        ptr_ptypes = ptypes->arr_body + offset_ptypes;
    }
    Instance *pids = env->localvar_getRefer(runtime, pos++);
    int offset_pids = env->localvar_getInt(runtime, pos++);
    __refer ptr_pids = NULL;
    if(pids){
        ptr_pids = pids->arr_body + offset_pids;
    }
    Instance *pseverities = env->localvar_getRefer(runtime, pos++);
    int offset_pseverities = env->localvar_getInt(runtime, pos++);
    __refer ptr_pseverities = NULL;
    if(pseverities){
        ptr_pseverities = pseverities->arr_body + offset_pseverities;
    }
    Instance *plengths = env->localvar_getRefer(runtime, pos++);
    int offset_plengths = env->localvar_getInt(runtime, pos++);
    __refer ptr_plengths = NULL;
    if(plengths){
        ptr_plengths = plengths->arr_body + offset_plengths;
    }
    Instance *pmessageLog = env->localvar_getRefer(runtime, pos++);
    __refer ptr_pmessageLog = NULL;
    Utf8String *u_pmessageLog;
    if(pmessageLog){
        u_pmessageLog = env->utf8_create();
        env->jstring_2_utf8(pmessageLog, u_pmessageLog);
        ptr_pmessageLog = env->utf8_cstr(u_pmessageLog);
    }

    GLuint _re_val = glGetDebugMessageLogKHR((GLuint)pcount, (GLsizei)pbufSize, (GLenum*)(ptr_psources), (GLenum*)(ptr_ptypes), (GLuint*)(ptr_pids), (GLenum*)(ptr_pseverities), (GLsizei*)(ptr_plengths), (GLchar*)(ptr_pmessageLog));
    s32 ret_value = *((s32*)&_re_val);env->push_int(runtime->stack, ret_value);
    env->utf8_destory(u_pmessageLog);
    return 0;
}

int org_mini_gl_GL_glPushDebugGroupKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 psource = env->localvar_getInt(runtime, pos++);
    s32 pid = env->localvar_getInt(runtime, pos++);
    s32 plength = env->localvar_getInt(runtime, pos++);
    Instance *pmessage = env->localvar_getRefer(runtime, pos++);
    __refer ptr_pmessage = NULL;
    Utf8String *u_pmessage;
    if(pmessage){
        u_pmessage = env->utf8_create();
        env->jstring_2_utf8(pmessage, u_pmessage);
        ptr_pmessage = env->utf8_cstr(u_pmessage);
    }

    glPushDebugGroupKHR((GLenum)psource, (GLuint)pid, (GLsizei)plength, (const GLchar*)(ptr_pmessage));
    
    env->utf8_destory(u_pmessage);
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
    
    s32 pidentifier = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    s32 plength = env->localvar_getInt(runtime, pos++);
    Instance *plabel = env->localvar_getRefer(runtime, pos++);
    __refer ptr_plabel = NULL;
    Utf8String *u_plabel;
    if(plabel){
        u_plabel = env->utf8_create();
        env->jstring_2_utf8(plabel, u_plabel);
        ptr_plabel = env->utf8_cstr(u_plabel);
    }

    glObjectLabelKHR((GLenum)pidentifier, (GLuint)pname, (GLsizei)plength, (const GLchar*)(ptr_plabel));
    
    env->utf8_destory(u_plabel);
    return 0;
}

int org_mini_gl_GL_glGetObjectLabelKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 pidentifier = env->localvar_getInt(runtime, pos++);
    s32 pname = env->localvar_getInt(runtime, pos++);
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *plength = env->localvar_getRefer(runtime, pos++);
    int offset_plength = env->localvar_getInt(runtime, pos++);
    __refer ptr_plength = NULL;
    if(plength){
        ptr_plength = plength->arr_body + offset_plength;
    }
    Instance *plabel = env->localvar_getRefer(runtime, pos++);
    __refer ptr_plabel = NULL;
    Utf8String *u_plabel;
    if(plabel){
        u_plabel = env->utf8_create();
        env->jstring_2_utf8(plabel, u_plabel);
        ptr_plabel = env->utf8_cstr(u_plabel);
    }

    glGetObjectLabelKHR((GLenum)pidentifier, (GLuint)pname, (GLsizei)pbufSize, (GLsizei*)(ptr_plength), (GLchar*)(ptr_plabel));
    
    env->utf8_destory(u_plabel);
    return 0;
}

int org_mini_gl_GL_glObjectPtrLabelKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pptr = env->localvar_getRefer(runtime, pos++);
    int offset_pptr = env->localvar_getInt(runtime, pos++);
    __refer ptr_pptr = NULL;
    if(pptr){
        ptr_pptr = pptr->arr_body + offset_pptr;
    }
    s32 plength = env->localvar_getInt(runtime, pos++);
    Instance *plabel = env->localvar_getRefer(runtime, pos++);
    __refer ptr_plabel = NULL;
    Utf8String *u_plabel;
    if(plabel){
        u_plabel = env->utf8_create();
        env->jstring_2_utf8(plabel, u_plabel);
        ptr_plabel = env->utf8_cstr(u_plabel);
    }

    glObjectPtrLabelKHR((const void*)(ptr_pptr), (GLsizei)plength, (const GLchar*)(ptr_plabel));
    
    env->utf8_destory(u_plabel);
    return 0;
}

int org_mini_gl_GL_glGetObjectPtrLabelKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    Instance *pptr = env->localvar_getRefer(runtime, pos++);
    int offset_pptr = env->localvar_getInt(runtime, pos++);
    __refer ptr_pptr = NULL;
    if(pptr){
        ptr_pptr = pptr->arr_body + offset_pptr;
    }
    s32 pbufSize = env->localvar_getInt(runtime, pos++);
    Instance *plength = env->localvar_getRefer(runtime, pos++);
    int offset_plength = env->localvar_getInt(runtime, pos++);
    __refer ptr_plength = NULL;
    if(plength){
        ptr_plength = plength->arr_body + offset_plength;
    }
    Instance *plabel = env->localvar_getRefer(runtime, pos++);
    __refer ptr_plabel = NULL;
    Utf8String *u_plabel;
    if(plabel){
        u_plabel = env->utf8_create();
        env->jstring_2_utf8(plabel, u_plabel);
        ptr_plabel = env->utf8_cstr(u_plabel);
    }

    glGetObjectPtrLabelKHR((const void*)(ptr_pptr), (GLsizei)pbufSize, (GLsizei*)(ptr_plength), (GLchar*)(ptr_plabel));
    
    env->utf8_destory(u_plabel);
    return 0;
}

int org_mini_gl_GL_glGetPointervKHR(Runtime *runtime, Class *clazz) {
    JniEnv *env = runtime->jnienv;
    s32 pos = 0;
    
    s32 ppname = env->localvar_getInt(runtime, pos++);
    Instance *pparams = env->localvar_getRefer(runtime, pos++);
    ReferArr *ptr_pparams = NULL;
    if(pparams){
        ptr_pparams = env->referarr_create(pparams);
    }

    glGetPointervKHR((GLenum)ppname, (void**)(ptr_pparams->arr_body));
    
    env->referarr_destory(ptr_pparams);
    return 0;
}




static java_native_method method_gl_table[] = {

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
{"org/mini/gl/GL",  "glGetPointerv",  "(I[Ljava/lang/Object;)V",  org_mini_gl_GL_glGetPointerv},
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
{"org/mini/gl/GL",  "glMultiDrawElements",  "(I[III[Ljava/lang/Object;I)V",  org_mini_gl_GL_glMultiDrawElements},
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
{"org/mini/gl/GL",  "glMapBuffer",  "(II)J",  org_mini_gl_GL_glMapBuffer},
{"org/mini/gl/GL",  "glUnmapBuffer",  "(I)I",  org_mini_gl_GL_glUnmapBuffer},
{"org/mini/gl/GL",  "glGetBufferParameteriv",  "(II[II)V",  org_mini_gl_GL_glGetBufferParameteriv},
{"org/mini/gl/GL",  "glGetBufferPointerv",  "(II[Ljava/lang/Object;)V",  org_mini_gl_GL_glGetBufferPointerv},
{"org/mini/gl/GL",  "glBlendEquationSeparate",  "(II)V",  org_mini_gl_GL_glBlendEquationSeparate},
{"org/mini/gl/GL",  "glDrawBuffers",  "(I[II)V",  org_mini_gl_GL_glDrawBuffers},
{"org/mini/gl/GL",  "glStencilOpSeparate",  "(IIII)V",  org_mini_gl_GL_glStencilOpSeparate},
{"org/mini/gl/GL",  "glStencilFuncSeparate",  "(IIII)V",  org_mini_gl_GL_glStencilFuncSeparate},
{"org/mini/gl/GL",  "glStencilMaskSeparate",  "(II)V",  org_mini_gl_GL_glStencilMaskSeparate},
{"org/mini/gl/GL",  "glAttachShader",  "(II)V",  org_mini_gl_GL_glAttachShader},
{"org/mini/gl/GL",  "glBindAttribLocation",  "(IILjava/lang/String;)V",  org_mini_gl_GL_glBindAttribLocation},
{"org/mini/gl/GL",  "glCompileShader",  "(I)V",  org_mini_gl_GL_glCompileShader},
{"org/mini/gl/GL",  "glCreateProgram",  "()I",  org_mini_gl_GL_glCreateProgram},
{"org/mini/gl/GL",  "glCreateShader",  "(I)I",  org_mini_gl_GL_glCreateShader},
{"org/mini/gl/GL",  "glDeleteProgram",  "(I)V",  org_mini_gl_GL_glDeleteProgram},
{"org/mini/gl/GL",  "glDeleteShader",  "(I)V",  org_mini_gl_GL_glDeleteShader},
{"org/mini/gl/GL",  "glDetachShader",  "(II)V",  org_mini_gl_GL_glDetachShader},
{"org/mini/gl/GL",  "glDisableVertexAttribArray",  "(I)V",  org_mini_gl_GL_glDisableVertexAttribArray},
{"org/mini/gl/GL",  "glEnableVertexAttribArray",  "(I)V",  org_mini_gl_GL_glEnableVertexAttribArray},
{"org/mini/gl/GL",  "glGetActiveAttrib",  "(III[II[II[IILjava/lang/String;)V",  org_mini_gl_GL_glGetActiveAttrib},
{"org/mini/gl/GL",  "glGetActiveUniform",  "(III[II[II[IILjava/lang/String;)V",  org_mini_gl_GL_glGetActiveUniform},
{"org/mini/gl/GL",  "glGetAttachedShaders",  "(II[II[II)V",  org_mini_gl_GL_glGetAttachedShaders},
{"org/mini/gl/GL",  "glGetAttribLocation",  "(ILjava/lang/String;)I",  org_mini_gl_GL_glGetAttribLocation},
{"org/mini/gl/GL",  "glGetProgramiv",  "(II[II)V",  org_mini_gl_GL_glGetProgramiv},
{"org/mini/gl/GL",  "glGetProgramInfoLog",  "(II[IILjava/lang/String;)V",  org_mini_gl_GL_glGetProgramInfoLog},
{"org/mini/gl/GL",  "glGetShaderiv",  "(II[II)V",  org_mini_gl_GL_glGetShaderiv},
{"org/mini/gl/GL",  "glGetShaderInfoLog",  "(II[IILjava/lang/String;)V",  org_mini_gl_GL_glGetShaderInfoLog},
{"org/mini/gl/GL",  "glGetShaderSource",  "(II[IILjava/lang/String;)V",  org_mini_gl_GL_glGetShaderSource},
{"org/mini/gl/GL",  "glGetUniformLocation",  "(ILjava/lang/String;)I",  org_mini_gl_GL_glGetUniformLocation},
{"org/mini/gl/GL",  "glGetUniformfv",  "(II[FI)V",  org_mini_gl_GL_glGetUniformfv},
{"org/mini/gl/GL",  "glGetUniformiv",  "(II[II)V",  org_mini_gl_GL_glGetUniformiv},
{"org/mini/gl/GL",  "glGetVertexAttribdv",  "(II[DI)V",  org_mini_gl_GL_glGetVertexAttribdv},
{"org/mini/gl/GL",  "glGetVertexAttribfv",  "(II[FI)V",  org_mini_gl_GL_glGetVertexAttribfv},
{"org/mini/gl/GL",  "glGetVertexAttribiv",  "(II[II)V",  org_mini_gl_GL_glGetVertexAttribiv},
{"org/mini/gl/GL",  "glGetVertexAttribPointerv",  "(II[Ljava/lang/Object;)V",  org_mini_gl_GL_glGetVertexAttribPointerv},
{"org/mini/gl/GL",  "glIsProgram",  "(I)I",  org_mini_gl_GL_glIsProgram},
{"org/mini/gl/GL",  "glIsShader",  "(I)I",  org_mini_gl_GL_glIsShader},
{"org/mini/gl/GL",  "glLinkProgram",  "(I)V",  org_mini_gl_GL_glLinkProgram},
{"org/mini/gl/GL",  "glShaderSource",  "(II[Ljava/lang/String;[II)V",  org_mini_gl_GL_glShaderSource},
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
{"org/mini/gl/GL",  "glTransformFeedbackVaryings",  "(II[Ljava/lang/String;I)V",  org_mini_gl_GL_glTransformFeedbackVaryings},
{"org/mini/gl/GL",  "glGetTransformFeedbackVarying",  "(III[II[II[IILjava/lang/String;)V",  org_mini_gl_GL_glGetTransformFeedbackVarying},
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
{"org/mini/gl/GL",  "glBindFragDataLocation",  "(IILjava/lang/String;)V",  org_mini_gl_GL_glBindFragDataLocation},
{"org/mini/gl/GL",  "glGetFragDataLocation",  "(ILjava/lang/String;)I",  org_mini_gl_GL_glGetFragDataLocation},
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
{"org/mini/gl/GL",  "glMapBufferRange",  "(IJJI)J",  org_mini_gl_GL_glMapBufferRange},
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
{"org/mini/gl/GL",  "glGetUniformIndices",  "(II[Ljava/lang/String;[II)V",  org_mini_gl_GL_glGetUniformIndices},
{"org/mini/gl/GL",  "glGetActiveUniformsiv",  "(II[III[II)V",  org_mini_gl_GL_glGetActiveUniformsiv},
{"org/mini/gl/GL",  "glGetActiveUniformName",  "(III[IILjava/lang/String;)V",  org_mini_gl_GL_glGetActiveUniformName},
{"org/mini/gl/GL",  "glGetUniformBlockIndex",  "(ILjava/lang/String;)I",  org_mini_gl_GL_glGetUniformBlockIndex},
{"org/mini/gl/GL",  "glGetActiveUniformBlockiv",  "(III[II)V",  org_mini_gl_GL_glGetActiveUniformBlockiv},
{"org/mini/gl/GL",  "glGetActiveUniformBlockName",  "(III[IILjava/lang/String;)V",  org_mini_gl_GL_glGetActiveUniformBlockName},
{"org/mini/gl/GL",  "glUniformBlockBinding",  "(III)V",  org_mini_gl_GL_glUniformBlockBinding},
{"org/mini/gl/GL",  "glDrawElementsBaseVertex",  "(IIILjava/lang/Object;II)V",  org_mini_gl_GL_glDrawElementsBaseVertex},
{"org/mini/gl/GL",  "glDrawRangeElementsBaseVertex",  "(IIIIILjava/lang/Object;II)V",  org_mini_gl_GL_glDrawRangeElementsBaseVertex},
{"org/mini/gl/GL",  "glDrawElementsInstancedBaseVertex",  "(IIILjava/lang/Object;III)V",  org_mini_gl_GL_glDrawElementsInstancedBaseVertex},
{"org/mini/gl/GL",  "glMultiDrawElementsBaseVertex",  "(I[III[Ljava/lang/Object;I[II)V",  org_mini_gl_GL_glMultiDrawElementsBaseVertex},
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
{"org/mini/gl/GL",  "glDebugMessageInsert",  "(IIIIILjava/lang/String;)V",  org_mini_gl_GL_glDebugMessageInsert},
{"org/mini/gl/GL",  "glDebugMessageCallback",  "(JLjava/lang/Object;I)V",  org_mini_gl_GL_glDebugMessageCallback},
{"org/mini/gl/GL",  "glGetDebugMessageLog",  "(II[II[II[II[II[IILjava/lang/String;)I",  org_mini_gl_GL_glGetDebugMessageLog},
{"org/mini/gl/GL",  "glPushDebugGroup",  "(IIILjava/lang/String;)V",  org_mini_gl_GL_glPushDebugGroup},
{"org/mini/gl/GL",  "glPopDebugGroup",  "()V",  org_mini_gl_GL_glPopDebugGroup},
{"org/mini/gl/GL",  "glObjectLabel",  "(IIILjava/lang/String;)V",  org_mini_gl_GL_glObjectLabel},
{"org/mini/gl/GL",  "glGetObjectLabel",  "(III[IILjava/lang/String;)V",  org_mini_gl_GL_glGetObjectLabel},
{"org/mini/gl/GL",  "glObjectPtrLabel",  "(Ljava/lang/Object;IILjava/lang/String;)V",  org_mini_gl_GL_glObjectPtrLabel},
{"org/mini/gl/GL",  "glGetObjectPtrLabel",  "(Ljava/lang/Object;II[IILjava/lang/String;)V",  org_mini_gl_GL_glGetObjectPtrLabel},
{"org/mini/gl/GL",  "glDebugMessageControlKHR",  "(IIII[III)V",  org_mini_gl_GL_glDebugMessageControlKHR},
{"org/mini/gl/GL",  "glDebugMessageInsertKHR",  "(IIIIILjava/lang/String;)V",  org_mini_gl_GL_glDebugMessageInsertKHR},
{"org/mini/gl/GL",  "glDebugMessageCallbackKHR",  "(JLjava/lang/Object;I)V",  org_mini_gl_GL_glDebugMessageCallbackKHR},
{"org/mini/gl/GL",  "glGetDebugMessageLogKHR",  "(II[II[II[II[II[IILjava/lang/String;)I",  org_mini_gl_GL_glGetDebugMessageLogKHR},
{"org/mini/gl/GL",  "glPushDebugGroupKHR",  "(IIILjava/lang/String;)V",  org_mini_gl_GL_glPushDebugGroupKHR},
{"org/mini/gl/GL",  "glPopDebugGroupKHR",  "()V",  org_mini_gl_GL_glPopDebugGroupKHR},
{"org/mini/gl/GL",  "glObjectLabelKHR",  "(IIILjava/lang/String;)V",  org_mini_gl_GL_glObjectLabelKHR},
{"org/mini/gl/GL",  "glGetObjectLabelKHR",  "(III[IILjava/lang/String;)V",  org_mini_gl_GL_glGetObjectLabelKHR},
{"org/mini/gl/GL",  "glObjectPtrLabelKHR",  "(Ljava/lang/Object;IILjava/lang/String;)V",  org_mini_gl_GL_glObjectPtrLabelKHR},
{"org/mini/gl/GL",  "glGetObjectPtrLabelKHR",  "(Ljava/lang/Object;II[IILjava/lang/String;)V",  org_mini_gl_GL_glGetObjectPtrLabelKHR},
{"org/mini/gl/GL",  "glGetPointervKHR",  "(I[Ljava/lang/Object;)V",  org_mini_gl_GL_glGetPointervKHR},
};

s32 count_GLFuncTable() {
    return sizeof(method_gl_table) / sizeof(java_native_method);
}

__refer ptr_GLFuncTable() {
    return &method_gl_table[0];
}