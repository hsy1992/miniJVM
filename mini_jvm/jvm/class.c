/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "jvm.h"
#include "jvm_util.h"
#include "garbage.h"


//===============================    创建及加载  ==================================



JClass *class_create(Runtime *runtime) {
    JClass *clazz = jvm_calloc(sizeof(JClass));
    clazz->mb.clazz = clazz;
    clazz->mb.type = MEM_TYPE_CLASS;
    clazz->field_instance_len = 0;
    clazz->field_static = NULL;
    clazz->status = CLASS_STATUS_RAW;
    clazz->_load_class_from_bytes = _LOAD_CLASS_FROM_BYTES;
    //
    jthreadlock_create(&clazz->mb);
    constant_list_create(clazz);
    clazz->arr_class_type = pairlist_create(16);
    clazz->insFieldPtrIndex = arraylist_create(8);
    clazz->staticFieldPtrIndex = arraylist_create(4);
    gc_refer_reg(runtime, clazz);
    return clazz;
}

s32 class_destory(JClass *clazz) {

    _DESTORY_CLASS(clazz);
    pairlist_destory(clazz->arr_class_type);
    arraylist_destory(clazz->insFieldPtrIndex);
    arraylist_destory(clazz->staticFieldPtrIndex);
    jvm_free(clazz);
    return 0;
}

void constant_list_create(JClass *clazz) {
    clazz->constantPool.utf8CP = arraylist_create(0);
    clazz->constantPool.integerCP = arraylist_create(0);
    clazz->constantPool.floatCP = arraylist_create(0);
    clazz->constantPool.longCP = arraylist_create(0);
    clazz->constantPool.doubleCP = arraylist_create(0);
    clazz->constantPool.classRef = arraylist_create(0);
    clazz->constantPool.stringRef = arraylist_create(0);
    clazz->constantPool.fieldRef = arraylist_create(0);
    clazz->constantPool.methodRef = arraylist_create(0);
    clazz->constantPool.interfaceRef = arraylist_create(0);
    clazz->constantPool.name_and_type = arraylist_create(0);
}

void constant_list_destory(JClass *clazz) {
    arraylist_destory(clazz->constantPool.utf8CP);
    arraylist_destory(clazz->constantPool.integerCP);
    arraylist_destory(clazz->constantPool.floatCP);
    arraylist_destory(clazz->constantPool.longCP);
    arraylist_destory(clazz->constantPool.doubleCP);
    arraylist_destory(clazz->constantPool.classRef);
    arraylist_destory(clazz->constantPool.stringRef);
    arraylist_destory(clazz->constantPool.fieldRef);
    arraylist_destory(clazz->constantPool.methodRef);
    arraylist_destory(clazz->constantPool.interfaceRef);
    arraylist_destory(clazz->constantPool.name_and_type);
}

void class_clear_refer(JClass *clazz) {
    s32 i, len;
    if (clazz->field_static) {
        FieldPool *fp = &clazz->fieldPool;
        for (i = 0; i < fp->field_used; i++) {
            FieldInfo *fi = &fp->field[i];
//        if (utf8_equals_c(fi->name, "zones")) {
//            s32 debug = 1;
//        }
            if ((fi->access_flags & ACC_STATIC) != 0 && isDataReferByIndex(fi->datatype_idx)) {
                c8 *ptr = getStaticFieldPtr(fi);
                if (ptr) {
                    setFieldRefer(ptr, NULL);
                }
            }
        }
        if (clazz->field_static)jvm_free(clazz->field_static);
        clazz->field_static = NULL;
    }
    ArrayList *utf8list = clazz->constantPool.utf8CP;
    for (i = 0, len = utf8list->length; i < len; i++) {
        ConstantUTF8 *cutf = arraylist_get_value(utf8list, i);
        gc_refer_release(cutf->jstr);
    }
}
//===============================    初始化相关  ==================================

/**
 * 需要在所有类加载入系统之后
 * 初始化静态变量区，及生成实例模板
 * @param clazz class
 * @return ret
 */
s32 class_prepar(JClass *clazz, Runtime *runtime) {
    if (clazz->status >= CLASS_STATUS_PREPARING)return 0;
    clazz->status = CLASS_STATUS_PREPARING;

//    if (utf8_equals_c(clazz->name, "java/lang/NullPointerException")) {
//        int debug = 1;
//    }


    s32 superid = clazz->cff.super_class;
    if (superid && !clazz->superclass) {
        ConstantClassRef *ccf = class_get_constant_classref(clazz, superid);
        if (ccf) {
            Utf8String *clsName_u = class_get_utf8_string(clazz, ccf->stringIndex);
            JClass *other = classes_load_get_without_clinit(clsName_u, runtime);
            clazz->superclass = other;
        } else {
            jvm_printf("error get superclass , class: %s\n", utf8_cstr(clazz->name));
        }
    }

    int i;


    FieldInfo *f = clazz->fieldPool.field;
    //计算不同种类变量长度
    s32 static_len = 0;
    s32 instance_len = 0;
    for (i = 0; i < clazz->fieldPool.field_used; i++) {
        s32 width = data_type_bytes[f[i].datatype_idx];
        if (f[i].access_flags & ACC_STATIC) {//静态变量
            f[i].offset = static_len;
            static_len += width;
        } else {//实例变量
            f[i].offset = instance_len;
            instance_len += width;
        }
        f[i]._this_class = clazz;
    }
    //静态变量分配
    clazz->field_static_len = static_len;
    clazz->field_static = jvm_calloc(clazz->field_static_len);


//    if (utf8_equals_c(clazz->name, "com/sun/cldc/i18n/mini/ISO8859_1_Writer") == 0) {
//        int debug = 1;
//    }
    //生成实例变量模板
    if (clazz->superclass) {
        clazz->field_instance_start = clazz->superclass->field_instance_len;
        clazz->field_instance_len = clazz->field_instance_start + instance_len;
        //实例变量区前面是继承的父类变量，后面是自己的变量
        //memcpy((clazz->field_instance_template), (superclass->field_instance_template), clazz->field_instance_start);
    } else {
        clazz->field_instance_start = 0;
        //实例变量区前面是继承的父类变量，后面是自己的变量
        clazz->field_instance_len = clazz->field_instance_start + instance_len;
    }


    //提前计算类成员的偏移量，提高执行速度
    for (i = 0; i < clazz->fieldPool.field_used; i++) {
        FieldInfo *fi = &clazz->fieldPool.field[i];
        fi->offset_instance = fi->_this_class->field_instance_start + fi->offset;
    }

    //预计算字段在实例内存中的偏移，节约运行时时间
    if (utf8_equals_c(clazz->name, STR_CLASS_JAVA_LANG_STRING)) {
        FieldInfo *fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_STRING, STR_FIELD_COUNT, "I");
        jvm_runtime_cache.string_count = fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_STRING, STR_FIELD_OFFSET, "I");
        jvm_runtime_cache.string_offset = fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_STRING, STR_FIELD_VALUE, "[C");
        jvm_runtime_cache.string_value = fi;
    } else if (utf8_equals_c(clazz->name, STR_CLASS_JAVA_LANG_THREAD)) {
        FieldInfo *fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_THREAD, STR_FIELD_NAME, "[C");
        jvm_runtime_cache.thread_name = fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_THREAD, STR_FIELD_STACKFRAME, "J");
        jvm_runtime_cache.thread_stackFrame = fi;
    } else if (utf8_equals_c(clazz->name, STR_CLASS_JAVA_LANG_STACKTRACE)) {
        FieldInfo *fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_STACKTRACE, "declaringClass", STR_INS_JAVA_LANG_STRING);
        jvm_runtime_cache.stacktrace_declaringClass = fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_STACKTRACE, "methodName", STR_INS_JAVA_LANG_STRING);
        jvm_runtime_cache.stacktrace_methodName = fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_STACKTRACE, "fileName", STR_INS_JAVA_LANG_STRING);
        jvm_runtime_cache.stacktrace_fileName = fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_STACKTRACE, "lineNumber", "I");
        jvm_runtime_cache.stacktrace_lineNumber = fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_STACKTRACE, "parent", STR_INS_JAVA_LANG_STACKTRACEELEMENT);
        jvm_runtime_cache.stacktrace_parent = fi;
    }
//    jvm_printf("prepared: %s\n", utf8_cstr(clazz->name));

    clazz->status = CLASS_STATUS_PREPARED;
    return 0;
}

/**
 * 执行静态代码，需要在类装入字节码，并初始化好静态变量区之后执行
 * @param clazz class
 * @param runtime  runtime
 */
void class_clinit(JClass *clazz, Runtime *runtime) {
    garbage_thread_lock();
    if (clazz->status < CLASS_STATUS_PREPARED) {
        class_prepar(clazz, runtime);
    }
    if (clazz->status < CLASS_STATUS_CLINITING) {
        clazz->status = CLASS_STATUS_CLINITING;

        s32 i, len;

        /**
         * 把一些索引引用，转为内存对象引用，以此加快字节码执行速度
         * 把ConstantMethodRef.index 指向具体的 MethodInfo ，可能在本类，可能在父类
         * 把ConstantFieldRef.index 指向具体的 FieldInfo 内存
         * @param clazz
         */
        for (i = 0; i < clazz->constantPool.methodRef->length; i++) {
            ConstantMethodRef *cmr = (ConstantMethodRef *) arraylist_get_value(clazz->constantPool.methodRef, i);
            cmr->methodInfo = find_methodInfo_by_methodref(clazz, cmr->index, runtime);
            cmr->virtual_methods = pairlist_create(0);
            //jvm_printf("%s.%s %llx\n", utf8_cstr(clazz->name), utf8_cstr(cmr->name), (s64) (intptr_t) cmr->virtual_methods);
        }

        for (i = 0; i < clazz->constantPool.fieldRef->length; i++) {
            ConstantFieldRef *cfr = (ConstantFieldRef *) arraylist_get_value(clazz->constantPool.fieldRef, i);
            FieldInfo *fi = find_fieldInfo_by_fieldref(clazz, cfr->index, runtime);
            cfr->fieldInfo = fi;
            if (!fi) {
                jvm_printf("field not found %s.%d \n", utf8_cstr(clazz->name), (cfr->nameAndTypeIndex));
            }
            if (fi->_this_class->status < CLASS_STATUS_CLINITED) {
                class_clinit(fi->_this_class, runtime);
            }
        }

        clazz->finalizeMethod = find_methodInfo_by_name_c(utf8_cstr(clazz->name), STR_METHOD_FINALIZE, "()V", runtime);

        // init javastring
        ArrayList *utf8list = clazz->constantPool.utf8CP;
        for (i = 0, len = utf8list->length; i < len; i++) {
            ConstantUTF8 *cutf = arraylist_get_value(utf8list, i);
            Instance *jstr = jstring_create(cutf->utfstr, runtime);
            gc_refer_hold(jstr);
            cutf->jstr = jstr;
        }


        //优先初始化基类
        JClass *superclass = getSuperClass(clazz);
        if (superclass && superclass->status < CLASS_STATUS_CLINITED) {
            class_clinit(superclass, runtime);
        }

        MethodPool *p = &(clazz->methodPool);
        for (i = 0; i < p->method_used; i++) {
            //jvm_printf("%s,%s\n", utf8_cstr(p->methodRef[i].name), utf8_cstr(p->methodRef[i].descriptor));
            if (utf8_equals_c(p->method[i].name, STR_METHOD_CLINIT)) {
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                jvm_printf(" <clinit>  :%s\n", utf8_cstr(clazz->name));
#endif

                s32 ret = execute_method_impl(&(p->method[i]), runtime, clazz);
                if (ret != RUNTIME_STATUS_NORMAL) {
                    print_exception(runtime);
                }
                break;
            }
        }

        clazz->status = CLASS_STATUS_CLINITED;
    }
    garbage_thread_unlock();
}
//===============================    实例化相关  ==================================

u8 instance_of(JClass *clazz, Instance *ins, Runtime *runtime) {
    JClass *ins_of_class = ins->mb.clazz;
    while (ins_of_class) {
        if (ins_of_class == clazz || isSonOfInterface(clazz, ins_of_class->mb.clazz, runtime)) {
            return 1;
        }
        ins_of_class = getSuperClass(ins_of_class);
    }

    return 0;
}

u8 isSonOfInterface(JClass *clazz, JClass *son, Runtime *runtime) {
    s32 i;
    for (i = 0; i < son->interfacePool.clasz_used; i++) {
        ConstantClassRef *ccr = (son->interfacePool.clasz + i);
        JClass *other = classes_load_get(class_get_constant_utf8(son, ccr->stringIndex)->utfstr, runtime);
        if (clazz == other) {
            return 1;
        } else {
            u8 sure = isSonOfInterface(clazz, other, runtime);
            if (sure)return 1;
        }
    }
    return 0;
}

u8 assignable_from(JClass *clazzSon, JClass *clazzSuper) {

    while (clazzSuper) {
        if (clazzSon == clazzSuper) {
            return 1;
        }
        clazzSuper = getSuperClass(clazzSuper);
    }
    return 0;
}

JClass *getSuperClass(JClass *clazz) {
    return clazz->superclass;
}

//===============================    类数据访问  ==================================


JClass *getClassByConstantClassRef(JClass *clazz, s32 index) {
    ConstantClassRef *ccr = class_get_constant_classref(clazz, index);
    return classes_get(ccr->name);
}


s32 find_constant_fieldref_index(JClass *clazz, Utf8String *fieldName, Utf8String *type) {
    s32 i = 0;
    ArrayList *cp = clazz->constantPool.fieldRef;
    for (; i < cp->length; i++) {
        ConstantFieldRef *cfr = ((ConstantFieldRef *) arraylist_get_value(cp, i));
        ConstantNameAndType *nat = class_get_constant_name_and_type(clazz, cfr->nameAndTypeIndex);
        if (utf8_equals(fieldName, class_get_utf8_string(clazz, nat->nameIndex)) == 1 &&
            utf8_equals(type, class_get_utf8_string(clazz, nat->typeIndex)) == 1) {
            return cfr->index;
        }
    }
    return -1;
}


/**
 * 取得类成员信息，成员信息存在以下几种情况
 * ConstantFieldRef中：
 * 父类的静态和实例成员 Fathar.x ，都会描述为  Son.x ,类名描述为本类
 * 而调用其他类（非父类）的静态变量比如：  System.out ，会被描述为 System.out ，类名描述为其他类
 *
 * @param clazz class
 * @param field_ref ref
 * @return fi
 */
FieldInfo *find_fieldInfo_by_fieldref(JClass *clazz, s32 field_ref, Runtime *runtime) {
    FieldInfo *fi = NULL;
    ConstantFieldRef *cfr = class_get_constant_fieldref(clazz, field_ref);
    ConstantNameAndType *nat = class_get_constant_name_and_type(clazz, cfr->nameAndTypeIndex);
    Utf8String *clsName = class_get_utf8_string(clazz,
                                                class_get_constant_classref(clazz, cfr->classIndex)->stringIndex);
    Utf8String *fieldName = class_get_utf8_string(clazz, nat->nameIndex);
    Utf8String *type = class_get_utf8_string(clazz, nat->typeIndex);
    JClass *other = classes_load_get_without_clinit(clsName, runtime);

    while (fi == NULL && other) {
        FieldPool *fp = &(other->fieldPool);
        s32 i = 0;
        for (; i < fp->field_used; i++) {
            if (utf8_equals(fieldName, fp->field[i].name) == 1
                && utf8_equals(type, fp->field[i].descriptor) == 1) {
                fi = &(other->fieldPool.field[i]);
                break;
            }
        }
        other = getSuperClass(other);
    }

    return fi;
}

FieldInfo *find_fieldInfo_by_name_c(c8 *pclsName, c8 *pfieldName, c8 *pfieldType) {
    Utf8String *clsName = utf8_create_c(pclsName);
    Utf8String *fieldName = utf8_create_c(pfieldName);
    Utf8String *fieldType = utf8_create_c(pfieldType);
    FieldInfo *fi = find_fieldInfo_by_name(clsName, fieldName, fieldType);
    utf8_destory(clsName);
    utf8_destory(fieldName);
    utf8_destory(fieldType);
    return fi;
}

FieldInfo *find_fieldInfo_by_name(Utf8String *clsName, Utf8String *fieldName, Utf8String *fieldType) {
    FieldInfo *fi = NULL;
    JClass *other = classes_get(clsName);

    while (fi == NULL && other) {
        FieldPool *fp = &(other->fieldPool);
        s32 i = 0;
        for (; i < fp->field_used; i++) {
            FieldInfo *tmp = &fp->field[i];
            if (utf8_equals(fieldName, tmp->name) == 1
                && utf8_equals(fieldType, tmp->descriptor) == 1
                    ) {
                fi = tmp;
                break;
            }
        }
        other = getSuperClass(other);
    }

    return fi;
}


ConstantMethodRef *
find_constant_methodref_by_name(Utf8String *clsName, Utf8String *methodName, Utf8String *methodType) {
    ConstantMethodRef *cmr = NULL;
    JClass *clazz = classes_get(clsName);
    ArrayList *mrarr = clazz->constantPool.methodRef;
    s32 i;
    for (i = 0; i < mrarr->length; i++) {
        cmr = arraylist_get_value(mrarr, i);
        if (utf8_equals(methodName, cmr->methodInfo->name) == 1
            && utf8_equals(methodType, cmr->methodInfo->descriptor) == 1
                ) {
            return cmr;
        }
    }
    return NULL;
}

/**
 * 查找实例的方法， invokevirtual
 * @param ins ins
 * @param methodName name
 * @param methodType type
 * @return mi
 */
MethodInfo *find_instance_methodInfo_by_name(Instance *ins, Utf8String *methodName, Utf8String *methodType, Runtime *runtime) {
    if (!ins)return NULL;
    return find_methodInfo_by_name(ins->mb.clazz->name, methodName, methodType, runtime);
}

MethodInfo *find_methodInfo_by_methodref(JClass *clazz, s32 method_ref, Runtime *runtime) {
    ConstantMethodRef *cmr = class_get_constant_method_ref(clazz, method_ref);
    Utf8String *clsName = cmr->clsName;
    Utf8String *methodName = cmr->name;
    Utf8String *methodType = cmr->descriptor;
    return find_methodInfo_by_name(clsName, methodName, methodType, runtime);
}

MethodInfo *find_methodInfo_by_name_c(c8 *pclsName, c8 *pmethodName, c8 *pmethodType, Runtime *runtime) {
    Utf8String *clsName = utf8_create_c(pclsName);
    Utf8String *methodName = utf8_create_c(pmethodName);
    Utf8String *methodType = utf8_create_c(pmethodType);
    MethodInfo *mi = find_methodInfo_by_name(clsName, methodName, methodType, runtime);
    utf8_destory(clsName);
    utf8_destory(methodName);
    utf8_destory(methodType);
    return mi;
}

MethodInfo *find_methodInfo_by_name(Utf8String *clsName, Utf8String *methodName, Utf8String *methodType, Runtime *runtime) {
    MethodInfo *mi = NULL;
    JClass *other = classes_load_get_without_clinit(clsName, runtime);

    while (mi == NULL && other) {
        MethodPool *fp = &(other->methodPool);
        s32 i = 0;
        for (; i < fp->method_used; i++) {
            MethodInfo *tmp = &fp->method[i];
            if (utf8_equals(methodName, tmp->name) == 1
                && utf8_equals(methodType, tmp->descriptor) == 1) {
                mi = tmp;
                if (!mi->_this_class)
                    mi->_this_class = other;
                break;
            }
        }
        other = getSuperClass(other);
    }

    return mi;
}


/* Get Major Version String */
c8 *getMajorVersionString(u16 major_number) {
    if (major_number == 0x33)
        return "J2SE 7";
    if (major_number == 0x32)
        return "J2SE 6.0";
    return "NONE";
}
