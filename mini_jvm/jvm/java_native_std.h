

#ifndef _JAVA_LIBRARY_H
#define _JAVA_LIBRARY_H

#include "jvm.h"


typedef struct _java_native_method {
    c8 *clzname;
    c8 *methodname;
    c8 *methodtype;
    java_native_fun func_pointer;
} java_native_method;

java_native_method *find_native_method(c8 *cls_name, c8 *method_name, c8 *method_type);

s32 invoke_native_method(Runtime *runtime, Class *p,
                         c8 *cls_name, c8 *method_name, c8 *type);


typedef struct _JavaNativeLib {
    java_native_method *methods;
    s32 methods_count;
} JavaNativeLib;

void reg_std_native_lib();

s32 native_reg_lib(java_native_method *methods, s32 method_size);

s32 native_remove_lib(JavaNativeLib *lib);

void reg_net_native_lib();

void reg_jdwp_native_lib();


#endif
