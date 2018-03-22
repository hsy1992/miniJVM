/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "jvm.h"
#include "jvm_util.h"
#include "garbage.h"


//===============================    创建及加载  ==================================



Class *class_create() {
    Class *clazz = jvm_calloc(sizeof(Class));
    clazz->mb.clazz = clazz;
    clazz->mb.type = MEM_TYPE_CLASS;
    clazz->field_instance_len = 0;
    clazz->field_static = NULL;
    clazz->status = CLASS_STATUS_RAW;
    clazz->_load_class_from_bytes = _LOAD_CLASS_FROM_BYTES;
    //
    jthreadlock_create(&clazz->mb);
    constant_list_create(clazz);
    return clazz;
}

s32 class_destory(Class *clazz) {

    _DESTORY_CLASS(clazz);
    jvm_free(clazz);
    return 0;
}

void constant_list_create(Class *clazz) {
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

void constant_list_destory(Class *clazz) {
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

void class_clear_refer(Class *clazz) {
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
        garbage_refer_release(cutf->jstr);
    }
}
//===============================    初始化相关  ==================================

/**
 * 需要在所有类加载入系统之后
 * 初始化静态变量区，及生成实例模板
 * @param clazz class
 * @return ret
 */
s32 class_prepar(Class *clazz) {
    if (clazz->status >= CLASS_STATUS_PREPARING)return 0;
    clazz->status = CLASS_STATUS_PREPARING;
    int i;

/**
 * 把一些索引引用，转为内存对象引用，以此加快字节码执行速度
 * 把ConstantMethodRef.index 指向具体的 MethodInfo ，可能在本类，可能在父类
 * 把ConstantFieldRef.index 指向具体的 FieldInfo 内存
 * @param clazz
 */
//    if (utf8_equals_c(clazz->name, "javax/mini/eio/socket/PrivateOutputStream")) {
//        int debug = 1;
//    }

    for (i = 0; i < clazz->constantPool.methodRef->length; i++) {
        ConstantMethodRef *cmr = (ConstantMethodRef *) arraylist_get_value(clazz->constantPool.methodRef, i);
        cmr->methodInfo = find_methodInfo_by_methodref(clazz, cmr->index);
        cmr->virtual_methods = pairlist_create(0);
        //jvm_printf("%s.%s %llx\n", utf8_cstr(clazz->name), utf8_cstr(cmr->name), (s64) (intptr_t) cmr->virtual_methods);
    }


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
    Class *superclass = getSuperClass(clazz);
    if (superclass) {
        if (superclass->status != CLASS_STATUS_PREPARED) {
            class_prepar(superclass);
        }
        clazz->field_instance_start = superclass->field_instance_len;
        clazz->field_instance_len = clazz->field_instance_start + instance_len;
        //实例变量区前面是继承的父类变量，后面是自己的变量
        //memcpy((clazz->field_instance_template), (superclass->field_instance_template), clazz->field_instance_start);
    } else {
        clazz->field_instance_start = 0;
        //实例变量区前面是继承的父类变量，后面是自己的变量
        clazz->field_instance_len = clazz->field_instance_start + instance_len;
    }

    //预计算字段在实例内存中的偏移，节约运行时时间
    if (utf8_equals_c(clazz->name, STR_CLASS_JAVA_LANG_STRING)) {
        FieldInfo *fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_STRING, STR_FIELD_COUNT, "I");
        ins_field_offset.string_count = fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_STRING, STR_FIELD_OFFSET, "I");
        ins_field_offset.string_offset = fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_STRING, STR_FIELD_VALUE, "[C");
        ins_field_offset.string_value = fi;
    } else if (utf8_equals_c(clazz->name, STR_CLASS_JAVA_LANG_THREAD)) {
        FieldInfo *fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_THREAD, STR_FIELD_NAME, "[C");
        ins_field_offset.thread_name = fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_THREAD, STR_FIELD_STACKFRAME, "J");
        ins_field_offset.thread_stackFrame = fi;
    } else if (utf8_equals_c(clazz->name, STR_CLASS_JAVA_LANG_STACKTRACE)) {
        FieldInfo *fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_STACKTRACE, "declaringClass", STR_INS_JAVA_LANG_STRING);
        ins_field_offset.stacktrace_declaringClass = fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_STACKTRACE, "methodName", STR_INS_JAVA_LANG_STRING);
        ins_field_offset.stacktrace_methodName = fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_STACKTRACE, "fileName", STR_INS_JAVA_LANG_STRING);
        ins_field_offset.stacktrace_fileName = fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_STACKTRACE, "lineNumber", "I");
        ins_field_offset.stacktrace_lineNumber = fi;
        fi = find_fieldInfo_by_name_c(STR_CLASS_JAVA_LANG_STACKTRACE, "parent", STR_INS_JAVA_LANG_STACKTRACEELEMENT);
        ins_field_offset.stacktrace_parent = fi;
    }

    clazz->status = CLASS_STATUS_PREPARED;
    return 0;
}

/**
 * 执行静态代码，需要在类装入字节码，并初始化好静态变量区之后执行
 * @param clazz class
 * @param runtime  runtime
 */
void class_clinit(Class *clazz, Runtime *runtime) {
    if (clazz->status < CLASS_STATUS_PREPARED) {
        class_prepar(clazz);
    }
    if (clazz->status >= CLASS_STATUS_CLINITING)return;
    clazz->status = CLASS_STATUS_CLINITING;
    // init javastring
    s32 i, len;
    for (i = 0; i < clazz->constantPool.fieldRef->length; i++) {
        ConstantFieldRef *cfr = (ConstantFieldRef *) arraylist_get_value(clazz->constantPool.fieldRef, i);
        FieldInfo *fi = find_fieldInfo_by_fieldref(clazz, cfr->index, runtime);
        cfr->fieldInfo = fi;
        if (cfr->fieldInfo == NULL) {
            int debug = 1;
        }
    }
    for (i = 0; i < clazz->fieldPool.field_used; i++) {
        FieldInfo *fi = &clazz->fieldPool.field[i];

        fi->offset_instance = fi->_this_class->field_instance_start + fi->offset;
    }
    ArrayList *utf8list = clazz->constantPool.utf8CP;
    for (i = 0, len = utf8list->length; i < len; i++) {
        ConstantUTF8 *cutf = arraylist_get_value(utf8list, i);
        Instance *jstr = jstring_create(cutf->utfstr, runtime);
        garbage_refer_hold(jstr);
        cutf->jstr = jstr;
    }


    //优先初始化基类
    Class *superclass = getSuperClass(clazz);
    if (superclass && superclass->status < CLASS_STATUS_CLINITED) {
        class_clinit(superclass, runtime);
    }

    MethodPool *p = &(clazz->methodPool);
    for (i = 0; i < p->method_used; i++) {
        //jvm_printf("%s,%s\n", utf8_cstr(p->methodRef[i].name), utf8_cstr(p->methodRef[i].descriptor));
        if (utf8_equals_c(p->method[i].name, "<clinit>") == 1) {
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
            jvm_printf("%s <clinit>\n", utf8_cstr(clazz->name));
#endif

            s32 ret = execute_method(&(p->method[i]), runtime, clazz);
            if (ret != RUNTIME_STATUS_NORMAL) {
                print_exception(runtime);
            }
            break;
        }
    }

    clazz->status = CLASS_STATUS_CLINITED;
}
//===============================    实例化相关  ==================================

u8 instance_of(Class *clazz, Instance *ins) {
    Class *ins_of_class = ins->mb.clazz;
    while (ins_of_class) {
        if (ins_of_class == clazz || isSonOfInterface(clazz, ins_of_class->mb.clazz)) {
            return 1;
        }
        ins_of_class = getSuperClass(ins_of_class);
    }

    return 0;
}

u8 isSonOfInterface(Class *clazz, Class *son) {
    s32 i;
    for (i = 0; i < son->interfacePool.clasz_used; i++) {
        ConstantClassRef *ccr = (son->interfacePool.clasz + i);
        Class *other = classes_get(find_constant_utf8(son, ccr->stringIndex)->utfstr);
        if (clazz == other) {
            return 1;
        } else {
            u8 sure = isSonOfInterface(clazz, other);
            if (sure)return 1;
        }
    }
    return 0;
}

u8 assignable_from(Class *clazzSon, Class *clazzSuper) {

    while (clazzSuper) {
        if (clazzSon == clazzSuper) {
            return 1;
        }
        clazzSuper = getSuperClass(clazzSuper);
    }
    return 0;
}

Class *getSuperClass(Class *clazz) {
    if (clazz->superclass) {
        return clazz->superclass;
    } else {
        s32 superid = clazz->cff.super_class;
        if (!superid)return NULL;
        ConstantClassRef *ccf = find_constant_classref(clazz, superid);
        if (ccf) {
            Utf8String *clsName_u = get_utf8_string(clazz, ccf->stringIndex);
            Class *other = classes_get(clsName_u);
            clazz->superclass = other;
            return other;
        }
        return NULL;
    }
}

//===============================    类数据访问  ==================================


/* find UTF8 */
inline ConstantUTF8 *find_constant_utf8(Class *clazz, s32 index) {
    return (ConstantUTF8 *) (clazz->constant_item_ptr[index]);
}

/* Find Class Reference */
inline ConstantStringRef *find_constant_stringref(Class *clazz, s32 index) {
    return (ConstantStringRef *) (clazz->constant_item_ptr[index]);
}


/* Find Class Reference */
inline ConstantClassRef *find_constant_classref(Class *clazz, s32 index) {
    return (ConstantClassRef *) (clazz->constant_item_ptr[index]);
}

inline Class *getClassByConstantClassRef(Class *clazz, s32 index) {
    ConstantClassRef *ccr = find_constant_classref(clazz, index);
    return classes_get(ccr->name);
}

inline ConstantFieldRef *find_constant_fieldref(Class *clazz, s32 index) {
    return (ConstantFieldRef *) (clazz->constant_item_ptr[index]);
}

inline ConstantItem *find_constant_item(Class *clazz, s32 index) {
    return (ConstantItem *) (clazz->constant_item_ptr[index]);
}

s32 find_constant_fieldref_index(Class *clazz, Utf8String *fieldName, Utf8String *type) {
    s32 i = 0;
    ArrayList *cp = clazz->constantPool.fieldRef;
    for (; i < cp->length; i++) {
        ConstantFieldRef *cfr = ((ConstantFieldRef *) arraylist_get_value(cp, i));
        ConstantNameAndType *nat = find_constant_name_and_type(clazz, cfr->nameAndTypeIndex);
        if (utf8_equals(fieldName, get_utf8_string(clazz, nat->nameIndex)) == 1 &&
            utf8_equals(type, get_utf8_string(clazz, nat->typeIndex)) == 1) {
            return cfr->index;
        }
    }
    return -1;
}

/* Find Method Reference */
inline ConstantMethodRef *find_constant_method_ref(Class *clazz, s32 index) {
    return (ConstantMethodRef *) (clazz->constant_item_ptr[index]);
}

inline ConstantInterfaceMethodRef *find_constant_interface_method_ref(Class *clazz, s32 index) {
    return (ConstantInterfaceMethodRef *) (clazz->constant_item_ptr[index]);
}

/* Find Name and Type Reference */
inline ConstantNameAndType *find_constant_name_and_type(Class *clazz, s32 index) {
    return (ConstantNameAndType *) (clazz->constant_item_ptr[index]);
}

/* get integer from constant pool */
inline s32 get_constant_integer(Class *clazz, s32 index) {
    return ((ConstantInteger *) (clazz->constant_item_ptr[index]))->value;
}

/* get long from constant pool */
s64 get_constant_long(Class *clazz, s32 index) {
    return ((ConstantLong *) (clazz->constant_item_ptr[index]))->value;
}

/* get f32 from constant pool */
inline f32 get_constant_float(Class *clazz, s32 index) {
    return ((ConstantFloat *) (clazz->constant_item_ptr[index]))->value;
}

/* get f64 from constant pool */
inline f64 get_double_from_constant_pool(Class *clazz, s32 index) {
    return ((ConstantDouble *) (clazz->constant_item_ptr[index]))->value;
}


inline Utf8String *get_utf8_string(Class *clazz, s32 index) {
    return ((ConstantUTF8 *) (clazz->constant_item_ptr[index]))->utfstr;
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
FieldInfo *find_fieldInfo_by_fieldref(Class *clazz, s32 field_ref, Runtime *runtime) {
    FieldInfo *fi = NULL;
    ConstantFieldRef *cfr = find_constant_fieldref(clazz, field_ref);
    ConstantNameAndType *nat = find_constant_name_and_type(clazz, cfr->nameAndTypeIndex);
    Utf8String *clsName = get_utf8_string(clazz, find_constant_classref(clazz, cfr->classIndex)->stringIndex);
    Utf8String *fieldName = get_utf8_string(clazz, nat->nameIndex);
    Utf8String *type = get_utf8_string(clazz, nat->typeIndex);
    Class *other = classes_load_get(clsName, runtime);

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
    Class *other = classes_get(clsName);

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
    Class *clazz = classes_get(clsName);
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
MethodInfo *find_instance_methodInfo_by_name(Instance *ins, Utf8String *methodName, Utf8String *methodType) {
    if (!ins)return NULL;
    return find_methodInfo_by_name(ins->mb.clazz->name, methodName, methodType);
}

MethodInfo *find_methodInfo_by_methodref(Class *clazz, s32 method_ref) {
    MethodInfo *mi = NULL;
    ConstantMethodRef *cmr = find_constant_method_ref(clazz, method_ref);
    Utf8String *clsName = cmr->clsName;
    Utf8String *methodName = cmr->name;
    Utf8String *methodType = cmr->descriptor;
    return find_methodInfo_by_name(clsName, methodName, methodType);
}


MethodInfo *find_methodInfo_by_name(Utf8String *clsName, Utf8String *methodName, Utf8String *methodType) {
    MethodInfo *mi = NULL;
    Class *other = classes_get(clsName);

    while (mi == NULL && other) {
        MethodPool *fp = &(other->methodPool);
        s32 i = 0;
        for (; i < fp->method_used; i++) {
            MethodInfo *tmp = &fp->method[i];
            if (utf8_equals(methodName, tmp->name) == 1
                && utf8_equals(methodType, tmp->descriptor) == 1
                    ) {
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
