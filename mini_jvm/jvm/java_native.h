

#ifndef _JAVA_LIBRARY_H
#define _JAVA_LIBRARY_H

#include "jvm.h"

typedef s32 (*java_native_lib)(Runtime *runtime, Class *p, c8 *type);

typedef struct _java_lang_method {
    c8 *clzname;
    c8 *methodname;
    java_native_lib method_runtime;
} java_native_method;

s32 invoke_native_method(Runtime *runtime, Class *p,
                         c8 *cls_name, c8 *method_name, c8 *type);


#endif
