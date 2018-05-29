//
// Created by gust on 2017/9/25.
//

#include "stdlib.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "../utils/bytebuf.h"
#include "../utils/miniz/miniz_wrapper.h"
#include "jvm.h"
#include "jvm_util.h"
#include "garbage.h"

/* parse UTF-8 String */
void *_parseCPString(JClass *_this, ByteBuf *buf, s32 index) {

    ConstantUTF8 *ptr = jvm_calloc(sizeof(ConstantUTF8));

    ptr->tag = CONSTANT_UTF8;
    ptr->index = index;

    //fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = (c8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (c8) bytebuf_read(buf);//short_tmp[1];
    ptr->string_size = s2c.s;

    ptr->utfstr = utf8_create();
    s32 i = 0;
    for (; i < ptr->string_size; i++) {
        u8 ch = (u8) bytebuf_read(buf);//0;
        //fread(&ch, 1, 1, fp);
        utf8_append_part_c(ptr->utfstr, &ch, 0, 1);
    }
    arraylist_push_back(_this->constantPool.utf8CP, ptr);
    return ptr;
}

/* parse Integer */
void *_parseCPInteger(JClass *_this, ByteBuf *buf, s32 index) {

    ConstantInteger *ptr = jvm_calloc(sizeof(ConstantInteger));

    ptr->tag = CONSTANT_INTEGER;
    ptr->index = index;

    //fread(tmp, 4, 1, fp);
    Int2Float i2c;
    i2c.c3 = (u8) bytebuf_read(buf);//tmp[0];
    i2c.c2 = (u8) bytebuf_read(buf);//tmp[1];
    i2c.c1 = (u8) bytebuf_read(buf);//tmp[2];
    i2c.c0 = (u8) bytebuf_read(buf);//tmp[3];
    ptr->value = i2c.i;
    arraylist_push_back(_this->constantPool.integerCP, ptr);

    return ptr;
}

/* parse Float */
void *_parseCPFloat(JClass *_this, ByteBuf *buf, s32 index) {

    ConstantFloat *ptr = jvm_calloc(sizeof(ConstantFloat));

    ptr->tag = CONSTANT_FLOAT;
    ptr->index = index;

    //fread(tmp, 4, 1, fp);
    Int2Float i2c;
    i2c.c3 = (u8) bytebuf_read(buf);//tmp[0];
    i2c.c2 = (u8) bytebuf_read(buf);//tmp[1];
    i2c.c1 = (u8) bytebuf_read(buf);//tmp[2];
    i2c.c0 = (u8) bytebuf_read(buf);//tmp[3];

    ptr->value = i2c.f;

    arraylist_push_back(_this->constantPool.floatCP, ptr);
    return ptr;
}

/* parse LONG */
void *_parseCPLong(JClass *_this, ByteBuf *buf, s32 index) {

    ConstantLong *ptr = jvm_calloc(sizeof(ConstantLong));

    ptr->tag = CONSTANT_LONG;
    ptr->index = index;

    //fread(tmp, 8, 1, fp);
    Long2Double l2d;
    l2d.c7 = (u8) bytebuf_read(buf);//tmp[0];
    l2d.c6 = (u8) bytebuf_read(buf);//tmp[1];
    l2d.c5 = (u8) bytebuf_read(buf);//tmp[2];
    l2d.c4 = (u8) bytebuf_read(buf);//tmp[3];
    l2d.c3 = (u8) bytebuf_read(buf);//tmp[4];
    l2d.c2 = (u8) bytebuf_read(buf);//tmp[5];
    l2d.c1 = (u8) bytebuf_read(buf);//tmp[6];
    l2d.c0 = (u8) bytebuf_read(buf);//tmp[7];
    ptr->value = l2d.l;

    arraylist_push_back(_this->constantPool.longCP, ptr);
    return ptr;
}

/* parse Double */
void *_parseCPDouble(JClass *_this, ByteBuf *buf, s32 index) {

    ConstantDouble *ptr = jvm_calloc(sizeof(ConstantDouble));

    ptr->tag = CONSTANT_DOUBLE;
    ptr->index = index;

    //fread(tmp, 8, 1, fp);
    Long2Double l2d;
    l2d.c7 = (u8) bytebuf_read(buf);//tmp[0];
    l2d.c6 = (u8) bytebuf_read(buf);//tmp[1];
    l2d.c5 = (u8) bytebuf_read(buf);//tmp[2];
    l2d.c4 = (u8) bytebuf_read(buf);//tmp[3];
    l2d.c3 = (u8) bytebuf_read(buf);//tmp[4];
    l2d.c2 = (u8) bytebuf_read(buf);//tmp[5];
    l2d.c1 = (u8) bytebuf_read(buf);//tmp[6];
    l2d.c0 = (u8) bytebuf_read(buf);//tmp[7];
    ptr->value = l2d.d;

    arraylist_push_back(_this->constantPool.doubleCP, ptr);
    return ptr;
}

/* parse Constant Pool Class */
void *_parseCPClass(JClass *_this, ByteBuf *buf, s32 index) {

    ConstantClassRef *ptr = jvm_calloc(sizeof(ConstantClassRef));

    ptr->tag = CONSTANT_CLASS;
    ptr->index = index;

    //fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
    ptr->stringIndex = s2c.s;

    arraylist_push_back(_this->constantPool.classRef, ptr);
    return ptr;
}

/* parse Constant Pool String Ref */
void *_parseCPStringRef(JClass *_this, ByteBuf *buf, s32 index) {

    ConstantStringRef *ptr = jvm_calloc(sizeof(ConstantStringRef));

    ptr->tag = CONSTANT_STRING_REF;
    ptr->index = index;

    //fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
    ptr->stringIndex = s2c.s;

    arraylist_push_back(_this->constantPool.stringRef, ptr);
    return ptr;
}

/* parse Constant Pool Field */
void *_parseCPField(JClass *_this, ByteBuf *buf, s32 index) {

    ConstantFieldRef *ptr = jvm_calloc(sizeof(ConstantFieldRef));

    ptr->tag = CONSTANT_FIELD_REF;
    ptr->index = index;

    //fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
    ptr->classIndex = s2c.s;

    //fread(short_tmp, 2, 1, fp);
    s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
    ptr->nameAndTypeIndex = s2c.s;

    arraylist_push_back(_this->constantPool.fieldRef, ptr);
    return ptr;
}

/* parse Constant Pool Method */
void *_parseCPMethod(JClass *_this, ByteBuf *buf, s32 index) {

    ConstantMethodRef *ptr = jvm_calloc(sizeof(ConstantMethodRef));
    ptr->methodParaCount = -1;

    ptr->tag = CONSTANT_METHOD_REF;
    ptr->index = index;

    //fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
    ptr->classIndex = s2c.s;

    //fread(short_tmp, 2, 1, fp);
    s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
    ptr->nameAndTypeIndex = s2c.s;

    arraylist_push_back(_this->constantPool.methodRef, ptr);
    return ptr;
}

/* parse Constant Pool Interface */
void *_parseCPInterface(JClass *_this, ByteBuf *buf, s32 index) {

    ConstantInterfaceMethodRef *ptr = jvm_calloc(sizeof(ConstantInterfaceMethodRef));

    ptr->tag = CONSTANT_INTERFACE_REF;
    ptr->index = index;

    //fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
    ptr->classIndex = s2c.s;

    //fread(short_tmp, 2, 1, fp);
    s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
    ptr->nameAndTypeIndex = s2c.s;

    arraylist_push_back(_this->constantPool.interfaceRef, ptr);
    return ptr;
}

/* parse Constant Pool Interface */
void *_parseCPNameAndType(JClass *_this, ByteBuf *buf, s32 index) {

    ConstantNameAndType *ptr = jvm_calloc(sizeof(ConstantNameAndType));

    ptr->tag = CONSTANT_NAME_AND_TYPE;
    ptr->index = index;

    //fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
    ptr->nameIndex = s2c.s;

    //fread(short_tmp, 2, 1, fp);
    s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
    ptr->typeIndex = s2c.s;

    arraylist_push_back(_this->constantPool.name_and_type, ptr);
    return ptr;
}

#ifdef _JVM_DEBUG_BYTECODE_DETAIL


#endif //_JVM_DEBUG

s32 _class_constant_pool_destory(JClass *clazz) {
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

s32 _parseAttr(FieldInfo *ptr, ByteBuf *buf) {
    s32 i;
    AttributeInfo *tmp = 0;

#if 0
    jvm_printf("fieldRef attributes_count = %d\n", arr_body->attributes_count);
#endif
    for (i = 0; i < ptr->attributes_count; i++) {
        tmp = &(ptr->attributes[i]);
        //fread(short_tmp, 2, 1, fp);
        Short2Char s2c;
        s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
        s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
        tmp->attribute_name_index = s2c.s;

        //fread(integer_tmp, 4, 1, fp);
        Int2Float i2c;
        i2c.c3 = (u8) bytebuf_read(buf);//integer_tmp[0];
        i2c.c2 = (u8) bytebuf_read(buf);//integer_tmp[1];
        i2c.c1 = (u8) bytebuf_read(buf);//integer_tmp[2];
        i2c.c0 = (u8) bytebuf_read(buf);//integer_tmp[3];
        tmp->attribute_length = i2c.i;

        if (tmp->attribute_length > 0) {
            tmp->info = (u8 *) jvm_calloc(sizeof(u8) * tmp->attribute_length);
            //fread(tmp->info, tmp->attribute_length, 1, fp);
            bytebuf_read_batch(buf, (c8 *) tmp->info, tmp->attribute_length);
        } else {
            tmp->info = NULL;
        }
    }
    return 0;
}

/* parse Field Pool */
s32 _parseFP(JClass *_this, ByteBuf *buf) {

    FieldInfo *ptr = &(_this->fieldPool.field[_this->fieldPool.field_used]);

    /* access flag */
    //fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
    ptr->access_flags = s2c.s;

    /* name index */
    //fread(short_tmp, 2, 1, fp);
    s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
    ptr->name_index = s2c.s;


    /* descriptor index */
    //fread(short_tmp, 2, 1, fp);
    s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
    ptr->descriptor_index = s2c.s;

    /* attributes count */
    //fread(short_tmp, 2, 1, fp);
    s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
    ptr->attributes_count = s2c.s;

    if (ptr->attributes_count > 0) {
        ptr->attributes = (AttributeInfo *) jvm_calloc(sizeof(AttributeInfo) * ptr->attributes_count);
    } else {
        ptr->attributes = NULL;
    }
    /* parse attributes */
    _parseAttr(ptr, buf);
    _this->fieldPool.field_used++;
    return 0;
}


s32 _parse_field_pool(JClass *_this, ByteBuf *buf, s32 count) {
    s32 size = sizeof(FieldInfo) * count;
    _this->fieldPool.field = jvm_calloc(size);

    s32 i;
    for (i = 0; i < count; i++)
        _parseFP(_this, buf);
    return 0;
}


s32 _class_field_info_destory(JClass *clazz) {
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
s32 _parseIPClass(JClass *_this, ByteBuf *buf, s32 index) {

    ConstantClassRef *ptr = &_this->interfacePool.clasz[_this->interfacePool.clasz_used];

    ptr->tag = CONSTANT_CLASS;
    ptr->index = index;

    //fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
    ptr->stringIndex = s2c.s;

    _this->interfacePool.clasz_used++;
    return 0;
}


s32 _parse_interface_pool(JClass *_this, ByteBuf *buf, s32 count) {

    s32 size = sizeof(ConstantClassRef) * count;
    _this->interfacePool.clasz = jvm_calloc(size);
    s32 i;
    for (i = 0; i < count; i++)
        _parseIPClass(_this, buf, i);
    return 0;
}

s32 _class_interface_pool_destory(JClass *clazz) {
    if (clazz->interfacePool.clasz)jvm_free(clazz->interfacePool.clasz);
    clazz->interfacePool.clasz = NULL;
    return 0;
}


//=================================     load         ======================================


s32 _parseMethodAttr(MethodInfo *ptr, ByteBuf *buf) {
    s32 i;
    AttributeInfo *tmp = 0;

    for (i = 0; i < ptr->attributes_count; i++) {
        tmp = &ptr->attributes[i];
        //fread(short_tmp, 2, 1, fp);
        Short2Char s2c;
        s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
        s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
        tmp->attribute_name_index = s2c.s;
        //fread(integer_tmp, 4, 1, fp);
        Int2Float i2c;
        i2c.c3 = (u8) bytebuf_read(buf);//integer_tmp[0];
        i2c.c2 = (u8) bytebuf_read(buf);//integer_tmp[1];
        i2c.c1 = (u8) bytebuf_read(buf);//integer_tmp[2];
        i2c.c0 = (u8) bytebuf_read(buf);//integer_tmp[3];
        tmp->attribute_length = i2c.i;

        tmp->info = (u8 *) jvm_calloc(sizeof(u8) * tmp->attribute_length);
        //fread(tmp->info, tmp->attribute_length, 1, fp);
        bytebuf_read_batch(buf, (c8 *) tmp->info, tmp->attribute_length);
    }
    return 0;
}


/* parse Method Pool */
s32 _parseMP(JClass *_this, ByteBuf *buf) {

    MethodInfo *ptr = &(_this->methodPool.method[_this->methodPool.method_used]);

    /* access flag */
    //fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
    ptr->access_flags = s2c.s;

    /* name index */
    //fread(short_tmp, 2, 1, fp);
    s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
    ptr->name_index = s2c.s;

    /* descriptor index */
    //fread(short_tmp, 2, 1, fp);
    s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
    ptr->descriptor_index = s2c.s;

    /* attributes count */
    //fread(short_tmp, 2, 1, fp);
    s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
    s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
    ptr->attributes_count = s2c.s;

    ptr->attributes = (AttributeInfo *) jvm_calloc(sizeof(AttributeInfo) * ptr->attributes_count);
    memset(ptr->attributes, 0, sizeof(AttributeInfo) * ptr->attributes_count);
    /* parse methodRef attributes */
    _parseMethodAttr(ptr, buf);

    _this->methodPool.method_used++;
    return 0;
}

s32 _parse_method_pool(JClass *_this, ByteBuf *buf, s32 count) {

    s32 size = sizeof(MethodInfo) * count;
    _this->methodPool.method = jvm_calloc(size);
    s32 i;
    for (i = 0; i < count; i++)
        _parseMP(_this, buf);
    return 0;
}


s32 _class_method_info_destory(JClass *clazz) {
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
        utf8_destory(mi->returnType);
        if (mi->breakpoint)jvm_free(mi->breakpoint);
        mi->breakpoint = NULL;
    }
    if (clazz->methodPool.method)jvm_free(clazz->methodPool.method);
    clazz->methodPool.method = NULL;
    return 0;
}

s32 _parse_attribute_pool(JClass *_this, ByteBuf *buf, s32 count) {

    s32 size = sizeof(AttributeInfo) * count;
    _this->attributePool.attribute = jvm_calloc(size);
    _this->attributePool.attribute_used = count;
    s32 i;

    for (i = 0; i < count; i++) {
        AttributeInfo *ptr = &(_this->attributePool.attribute[i]);

        /* access flag */
        //fread(short_tmp, 2, 1, fp);
        Short2Char s2c;
        s2c.c1 = (u8) bytebuf_read(buf);//short_tmp[0];
        s2c.c0 = (u8) bytebuf_read(buf);//short_tmp[1];
        ptr->attribute_name_index = s2c.s;

        //fread(integer_tmp, 4, 1, fp);
        Int2Float i2c;
        i2c.c3 = (u8) bytebuf_read(buf);//integer_tmp[0];
        i2c.c2 = (u8) bytebuf_read(buf);//integer_tmp[1];
        i2c.c1 = (u8) bytebuf_read(buf);//integer_tmp[2];
        i2c.c0 = (u8) bytebuf_read(buf);//integer_tmp[3];
        ptr->attribute_length = i2c.i;
        //
        ptr->info = jvm_calloc(ptr->attribute_length);
        //fread(ptr->info, ptr->attribute_length, 1, fp);
        bytebuf_read_batch(buf, (c8 *) ptr->info, ptr->attribute_length);
    }
    return 0;
}

s32 _class_attribute_info_destory(JClass *clazz) {
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

s32 _parse_constant_pool(JClass *_this, ByteBuf *buf, s32 count) {
    u8 tag = 0;
    s32 i = 0;
    u64 offset_start = 0;
    u64 offset_end = 0;
    _this->constant_item_ptr = jvm_calloc(count * sizeof(void *));
    for (i = 1; i < count; i++) {
        //fread(&tag, 1, 1, fp);
        tag = (u8) bytebuf_read(buf);
        offset_start = buf->rp;
        //jvm_printf("!!!read tag = %02x!!!\n", tag);
        __refer ptr = NULL;
        s32 idx = i;
        switch (tag) {
            case CONSTANT_UTF8:
                ptr = _parseCPString(_this, buf, i);
                break;
            case CONSTANT_INTEGER:
                ptr = _parseCPInteger(_this, buf, i);
                break;
            case CONSTANT_FLOAT:
                ptr = _parseCPFloat(_this, buf, i);
                break;
            case CONSTANT_LONG:
                ptr = _parseCPLong(_this, buf, i);
                i++;
                break;
            case CONSTANT_DOUBLE:
                ptr = _parseCPDouble(_this, buf, i);
                i++;
                break;
            case CONSTANT_STRING_REF:
                ptr = _parseCPStringRef(_this, buf, i);
                break;
            case CONSTANT_CLASS:
                ptr = _parseCPClass(_this, buf, i);
                break;
            case CONSTANT_FIELD_REF:
                ptr = _parseCPField(_this, buf, i);
                break;
            case CONSTANT_METHOD_REF:
                ptr = _parseCPMethod(_this, buf, i);
                break;
            case CONSTANT_INTERFACE_REF:
                ptr = _parseCPMethod(_this, buf, i);//parseCPInterface(_this, fp, i);
                break;
            case CONSTANT_NAME_AND_TYPE:
                ptr = _parseCPNameAndType(_this, buf, i);
                break;
            default:
                jvm_printf("\n!!!unknow tag = %02x!!!\n\n", tag);
                //fseek(fp, -1, SEEK_CUR);
                break;
        };
        offset_end = buf->rp;
        _this->constant_item_ptr[idx] = ptr;
    }

    return 0;
}


s32 _convert_to_code_attribute(CodeAttribute *ca, AttributeInfo *attr, JClass *clazz) {
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
    ca->code = (u8 *) jvm_calloc(sizeof(u8) * ca->code_length);
    memcpy(ca->code, attr->info + info_p, ca->code_length);
    info_p += ca->code_length;
    s2c.c1 = attr->info[info_p++];
    s2c.c0 = attr->info[info_p++];
    ca->exception_table_length = s2c.s;
    s32 bytelen = sizeof(ExceptionTable) * ca->exception_table_length;
    ca->exception_table = jvm_calloc(bytelen);
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
        if (utf8_equals_c(class_get_utf8_string(clazz, attribute_name_index), "LineNumberTable")) {
            s2c.c1 = attr->info[info_p++];
            s2c.c0 = attr->info[info_p++];
            ca->line_number_table_length = (u16) s2c.s;
            ca->line_number_table = jvm_calloc(sizeof(u32) * ca->line_number_table_length);
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
        } else if (utf8_equals_c(class_get_utf8_string(clazz, attribute_name_index), "LocalVariableTable")) {
            s2c.c1 = attr->info[info_p++];
            s2c.c0 = attr->info[info_p++];
            ca->local_var_table_length = (u16) s2c.s;
            ca->local_var_table = jvm_calloc(sizeof(LocalVarTable) * ca->local_var_table_length);
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


s32 parseMethodPara(Utf8String *methodType, Utf8String *out) {
    s32 count = 0;
    Utf8String *para = utf8_create_copy(methodType);
    utf8_substring(para, utf8_indexof_c(para, "(") + 1, utf8_last_indexof_c(para, ")"));
    //从后往前拆分方法参数，从栈中弹出放入本地变量
    int i = 0;
    while (para->length > 0) {
        c8 ch = utf8_char_at(para, 0);
        switch (ch) {
            case 'S':
            case 'C':
            case 'B':
            case 'I':
            case 'F':
            case 'Z':
                utf8_substring(para, 1, para->length);
                utf8_append_c(out, "4");
                count++;
                break;
            case 'D':
            case 'J': {
                utf8_substring(para, 1, para->length);
                utf8_append_c(out, "8");
                count += 2;
                break;
            }
            case 'L':
                utf8_substring(para, utf8_indexof_c(para, ";") + 1, para->length);
                utf8_append_c(out, "R");
                count += 1;
                break;
            case '[':
                while (utf8_char_at(para, 1) == '[') {
                    utf8_substring(para, 1, para->length);//去掉多维中的 [[[[LObject; 中的 [符
                }
                if (utf8_char_at(para, 1) == 'L') {
                    utf8_substring(para, utf8_indexof_c(para, ";") + 1, para->length);
                } else {
                    utf8_substring(para, 2, para->length);
                }
                utf8_append_c(out, "R");
                count += 1;
                break;
        }
        i++;
    }
    utf8_destory(para);
    return count;
}


/**
 * 把各个索引转为直接地址引用，加快处理速度
 * @param clazz class
 */
void _class_optimize(JClass *clazz) {
    Utf8String *ustr = class_get_utf8_string(clazz,
                                             class_get_constant_classref(clazz, clazz->cff.this_class)->stringIndex);
    clazz->name = utf8_create_copy(ustr);
//    if (utf8_equals_c(clazz->name, "javax/mini/eio/socket/PrivateOutputStream")) {
//        int debug = 1;
//    }
    s32 i;
    for (i = 0; i < clazz->interfacePool.clasz_used; i++) {
        ConstantClassRef *ptr = &clazz->interfacePool.clasz[i];
        ptr->name = class_get_utf8_string(clazz, class_get_constant_classref(clazz, ptr->stringIndex)->stringIndex);
    }

    for (i = 0; i < clazz->fieldPool.field_used; i++) {
        FieldInfo *ptr = &clazz->fieldPool.field[i];
        ptr->name = class_get_utf8_string(clazz, ptr->name_index);
        ptr->descriptor = class_get_utf8_string(clazz, ptr->descriptor_index);
        ptr->datatype_idx = getDataTypeIndex(utf8_char_at(ptr->descriptor, 0));
        ptr->isrefer = isDataReferByIndex(ptr->datatype_idx);
        ptr->datatype_bytes = data_type_bytes[ptr->datatype_idx];

        //for gc iterator fast
        if (isDataReferByIndex(ptr->datatype_idx)) {
            if (ptr->access_flags & ACC_STATIC) {
                arraylist_push_back_unsafe(clazz->staticFieldPtrIndex, (__refer) (intptr_t) i);
            } else {
                arraylist_push_back_unsafe(clazz->insFieldPtrIndex, (__refer) (intptr_t) i);
            }
        }
    }
    for (i = 0; i < clazz->methodPool.method_used; i++) {
        MethodInfo *ptr = &clazz->methodPool.method[i];
        ptr->name = class_get_utf8_string(clazz, ptr->name_index);
        ptr->descriptor = class_get_utf8_string(clazz, ptr->descriptor_index);
        ptr->_this_class = clazz;
        if (!ptr->paraType) {//首次执行
            // eg:  (Ljava/lang/Object;IBLjava/lang/String;[[[ILjava/lang/Object;)Ljava/lang/String;Z
            ptr->paraType = utf8_create();
            ptr->para_count = parseMethodPara(ptr->descriptor, ptr->paraType);
            if (!(ptr->access_flags & ACC_STATIC)) {
                ptr->para_count++;//add instance
            }
            s32 pos = utf8_indexof_c(ptr->descriptor, ")") + 1;
            ptr->returnType = utf8_create_part(ptr->descriptor, pos, ptr->descriptor->length - pos);
        }
        s32 j;

        //转attribute为CdoeAttribute
        for (j = 0; j < ptr->attributes_count; j++) {
            if (utf8_equals_c(class_get_utf8_string(clazz, ptr->attributes[j].attribute_name_index), "Code") == 1) {
                CodeAttribute *ca = jvm_calloc(sizeof(CodeAttribute));
                _convert_to_code_attribute(ca, &ptr->attributes[j], clazz);
                jvm_free(ptr->attributes[j].info);//无用删除
                ptr->attributes[j].info = NULL;
                ptr->attributes[j].converted_code = ca;
                ptr->code_attr_idx = j;
            }
        }
    }
    for (i = 0; i < clazz->attributePool.attribute_used; i++) {
        AttributeInfo *ptr = &clazz->attributePool.attribute[i];
        Utf8String *name = class_get_utf8_string(clazz, ptr->attribute_name_index);
        if (utf8_equals_c(name, "SourceFile")) {
            Short2Char s2c;
            s2c.c1 = ptr->info[0];
            s2c.c0 = ptr->info[1];
            clazz->source = class_get_utf8_string(clazz, s2c.s);
        }
    }

    for (i = 0; i < clazz->constantPool.classRef->length; i++) {
        ConstantClassRef *ccr = (ConstantClassRef *) arraylist_get_value(clazz->constantPool.classRef, i);
        ccr->name = class_get_utf8_string(clazz, ccr->stringIndex);
    }
    for (i = 0; i < clazz->constantPool.interfaceRef->length; i++) {
        ConstantInterfaceMethodRef *cir = (ConstantInterfaceMethodRef *) arraylist_get_value(
                clazz->constantPool.interfaceRef, i);
        cir->name = class_get_utf8_string(clazz,
                                          class_get_constant_name_and_type(clazz, cir->nameAndTypeIndex)->nameIndex);
    }
    for (i = 0; i < clazz->constantPool.methodRef->length; i++) {
        ConstantMethodRef *cmr = (ConstantMethodRef *) arraylist_get_value(clazz->constantPool.methodRef, i);
        cmr->nameAndType = class_get_constant_name_and_type(clazz, cmr->nameAndTypeIndex);
        cmr->name = class_get_utf8_string(clazz, cmr->nameAndType->nameIndex);
        cmr->descriptor = class_get_utf8_string(clazz, cmr->nameAndType->typeIndex);
        cmr->clsName = class_get_constant_classref(clazz, cmr->classIndex)->name;
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


/* Parse Class File */
s32 _LOAD_CLASS_FROM_BYTES(JClass *_this, ByteBuf *buf) {
    ClassFileFormat *cff = &(_this->cff);

    /* magic number */
    bytebuf_read_batch(buf, (c8 *) &cff->magic_number, 4);
//    fread(cff->magic_number, 4, 1, fp);

    /* minor_version */
    //fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = (c8) bytebuf_read(buf);
    s2c.c0 = (c8) bytebuf_read(buf);
    cff->minor_version = s2c.s;

    /* major_version */
    //fread(short_tmp, 2, 1, fp);
    s2c.c1 = (c8) bytebuf_read(buf);
    s2c.c0 = (c8) bytebuf_read(buf);
    cff->major_version = s2c.s;

    /* constant pool */
    //fread(short_tmp, 2, 1, fp);
    s2c.c1 = (c8) bytebuf_read(buf);
    s2c.c0 = (c8) bytebuf_read(buf);
    cff->constant_pool_count = s2c.s;
    /* constant pool table */
    _parse_constant_pool(_this, buf, cff->constant_pool_count);
    /* access flag */
    //fread(short_tmp, 2, 1, fp);
    s2c.c1 = (c8) bytebuf_read(buf);
    s2c.c0 = (c8) bytebuf_read(buf);
    cff->access_flags = s2c.s;

    /* this class */
    //fread(short_tmp, 2, 1, fp);
    s2c.c1 = (c8) bytebuf_read(buf);
    s2c.c0 = (c8) bytebuf_read(buf);
    cff->this_class = s2c.s;

    /* super class */
    //fread(short_tmp, 2, 1, fp);
    s2c.c1 = (c8) bytebuf_read(buf);
    s2c.c0 = (c8) bytebuf_read(buf);
    cff->super_class = s2c.s;

    /* interfaceRef count */
    //fread(short_tmp, 2, 1, fp);
    s2c.c1 = (c8) bytebuf_read(buf);
    s2c.c0 = (c8) bytebuf_read(buf);
    cff->interface_count = s2c.s;
    /* interfaceRef pool table */
    _parse_interface_pool(_this, buf, cff->interface_count);

    /* fieldRef count */
    //fread(short_tmp, 2, 1, fp);
    s2c.c1 = (c8) bytebuf_read(buf);
    s2c.c0 = (c8) bytebuf_read(buf);
    cff->fields_count = s2c.s;

    /* fieldRef pool table */
    _parse_field_pool(_this, buf, cff->fields_count);
    /* methodRef count */
    //fread(short_tmp, 2, 1, fp);
    s2c.c1 = (c8) bytebuf_read(buf);
    s2c.c0 = (c8) bytebuf_read(buf);
    cff->methods_count = s2c.s;

    /* methodRef pool table */
    _parse_method_pool(_this, buf, cff->methods_count);

    //attribute
    //fread(short_tmp, 2, 1, fp);
    s2c.c1 = (c8) bytebuf_read(buf);
    s2c.c0 = (c8) bytebuf_read(buf);
    cff->attributes_count = s2c.s;
    _parse_attribute_pool(_this, buf, cff->attributes_count);

    //fclose(fp);

    _class_optimize(_this);

    _this->status = CLASS_STATUS_LOADED;
    return 0;
}


s32 load_class(ClassLoader *loader, Utf8String *pClassName, Runtime *runtime) {
    if (!loader)return 0;
    s32 iret = 0;
    //
    Utf8String *clsName = utf8_create_copy(pClassName);
    utf8_replace_c(clsName, ".", "/");
    JClass *tmpclazz = classes_get(clsName);
    if (utf8_indexof_c(clsName, "[") == 0) {
        tmpclazz = array_class_get_by_name(runtime, clsName);
    }
    if (!tmpclazz) {
        ByteBuf *bytebuf = NULL;

        utf8_append_c(clsName, ".class");
        bytebuf = load_file_from_classpath(loader, clsName);
        if (bytebuf != NULL) {
            tmpclazz = class_create(runtime);
            iret = tmpclazz->_load_class_from_bytes(tmpclazz, bytebuf);//load file
            bytebuf_destory(bytebuf);
            if (iret == 0) {
                classes_put(tmpclazz);
                class_prepar(tmpclazz, runtime);
                gc_refer_hold(tmpclazz);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                jvm_printf("load class:  %s \n", utf8_cstr(clsName));
#endif
            } else {
                class_destory(tmpclazz);
            }
        }

    }
    if (!tmpclazz) {
        jvm_printf("class not found:  %s \n", utf8_cstr(clsName));
    }
    utf8_destory(clsName);
    return iret;
}


s32 _DESTORY_CLASS(JClass *clazz) {
    //
    _class_method_info_destory(clazz);
    _class_interface_pool_destory(clazz);
    _class_field_info_destory(clazz);
    _class_constant_pool_destory(clazz);
    _class_attribute_info_destory(clazz);
    clazz->field_static = NULL;
    if (clazz->constant_item_ptr)jvm_free(clazz->constant_item_ptr);
    clazz->constant_item_ptr = NULL;
    jthreadlock_destory(&clazz->mb);
    constant_list_destory(clazz);
    utf8_destory(clazz->name);
    return 0;
}
