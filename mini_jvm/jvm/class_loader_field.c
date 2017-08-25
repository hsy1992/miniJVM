
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jvm.h"
#include "garbage.h"


/**
 * 取得类成员信息，成员信息存在以下几种情况
 * ConstantFieldRef中：
 * 父类的静态和实例成员 Fathar.x ，都会描述为  Son.x ,类名描述为本类
 * 而调用其他类（非父类）的静态变量比如：  System.out ，会被描述为 System.out ，类名描述为其他类
 *
 * @param clazz
 * @param field_ref
 * @return
 */
FieldInfo *find_fieldInfo_by_fieldref(Class *clazz, s32 field_ref) {
    FieldInfo *fi = NULL;
    ConstantFieldRef *cfr = find_constant_fieldref(clazz, field_ref);
    ConstantNameAndType *nat = find_constant_name_and_type(clazz, cfr->nameAndTypeIndex);
    Utf8String *clsName = get_utf8_string(clazz, find_constant_classref(clazz, cfr->classIndex)->stringIndex);
    Utf8String *fieldName = get_utf8_string(clazz, nat->nameIndex);
    Utf8String *type = get_utf8_string(clazz, nat->typeIndex);
    Class *other = hashtable_get(classes, clsName);

    while (fi == NULL && other) {
        FieldPool *fp = &(other->fieldPool);
        s32 i = 0;
        for (; i < fp->field_used; i++) {
            if (utf8_equals(fieldName, fp->field[i].name) == 0
                && utf8_equals(type, fp->field[i].descriptor) == 0) {
                fi = &(other->fieldPool.field[i]);
                fi->_this_class = other;//把类引用赋值
                break;
            }
        }
        other = getSuperClass(other);
    }

    return fi;
}

FieldInfo *find_fieldInfo_by_name(Utf8String *clsName, Utf8String *fieldName, Utf8String *fieldType) {
    FieldInfo *fi = NULL;
    Class *other = hashtable_get(classes, clsName);

    while (fi == NULL && other) {
        FieldPool *fp = &(other->fieldPool);
        s32 i = 0;
        for (; i < fp->field_used; i++) {
            FieldInfo *tmp = &fp->field[i];
            if (utf8_equals(fieldName, tmp->name) == 0
                && utf8_equals(fieldType, tmp->descriptor) == 0
                    ) {
                fi = tmp;
                fi->_this_class = other;
                break;
            }
        }
        other = getSuperClass(other);
    }

    return fi;
}


static s32 parseAttr(FieldInfo *ptr, FILE *fp) {
    s32 i;
    AttributeInfo *tmp = 0;
    u8 short_tmp[2];
    u8 integer_tmp[4];
#if 0
    printf("fieldRef attributes_count = %d\n", arr_body->attributes_count);
#endif
    for (i = 0; i < ptr->attributes_count; i++) {
        tmp = &(ptr->attributes[i]);
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

        if (tmp->attribute_length > 0) {
            tmp->info = (u8 *) jvm_alloc(sizeof(u8) * tmp->attribute_length);
            fread(tmp->info, tmp->attribute_length, 1, fp);
        } else {
            tmp->info = NULL;
        }
    }
}

/* parse Field Pool */
static s32 parseFP(Class *_this, FILE *fp) {
    s32 i = 0;
    u8 short_tmp[2];
    FieldInfo *ptr = &(_this->fieldPool.field[_this->fieldPool.field_used]);

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

    if (ptr->attributes_count > 0) {
        ptr->attributes = (AttributeInfo *) jvm_alloc(sizeof(AttributeInfo) * ptr->attributes_count);
    } else {
        ptr->attributes = NULL;
    }
    /* parse attributes */
    parseAttr(ptr, fp);
    _this->fieldPool.field_used++;
    return 0;
}

void printFieldPool(Class *clazz, FieldPool *fp) {
    s32 i, j;

    printf("Field Pool==================== \n");
    if (fp->field_used > 0) {
        for (i = 0; i < fp->field_used; i++) {
            ConstantUTF8 *ptr = find_constant_utf8(clazz, fp->field[i].name_index);
            printf("fieldRef[%d], attr_count = %d, %d",
                   i, fp->field[i].attributes_count,
                   fp->field[i].name_index);
            if (ptr != 0) {
                printf(" %s\n", utf8_cstr(ptr->ptr));
            } else {
                printf("\n");
            }
        }
    }
}

s32 _parse_field_pool(Class *_this, FILE *fp, s32 count) {
    s32 size = sizeof(FieldInfo) * count;
    _this->fieldPool.field = jvm_alloc(size);

    s32 i;
    for (i = 0; i < count; i++)
        parseFP(_this, fp);
    return 0;
}


s32 _class_field_info_destory(Class *clazz) {
    s32 i, j;
    for (i = 0; i < clazz->fieldPool.field_used; i++) {
        FieldPool *fi = &clazz->fieldPool.field[i];
        for (j = 0; j < fi->field[i].attributes_count; j++) {
            AttributeInfo *attr = &fi->field[i].attributes[j];
            jvm_free(attr->info);//info已被转换为converted_attribute
            attr->info = NULL;
        }
        jvm_free(fi->field->attributes);
        fi->field->attributes = NULL;
    }
    jvm_free(clazz->fieldPool.field);
    clazz->fieldPool.field = NULL;
    return 0;
}