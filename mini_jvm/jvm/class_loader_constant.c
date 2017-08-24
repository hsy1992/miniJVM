
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jvm.h"
#include "../utils/utf8_string.h"
#include "garbage.h"
#include "jvm_util.h"


/* parse UTF-8 String */
static void *parseCPString(Class *_this, FILE *fp, s32 index) {
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

    ptr->ptr = utf8_create();
    s32 i = 0;
    for (; i < ptr->string_size; i++) {
        u8 ch = 0;
        fread(&ch, 1, 1, fp);
        utf8_append_part_c(ptr->ptr, &ch, 0, 1);
    }
    arraylist_append(_this->constantPool.utf8CP, ptr);
    return ptr;
}

/* parse Integer */
static void *parseCPInteger(Class *_this, FILE *fp, s32 index) {
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
static void *parseCPFloat(Class *_this, FILE *fp, s32 index) {
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
static void *parseCPLong(Class *_this, FILE *fp, s32 index) {
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
    ptr->value=l2d.l;

    arraylist_append(_this->constantPool.longCP, ptr);
    return ptr;
}

/* parse Double */
static void *parseCPDouble(Class *_this, FILE *fp, s32 index) {
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
    ptr->value=l2d.d;

    arraylist_append(_this->constantPool.doubleCP, ptr);
    return ptr;
}

/* parse Constant Pool Class */
static void *parseCPClass(Class *_this, FILE *fp, s32 index) {
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
static void *parseCPStringRef(Class *_this, FILE *fp, s32 index) {
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
static void *parseCPField(Class *_this, FILE *fp, s32 index) {
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
static void *parseCPMethod(Class *_this, FILE *fp, s32 index) {
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
static void *parseCPInterface(Class *_this, FILE *fp, s32 index) {
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
static void *parseCPNameAndType(Class *_this, FILE *fp, s32 index) {
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
        //printf("!!!read tag = %02x!!!\n", tag);
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
                printf("\n!!!unknow tag = %02x!!!\n\n", tag);
                fseek(fp, -1, SEEK_CUR);
                break;
        };
        offset_end = ftell(fp);
        _this->constant_item_ptr[idx] = ptr;
    }

    return 0;
}

/* find UTF8 */
ConstantUTF8 *find_constant_utf8(Class *clazz, s32 index) {
    return (ConstantUTF8 *) (clazz->constant_item_ptr[index]);
}

/* Find Class Reference */
ConstantStringRef *find_constant_stringref(Class *clazz, s32 index) {
    return (ConstantStringRef *) (clazz->constant_item_ptr[index]);
}


/* Find Class Reference */
ConstantClassRef *find_constant_classref(Class *clazz, s32 index) {
    return (ConstantClassRef *) (clazz->constant_item_ptr[index]);
}

Class *getClassByConstantClassRef(Class *clazz, s32 index) {
    ConstantClassRef *ccr = find_constant_classref(clazz, index);
    return hashtable_get(classes, ccr->name);
}

ConstantFieldRef *find_constant_fieldref(Class *clazz, s32 index) {
    return (ConstantFieldRef *) (clazz->constant_item_ptr[index]);
}

ConstantItem *find_constant_item(Class *clazz, s32 index) {
    return (ConstantItem *) (clazz->constant_item_ptr[index]);
}

s32 find_constant_fieldref_index(Class *clazz, Utf8String *fieldName, Utf8String *type) {
    s32 i = 0;
    ArrayList *cp = clazz->constantPool.fieldRef;
    for (; i < cp->length; i++) {
        ConstantFieldRef *cfr = ((ConstantFieldRef *) arraylist_get_value(cp, i));
        ConstantNameAndType *nat = find_constant_name_and_type(clazz, cfr->nameAndTypeIndex);
        if (utf8_equals(fieldName, get_utf8_string(clazz, nat->nameIndex)) == 0 &&
            utf8_equals(type, get_utf8_string(clazz, nat->typeIndex)) == 0) {
            return cfr->index;
        }
    }
    return -1;
}

/* Find Method Reference */
ConstantMethodRef *find_constant_method_ref(Class *clazz, s32 index) {
    return (ConstantMethodRef *) (clazz->constant_item_ptr[index]);
}

ConstantInterfaceMethodRef *find_constant_interface_method_ref(Class *clazz, s32 index) {
    return (ConstantInterfaceMethodRef *) (clazz->constant_item_ptr[index]);
}

/* Find Name and Type Reference */
ConstantNameAndType *find_constant_name_and_type(Class *clazz, s32 index) {
    return (ConstantNameAndType *) (clazz->constant_item_ptr[index]);
}

/* get integer from constant pool */
s32 get_constant_integer(Class *clazz, s32 index) {
    return ((ConstantInteger *) (clazz->constant_item_ptr[index]))->value;
}

/* get long from constant pool */
s64 get_constant_long(Class *clazz, s32 index) {
    return ((ConstantLong *) (clazz->constant_item_ptr[index]))->value;
}

/* get f32 from constant pool */
f32 get_constant_float(Class *clazz, s32 index) {
    return ((ConstantFloat *) (clazz->constant_item_ptr[index]))->value;
}

/* get f64 from constant pool */
f64 get_double_from_constant_pool(Class *clazz, s32 index) {
    return ((ConstantDouble *) (clazz->constant_item_ptr[index]))->value;
}


Utf8String *get_utf8_string(Class *clazz, s32 index) {
    return ((ConstantUTF8 *) (clazz->constant_item_ptr[index]))->ptr;
}

/* print constant pool table */
void printConstantPool(Class *clazz) {
    ConstantPool *p = &(clazz->constantPool);

    s32 i, j;
    printf("ConstantUTF8 = \n");
    for (i = 0; i < p->utf8CP->length; i++) {
        ConstantUTF8 *cutf = arraylist_get_value(p->utf8CP, i);
        printf("cp_index[%d], utf8[%d], tag = %d, size = %d, ",
               cutf->index, i, cutf->tag,
               cutf->string_size);
        printf("%s\n", utf8_cstr(cutf->ptr));
    }
    printf("p->integer_used = %d\n", p->integerCP->length);
    if (p->integerCP->length > 0) {
        printf("Constant Integer= \n");
        for (i = 0; i < p->integerCP->length; i++) {
            ConstantInteger *ci = arraylist_get_value(p->integerCP, i);
            printf(" cp_index[%d], integer[%d], tag = %d, size = %d, %d\n",
                   ci->index, i, ci->tag,
                   ci->additional_byte_size,
                   ci->value);
        }
    }
    printf("p->float_used = %d\n", p->floatCP->length);
    if (p->floatCP->length > 0) {
        printf("Constant Float= \n");
        for (i = 0; i < p->floatCP->length; i++) {
            ConstantFloat *cf = arraylist_get_value(p->floatCP, i);
            printf(" cp_index[%d], f32[%d], tag = %d, size = %d, %.4f\n",
                   cf->index, i, cf->tag,
                   cf->additional_byte_size,
                   cf->value);
        }
    }
    printf("p->double_used = %d\n", p->doubleCP->length);
    if (p->doubleCP->length > 0) {
        printf("Constant Double= \n");
        for (i = 0; i < p->doubleCP->length; i++) {
            ConstantDouble *cd = arraylist_get_value(p->doubleCP, i);
            printf(" cp_index[%d], f64[%d], tag = %d, size = %d, %.5f\n",
                   cd->index, i, cd->tag,
                   cd->additional_byte_size,
                   cd->value);
        }
    }

    printf("p->clasz_used = %d\n", p->classRef->length);
    if (p->classRef->length > 0) {
        printf("Constant Class Pool===================== \n");
        for (i = 0; i < p->classRef->length; i++) {
            ConstantClassRef *ccr = arraylist_get_value(p->classRef, i);
            ConstantUTF8 *ptr = find_constant_utf8(clazz, ccr->stringIndex);
            printf(" cp_index[%d], class[%d], tag = %d, size = %d, %d",
                   ccr->index, i, ccr->tag,
                   ccr->additional_byte_size,
                   ccr->stringIndex
            );
            if (ptr != 0) {
                printf(" ");
                printf(" %s\n", utf8_cstr(ptr->ptr));
            } else {
                printf("\n");
            }
        }
    }

    printf("p->stringRef_used = %d\n", p->stringRef->length);
    if (p->stringRef->length > 0) {
        printf("Constant String Reference===================== \n");
        for (i = 0; i < p->stringRef->length; i++) {
            ConstantStringRef *csr = arraylist_get_value(p->stringRef, i);
            ConstantUTF8 *ptr = find_constant_utf8(clazz, csr->stringIndex);
            printf(" cp_index[%d], strRef[%d], tag = %d, size = %d, %d",
                   csr->index, i, csr->tag,
                   csr->additional_byte_size,
                   csr->stringIndex);
            if (ptr != 0) {
                printf(" ");
                printf(" %s\n", utf8_cstr(ptr->ptr));
            } else {
                printf("\n");
            }
        }
    }
    printf("p->field_used = %d\n", p->fieldRef->length);
    if (p->fieldRef->length > 0) {
        printf("Constant Field ===================== \n");
        for (i = 0; i < p->fieldRef->length; i++) {
            ConstantFieldRef *cfr = arraylist_get_value(p->fieldRef, i);
            ConstantClassRef *ptr = find_constant_classref(clazz, cfr->classIndex);
            ConstantNameAndType *ptr2 = find_constant_name_and_type(clazz, cfr->nameAndTypeIndex);

            printf(" cp_index[%d], fieldRef[%d], tag = %d, size = %d, cls %d, nat= %d",
                   cfr->index, i, cfr->tag,
                   cfr->additional_byte_size,
                   cfr->classIndex,
                   cfr->nameAndTypeIndex);
            if (ptr != 0) {
                ConstantUTF8 *name = find_constant_utf8(clazz, ptr->stringIndex);
                if (name != 0) {
                    printf(" %s", utf8_cstr(name->ptr));
                }
            }
            if (ptr2 != 0) {
                ConstantUTF8 *name = find_constant_utf8(clazz, ptr2->nameIndex);
                ConstantUTF8 *type = find_constant_utf8(clazz, ptr2->typeIndex);
                if (name != 0) {
                    printf(".%s", utf8_cstr(name->ptr));
                }
                if (type != 0) {
                    printf(":%s\n", utf8_cstr(type->ptr));
                }
            }
        }
    }
    printf("p->method_used= %d\n", p->methodRef->length);
    if (p->methodRef->length > 0) {
        printf("Constant Method===================== \n");
        for (i = 0; i < p->methodRef->length; i++) {
            ConstantMethodRef *cmr = arraylist_get_value(p->methodRef, i);
            ConstantClassRef *ptr = find_constant_classref(clazz, cmr->classIndex);
            ConstantNameAndType *ptr2 = find_constant_name_and_type(clazz, cmr->nameAndTypeIndex);

            printf(" cp_index[%d], methodRef[%d], tag = %d, size = %d, cls %d, nat= %d",
                   cmr->index, i, cmr->tag,
                   cmr->additional_byte_size,
                   cmr->classIndex,
                   cmr->nameAndTypeIndex);
            if (ptr != 0) {
                ConstantUTF8 *name = find_constant_utf8(clazz, ptr->stringIndex);
                if (name != 0) {
                    printf(" %s", utf8_cstr(name->ptr));
                }
            }
            if (ptr2 != 0) {
                ConstantUTF8 *name = find_constant_utf8(clazz, ptr2->nameIndex);
                ConstantUTF8 *type = find_constant_utf8(clazz, ptr2->typeIndex);
                if (name != 0) {
                    printf(".%s", utf8_cstr(name->ptr));
                }
                if (type != 0) {
                    printf("%s\n", utf8_cstr(type->ptr));
                }
            }
        }
    }
}
