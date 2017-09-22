//
// Created by gust on 2017/8/8.
//


#include <stdarg.h>
#include "jvm.h"
#include "jvm_util.h"
#include "garbage.h"
#include "java_native_reflect.h"
#include "jdwp.h"

//==================================================================================

Class *classes_get(Utf8String *clsName) {
    if (clsName) {
        Class *cl = hashtable_get(classes, clsName);
        return cl;
    }
    return NULL;
}

Class *classes_load_get_c(c8 *pclassName, Runtime *runtime) {
    Utf8String *ustr = utf8_create_c(pclassName);
    Class *clazz = classes_load_get(ustr, runtime);
    utf8_destory(ustr);
    return clazz;
}

Class *classes_load_get(Utf8String *ustr, Runtime *runtime) {
    if (!ustr)return NULL;
    Class *cl;
    utf8_replace_c(ustr, ".", "/");
    cl = classes_get(ustr);

    if (!cl) {
        load_class(classpath, ustr, classes);
        cl = classes_get(ustr);
        //if (java_debug)event_on_class_prepar(runtime, cl);
    }
    if (cl) {
        class_clinit(cl, runtime);
    }
    return cl;
}

s32 classes_put(Class *clazz) {
    if (clazz) {
        hashtable_put(classes, clazz->name, clazz);
        garbage_refer(clazz, JVM_CLASS);
        return 0;
    }
    return 1;
}

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
    return outputSize;
}

int unicode_2_utf8(u16 *jchar_arr, Utf8String *ustr, s32 totalSize) {
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
            jvm_printf("datatype not found %c\n", ch);
    }
}


void array_classes_create() {
    if (!array_classes) {
        array_classes = hashtable_create(UNICODE_STR_HASH_FUNC, UNICODE_STR_EQUALS_FUNC);
    }

}

Class *array_class_get(Utf8String *descript) {
    Class *clazz = hashtable_get(array_classes, descript);
    if (!clazz) {
        clazz = class_create();
        clazz->arr_data_type = getDataTypeIndex(utf8_char_at(descript, 1));
        clazz->name = utf8_create_copy(descript);
        hashtable_put(array_classes, clazz->name, clazz);
    }
    return clazz;
}

void array_classes_destory() {
    HashtableIterator hti;
    hashtable_iterate(array_classes, &hti);
    for (; hashtable_iter_has_more(&hti);) {
        Utf8String *k = hashtable_iter_next_key(&hti);
        Class *clazz = classes_get(k);
        utf8_destory(clazz->name);
        class_destory(clazz);
    }
    hashtable_destory(array_classes);
}

u8 getDataTypeTag(s32 index) {
    return data_type_str[index];
}

s32 isDataReferByTag(c8 c) {
    if (c == 'L' || c == '[') {
        return 1;
    }
    return 0;
}

s32 isDataReferByIndex(s32 index) {
    if (index == DATATYPE_REFERENCE || index == DATATYPE_ARRAY) {
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
    peek_entry(stack, &entry, stack->size - 1 - cmr->methodParaCount);
    Instance *ins = (Instance *) entry_2_refer(&entry);
    return ins;
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

void printDumpOfClasses() {
    HashtableIterator hti;
    hashtable_iterate(classes, &hti);
    for (; hashtable_iter_has_more(&hti);) {
        Utf8String *k = hashtable_iter_next_key(&hti);
        Class *clazz = classes_get(k);
        jvm_printf("classes entry : hash( %x )%s,%d\n", k->hash, utf8_cstr(k), clazz);
    }
}

s32 loadSysProperties(Utf8String *path) {
    sys_prop = hashtable_create(UNICODE_STR_HASH_FUNC, UNICODE_STR_EQUALS_FUNC);

    FILE *fp = 0;
    Utf8String *filepath = utf8_create_copy(path);
    utf8_append_c(filepath, "sys.properties");
    fp = fopen(utf8_cstr(filepath), "rb");
    utf8_destory(filepath);
    if (fp == 0) {
        jvm_printf("Open file properties failed\n");
        return -1;
    }
    Utf8String *ustr = utf8_create();
    c8 buf[256];
    while (1) {
        u32 len = fread(buf, 1, 256, fp);
        utf8_append_part_c(ustr, buf, 0, len);
        if (feof(fp)) {
            break;
        }
    }
    fclose(fp);

    //parse
    utf8_replace_c(ustr, "\r\n", "\n");
    utf8_replace_c(ustr, "\r", "\n");
    Utf8String *line = utf8_create();
    Utf8String *key = utf8_create();
    Utf8String *val = utf8_create();
    while (ustr->length > 0) {
        s32 lineEndAt = utf8_indexof_c(ustr, "\n");
        utf8_clear(line);
        if (lineEndAt >= 0) {
            utf8_append_part(line, ustr, 0, lineEndAt);
            utf8_substring(ustr, lineEndAt + 1, ustr->length);
        } else {
            utf8_append_part(line, ustr, 0, ustr->length);
            utf8_substring(ustr, ustr->length, ustr->length);
        }
        s32 eqAt = utf8_indexof_c(line, "=");
        if (eqAt > 0) {
            utf8_clear(key);
            utf8_clear(val);
            utf8_append_part(key, line, 0, eqAt);
            utf8_append_part(val, line, eqAt + 1, line->length - (eqAt + 1));
            hashtable_put(sys_prop, key, val);
        }
    }
    utf8_destory(line);
    utf8_destory(ustr);
    return 0;
}

FILE *logfile = NULL;
s64 last_flush = 0;

void open_log() {
#if _JVM_DEBUG_PRINT_FILE
    if (!logfile) {
        logfile = fopen("./jvmlog.txt", "wb+");
    }
#endif
}

void close_log() {
#if _JVM_DEBUG_PRINT_FILE
    if (!logfile) {
        fclose(logfile);
    }
#endif
}

int jvm_printf(const char *format, ...) {
    //garbage_thread_lock();
    va_list vp;
    va_start(vp, format);
    int result = 0;
#if _JVM_DEBUG_PRINT_FILE
    if (logfile) {

        result = vfprintf(logfile, format, vp);
        if (currentTimeMillis() - last_flush > 1000) {
            fflush(logfile);
            last_flush = currentTimeMillis();
        }
    }
#else
    result = vprintf(format, vp);
#endif
    va_end(vp);
    garbage_thread_unlock();
    return result;
}

void invoke_deepth(Runtime *runtime) {
    garbage_thread_lock();
    int i = 0;
    while (runtime) {
        i++;
        runtime = runtime->parent;
    }
    s32 len = i;

#if _JVM_DEBUG_PRINT_FILE
    fprintf(logfile, "%lx", (s64) (long) pthread_self().p);
    for (i = 0; i < len; i++) {
        fprintf(logfile, "    ");
    }
#else
    printf("%lx", (s64) (long) pthread_self().p);
    for (i = 0; i < len; i++) {
        printf("    ");
    }
#endif
    garbage_thread_unlock();
}

//===============================    java 线程  ==================================
void *jtherad_loader(void *para) {
    Instance *jthread = (Instance *) para;
    s32 ret = 0;
    Runtime runtime;
    runtime_create(&runtime);
    localvar_init(&runtime, 1);

    runtime.clazz = jthread->mb.clazz;
    runtime.threadInfo->jthread = jthread;
    runtime.threadInfo->pthread = pthread_self();
    Utf8String *methodName = utf8_create_c("run");
    Utf8String *methodType = utf8_create_c("()V");
    MethodInfo *method = NULL;
    method = find_instance_methodInfo_by_name(jthread, methodName, methodType);
    utf8_destory(methodName);
    utf8_destory(methodType);
#if _JVM_DEBUG > 5
    jvm_printf("therad_loader    %s.%s%s  \n", utf8_cstr(method->_this_class->name),
           utf8_cstr(method->name), utf8_cstr(method->descriptor));
#endif

    if (method) {
        if (java_debug)event_on_thread_start(&runtime);
        jthread_set_threadq_value(jthread, &runtime);
        arraylist_append(thread_list, &runtime);
        push_ref(runtime.stack, (__refer) jthread);

        ret = execute_method(method, &runtime, method->_this_class);

        arraylist_remove(thread_list, &runtime);
        runtime.threadInfo->thread_status = THREAD_STATUS_ZOMBIE;
        if (java_debug)event_on_thread_death(&runtime);
    }
    jthread_set_threadq_value(jthread, NULL);
    runtime_destory(&runtime);
    return (void *) (long) ret;
}

pthread_t jthread_create_and_start(Instance *ins) {//
    pthread_t pt;
    pthread_create(&pt, NULL, jtherad_loader, ins);
    return pt;
}

__refer jthread_get_name_value(Instance *ins) {
    c8 *ptr = getFieldPtr_byName_c(ins, STR_CLASS_JAVA_LANG_THREAD, STR_FIELD_NAME, "[C");
    return getFieldRefer(ptr);
}

__refer jthread_get_threadq_value(Instance *ins) {
    c8 *ptr = getFieldPtr_byName_c(ins, STR_CLASS_JAVA_LANG_THREAD, STR_FIELD_THREADQ, "Ljava/lang/Thread;");
    return getFieldRefer(ptr);
}

void jthread_set_threadq_value(Instance *ins, void *val) {
    c8 *ptr = getFieldPtr_byName_c(ins, STR_CLASS_JAVA_LANG_THREAD, STR_FIELD_THREADQ, "Ljava/lang/Thread;");
    setFieldRefer(ptr, (__refer) val);
}

JavaThreadLock *jthreadlock_create() {
    JavaThreadLock *jtl = jvm_alloc(sizeof(JavaThreadLock));
    pthread_cond_init(&jtl->thread_cond, NULL);
    pthread_mutexattr_init(&jtl->lock_attr);
    pthread_mutexattr_settype(&jtl->lock_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&jtl->mutex_lock, &jtl->lock_attr);
    return jtl;
}

void jthreadlock_destory(JavaThreadLock *jtl) {
    if (jtl) {
        pthread_cond_destroy(&jtl->thread_cond);
        pthread_mutexattr_destroy(&jtl->lock_attr);
        pthread_mutex_destroy(&jtl->mutex_lock);
        jtl->jthread_holder = NULL;
        jtl->hold_count = 0;
        jvm_free(jtl);
    }
}

s32 jthread_lock(MemoryBlock *ins, Runtime *runtime) { //可能会重入，同一个线程多次锁同一对象
    if (ins == NULL)return -1;
    if (!ins->thread_lock) {
        ins->thread_lock = jthreadlock_create();
    }
    JavaThreadLock *jtl = ins->thread_lock;
    if (jtl->jthread_holder == NULL) {//没人锁
        pthread_mutex_lock(&jtl->mutex_lock);
//        hashset_put(runtime->threadInfo->hold_locks, ins);
        jtl->jthread_holder = runtime->threadInfo->jthread;
        jtl->hold_count++;
    } else if (jtl->jthread_holder == runtime->threadInfo->jthread) { //重入
        jtl->hold_count++;
    } else {
        pthread_mutex_lock(&jtl->mutex_lock);
//        hashset_put(runtime->threadInfo->hold_locks, ins);
        jtl->jthread_holder = runtime->threadInfo->jthread;
        jtl->hold_count++;
    }
#if _JVM_DEBUG > 5
    jvm_printf("  lock: %llx   lock holder: %llx, count: %d \n", (s64) (long) (runtime->threadInfo->jthread),
           (s64) (long) (jtl->jthread_holder), jtl->hold_count);
#endif
}

s32 jthread_unlock(MemoryBlock *ins, Runtime *runtime) {
    if (ins == NULL)return -1;
    if (!ins->thread_lock) {
        ins->thread_lock = jthreadlock_create();
    }
    JavaThreadLock *jtl = ins->thread_lock;
    if (jtl->jthread_holder == runtime->threadInfo->jthread) {
        jtl->hold_count--;
        if (jtl->hold_count == 0) {//must change holder first, and unlock later
            jtl->jthread_holder = NULL;
            pthread_mutex_unlock(&jtl->mutex_lock);
//            hashset_remove(runtime->threadInfo->hold_locks, ins, 0);
        }
    }
    return 0;
#if _JVM_DEBUG > 5
    jvm_printf("unlock: %llx   lock holder: %llx, count: %d \n", (s64) (long) (runtime->threadInfo->jthread),
           (s64) (long) (jtl->jthread_holder), jtl->hold_count);
#endif
}

s32 jthread_notify(MemoryBlock *ins, Runtime *runtime) {
    if (ins == NULL)return -1;
    if (!ins->thread_lock) {
        ins->thread_lock = jthreadlock_create();
    }
    pthread_cond_signal(&ins->thread_lock->thread_cond);
    return 0;
}

s32 jthread_notifyAll(MemoryBlock *ins, Runtime *runtime) {
    if (ins == NULL)return -1;
    if (!ins->thread_lock) {
        ins->thread_lock = jthreadlock_create();
    }
    pthread_cond_broadcast(&ins->thread_lock->thread_cond);
    return 0;
}

s32 jthread_yield(Runtime *runtime) {
    sched_yield();
    return 0;
}

s32 jthread_suspend(Runtime *runtime) {
    runtime->threadInfo->suspend_count++;
}

s32 jthread_resume(Runtime *runtime) {
    if (runtime->threadInfo->suspend_count > 0)runtime->threadInfo->suspend_count--;
}

s32 jthread_waitTime(MemoryBlock *ins, Runtime *runtime, long waitms) {
    if (ins == NULL)return -1;
    if (!ins->thread_lock) {
        ins->thread_lock = jthreadlock_create();
    }
    struct timespec t;
    t.tv_sec = waitms / 1000;
    t.tv_nsec = (waitms % 1000) * 1000000;
    runtime->threadInfo->thread_status = THREAD_STATUS_WAIT;
    pthread_cond_timedwait(&ins->thread_lock->thread_cond, &ins->thread_lock->mutex_lock, &t);
    runtime->threadInfo->thread_status = THREAD_STATUS_RUNNING;
    return 0;
}

//===============================    实例化数组  ==================================
Instance *jarray_create_des(s32 count, Utf8String *descript) {
    u8 ch = utf8_char_at(descript, 1);
    s32 typeIdx = getDataTypeIndex(ch);
    s32 width = data_type_bytes[typeIdx];
    Instance *arr = jvm_alloc(sizeof(Instance));
    arr->mb.type = MEM_TYPE_ARR;
    arr->mb.garbage_mark = GARBAGE_MARK_UNDEF;//防止在上次回收过程中，此对象刚被放入池子就被回收
    arr->mb.clazz = array_class_get(descript);
    arr->arr_length = count;
    arr->arr_body = jvm_alloc(width * count);
    return arr;
}

Instance *jarray_create(s32 count, s32 typeIdx, Utf8String *type) {
    Utf8String *ustr = utf8_create_c("[");
    if (type) {
        if (!isDataReferByTag(utf8_char_at(type, 0)))utf8_append_c(ustr, "L");
        utf8_append(ustr, type);
        if (utf8_char_at(type, type->length - 1) != ';')
            utf8_append_c(ustr, ";");
    } else {
        utf8_insert(ustr, ustr->length, getDataTypeTag(typeIdx));
    }
    Instance *arr = jarray_create_des(count, ustr);
    utf8_destory(ustr);
    return arr;
}

s32 jarray_destory(Instance *arr) {
    if (arr && arr->mb.type == MEM_TYPE_ARR) {
        if (isDataReferByIndex(arr->mb.clazz->arr_data_type)) {
            s32 i;
            Long2Double l2d;
            l2d.l = 0;
            for (i = 0; i < arr->arr_length; i++) {//把所有引用去除，否则不会垃圾回收
                jarray_set_field(arr, i, &l2d, data_type_bytes[DATATYPE_REFERENCE]);
            }
        }
        jthreadlock_destory(arr->mb.thread_lock);
        arr->mb.thread_lock = NULL;
        jvm_free(arr->arr_body);
        arr->arr_body = NULL;
        jvm_free(arr);
    }
}

/**
 * create multi array
 * @param dim
 * @param desc
 * @return
 */
Instance *jarray_multi_create(ArrayList *dim, Utf8String *pdesc, s32 deep) {
    Utf8String *desc = utf8_create_copy(pdesc);
    s32 len = (s32) (long) arraylist_get_value(dim, dim->length - 1 - deep);
    if (len == -1)return NULL;
    c8 ch = utf8_char_at(desc, 1);
    s32 typeIdx = getDataTypeIndex(ch);
    Instance *arr = jarray_create_des(len, desc);
    utf8_substring(desc, 1, desc->length);
#if _JVM_DEBUG > 5
    jvm_printf("multi arr deep :%d  type(%c) arr[%x] size:%d\n", deep, ch, arr, len);
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
    utf8_destory(desc);
    return arr;
}


void jarray_set_field(Instance *arr, s32 index, Long2Double *l2d, s32 bytes) {
    if (isDataReferByIndex(arr->mb.clazz->arr_data_type)) {
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
    if (isDataReferByIndex(arr->mb.clazz->arr_data_type)) {
        l2d->r = getFieldRefer(arr->arr_body + index * bytes);
    } else {
        switch (bytes) {
            case 1:
                l2d->i2l.i1 = getFieldByte(arr->arr_body + index * bytes);
                break;
            case 2:
                l2d->i2l.i1 = getFieldShort(arr->arr_body + index * bytes);
                if (arr->mb.clazz->arr_data_type == DATATYPE_JCHAR) {
                    l2d->i2l.i1 = (u16) l2d->i2l.i1;
                }
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
    instance->mb.type = MEM_TYPE_INS;
    instance->mb.garbage_mark = GARBAGE_MARK_UNDEF;
    instance->mb.clazz = clazz;

    instance->obj_fields = jvm_alloc(instance->mb.clazz->field_instance_len);
//    memcpy(instance->obj_fields, instance->obj_of_clazz->field_instance_template,
//           instance->obj_of_clazz->field_instance_len);
    return instance;
}

void instance_init(Instance *ins, Runtime *runtime) {
    if (ins) {
        Utf8String *methodName = utf8_create_c("<init>");
        Utf8String *methodType = utf8_create_c("()V");
        MethodInfo *mi = find_methodInfo_by_name(ins->mb.clazz->name, methodName, methodType);
        push_ref(runtime->stack, (__refer) ins);
        execute_method(mi, runtime, ins->mb.clazz);
        utf8_destory(methodName);
        utf8_destory(methodType);
    }
}


s32 instance_destory(Instance *ins) {
    if (ins->mb.type == MEM_TYPE_INS) {
        jthreadlock_destory(ins->mb.thread_lock);
        jvm_free(ins->obj_fields);
        jvm_free(ins);
    } else if (ins->mb.type == MEM_TYPE_ARR) {
        jarray_destory(ins);
    } else if (ins->mb.type == MEM_TYPE_CLASS) {
        class_destory((Class *) ins);
    }
}

//===============================    实例化字符串  ==================================
Instance *jstring_create(Utf8String *src, Runtime *runtime) {
    if (!src)return NULL;
    Utf8String *clsName = utf8_create_c(STR_CLASS_JAVA_LANG_STRING);
    Class *jstr_clazz = classes_get(clsName);
    Instance *jstring = instance_create(jstr_clazz);
    jstring->mb.clazz = jstr_clazz;
    instance_init(jstring, runtime);

    c8 *ptr = jstring_get_value_ptr(jstring);
    if (src->length) {
        u16 *buf = jvm_alloc(src->length * data_type_bytes[DATATYPE_JCHAR]);
        s32 len = utf8_2_unicode(src, buf);
        Instance *arr = jarray_create(len, DATATYPE_JCHAR, NULL);//u16 type is 5
        memcpy(arr->arr_body, buf, len * data_type_bytes[DATATYPE_JCHAR]);
        jvm_free(buf);
        __refer oldarr = getFieldRefer(ptr);//调用了String.init之后，已经有值了
        if (oldarr) {
            garbage_derefer(oldarr, jstring);
        }
        setFieldRefer(ptr, (__refer) arr);//设置数组
        jstring_set_count(jstring, len);//设置长度
        garbage_refer(arr, jstring);
    } else {
        setFieldRefer(ptr, 0);
    }
    utf8_destory(clsName);

    return jstring;
}

s32 jstring_get_count(Instance *jstr) {
    return getFieldInt(getFieldPtr_byName_c(jstr, STR_CLASS_JAVA_LANG_STRING, STR_FIELD_COUNT, "I"));
}

void jstring_set_count(Instance *jstr, s32 count) {
    setFieldInt(getFieldPtr_byName_c(jstr, STR_CLASS_JAVA_LANG_STRING, STR_FIELD_COUNT, "I"), count);
}

s32 jstring_get_offset(Instance *jstr) {
    return getFieldInt(getFieldPtr_byName_c(jstr, STR_CLASS_JAVA_LANG_STRING, STR_FIELD_OFFSET, "I"));
}

u8 *jstring_get_value_ptr(Instance *jstr) {
    return getFieldPtr_byName_c(jstr, STR_CLASS_JAVA_LANG_STRING, STR_FIELD_VALUE, "[C");
}

Instance *jstring_get_value_array(Instance *jstr) {
    c8 *fieldPtr = jstring_get_value_ptr(jstr);
    Instance *arr = (Instance *) getFieldRefer(fieldPtr);
    return arr;
}

s16 jstring_char_at(Instance *jstr, s32 index) {
    Instance *ptr = jstring_get_value_array(jstr);
    s32 offset = jstring_get_offset(jstr);
    s32 count = jstring_get_count(jstr);
    if (index >= count) {
        return -1;
    }
    if (ptr && ptr->arr_body) {
        u16 *jchar_arr = (u16 *) ptr->arr_body;
        return jchar_arr[offset + index];
    }
    return -1;
}


s32 jstring_index_of(Instance *jstr, uni_char ch, s32 startAt) {
    c8 *fieldPtr = jstring_get_value_ptr(jstr);
    Instance *ptr = (Instance *) getFieldRefer(fieldPtr);//char[]数组实例
    if (ptr && ptr->arr_body) {
        u16 *jchar_arr = (u16 *) ptr->arr_body;
        s32 count = jstring_get_count(jstr);
        s32 offset = jstring_get_offset(jstr);
        s32 i;
        for (i = 0; i < count; i++) {
            if (jchar_arr[i + offset + startAt] == ch) {
                return i + startAt;
            }
        }
    }
    return -1;
}

s32 jstring_equals(Instance *jstr1, Instance *jstr2) {
    if (!jstr1 && !jstr2) { //两个都是null
        return 1;
    } else if (!jstr1) {
        return 0;
    } else if (!jstr2) {
        return 0;
    }
    Instance *arr1 = jstring_get_value_array(jstr1);//取得 String[] value
    Instance *arr2 = jstring_get_value_array(jstr2);//取得 String[] value
    s32 count1 = 0, offset1 = 0, count2 = 0, offset2 = 0;
    //0长度字符串可能value[] 是空值，也可能不是空值但count是0
    if (arr1) {
        count1 = jstring_get_count(jstr1);
        offset1 = jstring_get_offset(jstr1);
    }
    if (arr2) {
        count2 = jstring_get_count(jstr2);
        offset2 = jstring_get_offset(jstr2);
    }
    if (count1 != count2) {
        return 0;
    } else if (count1 == 0 && count2 == 0) {
        return 1;
    }
    if (arr1 && arr2 && arr1->arr_body && arr2->arr_body) {
        if (count1 != count2) {
            return 0;
        }
        u16 *jchar_arr1 = (u16 *) arr1->arr_body;
        u16 *jchar_arr2 = (u16 *) arr2->arr_body;
        s32 i;
        for (i = 0; i < count1; i++) {
            if (jchar_arr1[i + offset1] != jchar_arr2[i + offset2]) {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

s32 jstring_2_utf8(Instance *jstr, Utf8String *utf8) {
    Instance *arr = jstring_get_value_array(jstr);
    if (arr) {
        s32 count = jstring_get_count(jstr);
        s32 offset = jstring_get_offset(jstr);
        unicode_2_utf8(((u16 *) arr->arr_body) + offset, utf8, count);
    }
    return 0;
}
//===============================    例外  ==================================

Instance *exception_create(s32 exception_type, Runtime *runtime) {
#if _JVM_DEBUG > 5
    jvm_printf("create exception : %s\n", exception_class_name[exception_type]);
#endif
    Utf8String *clsName = utf8_create_c(exception_class_name[exception_type]);
    Class *clazz = classes_load_get(clsName, runtime);
    Instance *ins = instance_create(clazz);
    instance_init(ins, runtime);
    garbage_refer(ins, NULL);
    utf8_destory(clsName);
    return ins;
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


c8 *getFieldPtr_byName_c(Instance *instance, c8 *pclassName, c8 *pfieldName, c8 *pfieldType) {
    Utf8String *clsName = utf8_create_c(pclassName);
    Class *clazz = classes_get(clsName);

    //set value
    Utf8String *fieldName = utf8_create_c(pfieldName);
    Utf8String *fieldType = utf8_create_c(pfieldType);
    c8 *ptr = getFieldPtr_byName(instance, clsName, fieldName, fieldType);
    utf8_destory(clsName);
    utf8_destory(fieldName);
    utf8_destory(fieldType);
    return ptr;
}


c8 *getFieldPtr_byName(Instance *instance, Utf8String *clsName, Utf8String *fieldName, Utf8String *fieldType) {
    Class *clazz = classes_get(clsName);
    //set value
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
    return ptr;
}

