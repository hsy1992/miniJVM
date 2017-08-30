

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jvm.h"
#include "garbage.h"


ConstantMethodRef *
find_constant_methodref_by_name(Utf8String *clsName, Utf8String *methodName, Utf8String *methodType) {
    ConstantMethodRef *cmr = NULL;
    Class *clazz = classes_get( clsName);
    ArrayList *mrarr = clazz->constantPool.methodRef;
    s32 i;
    for (; i < mrarr->length; i++) {
        cmr = arraylist_get_value(mrarr, i);
        if (utf8_equals(methodName, cmr->methodInfo->name) == 0
            && utf8_equals(methodType, cmr->methodInfo->descriptor) == 0
                ) {
            return cmr;
        }
    }
    return NULL;
}

/**
 * 查找实例的方法， invokevirtual
 * @param ins
 * @param methodName
 * @param methodType
 * @return
 */
MethodInfo *find_instance_methodInfo_by_name(Instance *ins, Utf8String *methodName, Utf8String *methodType) {
    if (!ins)return NULL;
    return find_methodInfo_by_name(ins->obj_of_clazz->name, methodName, methodType);
}

MethodInfo *find_methodInfo_by_methodref(Class *clazz, s32 method_ref) {
    MethodInfo *mi = NULL;
    ConstantMethodRef *cfr = find_constant_method_ref(clazz, method_ref);
    ConstantNameAndType *nat = find_constant_name_and_type(clazz, cfr->nameAndTypeIndex);
    Utf8String *clsName = get_utf8_string(clazz, find_constant_classref(clazz, cfr->classIndex)->stringIndex);
    Utf8String *methodName = get_utf8_string(clazz, nat->nameIndex);
    Utf8String *methodType = get_utf8_string(clazz, nat->typeIndex);
    return find_methodInfo_by_name(clsName, methodName, methodType);
}


MethodInfo *find_methodInfo_by_name(Utf8String *clsName, Utf8String *methodName, Utf8String *methodType) {
    MethodInfo *mi = NULL;
    Class *other = classes_get( clsName);

    while (mi == NULL && other) {
        MethodPool *fp = &(other->methodPool);
        s32 i = 0;
        for (; i < fp->method_used; i++) {
            MethodInfo *tmp = &fp->method[i];
            if (utf8_equals(methodName, tmp->name) == 0
                && utf8_equals(methodType, tmp->descriptor) == 0
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

//=================================     load         ======================================


static s32 parseMethodAttr(MethodInfo *ptr, FILE *fp) {
    s32 i;
    AttributeInfo *tmp = 0;
    u8 short_tmp[2];
    u8 integer_tmp[4];
    for (i = 0; i < ptr->attributes_count; i++) {
        tmp = &ptr->attributes[i];
        fread(short_tmp, 2, 1, fp);
        Short2Char s2c;
        s2c.c1 = short_tmp[0];
        s2c.c0 = short_tmp[1];
        tmp->attribute_name_index = s2c.s;
        fread(integer_tmp, 4, 1, fp);
        Int2Float i2c;
        i2c.c3 = integer_tmp[0];
        i2c.c2 = integer_tmp[1];
        i2c.c1 = integer_tmp[2];
        i2c.c0 = integer_tmp[3];
        tmp->attribute_length = i2c.i;

        tmp->info = (u8 *) jvm_alloc(sizeof(u8) * tmp->attribute_length);
        fread(tmp->info, tmp->attribute_length, 1, fp);
    }
}


/* parse Method Pool */
static s32 parseMP(Class *_this, FILE *fp) {
    s32 i = 0;
    u8 short_tmp[2];
    MethodInfo *ptr = &(_this->methodPool.method[_this->methodPool.method_used]);

    /* access flag */
    fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    ptr->access_flags = s2c.s;

    /* name index */
    fread(short_tmp, 2, 1, fp);
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    ptr->name_index = s2c.s;

    /* descriptor index */
    fread(short_tmp, 2, 1, fp);
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    ptr->descriptor_index = s2c.s;

    /* attributes count */
    fread(short_tmp, 2, 1, fp);
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    ptr->attributes_count = s2c.s;

    ptr->attributes = (AttributeInfo *) jvm_alloc(sizeof(AttributeInfo) * ptr->attributes_count);
    memset(ptr->attributes, 0, sizeof(AttributeInfo) * ptr->attributes_count);
    /* parse methodRef attributes */
    parseMethodAttr(ptr, fp);

    _this->methodPool.method_used++;
    return 0;
}

/* Print Method Attributes */
void printMethodAttributes(Class *p, MethodInfo *method) {
    s32 i;
    AttributeInfo *attr = 0;

    for (i = 0; i < method->attributes_count; i++) {
        attr = &method->attributes[i];

        printf("attribute name = %s\n", utf8_cstr(get_utf8_string(p, attr->attribute_name_index)));
    }
}

/* Print Method Pool */
void printMethodPool(Class *p, MethodPool *mp) {
    s32 i, j;
    printf("Method Pool===================== \n");
    if (mp->method_used > 0) {
        for (i = 0; i < mp->method_used; i++) {
            ConstantUTF8 *ptr = find_constant_utf8(p, mp->method[i].name_index);
            ConstantUTF8 *ptr2 = find_constant_utf8(p, mp->method[i].descriptor_index);
            ConstantMethodRef *mRefPtr = find_constant_method_ref(p, mp->method[i].name_index);
            printf("methodRef[%d], attr_count = %d, %d",
                   i, mp->method[i].attributes_count,
                   mp->method[i].name_index);
            if (ptr != 0) {
                printf(" %s", utf8_cstr(ptr->utfstr));
            }
#if 0
                else if (mRefPtr != 0) {
                    ConstantClassRef *obj_of_clazz = find_class_ref(p, mRefPtr->classIndex);
                    if (obj_of_clazz != 0) {
                        ConstantUTF8 *name = find_utf8(p, obj_of_clazz->stringIndex);
                        if (name != 0) {
                            printf(" ");
                            for (j = 0; j < name->string_size; j++)
                                printf("%c", name->arr_body[j]);
                            printf(" ");

                        }
                    }
                    ConstantNameAndType *nameAndType = find_name_and_type(p, mRefPtr->nameAndTypeIndex);
                    if (nameAndType != 0) {
                        ConstantUTF8 *name = find_utf8(p, nameAndType->nameIndex);
                        ConstantUTF8 *type = find_utf8(p, nameAndType->typeIndex);

                        if (name != 0) {
                            printf(" ");
                            for (j = 0; j < name->string_size; j++)
                                printf("%c", name->arr_body[j]);
                            printf(" ");
                        }
                        if (type != 0) {
                            printf(" ");
                            for (j = 0; j < type->string_size; j++)
                                printf("%c", type->arr_body[j]);
                            printf(" ");
                        }
                    }
                    printf("\n");
                }
#endif
            else {
                printf("\n");
                continue;
            }
            if (ptr2 != 0) {
                printf("");
                printf("%s\n", utf8_cstr(ptr2->utfstr));
            }
            printMethodAttributes(p, &mp->method[i]);
        }
    }
}

s32 _parse_method_pool(Class *_this, FILE *fp, s32 count) {

    s32 size = sizeof(MethodInfo) * count;
    _this->methodPool.method = jvm_alloc(size);
    memset(_this->methodPool.method, 0, size);
    s32 i;
    for (i = 0; i < count; i++)
        parseMP(_this, fp);
    return 0;
}

s32 _class_method_info_destory(Class *clazz) {
    s32 i, j;
    for (i = 0; i < clazz->methodPool.method_used; i++) {
        MethodInfo *mi = &clazz->methodPool.method[i];
        for (j = 0; j < mi->attributes_count; j++) {
            AttributeInfo *attr = &mi->attributes[j];
            jvm_free(attr->info);//某些没有转
            attr->info = NULL;
            jvm_free(attr->converted_code);//info已被转换为converted_attribute
            attr->converted_code = NULL;
        }
        jvm_free(mi->attributes);
        mi->attributes = NULL;
    }
    jvm_free(clazz->methodPool.method);
    clazz->methodPool.method = NULL;
    return 0;
}