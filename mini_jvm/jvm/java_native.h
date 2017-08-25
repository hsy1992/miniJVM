

#ifndef _JAVA_LIBRARY_H
#define _JAVA_LIBRARY_H

#include "jvm.h"



typedef struct _java_lang_method {
    c8 *clzname;
    c8 *methodname;
    java_native_fun func_pointer;
} java_native_method;

java_native_method *find_native_method(c8 *cls_name, c8 *method_name, c8 *method_type);

s32 invoke_native_method(Runtime *runtime, Class *p,
                         c8 *cls_name, c8 *method_name, c8 *type);


#endif
