//
// Created by gust on 2017/9/16.
//

#ifndef MINI_JVM_JAVA_NATIVE_JDWP_H
#define MINI_JVM_JAVA_NATIVE_JDWP_H


#include "jvm.h"


#ifdef __cplusplus
extern "C" {
#endif

static c8 *JDWP_CLASS_REFERENCE = "javax/mini/reflect/Reference";
static c8 *JDWP_CLASS_FIELD = "javax/mini/reflect/Field";
static c8 *JDWP_CLASS_METHOD = "javax/mini/reflect/Method";
static c8 *JDWP_CLASS_ARRAY = "javax/mini/reflect/Array";
static c8 *JDWP_CLASS_RUNTIME = "javax/mini/reflect/StackFrame";
static c8 *JDWP_CLASS_LOCALVARTABLE = "javax/mini/reflect/LocalVarTable";
static c8 *JDWP_CLASS_VALUETYPE = "javax/mini/reflect/vm/ValueType";


#ifdef __cplusplus
}
#endif


#endif //MINI_JVM_JAVA_NATIVE_JDWP_H
