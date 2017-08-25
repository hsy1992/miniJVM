//
// Created by gust on 2017/8/8.
//


#include "jvm.h"
#include "jvm_util.h"
#include "garbage.h"

//==================================================================================
/**
 * 把utf字符串转为 java unicode 双字节串
 * @param ustr
 * @param arr
 */
s32 utf8_2_unicode(Utf8String *ustr, u16 *arr) {
    char *pInput = utf8_cstr(ustr);

    int outputSize = 0; //记录转换后的Unicode字符串的字节数

    char *tmp = (c8 *) arr; //临时变量，用于遍历输出字符串

    while (*pInput) {
        if (*pInput > 0x00 && *pInput <= 0x7F) //处理单字节UTF8字符（英文字母、数字）
        {
            *tmp = *pInput;
            tmp++;
            *tmp = 0; //小端法表示，在高地址填补0
        } else if (((*pInput) & 0xE0) == 0xC0) //处理双字节UTF8字符
        {
            char high = *pInput;
            pInput++;
            char low = *pInput;

            if ((low & 0xC0) != 0x80)  //检查是否为合法的UTF8字符表示
            {
                return -1; //如果不是则报错
            }

            *tmp = (high << 6) + (low & 0x3F);
            tmp++;
            *tmp = (high >> 2) & 0x07;
        } else if (((*pInput) & 0xF0) == 0xE0) //处理三字节UTF8字符
        {
            char high = *pInput;
            pInput++;
            char middle = *pInput;
            pInput++;
            char low = *pInput;

            if (((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80)) {
                return -1;
            }

            *tmp = (middle << 6) + (low & 0x7F);
            tmp++;
            *tmp = (high << 4) + ((middle >> 2) & 0x0F);
        } else //对于其他字节数的UTF8字符不进行处理
        {
            return -1;
        }

        pInput++;
        tmp++;
        outputSize += 1;
    }

    *tmp = 0;
    tmp++;
    *tmp = 0;

    return outputSize;
}

int unicode_2_utf8(Utf8String *ustr, u16 *jchar_arr, s32 totalSize) {
    s32 i;
    s32 utf_len;
    for (i = 0; i < totalSize; i++) {
        s32 unic = jchar_arr[i];

        if (unic <= 0x0000007F) {
            // * U-00000000 - U-0000007F:  0xxxxxxx
            utf8_pushback(ustr, unic & 0x7F);

        } else if (unic >= 0x00000080 && unic <= 0x000007FF) {
            // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
            utf8_pushback(ustr, ((unic >> 6) & 0x1F) | 0xC0);
            utf8_pushback(ustr, (unic & 0x3F) | 0x80);

        } else if (unic >= 0x00000800 && unic <= 0x0000FFFF) {
            // * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
            utf8_pushback(ustr, ((unic >> 12) & 0x0F) | 0xE0);
            utf8_pushback(ustr, ((unic >> 6) & 0x3F) | 0x80);
            utf8_pushback(ustr, (unic & 0x3F) | 0x80);

        } else if (unic >= 0x00010000 && unic <= 0x001FFFFF) {
            // * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            utf8_pushback(ustr, ((unic >> 18) & 0x07) | 0xF0);
            utf8_pushback(ustr, ((unic >> 12) & 0x3F) | 0x80);
            utf8_pushback(ustr, ((unic >> 6) & 0x3F) | 0x80);
            utf8_pushback(ustr, (unic & 0x3F) | 0x80);

        } else if (unic >= 0x00200000 && unic <= 0x03FFFFFF) {
            // * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
            utf8_pushback(ustr, ((unic >> 24) & 0x03) | 0xF8);
            utf8_pushback(ustr, ((unic >> 18) & 0x3F) | 0x80);
            utf8_pushback(ustr, ((unic >> 12) & 0x3F) | 0x80);
            utf8_pushback(ustr, ((unic >> 6) & 0x3F) | 0x80);
            utf8_pushback(ustr, (unic & 0x3F) | 0x80);

        } else if (unic >= 0x04000000 && unic <= 0x7FFFFFFF) {
            // * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
            utf8_pushback(ustr, ((unic >> 30) & 0x01) | 0xFC);
            utf8_pushback(ustr, ((unic >> 24) & 0x3F) | 0x80);
            utf8_pushback(ustr, ((unic >> 18) & 0x3F) | 0x80);
            utf8_pushback(ustr, ((unic >> 12) & 0x3F) | 0x80);
            utf8_pushback(ustr, ((unic >> 6) & 0x3F) | 0x80);
            utf8_pushback(ustr, (unic & 0x3F) | 0x80);

        }
        utf_len++;
    }
    return i;
}

/**
 * 交换高低位，little endian 和 big endian 互转时用到
 * @param ptr
 * @param size
 */
void swap_endian_little_big(u8 *ptr, s32 size) {
    int i;
    for (i = 0; i < size / 2; i++) {
        u8 tmp = ptr[i];
        ptr[i] = ptr[size - 1 - i];
        ptr[size - 1 - i] = tmp;
    }
}

/*
boolean   4
char  5
float  6
double 7
unsigned char 8
short   9
int  10
long  11
 reference 12
 */
s32 getDataTypeIndex(c8 ch) {
    switch (ch) {
        case 'I':
            return 10;
        case 'L':
        case '[':
            return 12;
        case 'B':
            return 8;
        case 'C':
            return 5;
        case 'J':
            return 11;
        case 'F':
            return 6;
        case 'D':
            return 7;
        case 'S':
            return 9;
        case 'Z':
            return 4;
        default:
            printf("datatype not found %c\n", ch);
    }
}

s32 isReference(c8 c) {
    if (c == 'L' || c == '[') {
        return 1;
    }
    return 0;
}

/**
 * 从栈中取得实例对象，中间穿插着调用参数
 * @param cmr
 * @param stack
 * @return
 */
Instance *getInstanceInStack(Class *clazz, ConstantMethodRef *cmr, StackFrame *stack) {

    StackEntry entry;
    peekEntry(stack, &entry, stack->size - 1 - cmr->methodParaCount);
    Instance *ins = (Instance *) entry_2_refer(&entry);
    return ins;
}

void parseMethodPara(Utf8String *methodType, Utf8String *out) {
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
                break;
            case 'D':
            case 'J': {
                utf8_substring(para, 1, para->length);
                utf8_append_c(out, "8");
                break;
            }
            case 'L':
                utf8_substring(para, utf8_indexof_c(para, ";") + 1, para->length);
                utf8_append_c(out, "R");
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
                break;
        }
        i++;
    }
    utf8_destory(para);
}

void printDumpOfClasses() {
    HashtableIterator hti;
    hashtable_iterate(classes, &hti);
    for (; hashtable_iter_has_more(&hti);) {
        Utf8String *k = hashtable_iter_next_key(&hti);
        Class *clazz = hashtable_get(classes, k);
        printf("classes entry : hash( %x )%s,%d\n", k->hash, utf8_cstr(k), clazz);
    }
}

Class *getClass(c8 *pclassName, Runtime *runtime) {
    Utf8String *ustr = utf8_create_c(pclassName);
    Class *cl;
    utf8_replace_c(ustr, ".", "/");
    cl = hashtable_get(classes, ustr);

    if (!cl) {
        load_class(classpath, ustr, classes);
        cl = hashtable_get(classes, ustr);
        class_link(cl);
        class_clinit(cl, runtime);
    }
    return cl;
}

void runtime_create(Runtime *runtime) {
    memset(runtime, 0, sizeof(Runtime));
    runtime->stack = stack_init(STACK_LENGHT);
}

//===============================    java 线程  ==================================
void *therad_loader(void *pthread_para) {
    Instance *jthread = (Instance *) pthread_para;
    s32 ret = 0;
    Runtime runtime;
    runtime_create(&runtime);

    runtime.clazz = jthread->obj_of_clazz;
    runtime.thread = jthread;
    Utf8String *methodName = utf8_create_c("run");
    Utf8String *methodType = utf8_create_c("()V");
    MethodInfo *method = NULL;
    method = find_instance_methodInfo_by_name(jthread, methodName, methodType);

#if _JVM_DEBUG
    printf("therad_loader    %s.%s%s  \n", utf8_cstr(method->_this_class->name),
           utf8_cstr(method->name), utf8_cstr(method->descriptor));
#endif

    if (method) {
        arraylist_append(thread_list, jthread);
        push_ref(runtime.stack, (__refer) jthread);
        //printf("Thread [%x] run...\n",jthread);
        ret = execute_method(method, &runtime, method->_this_class, METHOD_INVOKE_VIRTUAL);
        arraylist_remove(thread_list, jthread);
    }
    return (void *) jthread;
}

pthread_t *thread_create_reg(Instance *ins, pthread_t *pthread) {//
    pthread_t *pt = jvm_alloc(sizeof(pthread_t));
    if (pthread) {
        memcpy(pt, pthread, sizeof(pthread_t));
    } else {
        pthread_create(pt, NULL, therad_loader, ins);
    }
    jthread_set_threadq_value(ins, pt);

    return pt;
}

__refer jthread_get_threadq_value(Instance *ins) {
    c8 *ptr = getFieldPtr_byName(ins, "java/lang/Thread", "threadQ", "Ljava/lang/Thread;");
    return getFieldRefer(ptr);
}

void jthread_set_threadq_value(Instance *ins, void *val) {
    c8 *ptr = getFieldPtr_byName(ins, "java/lang/Thread", "threadQ", "Ljava/lang/Thread;");
    setFieldRefer(ptr, (__refer) val);
}

JavaThreadLock *jthreadlock_create() {
    JavaThreadLock *jtl = jvm_alloc(sizeof(JavaThreadLock));
    pthread_mutex_init(&jtl->f_lock, NULL);
    return jtl;
}

void jthreadlock_destory(JavaThreadLock *jtl) {
    pthread_mutex_destroy(&jtl->f_lock);
    jvm_free(jtl);
}

s32 jthread_lock(Instance *ins, Runtime *runtime) { //可能会重入，同一个线程多次锁同一对象
    if (ins == NULL)return -1;
    JavaThreadLock *jtl = ins->thread_lock;
    if (jtl->jthread_holder == NULL) {//没人锁
        pthread_mutex_lock(&jtl->f_lock);
        jtl->jthread_holder = runtime->thread;
        jtl->hold_count++;
    } else if (jtl->jthread_holder == runtime->thread) { //重入
        jtl->hold_count++;
    } else {
        pthread_mutex_lock(&jtl->f_lock);
        jtl->jthread_holder = runtime->thread;
        jtl->hold_count++;
    }
#if _JVM_DEBUG
    printf("  lock: %llx   lock holder: %llx, count: %d \n",(s64)(runtime->thread),(s64)(jtl->jthread_holder),jtl->hold_count);
#endif
}

s32 jthread_unlock(Instance *ins, Runtime *runtime) {
    if (ins == NULL)return -1;
    JavaThreadLock *jtl = ins->thread_lock;
    if (jtl->jthread_holder == runtime->thread) {
        jtl->hold_count--;
        if (jtl->hold_count == 0) {//must change holder first, and unlock later
            jtl->jthread_holder = NULL;
            pthread_mutex_unlock(&jtl->f_lock);
        }
    }
#if _JVM_DEBUG
    printf("unlock: %llx   lock holder: %llx, count: %d \n",(s64)(runtime->thread),(s64)(jtl->jthread_holder),jtl->hold_count);
#endif
}

//===============================    实例化相关  ==================================

u8 instance_of(Class *clazz, Instance *ins) {
    Class *ins_of_class = ins->obj_of_clazz;
    while (ins_of_class) {
        if (ins_of_class == clazz) {
            return 1;
        }
        ins_of_class = getSuperClass(ins_of_class);
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

//===============================    实例化数组  ==================================
Instance *jarray_create(s32 count, s32 typeIdx) {
    s32 width = data_type_bytes[typeIdx];
    Instance *arr = jvm_alloc(sizeof(Instance));
    arr->type = MEM_TYPE_ARR;
    Utf8String *clsName = utf8_create_c("java/lang/Object");
    arr->obj_of_clazz = hashtable_get(classes, clsName);
    utf8_destory(clsName);
    arr->arr_length = count;
    arr->arr_data_type = typeIdx;
    arr->arr_body = jvm_alloc(width * count);
    arr->thread_lock = jthreadlock_create();
    return arr;
}

s32 jarray_destory(Instance *arr) {
    if (arr && arr->type == MEM_TYPE_ARR) {
        if (arr->arr_data_type == ARRAY_REFERENCE_TYPE) {
            s32 i;
            Long2Double l2d;
            l2d.l = 0;
            for (i = 0; i < arr->arr_length; i++) {//把所有引用去除，否则不会垃圾回收
                jarray_set_field(arr, i, &l2d, data_type_bytes[ARRAY_REFERENCE_TYPE]);
            }
        }
        jthreadlock_destory(arr->thread_lock);
        jvm_free(arr->arr_body);
        jvm_free(arr);
    }
}

/**
 * create multi array
 * @param dim
 * @param desc
 * @return
 */
Instance *jarray_multi_create(ArrayList *dim, Utf8String *desc, s32 deep) {
    s32 len = (s32) arraylist_get_value(dim, dim->length - 1 - deep);
    if (len == -1)return NULL;
    c8 ch = utf8_char_at(desc, deep + 1);
    s32 typeIdx = getDataTypeIndex(ch);
    Instance *arr = jarray_create(len, typeIdx);
#if _JVM_DEBUG
    printf("multi arr deep :%d  type(%c) arr[%x] size:%d\n", deep, ch, arr, len);
#endif
    if (ch == '[') {
        int i;
        Long2Double l2d;
        s32 bytes = data_type_bytes[typeIdx];
        for (i = 0; i < len; i++) {
            Instance *elem = jarray_multi_create(dim, desc, deep + 1);
            l2d.r = (__refer) elem;
            jarray_set_field(arr, i, &l2d, bytes);
        }
    }
    return arr;
}


void jarray_set_field(Instance *arr, s32 index, Long2Double *l2d, s32 bytes) {
    if (arr->arr_data_type == ARRAY_REFERENCE_TYPE) {
        __refer ref = (__refer) getFieldRefer(arr->arr_body + index * bytes);
        if (ref) { //把老索引关闭
            garbage_derefer((__refer) ref, arr);
        }
        if (l2d->r) {//建立新索引
            garbage_refer((__refer) l2d->r, arr);
        }
        setFieldRefer(arr->arr_body + index * bytes, l2d->r);
    } else {
        switch (bytes) {
            case 1:
                setFieldByte(arr->arr_body + index * bytes, (c8) l2d->i2l.i1);
                break;
            case 2:
                setFieldShort(arr->arr_body + index * bytes, (s16) l2d->i2l.i1);
                break;
            case 4:
                setFieldInt(arr->arr_body + index * bytes, (s32) l2d->i2l.i1);
                break;
            case 8:
                setFieldLong(arr->arr_body + index * bytes, l2d->l);
                break;
        }
    }
}

void jarray_get_field(Instance *arr, s32 index, Long2Double *l2d, s32 bytes) {
    if (arr->arr_data_type == ARRAY_REFERENCE_TYPE) {
        l2d->r = getFieldRefer(arr->arr_body + index * bytes);
    } else {
        switch (bytes) {
            case 1:
                l2d->i2l.i1 = getFieldByte(arr->arr_body + index * bytes);
                break;
            case 2:
                l2d->i2l.i1 = getFieldShort(arr->arr_body + index * bytes);
                break;
            case 4:
                l2d->i2l.i1 = getFieldInt(arr->arr_body + index * bytes);
                break;
            case 8:
                l2d->l = getFieldLong(arr->arr_body + index * bytes);
                break;
        }
    }
}

//===============================    实例化对象  ==================================
Instance *instance_create(Class *clazz) {
    Instance *instance = jvm_alloc(sizeof(Instance));
    instance->type = MEM_TYPE_OBJ;
    instance->obj_of_clazz = clazz;
    instance->thread_lock = jthreadlock_create();

    instance->obj_fields = jvm_alloc(instance->obj_of_clazz->field_instance_len);
    memcpy(instance->obj_fields, instance->obj_of_clazz->field_instance_template,
           instance->obj_of_clazz->field_instance_len);
    return instance;
}

void instance_init(Instance *ins, Runtime *runtime) {
    if (ins) {
        Utf8String *methodName = utf8_create_c("<init>");
        Utf8String *methodType = utf8_create_c("()V");
        MethodInfo *mi = find_methodInfo_by_name(ins->obj_of_clazz->name, methodName, methodType);
        push_ref(runtime->stack, (__refer) ins);
        execute_method(mi, runtime, ins->obj_of_clazz, METHOD_INVOKE_SPECIAL);
        utf8_destory(methodName);
        utf8_destory(methodType);
    }
}


s32 instance_destory(Instance *ins) {
    if (ins->type == MEM_TYPE_OBJ) {
        jthreadlock_destory(ins->thread_lock);
        jvm_free(ins->obj_fields);
        jvm_free(ins);
    } else if (ins->type == MEM_TYPE_ARR) {
        jarray_destory(ins);
    } else if (ins->type == MEM_TYPE_CLASS) {
        class_destory((Class *) ins);
    }
}

//===============================    实例化字符串  ==================================
Instance *jstring_create(Utf8String *src, Runtime *runtime) {
    Utf8String *clsName = utf8_create_c("java/lang/String");
    Class *jstr_clazz = hashtable_get(classes, clsName);
    Instance *jstring = instance_create(jstr_clazz);
    jstring->obj_of_clazz = jstr_clazz;
    instance_init(jstring, runtime);

    c8 *ptr = jstring_get_value_ptr(jstring);
    if (src->length) {
        u16 *buf = jvm_alloc(src->length * data_type_bytes[ARRAY_CHAR_TYPE]);
        s32 len = utf8_2_unicode(src, buf);
        Instance *arr = jarray_create(len, ARRAY_CHAR_TYPE);//c8 type is 5
        memcpy(arr->arr_body, buf, len * data_type_bytes[ARRAY_CHAR_TYPE]);
        setFieldRefer(ptr, (__refer) arr);//设置数组
        jstring_set_count(jstring, len);//设置长度
        garbage_refer(arr, jstring);
    } else {
        setFieldRefer(ptr, 0);
    }
    utf8_destory(clsName);

    return jstring;
}

Instance *exception_create(s32 exception_type, Runtime *runtime) {
#if _JVM_DEBUG
    printf("create exception : %s", exception_class_name[exception_type]);
#endif
    Utf8String *clsName = utf8_create_c(exception_class_name[exception_type]);
    Class *clazz = hashtable_get(classes, clsName);
    if (!clazz) {
        load_class(classpath, clsName, classes);
        clazz = hashtable_get(classes, clsName);
    }
    Instance *ins = instance_create(clazz);
    ins->obj_of_clazz = clazz;
    instance_init(ins, runtime);
    utf8_destory(clsName);
    return ins;
}

s32 jstring_get_count(Instance *jstr) {
    return getFieldInt(getFieldPtr_byName(jstr, "java/lang/String", "count", "I"));
}

void jstring_set_count(Instance *jstr, s32 count) {
    setFieldInt(getFieldPtr_byName(jstr, "java/lang/String", "count", "I"), count);
}


u8 *jstring_get_value_ptr(Instance *jstr) {
    return getFieldPtr_byName(jstr, "java/lang/String", "value", "[C");
}

Instance *jstring_get_value_array(Instance *jstr) {
    c8 *fieldPtr = jstring_get_value_ptr(jstr);
    Instance *arr = (Instance *) getFieldRefer(fieldPtr);
    return arr;
}

s16 jstring_char_at(Instance *jstr, s32 index) {
    Instance *ptr = jstring_get_value_array(jstr);
    if (ptr && ptr->arr_body) {
        u16 *jchar_arr = (u16 *) ptr->arr_body;
        return jchar_arr[index];
    }
    return -1;
}


s32 jstring_index_of(Instance *jstr, uni_char ch, s32 startAt) {
    c8 *fieldPtr = jstring_get_value_ptr(jstr);
    Instance *ptr = (Instance *) getFieldRefer(fieldPtr);//char[]数组实例
    if (ptr && ptr->arr_body) {
        u16 *jchar_arr = (u16 *) ptr->arr_body;
        s32 i;
        for (i = startAt; i < ptr->arr_length; i++) {
            if (jchar_arr[i] == ch) {
                return i;
            }
        }
    }
    return -1;
}

s32 jstring_equals(Instance *jstr1, Instance *jstr2) {
    if (!jstr1 && !jstr2) {
        return 0;
    } else if (!jstr1) {
        return -1;
    } else if (!jstr2) {
        return 1;
    }
    Instance *arr1 = jstring_get_value_array(jstr1);//取得 String[] value
    Instance *arr2 = jstring_get_value_array(jstr2);//取得 String[] value
    if (arr1 && arr1->arr_body && arr2 && arr2->arr_body) {
        u16 *jchar_arr1 = (u16 *) arr1->arr_body;
        u16 *jchar_arr2 = (u16 *) arr2->arr_body;
        s32 i;
        for (i = 0; i < arr1->arr_length; i++) {
            if (jchar_arr1[i] > jchar_arr2[i]) {
                return 0;
            } else if (jchar_arr1[i] < jchar_arr2[i]) {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

//===============================    实例操作  ==================================
/**
 * get instance field value address
 * @param clazz maybe the ins 's superclass
 * @param ins
 * @param field_ref
 * @return
 */
c8 *getInstanceFieldPtr(Instance *ins, FieldInfo *fi) {
    return &(ins->obj_fields[fi->_this_class->field_instance_start + fi->offset]);
}

c8 *getStaticFieldPtr(FieldInfo *fi) {
    return &(fi->_this_class->field_static[fi->offset]);
}


void setFieldInt(c8 *ptr, s32 v) {
    memcpy(ptr, &v, sizeof(s32));
}

void setFieldRefer(c8 *ptr, __refer v) {
    memcpy(ptr, &v, sizeof(__refer));
}

void setFieldLong(c8 *ptr, s64 v) {
    memcpy(ptr, &v, sizeof(s64));
}

void setFieldShort(c8 *ptr, s16 v) {
    memcpy(ptr, &v, sizeof(s16));
}

void setFieldByte(c8 *ptr, c8 v) {
    ptr[0] = v;
}

void setFieldDouble(c8 *ptr, f64 v) {
    memcpy(ptr, &v, sizeof(f64));
}

void setFieldFloat(c8 *ptr, f32 v) {
    memcpy(ptr, &v, sizeof(f32));
}

s32 getFieldInt(c8 *ptr) {
    s32 v = 0;
    memcpy((c8 *) &v, ptr, sizeof(s32));
    return v;
}

__refer getFieldRefer(c8 *ptr) {
    __refer v = 0;
    memcpy((c8 *) &v, ptr, sizeof(__refer));
    return v;
}

s16 getFieldShort(c8 *ptr) {
    s16 v = 0;
    memcpy((c8 *) &v, ptr, sizeof(s16));
    return v;
}

c8 getFieldByte(c8 *ptr) {
    c8 v = ptr[0];
    return v;
}

s64 getFieldLong(c8 *ptr) {
    s64 v = 0;
    memcpy((c8 *) &v, ptr, sizeof(s64));
    return v;
}

f32 getFieldFloat(c8 *ptr) {
    f32 v = 0;
    memcpy((c8 *) &v, ptr, sizeof(f32));
    return v;
}


f32 getFieldDouble(c8 *ptr) {
    f64 v = 0;
    memcpy((c8 *) &v, ptr, sizeof(f64));
    return v;
}

Class *getSuperClass(Class *clazz) {
    s32 superid = clazz->cff.super_class;
    ConstantClassRef *ccf = find_constant_classref(clazz, superid);
    if (ccf) {
        Utf8String *clsName_u = get_utf8_string(clazz, ccf->stringIndex);
        Class *other = hashtable_get(classes, clsName_u);
        return other;
    }
    return NULL;
}


c8 *getFieldPtr_byName(Instance *instance, c8 *pclassName, c8 *pfieldName, c8 *pfieldType) {
    Utf8String *clsName = utf8_create_c(pclassName);
    Class *clazz = hashtable_get(classes, clsName);

    //set value
    Utf8String *fieldName = utf8_create_c(pfieldName);
    Utf8String *fieldType = utf8_create_c(pfieldType);
    s32 fieldIdx = find_constant_fieldref_index(clazz, fieldName, fieldType);
    c8 *ptr = NULL;
    FieldInfo *fi = NULL;
    if (fieldIdx >= 0) {//不在常量表中
        ConstantFieldRef *cfr = find_constant_fieldref(clazz, fieldIdx);
        fi = cfr->fieldInfo;;
    } else {//找字段信息field_info
        fi = find_fieldInfo_by_name(clsName, fieldName, fieldType);
    }
    if (fi) {
        if (fi->access_flags & ACC_STATIC) {
            ptr = getStaticFieldPtr(fi);
        } else {
            ptr = getInstanceFieldPtr(instance, fi);
        }
    }
    utf8_destory(clsName);
    utf8_destory(fieldName);
    utf8_destory(fieldType);
    return ptr;
}


