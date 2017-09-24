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
    Class *clazz = jvm_alloc(sizeof(Class));
    clazz->mb.clazz = clazz;
    clazz->mb.type = MEM_TYPE_CLASS;
    clazz->mb.garbage_mark = GARBAGE_MARK_UNDEF;
    clazz->field_instance_len = 0;
    clazz->field_static = NULL;
    clazz->field_instance_template = NULL;
    clazz->status = CLASS_STATUS_RAW;
    clazz->_load_from_file = _LOAD_FROM_FILE;
    //
    _INIT_CLASS(clazz);
    constant_list_create(clazz);
    return clazz;
}

void _INIT_CLASS(Class *_this) {
    jthreadlock_create(&_this->mb);
}

s32 class_destory(Class *clazz) {
    _DESTORY_CLASS(clazz);
    jvm_free(clazz);
}

s32 _DESTORY_CLASS(Class *_this) {
    _class_method_info_destory(_this);
    _class_interface_pool_destory(_this);
    _class_field_info_destory(_this);
    _class_constant_pool_destory(_this);
    jvm_free(_this->field_static);
    _this->field_static = NULL;
    jvm_free(_this->field_instance_template);
    _this->field_instance_template = NULL;
    jvm_free(_this->constant_item_ptr);
    _this->constant_item_ptr = NULL;
    jthreadlock_destory(_this->mb.thread_lock);
    constant_list_destory(_this);
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
//===============================    初始化相关  ==================================

/**
 * 需要在所有类加载入系统之后
 * 初始化静态变量区，及生成实例模板
 * @param clazz
 * @return
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
    for (i = 0; i < clazz->constantPool.fieldRef->length; i++) {
        ConstantFieldRef *cfr = (ConstantFieldRef *) arraylist_get_value(clazz->constantPool.fieldRef, i);
        cfr->fieldInfo = find_fieldInfo_by_fieldref(clazz, cfr->index);
    }
    for (i = 0; i < clazz->constantPool.methodRef->length; i++) {
        ConstantMethodRef *cmr = (ConstantMethodRef *) arraylist_get_value(clazz->constantPool.methodRef, i);
        cmr->methodInfo = find_methodInfo_by_methodref(clazz, cmr->index);
    }


    FieldInfo *f = clazz->fieldPool.field;
    //计算不同种类变量长度
    s32 static_len = 0;
    s32 instance_len = 0;
    for (i = 0; i < clazz->fieldPool.field_used; i++) {
        s32 width = data_type_bytes[getDataTypeIndex(utf8_char_at(f[i].descriptor, 0))];
        if (f[i].access_flags & ACC_STATIC) {//静态变量
            f[i].offset = static_len;
            static_len += width;
        } else {//实例变量
            f[i].offset = instance_len;
            instance_len += width;
        }
    }
    //静态变量分配
    clazz->field_static_len = static_len;
    clazz->field_static = jvm_alloc(clazz->field_static_len);

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
        clazz->field_instance_template = jvm_alloc(clazz->field_instance_len);
        //实例变量区前面是继承的父类变量，后面是自己的变量
        //memcpy((clazz->field_instance_template), (superclass->field_instance_template), clazz->field_instance_start);
    } else {
        clazz->field_instance_start = 0;
        //实例变量区前面是继承的父类变量，后面是自己的变量
        clazz->field_instance_len = clazz->field_instance_start + instance_len;
        clazz->field_instance_template = jvm_alloc(clazz->field_instance_len);
    }

    clazz->status = CLASS_STATUS_PREPARED;
    return 0;
}

/**
 * 执行静态代码，需要在类装入字节码，并初始化好静态变量区之后执行
 * @param clazz
 * @param runtime
 */
void class_clinit(Class *clazz, Runtime *runtime) {
    if (clazz->status < CLASS_STATUS_PREPARED) {
        class_prepar(clazz);
    }
    if (clazz->status >= CLASS_STATUS_CLINITING)return;
    clazz->status = CLASS_STATUS_CLINITING;
    //优先初始化基类
    Class *superclass = getSuperClass(clazz);
    if (superclass && superclass->status < CLASS_STATUS_CLINITED) {
        class_clinit(superclass, runtime);
    }

    MethodPool *p = &(clazz->methodPool);
    s32 i;
    for (i = 0; i < p->method_used; i++) {
        //jvm_printf("%s,%s\n", utf8_cstr(p->methodRef[i].name), utf8_cstr(p->methodRef[i].descriptor));
        if (utf8_equals_c(p->method[i].name, "<clinit>") == 1) {
#if _JVM_DEBUG > 5
            jvm_printf("%s <clinit>\n", utf8_cstr(clazz->name));
#endif
            execute_method(&(p->method[i]), runtime, clazz);
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
    s32 superid = clazz->cff.super_class;
    if (!superid)return NULL;
    ConstantClassRef *ccf = find_constant_classref(clazz, superid);
    if (ccf) {
        Utf8String *clsName_u = get_utf8_string(clazz, ccf->stringIndex);
        Class *other = classes_get(clsName_u);
        return other;
    }
    return NULL;
}
