
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jvm.h"
#include "jvm_util.h"
#include "garbage.h"


/* Get Major Version String */
static c8 *getMajorVersionString(u16 major_number) {
    if (major_number == 0x33)
        return "J2SE 7";
    if (major_number == 0x32)
        return "J2SE 6.0";
    return "NONE";
}

/* Print Class File Format */
void printClassFileFormat(ClassFileFormat *cff) {
    s32 i;
    printf("Magic Number = ");
    for (i = 0; i < 4; i++)
        printf("0x%02X ", cff->magic_number[i]);
    printf("\n");
    printf("Minor Number = 0x(%02X) %d \n",
           cff->minor_version, cff->minor_version);
    printf("Major Number(%s) = 0x(%02X) %d \n",
           getMajorVersionString(cff->major_version),
           cff->major_version, cff->major_version);
    printf("Constant Pool Count = 0x(%02X) %d \n",
           cff->constant_pool_count, cff->constant_pool_count);
    printf("access flag = 0x(%02X) %d \n", cff->access_flags, cff->access_flags);
    printf("this class = 0x(%02X) %d \n",
           cff->this_class, cff->this_class);
    printf("super class = 0x(%02X) %d \n",
           cff->super_class, cff->super_class);
    printf("interfaceRef count = 0x(%02X) %d \n",
           cff->interface_count, cff->interface_count);
    printf("fieldRef count = 0x(%02X) %d \n",
           cff->fields_count, cff->fields_count);
    printf("methodRef count = 0x(%02X) %d \n",
           cff->methods_count, cff->methods_count);
}

/* Parse Class File */
s32 _LOAD_FROM_FILE(Class *_this, c8 *file) {
    ClassFileFormat *cff = &(_this->cff);
    FILE *fp = 0;
    u8 short_tmp[2];
    fp = fopen(file, "rb");
    if (fp == 0) {
        printf("Open file %s failed\n", file);
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
    fclose(fp);

    class_optmize(_this);

    _this->status = CLASS_STATUS_LOADED;
    return 0;
}

/**
 * 把各个索引转为直接地址引用，加快处理速度
 * @param clazz
 */
void class_optmize(Class *clazz) {
    clazz->name = get_utf8_string(clazz, find_constant_classref(clazz, clazz->cff.this_class)->stringIndex);

    s32 i;
    for (i = 0; i < clazz->fieldPool.field_used; i++) {
        FieldInfo *ptr = &clazz->fieldPool.field[i];
        ptr->name = get_utf8_string(clazz, ptr->name_index);
        ptr->descriptor = get_utf8_string(clazz, ptr->descriptor_index);

    }
    for (i = 0; i < clazz->methodPool.method_used; i++) {
        MethodInfo *ptr = &clazz->methodPool.method[i];
        ptr->name = get_utf8_string(clazz, ptr->name_index);
        ptr->descriptor = get_utf8_string(clazz, ptr->descriptor_index);
        if (!ptr->paraType) {//首次执行
            // eg:  (Ljava/lang/Object;IBLjava/lang/String;[[[ILjava/lang/Object;)Ljava/lang/String;Z
            ptr->paraType = utf8_create();
            parseMethodPara(ptr->descriptor, ptr->paraType);
        }
        s32 j;

        //转attribute为CdoeAttribute
        for (j = 0; j < ptr->attributes_count; j++) {
            if (utf8_equals_c(get_utf8_string(clazz, ptr->attributes[j].attribute_name_index), "Code") == 0) {
                CodeAttribute *ca = jvm_alloc(sizeof(CodeAttribute));
//                if (utf8_equals_c(clazz->name, "com/sun/cldc/i18n/Helper") == 0 &&
//                    0==utf8_equals_c(ptr->descriptor, "(Ljava/io/OutputStream;)Ljava/io/Writer;")) {
//                    int debug = 1;
//                }
                convert_to_code_attribute(ca, &ptr->attributes[j]);
                jvm_free(ptr->attributes[j].info);//无用删除
                ptr->attributes[j].info = NULL;
                ptr->attributes[j].converted_code = (u8 *) ca;
            }else if (utf8_equals_c(get_utf8_string(clazz, ptr->attributes[j].attribute_name_index), "Exceptions") == 0) {

            }
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
        if (cmr->methodParaCount == -1) {
            Utf8String *tmps = utf8_create();
            parseMethodPara(cmr->descriptor, tmps);
            cmr->methodParaCount = tmps->length;
            utf8_destory(tmps);
        }
    }

}


s32 convert_to_code_attribute(CodeAttribute *ca, AttributeInfo *attr) {
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

}


void load_related_class(Utf8String *classpath, Class *clazz, hmap_t classes) {
    s32 classid = -1;
    ConstantPool *p = &(clazz->constantPool);
    if (p->classRef->length > 0) {
        s32 i, j;
        for (i = 0; i < p->classRef->length; i++) {
            ConstantClassRef *ccr = (ConstantClassRef *) arraylist_get_value(p->classRef, i);
            Utf8String *tmpname = utf8_create();
            ConstantUTF8 *utfptr = find_constant_utf8(clazz, ccr->stringIndex);
            utf8_append(tmpname, utfptr->utfstr);
            s32 classid = ccr->index;

            //printf("debug= %s\n", utf8_cstr(tmpname));
            Class *tmpclazz = classes_get(tmpname);
            if (classid != clazz->cff.this_class && 0 == tmpclazz && utf8_indexof_c(tmpname, "[") != 0) {

                Utf8String *fullpath = utf8_create_part(classpath, 0, classpath->length);
                utf8_cstr(tmpname);
                utf8_append(fullpath, tmpname);
                utf8_append_c(fullpath, ".class");

                tmpclazz = class_create();
                s32 rec = tmpclazz->_load_from_file(tmpclazz, utf8_cstr(fullpath));
                if (0 == rec) {
#if _JVM_DEBUG
                    printf("load class:%s \n", utf8_cstr(fullpath));
#endif
                    classes_put(tmpclazz);
                    load_related_class(classpath, tmpclazz, classes); //递归调用载入类
                }
                utf8_destory(fullpath);
            }
            utf8_destory(tmpname);
        }
    }
}

void load_class(Utf8String *pClassPath, Utf8String *pClassName, hmap_t classes) {
    if (!pClassName)return;
    if (utf8_last_indexof_c(pClassPath, "/") != pClassPath->length - 1)utf8_append_c(pClassPath, "/");

    Utf8String *clsName = utf8_create_copy(pClassName);
    utf8_replace_c(clsName, ".", "/");

    classpath = utf8_create_copy(pClassPath);

    Utf8String *tmppath = utf8_create_part(pClassPath, 0, pClassPath->length);
    utf8_append(tmppath, clsName);
    utf8_append_c(tmppath, ".class");
    Class *tmpclazz = classes_get(pClassName);
    if (tmpclazz) {
        return;
    }
    Class *clazz = class_create();
    s32 iret = clazz->_load_from_file(clazz, utf8_cstr(tmppath));
    if (iret != 0) {
        printf(" main class not found : %s", utf8_cstr(pClassName));
        exit(1);
    }
#if _JVM_DEBUG
    printf("load main class:  %s \n", utf8_cstr(tmppath));
#endif
    classes_put(clazz);
    utf8_destory(tmppath);
    utf8_destory(clsName);
    load_related_class(pClassPath, clazz, classes);
}