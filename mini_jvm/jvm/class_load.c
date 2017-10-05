//
// Created by gust on 2017/9/25.
//

#include "stdlib.h"
#include <stdio.h>
#include <string.h>
#include "global.h"
#include "jvm.h"
#include "jvm_util.h"

/* parse UTF-8 String */
void *parseCPString(Class *_this, FILE *fp, s32 index) {
    u8 short_tmp[2];
    ConstantUTF8 *ptr = jvm_alloc(sizeof(ConstantUTF8));

    ptr->tag = CONSTANT_UTF8;
    ptr->index = index;

    fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    ptr->string_size = s2c.s;
    ptr->additional_byte_size = tag_additional_byte_size[CONSTANT_UTF8] + ptr->string_size;

    ptr->utfstr = utf8_create();
    s32 i = 0;
    for (; i < ptr->string_size; i++) {
        u8 ch = 0;
        fread(&ch, 1, 1, fp);
        utf8_append_part_c(ptr->utfstr, &ch, 0, 1);
    }
    arraylist_append(_this->constantPool.utf8CP, ptr);
    return ptr;
}

/* parse Integer */
void *parseCPInteger(Class *_this, FILE *fp, s32 index) {
    u8 tmp[4];
    ConstantInteger *ptr = jvm_alloc(sizeof(ConstantInteger));

    ptr->tag = CONSTANT_INTEGER;
    ptr->index = index;
    ptr->additional_byte_size = tag_additional_byte_size[CONSTANT_INTEGER];

    fread(tmp, 4, 1, fp);
    Int2Float i2c;
    i2c.c3 = tmp[0];
    i2c.c2 = tmp[1];
    i2c.c1 = tmp[2];
    i2c.c0 = tmp[3];
    ptr->value = i2c.i;
    arraylist_append(_this->constantPool.integerCP, ptr);

    return ptr;
}

/* parse Float */
void *parseCPFloat(Class *_this, FILE *fp, s32 index) {
    c8 tmp[4];
    ConstantFloat *ptr = jvm_alloc(sizeof(ConstantFloat));

    ptr->tag = CONSTANT_FLOAT;
    ptr->index = index;
    ptr->additional_byte_size = tag_additional_byte_size[CONSTANT_FLOAT];

    fread(tmp, 4, 1, fp);
    Int2Float i2c;
    i2c.c3 = tmp[0];
    i2c.c2 = tmp[1];
    i2c.c1 = tmp[2];
    i2c.c0 = tmp[3];

    ptr->value = i2c.f;

    arraylist_append(_this->constantPool.floatCP, ptr);
    return ptr;
}

/* parse LONG */
void *parseCPLong(Class *_this, FILE *fp, s32 index) {
    u8 tmp[8];
    ConstantLong *ptr = jvm_alloc(sizeof(ConstantLong));

    ptr->tag = CONSTANT_LONG;
    ptr->index = index;
    ptr->additional_byte_size = tag_additional_byte_size[CONSTANT_LONG];

    fread(tmp, 8, 1, fp);
    Long2Double l2d;
    l2d.c7 = tmp[0];
    l2d.c6 = tmp[1];
    l2d.c5 = tmp[2];
    l2d.c4 = tmp[3];
    l2d.c3 = tmp[4];
    l2d.c2 = tmp[5];
    l2d.c1 = tmp[6];
    l2d.c0 = tmp[7];
    ptr->value = l2d.l;

    arraylist_append(_this->constantPool.longCP, ptr);
    return ptr;
}

/* parse Double */
void *parseCPDouble(Class *_this, FILE *fp, s32 index) {
    u8 tmp[8];
    ConstantDouble *ptr = jvm_alloc(sizeof(ConstantDouble));

    ptr->tag = CONSTANT_DOUBLE;
    ptr->index = index;
    ptr->additional_byte_size = tag_additional_byte_size[CONSTANT_DOUBLE];

    fread(tmp, 8, 1, fp);
    Long2Double l2d;
    l2d.c7 = tmp[0];
    l2d.c6 = tmp[1];
    l2d.c5 = tmp[2];
    l2d.c4 = tmp[3];
    l2d.c3 = tmp[4];
    l2d.c2 = tmp[5];
    l2d.c1 = tmp[6];
    l2d.c0 = tmp[7];
    ptr->value = l2d.d;

    arraylist_append(_this->constantPool.doubleCP, ptr);
    return ptr;
}

/* parse Constant Pool Class */
void *parseCPClass(Class *_this, FILE *fp, s32 index) {
    u8 short_tmp[2];
    ConstantClassRef *ptr = jvm_alloc(sizeof(ConstantClassRef));

    ptr->tag = CONSTANT_CLASS;
    ptr->index = index;
    ptr->additional_byte_size = tag_additional_byte_size[CONSTANT_CLASS];

    fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    ptr->stringIndex = s2c.s;

    arraylist_append(_this->constantPool.classRef, ptr);
    return ptr;
}

/* parse Constant Pool String Ref */
void *parseCPStringRef(Class *_this, FILE *fp, s32 index) {
    u8 short_tmp[2];
    ConstantStringRef *ptr = jvm_alloc(sizeof(ConstantStringRef));

    ptr->tag = CONSTANT_STRING_REF;
    ptr->index = index;
    ptr->additional_byte_size = tag_additional_byte_size[CONSTANT_STRING_REF];

    fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    ptr->stringIndex = s2c.s;

    arraylist_append(_this->constantPool.stringRef, ptr);
    return ptr;
}

/* parse Constant Pool Field */
void *parseCPField(Class *_this, FILE *fp, s32 index) {
    u8 short_tmp[2];
    ConstantFieldRef *ptr = jvm_alloc(sizeof(ConstantFieldRef));

    ptr->tag = CONSTANT_FIELD_REF;
    ptr->index = index;
    ptr->additional_byte_size = tag_additional_byte_size[CONSTANT_FIELD_REF];

    fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    ptr->classIndex = s2c.s;

    fread(short_tmp, 2, 1, fp);
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    ptr->nameAndTypeIndex = s2c.s;

    arraylist_append(_this->constantPool.fieldRef, ptr);
    return ptr;
}

/* parse Constant Pool Method */
void *parseCPMethod(Class *_this, FILE *fp, s32 index) {
    u8 short_tmp[2];
    ConstantMethodRef *ptr = jvm_alloc(sizeof(ConstantMethodRef));
    ptr->methodParaCount = -1;

    ptr->tag = CONSTANT_METHOD_REF;
    ptr->index = index;
    ptr->additional_byte_size = tag_additional_byte_size[CONSTANT_METHOD_REF];

    fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    ptr->classIndex = s2c.s;

    fread(short_tmp, 2, 1, fp);
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    ptr->nameAndTypeIndex = s2c.s;

    arraylist_append(_this->constantPool.methodRef, ptr);
    return ptr;
}

/* parse Constant Pool Interface */
void *parseCPInterface(Class *_this, FILE *fp, s32 index) {
    u8 short_tmp[2];
    ConstantInterfaceMethodRef *ptr = jvm_alloc(sizeof(ConstantInterfaceMethodRef));

    ptr->tag = CONSTANT_INTERFACE_REF;
    ptr->index = index;
    ptr->additional_byte_size = tag_additional_byte_size[CONSTANT_INTERFACE_REF];

    fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    ptr->classIndex = s2c.s;

    fread(short_tmp, 2, 1, fp);
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    ptr->nameAndTypeIndex = s2c.s;

    arraylist_append(_this->constantPool.interfaceRef, ptr);
    return ptr;
}

/* parse Constant Pool Interface */
void *parseCPNameAndType(Class *_this, FILE *fp, s32 index) {
    u8 short_tmp[2];
    ConstantNameAndType *ptr = jvm_alloc(sizeof(ConstantNameAndType));

    ptr->tag = CONSTANT_NAME_AND_TYPE;
    ptr->index = index;
    ptr->additional_byte_size = tag_additional_byte_size[CONSTANT_NAME_AND_TYPE];

    fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    ptr->nameIndex = s2c.s;

    fread(short_tmp, 2, 1, fp);
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    ptr->typeIndex = s2c.s;

    arraylist_append(_this->constantPool.name_and_type, ptr);
    return ptr;
}

s32 _parse_constant_pool(Class *_this, FILE *fp, s32 count) {
    u8 tag = 0;
    s32 i = 0;
    s32 offset_start = 0;
    s32 offset_end = 0;
    _this->constant_item_ptr = jvm_alloc(count * sizeof(void *));
    for (i = 1; i < count; i++) {
        fread(&tag, 1, 1, fp);
        offset_start = ftell(fp);
        //jvm_printf("!!!read tag = %02x!!!\n", tag);
        __refer ptr = NULL;
        s32 idx = i;
        switch (tag) {
            case CONSTANT_UTF8:
                ptr = parseCPString(_this, fp, i);
                break;
            case CONSTANT_INTEGER:
                ptr = parseCPInteger(_this, fp, i);
                break;
            case CONSTANT_FLOAT:
                ptr = parseCPFloat(_this, fp, i);
                break;
            case CONSTANT_LONG:
                ptr = parseCPLong(_this, fp, i);
                i++;
                break;
            case CONSTANT_DOUBLE:
                ptr = parseCPDouble(_this, fp, i);
                i++;
                break;
            case CONSTANT_STRING_REF:
                ptr = parseCPStringRef(_this, fp, i);
                break;
            case CONSTANT_CLASS:
                ptr = parseCPClass(_this, fp, i);
                break;
            case CONSTANT_FIELD_REF:
                ptr = parseCPField(_this, fp, i);
                break;
            case CONSTANT_METHOD_REF:
                ptr = parseCPMethod(_this, fp, i);
                break;
            case CONSTANT_INTERFACE_REF:
                ptr = parseCPMethod(_this, fp, i);//parseCPInterface(_this, fp, i);
                break;
            case CONSTANT_NAME_AND_TYPE:
                ptr = parseCPNameAndType(_this, fp, i);
                break;
            default:
                jvm_printf("\n!!!unknow tag = %02x!!!\n\n", tag);
                fseek(fp, -1, SEEK_CUR);
                break;
        };
        offset_end = ftell(fp);
        _this->constant_item_ptr[idx] = ptr;
    }

    return 0;
}
#ifdef _JVM_DEBUG
/* print constant pool table */
void printConstantPool(Class *clazz) {
    ConstantPool *p = &(clazz->constantPool);

    s32 i, j;
    jvm_printf("ConstantUTF8 = \n");
    for (i = 0; i < p->utf8CP->length; i++) {
        ConstantUTF8 *cutf = arraylist_get_value(p->utf8CP, i);
        jvm_printf("cp_index[%d], utf8[%d], tag = %d, size = %d, ",
                   cutf->index, i, cutf->tag,
                   cutf->string_size);
        jvm_printf("%s\n", utf8_cstr(cutf->utfstr));
    }
    jvm_printf("p->integer_used = %d\n", p->integerCP->length);
    if (p->integerCP->length > 0) {
        jvm_printf("Constant Integer= \n");
        for (i = 0; i < p->integerCP->length; i++) {
            ConstantInteger *ci = arraylist_get_value(p->integerCP, i);
            jvm_printf(" cp_index[%d], integer[%d], tag = %d, size = %d, %d\n",
                       ci->index, i, ci->tag,
                       ci->additional_byte_size,
                       ci->value);
        }
    }
    jvm_printf("p->float_used = %d\n", p->floatCP->length);
    if (p->floatCP->length > 0) {
        jvm_printf("Constant Float= \n");
        for (i = 0; i < p->floatCP->length; i++) {
            ConstantFloat *cf = arraylist_get_value(p->floatCP, i);
            jvm_printf(" cp_index[%d], f32[%d], tag = %d, size = %d, %.4f\n",
                       cf->index, i, cf->tag,
                       cf->additional_byte_size,
                       cf->value);
        }
    }
    jvm_printf("p->double_used = %d\n", p->doubleCP->length);
    if (p->doubleCP->length > 0) {
        jvm_printf("Constant Double= \n");
        for (i = 0; i < p->doubleCP->length; i++) {
            ConstantDouble *cd = arraylist_get_value(p->doubleCP, i);
            jvm_printf(" cp_index[%d], f64[%d], tag = %d, size = %d, %.5f\n",
                       cd->index, i, cd->tag,
                       cd->additional_byte_size,
                       cd->value);
        }
    }

    jvm_printf("p->clasz_used = %d\n", p->classRef->length);
    if (p->classRef->length > 0) {
        jvm_printf("Constant Class Pool===================== \n");
        for (i = 0; i < p->classRef->length; i++) {
            ConstantClassRef *ccr = arraylist_get_value(p->classRef, i);
            ConstantUTF8 *ptr = find_constant_utf8(clazz, ccr->stringIndex);
            jvm_printf(" cp_index[%d], class[%d], tag = %d, size = %d, %d",
                       ccr->index, i, ccr->tag,
                       ccr->additional_byte_size,
                       ccr->stringIndex
            );
            if (ptr != 0) {
                jvm_printf(" ");
                jvm_printf(" %s\n", utf8_cstr(ptr->utfstr));
            } else {
                jvm_printf("\n");
            }
        }
    }

    jvm_printf("p->stringRef_used = %d\n", p->stringRef->length);
    if (p->stringRef->length > 0) {
        jvm_printf("Constant String Reference===================== \n");
        for (i = 0; i < p->stringRef->length; i++) {
            ConstantStringRef *csr = arraylist_get_value(p->stringRef, i);
            ConstantUTF8 *ptr = find_constant_utf8(clazz, csr->stringIndex);
            jvm_printf(" cp_index[%d], strRef[%d], tag = %d, size = %d, %d",
                       csr->index, i, csr->tag,
                       csr->additional_byte_size,
                       csr->stringIndex);
            if (ptr != 0) {
                jvm_printf(" ");
                jvm_printf(" %s\n", utf8_cstr(ptr->utfstr));
            } else {
                jvm_printf("\n");
            }
        }
    }
    jvm_printf("p->field_used = %d\n", p->fieldRef->length);
    if (p->fieldRef->length > 0) {
        jvm_printf("Constant Field ===================== \n");
        for (i = 0; i < p->fieldRef->length; i++) {
            ConstantFieldRef *cfr = arraylist_get_value(p->fieldRef, i);
            ConstantClassRef *ptr = find_constant_classref(clazz, cfr->classIndex);
            ConstantNameAndType *ptr2 = find_constant_name_and_type(clazz, cfr->nameAndTypeIndex);

            jvm_printf(" cp_index[%d], fieldRef[%d], tag = %d, size = %d, cls %d, nat= %d",
                       cfr->index, i, cfr->tag,
                       cfr->additional_byte_size,
                       cfr->classIndex,
                       cfr->nameAndTypeIndex);
            if (ptr != 0) {
                ConstantUTF8 *name = find_constant_utf8(clazz, ptr->stringIndex);
                if (name != 0) {
                    jvm_printf(" %s", utf8_cstr(name->utfstr));
                }
            }
            if (ptr2 != 0) {
                ConstantUTF8 *name = find_constant_utf8(clazz, ptr2->nameIndex);
                ConstantUTF8 *type = find_constant_utf8(clazz, ptr2->typeIndex);
                if (name != 0) {
                    jvm_printf(".%s", utf8_cstr(name->utfstr));
                }
                if (type != 0) {
                    jvm_printf(":%s\n", utf8_cstr(type->utfstr));
                }
            }
        }
    }
    jvm_printf("p->method_used= %d\n", p->methodRef->length);
    if (p->methodRef->length > 0) {
        jvm_printf("Constant Method===================== \n");
        for (i = 0; i < p->methodRef->length; i++) {
            ConstantMethodRef *cmr = arraylist_get_value(p->methodRef, i);
            ConstantClassRef *ptr = find_constant_classref(clazz, cmr->classIndex);
            ConstantNameAndType *ptr2 = find_constant_name_and_type(clazz, cmr->nameAndTypeIndex);

            jvm_printf(" cp_index[%d], methodRef[%d], tag = %d, size = %d, cls %d, nat= %d",
                       cmr->index, i, cmr->tag,
                       cmr->additional_byte_size,
                       cmr->classIndex,
                       cmr->nameAndTypeIndex);
            if (ptr != 0) {
                ConstantUTF8 *name = find_constant_utf8(clazz, ptr->stringIndex);
                if (name != 0) {
                    jvm_printf(" %s", utf8_cstr(name->utfstr));
                }
            }
            if (ptr2 != 0) {
                ConstantUTF8 *name = find_constant_utf8(clazz, ptr2->nameIndex);
                ConstantUTF8 *type = find_constant_utf8(clazz, ptr2->typeIndex);
                if (name != 0) {
                    jvm_printf(".%s", utf8_cstr(name->utfstr));
                }
                if (type != 0) {
                    jvm_printf("%s\n", utf8_cstr(type->utfstr));
                }
            }
        }
    }
}
#endif //_JVM_DEBUG

s32 _class_constant_pool_destory(Class *clazz) {
    int i;
    for (i = 0; i < clazz->constantPool.fieldRef->length; i++) {
        __refer ptr = arraylist_get_value(clazz->constantPool.fieldRef, i);
        jvm_free(ptr);
    }
    for (i = 0; i < clazz->constantPool.methodRef->length; i++) {
        ConstantMethodRef *ptr = (ConstantMethodRef *) arraylist_get_value(clazz->constantPool.methodRef, i);
        pairlist_destory(ptr->virtual_methods);
        jvm_free(ptr);
    }
    for (i = 0; i < clazz->constantPool.interfaceRef->length; i++) {
        __refer ptr = arraylist_get_value(clazz->constantPool.interfaceRef, i);
        jvm_free(ptr);
    }
    for (i = 0; i < clazz->constantPool.doubleCP->length; i++) {
        __refer ptr = arraylist_get_value(clazz->constantPool.doubleCP, i);
        jvm_free(ptr);
    }
    for (i = 0; i < clazz->constantPool.floatCP->length; i++) {
        __refer ptr = arraylist_get_value(clazz->constantPool.floatCP, i);
        jvm_free(ptr);
    }
    for (i = 0; i < clazz->constantPool.integerCP->length; i++) {
        __refer ptr = arraylist_get_value(clazz->constantPool.integerCP, i);
        jvm_free(ptr);
    }
    for (i = 0; i < clazz->constantPool.classRef->length; i++) {
        __refer ptr = arraylist_get_value(clazz->constantPool.classRef, i);
        jvm_free(ptr);
    }
    for (i = 0; i < clazz->constantPool.stringRef->length; i++) {
        __refer ptr = arraylist_get_value(clazz->constantPool.stringRef, i);
        jvm_free(ptr);
    }
    for (i = 0; i < clazz->constantPool.longCP->length; i++) {
        __refer ptr = arraylist_get_value(clazz->constantPool.longCP, i);
        jvm_free(ptr);
    }
    for (i = 0; i < clazz->constantPool.utf8CP->length; i++) {
        ConstantUTF8 *ptr = (ConstantUTF8 *) arraylist_get_value(clazz->constantPool.utf8CP, i);
        utf8_destory(ptr->utfstr);
        ptr->utfstr = NULL;
        jvm_free(ptr);
    }
    for (i = 0; i < clazz->constantPool.name_and_type->length; i++) {
        __refer ptr = arraylist_get_value(clazz->constantPool.name_and_type, i);
        jvm_free(ptr);
    }
    return 0;
}

s32 parseAttr(FieldInfo *ptr, FILE *fp) {
    s32 i;
    AttributeInfo *tmp = 0;
    u8 short_tmp[2];
    u8 integer_tmp[4];
#if 0
    jvm_printf("fieldRef attributes_count = %d\n", arr_body->attributes_count);
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
    return 0;
}

/* parse Field Pool */
s32 parseFP(Class *_this, FILE *fp) {
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

    jvm_printf("Field Pool==================== \n");
    if (fp->field_used > 0) {
        for (i = 0; i < fp->field_used; i++) {
            ConstantUTF8 *ptr = find_constant_utf8(clazz, fp->field[i].name_index);
            jvm_printf("fieldRef[%d], attr_count = %d, %d",
                       i, fp->field[i].attributes_count,
                       fp->field[i].name_index);
            if (ptr != 0) {
                jvm_printf(" %s\n", utf8_cstr(ptr->utfstr));
            } else {
                jvm_printf("\n");
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
        FieldInfo *fi = &clazz->fieldPool.field[i];
        for (j = 0; j < fi->attributes_count; j++) {
            AttributeInfo *attr = &fi->attributes[j];
            jvm_free(attr->info);
            attr->info = NULL;
        }
        if (fi->attributes)jvm_free(fi->attributes);
        fi->attributes = NULL;
    }
    if (clazz->fieldPool.field)jvm_free(clazz->fieldPool.field);
    clazz->fieldPool.field = NULL;
    return 0;
}


/* parse Interface Pool Class */
s32 parseIPClass(Class *_this, FILE *fp, s32 index) {
    u8 short_tmp[2];
    ConstantClassRef *ptr = &_this->interfacePool.clasz[_this->interfacePool.clasz_used];

    ptr->tag = CONSTANT_CLASS;
    ptr->index = index;
    ptr->additional_byte_size = 2;

    fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    ptr->stringIndex = s2c.s;

    _this->interfacePool.clasz_used++;
    return 0;
}

void printInterfacePool(Class *clazz, InterfacePool *ip) {
    s32 i, j;

    if (ip->clasz_used > 0) {
        jvm_printf("Interface Class Pool= \n");
        for (i = 0; i < ip->clasz_used; i++) {
            ConstantUTF8 *ptr = find_constant_utf8(clazz, ip->clasz[i].stringIndex);
            jvm_printf(" ip_index[%d], class[%d], tag = %d, size = %d, %d",
                       ip->clasz[i].index, i, ip->clasz[i].tag,
                       ip->clasz[i].additional_byte_size,
                       ip->clasz[i].stringIndex);
            if (ptr != 0) {
                jvm_printf(" ");
                jvm_printf(" %s\n", utf8_cstr(ptr->utfstr));
            } else {
                jvm_printf("\n");
            }
        }
    }
}

s32 _parse_interface_pool(Class *_this, FILE *fp, s32 count) {

    s32 size = sizeof(ConstantClassRef) * count;
    _this->interfacePool.clasz = jvm_alloc(size);
    s32 i;
    for (i = 0; i < count; i++)
        parseIPClass(_this, fp, i);
    return 0;
}

s32 _class_interface_pool_destory(Class *clazz) {
    if (clazz->interfacePool.clasz)jvm_free(clazz->interfacePool.clasz);
    clazz->interfacePool.clasz = NULL;
    return 0;
}


//=================================     load         ======================================


s32 parseMethodAttr(MethodInfo *ptr, FILE *fp) {
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
    return 0;
}


/* parse Method Pool */
s32 parseMP(Class *_this, FILE *fp) {
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

        jvm_printf("attribute name = %s\n", utf8_cstr(get_utf8_string(p, attr->attribute_name_index)));
    }
}

/* Print Method Pool */
void printMethodPool(Class *p, MethodPool *mp) {
    s32 i, j;
    jvm_printf("Method Pool===================== \n");
    if (mp->method_used > 0) {
        for (i = 0; i < mp->method_used; i++) {
            ConstantUTF8 *ptr = find_constant_utf8(p, mp->method[i].name_index);
            ConstantUTF8 *ptr2 = find_constant_utf8(p, mp->method[i].descriptor_index);
            ConstantMethodRef *mRefPtr = find_constant_method_ref(p, mp->method[i].name_index);
            jvm_printf("methodRef[%d], attr_count = %d, %d",
                       i, mp->method[i].attributes_count,
                       mp->method[i].name_index);
            if (ptr != 0) {
                jvm_printf(" %s", utf8_cstr(ptr->utfstr));
            }
#if 0
                else if (mRefPtr != 0) {
                    ConstantClassRef *obj_of_clazz = find_class_ref(p, mRefPtr->classIndex);
                    if (obj_of_clazz != 0) {
                        ConstantUTF8 *name = find_utf8(p, obj_of_clazz->stringIndex);
                        if (name != 0) {
                            jvm_printf(" ");
                            for (j = 0; j < name->string_size; j++)
                                jvm_printf("%c", name->arr_body[j]);
                            jvm_printf(" ");

                        }
                    }
                    ConstantNameAndType *nameAndType = find_name_and_type(p, mRefPtr->nameAndTypeIndex);
                    if (nameAndType != 0) {
                        ConstantUTF8 *name = find_utf8(p, nameAndType->nameIndex);
                        ConstantUTF8 *type = find_utf8(p, nameAndType->typeIndex);

                        if (name != 0) {
                            jvm_printf(" ");
                            for (j = 0; j < name->string_size; j++)
                                jvm_printf("%c", name->arr_body[j]);
                            jvm_printf(" ");
                        }
                        if (type != 0) {
                            jvm_printf(" ");
                            for (j = 0; j < type->string_size; j++)
                                jvm_printf("%c", type->arr_body[j]);
                            jvm_printf(" ");
                        }
                    }
                    jvm_printf("\n");
                }
#endif
            else {
                jvm_printf("\n");
                continue;
            }
            if (ptr2 != 0) {
                jvm_printf("");
                jvm_printf("%s\n", utf8_cstr(ptr2->utfstr));
            }
            printMethodAttributes(p, &mp->method[i]);
        }
    }
}

s32 _parse_method_pool(Class *_this, FILE *fp, s32 count) {

    s32 size = sizeof(MethodInfo) * count;
    _this->methodPool.method = jvm_alloc(size);
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
            if (attr->info)jvm_free(attr->info);//某些没有转
            attr->info = NULL;
            if (attr->converted_code) {
                CodeAttribute *ca = (CodeAttribute *) attr->converted_code;
                jvm_free(ca->code);//info已被转换为converted_attribute
                ca->code = NULL;
                jvm_free(ca->exception_table);//info已被转换为converted_attribute
                ca->exception_table = NULL;
                jvm_free(ca->line_number_table);
                ca->line_number_table = NULL;
                if (ca->local_var_table)jvm_free(ca->local_var_table);
                ca->local_var_table = NULL;
                //
                jvm_free(attr->converted_code);
                attr->converted_code = NULL;
            }
        }
        if (mi->attributes)jvm_free(mi->attributes);
        mi->attributes = NULL;
        utf8_destory(mi->paraType);
        if (mi->breakpoint)jvm_free(mi->breakpoint);
        mi->breakpoint = NULL;
    }
    if (clazz->methodPool.method)jvm_free(clazz->methodPool.method);
    clazz->methodPool.method = NULL;
    return 0;
}

s32 _parse_attribute_pool(Class *_this, FILE *fp, s32 count) {

    s32 size = sizeof(AttributeInfo) * count;
    _this->attributePool.attribute = jvm_alloc(size);
    _this->attributePool.attribute_used = count;
    s32 i;
    u8 short_tmp[2];
    u8 integer_tmp[4];
    for (i = 0; i < count; i++) {
        AttributeInfo *ptr = &(_this->attributePool.attribute[i]);

        /* access flag */
        fread(short_tmp, 2, 1, fp);
        Short2Char s2c;
        s2c.c1 = short_tmp[0];
        s2c.c0 = short_tmp[1];
        ptr->attribute_name_index = s2c.s;

        fread(integer_tmp, 4, 1, fp);
        Int2Float i2c;
        i2c.c3 = integer_tmp[0];
        i2c.c2 = integer_tmp[1];
        i2c.c1 = integer_tmp[2];
        i2c.c0 = integer_tmp[3];
        ptr->attribute_length = i2c.i;
        //
        ptr->info = jvm_alloc(ptr->attribute_length);
        fread(ptr->info, ptr->attribute_length, 1, fp);
    }
    return 0;
}

s32 _class_attribute_info_destory(Class *clazz) {
    s32 i;
    for (i = 0; i < clazz->attributePool.attribute_used; i++) {
        AttributeInfo *ptr = &(clazz->attributePool.attribute[i]);
        if (ptr->info) {
            jvm_free(ptr->info);
            ptr->info = NULL;
        }
    }
    if (clazz->attributePool.attribute)jvm_free(clazz->attributePool.attribute);
    return 0;
}

/* Print Class File Format */
void printClassFileFormat(ClassFileFormat *cff) {
    s32 i;
    jvm_printf("Magic Number = ");
    for (i = 0; i < 4; i++)
        jvm_printf("0x%02X ", cff->magic_number[i]);
    jvm_printf("\n");
    jvm_printf("Minor Number = 0x(%02X) %d \n",
               cff->minor_version, cff->minor_version);
    jvm_printf("Major Number(%s) = 0x(%02X) %d \n",
               getMajorVersionString(cff->major_version),
               cff->major_version, cff->major_version);
    jvm_printf("Constant Pool Count = 0x(%02X) %d \n",
               cff->constant_pool_count, cff->constant_pool_count);
    jvm_printf("access flag = 0x(%02X) %d \n", cff->access_flags, cff->access_flags);
    jvm_printf("this class = 0x(%02X) %d \n",
               cff->this_class, cff->this_class);
    jvm_printf("super class = 0x(%02X) %d \n",
               cff->super_class, cff->super_class);
    jvm_printf("interfaceRef count = 0x(%02X) %d \n",
               cff->interface_count, cff->interface_count);
    jvm_printf("fieldRef count = 0x(%02X) %d \n",
               cff->fields_count, cff->fields_count);
    jvm_printf("methodRef count = 0x(%02X) %d \n",
               cff->methods_count, cff->methods_count);
}

/* Parse Class File */
s32 _LOAD_FROM_FILE(Class *_this, c8 *file) {
    ClassFileFormat *cff = &(_this->cff);
    FILE *fp = 0;
    u8 short_tmp[2];
    fp = fopen(file, "rb");
    if (fp == 0) {
        jvm_printf("Open file %s failed\n", file);
        return -1;
    }
    /* magic number */
    fread(cff->magic_number, 4, 1, fp);

    /* minor_version */
    fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    cff->minor_version = s2c.s;

    /* major_version */
    fread(short_tmp, 2, 1, fp);
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    cff->major_version = s2c.s;

    /* constant pool */
    fread(short_tmp, 2, 1, fp);
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    cff->constant_pool_count = s2c.s;
    /* constant pool table */
    _parse_constant_pool(_this, fp, cff->constant_pool_count);
    /* access flag */
    fread(short_tmp, 2, 1, fp);
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    cff->access_flags = s2c.s;

    /* this class */
    fread(short_tmp, 2, 1, fp);
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    cff->this_class = s2c.s;

    /* super class */
    fread(short_tmp, 2, 1, fp);
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    cff->super_class = s2c.s;

    /* interfaceRef count */
    fread(short_tmp, 2, 1, fp);
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    cff->interface_count = s2c.s;
    /* interfaceRef pool table */
    _parse_interface_pool(_this, fp, cff->interface_count);

    /* fieldRef count */
    fread(short_tmp, 2, 1, fp);
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    cff->fields_count = s2c.s;

    /* fieldRef pool table */
    _parse_field_pool(_this, fp, cff->fields_count);
    /* methodRef count */
    fread(short_tmp, 2, 1, fp);
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    cff->methods_count = s2c.s;

    /* methodRef pool table */
    _parse_method_pool(_this, fp, cff->methods_count);

    //attribute
    fread(short_tmp, 2, 1, fp);
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    cff->attributes_count = s2c.s;
    _parse_attribute_pool(_this, fp, cff->attributes_count);

    fclose(fp);

    class_link(_this);

    _this->status = CLASS_STATUS_LOADED;
    return 0;
}

/**
 * 把各个索引转为直接地址引用，加快处理速度
 * @param clazz
 */
void class_link(Class *clazz) {
    Utf8String* ustr=get_utf8_string(clazz, find_constant_classref(clazz, clazz->cff.this_class)->stringIndex);
    clazz->name = utf8_create_copy(ustr);
//    if (utf8_equals_c(clazz->name, "javax/mini/eio/socket/PrivateOutputStream")) {
//        int debug = 1;
//    }
    s32 i;
    for (i = 0; i < clazz->interfacePool.clasz_used; i++) {
        ConstantClassRef *ptr = &clazz->interfacePool.clasz[i];
        ptr->name = get_utf8_string(clazz, find_constant_classref(clazz, ptr->stringIndex)->stringIndex);
    }
    for (i = 0; i < clazz->fieldPool.field_used; i++) {
        FieldInfo *ptr = &clazz->fieldPool.field[i];
        ptr->name = get_utf8_string(clazz, ptr->name_index);
        ptr->descriptor = get_utf8_string(clazz, ptr->descriptor_index);

    }
    for (i = 0; i < clazz->methodPool.method_used; i++) {
        MethodInfo *ptr = &clazz->methodPool.method[i];
        ptr->name = get_utf8_string(clazz, ptr->name_index);
        ptr->descriptor = get_utf8_string(clazz, ptr->descriptor_index);
        ptr->_this_class = clazz;
        if (!ptr->paraType) {//首次执行
            // eg:  (Ljava/lang/Object;IBLjava/lang/String;[[[ILjava/lang/Object;)Ljava/lang/String;Z
            ptr->paraType = utf8_create();
            ptr->para_count = parseMethodPara(ptr->descriptor, ptr->paraType);
            if (!(ptr->access_flags & ACC_STATIC)) {
                ptr->para_count++;//add instance
            }
        }
        s32 j;

        //转attribute为CdoeAttribute
        for (j = 0; j < ptr->attributes_count; j++) {
            if (utf8_equals_c(get_utf8_string(clazz, ptr->attributes[j].attribute_name_index), "Code") == 1) {
                CodeAttribute *ca = jvm_alloc(sizeof(CodeAttribute));
                convert_to_code_attribute(ca, &ptr->attributes[j], clazz);
                jvm_free(ptr->attributes[j].info);//无用删除
                ptr->attributes[j].info = NULL;
                ptr->attributes[j].converted_code = ca;
            }
        }
    }
    for (i = 0; i < clazz->attributePool.attribute_used; i++) {
        AttributeInfo *ptr = &clazz->attributePool.attribute[i];
        Utf8String *name = get_utf8_string(clazz, ptr->attribute_name_index);
        if (utf8_equals_c(name, "SourceFile")) {
            Short2Char s2c;
            s2c.c1 = ptr->info[0];
            s2c.c0 = ptr->info[1];
            clazz->source = get_utf8_string(clazz, s2c.s);
        }
    }

    for (i = 0; i < clazz->constantPool.classRef->length; i++) {
        ConstantClassRef *ccr = (ConstantClassRef *) arraylist_get_value(clazz->constantPool.classRef, i);
        ccr->name = get_utf8_string(clazz, ccr->stringIndex);
    }
    for (i = 0; i < clazz->constantPool.interfaceRef->length; i++) {
        ConstantInterfaceMethodRef *cir = (ConstantInterfaceMethodRef *) arraylist_get_value(
                clazz->constantPool.interfaceRef, i);
        cir->name = get_utf8_string(clazz, find_constant_name_and_type(clazz, cir->nameAndTypeIndex)->nameIndex);
    }
    for (i = 0; i < clazz->constantPool.methodRef->length; i++) {
        ConstantMethodRef *cmr = (ConstantMethodRef *) arraylist_get_value(clazz->constantPool.methodRef, i);
        cmr->nameAndType = find_constant_name_and_type(clazz, cmr->nameAndTypeIndex);
        cmr->name = get_utf8_string(clazz, cmr->nameAndType->nameIndex);
        cmr->descriptor = get_utf8_string(clazz, cmr->nameAndType->typeIndex);
        cmr->clsName = find_constant_classref(clazz, cmr->classIndex)->name;
//        if (utf8_equals_c(clazz->name, "java/lang/String")) {
//            printf("%s,%s\n", utf8_cstr(cmr->name), utf8_cstr(cmr->clsName));
//            int debug = 1;
//        }
        if (cmr->methodParaCount == -1) {
            Utf8String *tmps = utf8_create();
            parseMethodPara(cmr->descriptor, tmps);
            cmr->methodParaCount = tmps->length;
            utf8_destory(tmps);
        }
    }

}

s32 convert_to_code_attribute(CodeAttribute *ca, AttributeInfo *attr, Class *clazz) {
    s32 info_p = 0;

    ca->attribute_name_index = attr->attribute_name_index;
    ca->attribute_length = attr->attribute_length;
    Short2Char s2c;
    s2c.c1 = attr->info[info_p++];
    s2c.c0 = attr->info[info_p++];
    ca->max_stack = s2c.s;
    s2c.c1 = attr->info[info_p++];
    s2c.c0 = attr->info[info_p++];
    ca->max_locals = s2c.s;
    Int2Float i2c;
    i2c.c3 = attr->info[info_p++];
    i2c.c2 = attr->info[info_p++];
    i2c.c1 = attr->info[info_p++];
    i2c.c0 = attr->info[info_p++];
    ca->code_length = i2c.i;
    ca->code = (u8 *) jvm_alloc(sizeof(u8) * ca->code_length);
    memcpy(ca->code, attr->info + info_p, ca->code_length);
    info_p += ca->code_length;
    s2c.c1 = attr->info[info_p++];
    s2c.c0 = attr->info[info_p++];
    ca->exception_table_length = s2c.s;
    s32 bytelen = sizeof(ExceptionTable) * ca->exception_table_length;
    ca->exception_table = jvm_alloc(bytelen);
    int i;
    for (i = 0; i < 4 * ca->exception_table_length; i++) {
        s2c.c1 = attr->info[info_p++];
        s2c.c0 = attr->info[info_p++];
        ((u16 *) ca->exception_table)[i] = s2c.s;
    }
    //line number
    s2c.c1 = attr->info[info_p++];
    s2c.c0 = attr->info[info_p++];
    s32 attr_count = (u16) s2c.s;
    for (i = 0; i < attr_count; i++) {
        s2c.c1 = attr->info[info_p++];
        s2c.c0 = attr->info[info_p++];
        s32 attribute_name_index = (u16) s2c.s;
        i2c.c3 = attr->info[info_p++];
        i2c.c2 = attr->info[info_p++];
        i2c.c1 = attr->info[info_p++];
        i2c.c0 = attr->info[info_p++];
        s32 attribute_lenth = (u32) i2c.i;
        //转行号表
        if (utf8_equals_c(get_utf8_string(clazz, attribute_name_index), "LineNumberTable")) {
            s2c.c1 = attr->info[info_p++];
            s2c.c0 = attr->info[info_p++];
            ca->line_number_table_length = (u16) s2c.s;
            ca->line_number_table = jvm_alloc(sizeof(u32) * ca->line_number_table_length);
            s32 j;
            for (j = 0; j < ca->line_number_table_length; j++) {
                s2c.c1 = attr->info[info_p++];
                s2c.c0 = attr->info[info_p++];
                //setFieldShort(ca->line_number_table[j].start_pc, s2c.s);
                ca->line_number_table[j].start_pc = s2c.s;
                s2c.c1 = attr->info[info_p++];
                s2c.c0 = attr->info[info_p++];
                //setFieldShort(ca->line_number_table[j].line_number, s2c.s);
                ca->line_number_table[j].line_number = s2c.s;
            }
        } else if (utf8_equals_c(get_utf8_string(clazz, attribute_name_index), "LocalVariableTable")) {
            s2c.c1 = attr->info[info_p++];
            s2c.c0 = attr->info[info_p++];
            ca->local_var_table_length = (u16) s2c.s;
            ca->local_var_table = jvm_alloc(sizeof(LocalVarTable) * ca->local_var_table_length);
            s32 j;
            for (j = 0; j < ca->local_var_table_length; j++) {
                s2c.c1 = attr->info[info_p++];
                s2c.c0 = attr->info[info_p++];
                ca->local_var_table[j].start_pc = s2c.s;
                s2c.c1 = attr->info[info_p++];
                s2c.c0 = attr->info[info_p++];
                ca->local_var_table[j].length = s2c.s;
                s2c.c1 = attr->info[info_p++];
                s2c.c0 = attr->info[info_p++];
                ca->local_var_table[j].name_index = s2c.s;
                s2c.c1 = attr->info[info_p++];
                s2c.c0 = attr->info[info_p++];
                ca->local_var_table[j].descriptor_index = s2c.s;
                s2c.c1 = attr->info[info_p++];
                s2c.c0 = attr->info[info_p++];
                ca->local_var_table[j].index = s2c.s;
            }
        } else {
            info_p += attribute_lenth;
        }
    }
    return 0;
}


s32 load_related_class(Utf8String *classpath, Class *clazz, hmap_t classes) {
    ConstantPool *p = &(clazz->constantPool);
    if (p->classRef->length > 0) {
        s32 i;
        for (i = 0; i < p->classRef->length; i++) {
            ConstantClassRef *ccr = (ConstantClassRef *) arraylist_get_value(p->classRef, i);
            s32 ret = load_class(classpath, find_constant_utf8(clazz, ccr->stringIndex)->utfstr, classes);
            if (ret != 0) {
                return ret;
            }
        }
    }
    return 0;
}

s32 load_class(Utf8String *pClassPath, Utf8String *pClassName, hmap_t classes) {
    if (!pClassName)return 0;
    s32 iret = 0;
    if (utf8_last_indexof_c(pClassPath, "/") != pClassPath->length - 1)utf8_append_c(pClassPath, "/");

    Utf8String *clsName = utf8_create_copy(pClassName);
    utf8_replace_c(clsName, ".", "/");

    Utf8String *tmppath = utf8_create_copy(pClassPath);
    utf8_append(tmppath, clsName);
    utf8_append_c(tmppath, ".class");
    Class *tmpclazz = classes_get(clsName);
    if (!tmpclazz) {
        if (utf8_indexof_c(tmppath, "[") < 0) {
            Class *clazz = class_create();
            iret = clazz->_load_from_file(clazz, utf8_cstr(tmppath));
            //jvm_printf("load class :%llx , %llx\n", (long long) (long) tmppath, (long long) (long) clsName);
            //回收
            if (iret != 0) {
                jvm_printf(" class not found : %s\n", utf8_cstr(pClassName));
            } else {
#if _JVM_DEBUG > 5
                jvm_printf("load class:  %s \n", utf8_cstr(tmppath));
#endif
                classes_put(clazz);
                load_related_class(pClassPath, clazz, classes);
            }
        }
    }
    utf8_destory(tmppath);
    utf8_destory(clsName);
    return iret;
}


