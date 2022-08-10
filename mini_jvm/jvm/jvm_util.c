//
// Created by gust on 2017/8/8.
//


#include <stdarg.h>
#include "../utils/d_type.h"
#include "jvm.h"

#include <sys/stat.h>
#include "../utils/miniz_wrapper.h"
#include "jvm_util.h"
#include "garbage.h"
#include "java_native_reflect.h"
#include "java_native_io.h"
#include "jdwp.h"



//==================================================================================

void thread_lock_init(ThreadLock *lock) {
    if (lock) {
        cnd_init(&lock->thread_cond);
//        pthread_mutexattr_init(&lock->lock_attr);
//        pthread_mutexattr_settype(&lock->lock_attr, PTHREAD_MUTEX_RECURSIVE);
        mtx_init(&lock->mutex_lock, mtx_recursive);
    }
}

void thread_lock_dispose(ThreadLock *lock) {
    if (lock) {
        cnd_destroy(&lock->thread_cond);
//        pthread_mutexattr_destroy(&lock->lock_attr);
        mtx_destroy(&lock->mutex_lock);
    }
}


JClass *classes_get_c(c8 *clsName) {
    Utf8String *ustr = utf8_create_c(clsName);
    JClass *clazz = classes_get(ustr);
    utf8_destory(ustr);
    return clazz;
}

//读取缓存
JClass *classes_get(Utf8String *clsName) {
    JClass *cl = NULL;
    if (clsName) {
        cl = hashtable_get(sys_classloader->classes, clsName);
    }
    return cl;
}

JClass *classes_load_get_without_clinit(Utf8String *ustr, Runtime *runtime) {
    if (!ustr)return NULL;
    JClass *cl;
    //锁
    spin_lock(&sys_classloader->lock);//fast lock
    if (utf8_index_of(ustr, '.') >= 0)
        utf8_replace_c(ustr, ".", "/");
    spin_unlock(&sys_classloader->lock);
    cl = classes_get(ustr);
    if (!cl) {
        garbage_thread_lock();//slow lock
        cl = classes_get(ustr);
        if (!cl) {
            load_class(sys_classloader, ustr, runtime);
        }
        if (!cl) {
            array_class_create_get(runtime, ustr);
        }
        cl = classes_get(ustr);
        garbage_thread_unlock();
        //if (java_debug)event_on_class_prepar(runtime, cl);
    }
    return cl;
}

//根据包名 runtime 获取JClass
JClass *classes_load_get_c(c8 *pclassName, Runtime *runtime) {
    Utf8String *ustr = utf8_create_c(pclassName);
    JClass *clazz = classes_load_get(ustr, runtime);
    utf8_destory(ustr);
    return clazz;
}

//类名 -> JClass
JClass *classes_load_get(Utf8String *ustr, Runtime *runtime) {
    JClass *cl = classes_load_get_without_clinit(ustr, runtime);
    if (cl && cl->status < CLASS_STATUS_CLINITED) {
        //初始化类，调用静态区块
        class_clinit(cl, runtime);
    }
    return cl;
}

s32 classes_put(JClass *clazz) {
    if (clazz) {
        //jvm_printf("sys_classloader %d : %s\n", sys_classloader->classes->entries, utf8_cstr(clazz->name));
        hashtable_put(sys_classloader->classes, clazz->name, clazz);
        return 0;
    }
    return -1;
}

JClass *primitive_class_create_get(Runtime *runtime, Utf8String *ustr) {
    JClass *cl = classes_get(ustr);
    if (!cl) {
        garbage_thread_lock();
        cl = class_create(runtime);
        cl->name = ustr;
        cl->primitive = 1;
        classes_put(cl);
        gc_refer_hold(cl);
        garbage_thread_unlock();
    } else {
        utf8_destory(ustr);
    }
    if (!cl->ins_class) {
        cl->ins_class = insOfJavaLangClass_create_get(runtime, cl);
    }
    return cl;
}

JClass *array_class_create_get(Runtime *runtime, Utf8String *desc) {
    if (desc && desc->length && utf8_char_at(desc, 0) == '[') {
        JClass *clazz = hashtable_get(sys_classloader->classes, desc);
        if (!clazz) {
            garbage_thread_lock();
            clazz = hashtable_get(sys_classloader->classes, desc);//maybe other thread created
            if (!clazz) {
                clazz = class_create(runtime);
                clazz->mb.arr_type_index = getDataTypeIndex(utf8_char_at(desc, 1));
                clazz->name = utf8_create_copy(desc);
                clazz->superclass = classes_get_c(STR_CLASS_JAVA_LANG_OBJECT);
                classes_put(clazz);
                gc_refer_hold(clazz);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
                jvm_printf("load class:  %s \n", utf8_cstr(desc));
#endif

            }
            garbage_thread_unlock();
        }
        return clazz;
    }
    return NULL;
}

/**
 * array class get accepted name :
 *
 *    7                     -> [D
 *
 * @param runtime
 * @param name
 * @return
 */
JClass *array_class_get_by_index(Runtime *runtime, s32 typeIdx) {
    JClass *clazz = NULL;
    if (jvm_runtime_cache.array_classes[typeIdx] == NULL) {
        Utf8String *ustr = utf8_create_c("[");
        utf8_insert(ustr, ustr->length, getDataTypeTag(typeIdx));
        clazz = jvm_runtime_cache.array_classes[typeIdx] = array_class_create_get(runtime, ustr);
        utf8_destory(ustr);
    } else {
        clazz = jvm_runtime_cache.array_classes[typeIdx];
    }
    return clazz;
}


/**
 * array class get accepted name :
 *
 *    [D                    -> [[D
 *    java/lang/String      -> [Ljava/lang/String;
 *    Ljava/lang/Float;     -> [Ljava/lang/Float;
 *
 *
 * @param runtime
 * @param name
 * @return
 */
JClass *array_class_get_by_name(Runtime *runtime, Utf8String *name) {
    JClass *clazz = NULL;
    if (name) {
        Utf8String *ustr = utf8_create_c("[");
        if (!isDataReferByTag(utf8_char_at(name, 0))) {  //not : L [
            if (!isDataReferByTag(utf8_char_at(name, 0))) {
                utf8_append_c(ustr, "L");
            }
            utf8_append(ustr, name);
            if (utf8_char_at(name, name->length - 1) != ';') {
                utf8_append_c(ustr, ";");
            }
        } else {
            utf8_append(ustr, name);
        }
        clazz = array_class_create_get(runtime, ustr);
        utf8_destory(ustr);

    }
    return clazz;
}

Runtime *threadlist_get(s32 i) {
    Runtime *r = NULL;
    if (i < thread_list->length) {
        r = (Runtime *) arraylist_get_value(thread_list, i);
    }
    return r;
}

void threadlist_remove(Runtime *r) {
    arraylist_remove(thread_list, r);
}

void threadlist_add(Runtime *r) {
    arraylist_push_back(thread_list, r);
}

s32 threadlist_count_none_daemon() {
    spin_lock(&thread_list->spinlock);
    s32 count = 0;
    s32 i;
    for (i = 0; i < thread_list->length; i++) {
        Runtime *r = (Runtime *) arraylist_get_value_unsafe(thread_list, i);
        Instance *ins = r->threadInfo->jthread;
        s32 daemon = jthread_get_daemon_value(ins, r);
        if (!daemon) {
            count++;
        }
    }
    spin_unlock(&thread_list->spinlock);
    return count;
}

/**
 * 把utf字符串转为 java unicode 双字节串
 * @param ustr in
 * @param arr out
 */
s32 utf8_2_unicode(Utf8String *ustr, u16 *arr) {
    char *pInput = utf8_cstr(ustr);

    int outputSize = 0; //记录转换后的Unicode字符串的字节数

    char *tmp = (c8 *) arr; //临时变量，用于遍历输出字符串
    while (*pInput) {
        if (*pInput > 0x00 && *pInput <= 0x7F) //处理单字节UTF8字符（英文字母、数字）
        {
            *tmp = *pInput;
            pInput++;
            tmp++;
            *tmp = 0; //小端法表示，在高地址填补0
            tmp++;
        } else if (((*pInput) & 0xE0) == 0xC0) //处理双字节UTF8字符
        {
            char high = *pInput;
            pInput++;
            char low = *pInput;
            pInput++;

            if ((low & 0xC0) != 0x80)  //检查是否为合法的UTF8字符表示
            {
                return -1; //如果不是则报错
            }

            *tmp = (high << 6) + (low & 0x3F);
            tmp++;
            *tmp = (high >> 2) & 0x07;
            tmp++;
        } else if (((*pInput) & 0xF0) == 0xE0) //处理三字节UTF8字符
        {
            char high = *pInput;
            pInput++;
            char middle = *pInput;
            pInput++;
            char low = *pInput;
            pInput++;
            if (((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80)) {
                return -1;
            }

            *tmp = (middle << 6) + (low & 0x7F);
            tmp++;
            *tmp = (high << 4) + ((middle >> 2) & 0x0F);
            tmp++;
        } else if (((*pInput) & 0xF8) == 0xF0) //处理四字节UTF8字符
        {
            Int2Float i2f;
            i2f.c0 = *pInput;
            pInput++;
            i2f.c1 = *pInput;
            pInput++;
            i2f.c2 = *pInput;
            pInput++;
            i2f.c3 = *pInput;
            pInput++;
            if (((i2f.c1 & 0xC0) != 0x80) || ((i2f.c2 & 0xC0) != 0x80) || ((i2f.c3 & 0xC0) != 0x80)) {
                return -1;
            }
            i2f.c0 = (c8) (i2f.c0 << 5) >> 5;
            i2f.c1 = (c8) (i2f.c1 << 2) >> 2;
            i2f.c2 = (c8) (i2f.c2 << 2) >> 2;
            i2f.c3 = (c8) (i2f.c3 << 2) >> 2;
            s32 code = i2f.c3 + (i2f.c2 << 6) + (i2f.c1 << 12) + (i2f.c0 << 18);
            //jchar 两字节表示不下， 得使用双jchar,这里简易处理，替换成了空格
            *tmp = ' ';
            tmp++;
            *tmp = 0;
            tmp++;
        } else //对于其他字节数的UTF8字符不进行处理
        {
            return -1;
        }
        outputSize += 1;
    }
    return outputSize;
}

int unicode_2_utf8(u16 *jchar_arr, Utf8String *ustr, s32 totalSize) {
    s32 i;
    s32 utf_len = 0;
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
 * @param ptr addr
 * @param size len
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
        case 'C':
            return 5;
        case 'B':
            return 8;
        case 'Z':
            return 4;
        case 'J':
            return 11;
        case 'F':
            return 6;
        case 'D':
            return 7;
        case 'S':
            return 9;
        default:
            jvm_printf("datatype not found %c\n", ch);
    }
    return 0;
}

c8 *getDataTypeFullName(c8 ch) {
    switch (ch) {
        case 'I':
            return "int";
        case 'C':
            return "char";
        case 'B':
            return "byte";
        case 'Z':
            return "boolean";
        case 'J':
            return "long";
        case 'F':
            return "float";
        case 'D':
            return "double";
        case 'S':
            return "short";
    }
    return NULL;
}

u8 getDataTypeTagByName(Utf8String *name) {
    if (utf8_equals_c(name, "int")) {
        return 'I';
    }
    if (utf8_equals_c(name, "long")) {
        return 'J';
    }
    if (utf8_equals_c(name, "char")) {
        return 'C';
    }
    if (utf8_equals_c(name, "byte")) {
        return 'B';
    }
    if (utf8_equals_c(name, "boolean")) {
        return 'Z';
    }
    if (utf8_equals_c(name, "short")) {
        return 'S';
    }
    if (utf8_equals_c(name, "double")) {
        return 'D';
    }
    if (utf8_equals_c(name, "float")) {
        return 'F';
    }
    return 'V';
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

s32 isData8ByteByTag(c8 c) {
    if (c == 'D' || c == 'J') {
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


void printDumpOfClasses() {
    HashtableIterator hti;
    hashtable_iterate(sys_classloader->classes, &hti);
    for (; hashtable_iter_has_more(&hti);) {
        Utf8String *k = hashtable_iter_next_key(&hti);
        JClass *clazz = classes_get(k);
        jvm_printf("classes entry : hash( %x )%s,%d\n", k->hash, utf8_cstr(k), clazz);
    }
}

void sys_properties_set_c(c8 *key, c8 *val) {
    Utf8String *ukey = utf8_create_c(key);
    Utf8String *uval = utf8_create_c(val);
    hashtable_put(sys_prop, ukey, uval);
}

s32 sys_properties_load(ClassLoader *loader) {
    sys_prop = hashtable_create(UNICODE_STR_HASH_FUNC, UNICODE_STR_EQUALS_FUNC);
    hashtable_register_free_functions(sys_prop,
                                      (HashtableKeyFreeFunc) utf8_destory,
                                      (HashtableValueFreeFunc) utf8_destory);
    Utf8String *ustr = NULL;
    Utf8String *prop_name = utf8_create_c("sys.properties");
    ByteBuf *buf = load_file_from_classpath(sys_classloader, prop_name);
    if (buf) {
        ustr = utf8_create();
        while (bytebuf_available(buf)) {
            c8 ch = (c8) bytebuf_read(buf);
            utf8_insert(ustr, ustr->length, ch);
        }
        bytebuf_destory(buf);
    }
    utf8_destory(prop_name);
    //parse
    if (ustr) {
        utf8_replace_c(ustr, "\r\n", "\n");
        utf8_replace_c(ustr, "\r", "\n");
        Utf8String *line = utf8_create();
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
                Utf8String *key = utf8_create();
                Utf8String *val = utf8_create();
                utf8_append_part(key, line, 0, eqAt);
                utf8_append_part(val, line, eqAt + 1, line->length - (eqAt + 1));
                hashtable_put(sys_prop, key, val);
            }
        }
        utf8_destory(line);
        utf8_destory(ustr);
    }

    //modify os para
#if __JVM_OS_MAC__
    sys_properties_set_c("os.name", "Mac");
    sys_properties_set_c("path.separator", ":");
    sys_properties_set_c("file.separator", "/");
    sys_properties_set_c("line.separator", "\n");
#elif __JVM_OS_LINUX__
    sys_properties_set_c("os.name","Linux");
    sys_properties_set_c("path.separator",":");
    sys_properties_set_c("file.separator","/");
    sys_properties_set_c("line.separator", "\n");
#elif __JVM_OS_MINGW__ || __JVM_OS_CYGWIN__ || __JVM_OS_VS__
    sys_properties_set_c("os.name","Windows");
    sys_properties_set_c("path.separator",";");
    sys_properties_set_c("file.separator","\\");
    sys_properties_set_c("line.separator", "\r\n");
#endif


    return 0;
}

void sys_properties_dispose() {
    hashtable_destory(sys_prop);
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
        logfile = NULL;
        last_flush = 0;
    }
#endif
}

int jvm_printf(const char *format, ...) {
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
    result = vfprintf(stderr, format, vp);
#endif
    va_end(vp);
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
    fprintf(logfile, "%llx", (s64) (intptr_t) thrd_current());
    for (i = 0; i < len; i++) {
        fprintf(logfile, "  ");
    }
#else
#if __JVM_OS_MAC__ || __JVM_OS_CYGWIN__
    fprintf(stderr, "%llx", (s64) (intptr_t) thrd_current());
#else
    fprintf(stderr, "%llx", (s64) (intptr_t) thrd_current());
#endif //
    for (i = 0; i < len; i++) {
        fprintf(stderr, "  ");
    }
#endif
    garbage_thread_unlock();
}

//===============================    java 线程  ==================================

s32 jthread_init(Instance *jthread, Runtime *runtime) {
    localvar_init(runtime, 1);
    jthread_set_stackframe_value(jthread, runtime);
    runtime->clazz = jthread->mb.clazz;
    runtime->threadInfo->jthread = jthread;
    runtime->threadInfo->thread_status = THREAD_STATUS_RUNNING;
    threadlist_add(runtime);
    return 0;
}

s32 jthread_dispose(Instance *jthread) {
    Runtime *runtime = (Runtime *) jthread_get_stackframe_value(jthread);
    gc_move_refer_thread_2_gc(runtime);
    threadlist_remove(runtime);
    if (java_debug)event_on_thread_death(runtime->threadInfo->jthread);
    //destory
    jthread_set_stackframe_value(jthread, NULL);

    return 0;
}

s32 jtherad_run(void *para) {
    Instance *jthread = (Instance *) para;
    jvm_printf("thread start %llx\n", (s64) (intptr_t) jthread);

    s32 ret = 0;
    Runtime *runtime = (Runtime *) jthread_get_stackframe_value(jthread);
    runtime->threadInfo->pthread = thrd_current();

    Utf8String *methodName = utf8_create_c("run");
    Utf8String *methodType = utf8_create_c("()V");
    MethodInfo *method = NULL;
    method = find_instance_methodInfo_by_name(jthread, methodName, methodType, runtime);
    utf8_destory(methodName);
    utf8_destory(methodType);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("therad_loader    %s.%s%s  \n", utf8_cstr(method->_this_class->name),
               utf8_cstr(method->name), utf8_cstr(method->descriptor));
#endif
    gc_refer_reg(runtime, jthread);
    if (java_debug)event_on_thread_start(runtime->threadInfo->jthread);
    runtime->threadInfo->thread_status = THREAD_STATUS_RUNNING;
    push_ref(runtime->stack, (__refer) jthread);
    ret = execute_method_impl(method, runtime, method->_this_class);
    if (ret != RUNTIME_STATUS_NORMAL && ret != RUNTIME_STATUS_INTERRUPT) {
        print_exception(runtime);
    }
    runtime->threadInfo->thread_status = THREAD_STATUS_ZOMBIE;
    jthread_dispose(jthread);
    localvar_dispose(runtime);
    runtime_destory(runtime);
    jvm_printf("thread over %llx\n", (s64) (intptr_t) jthread);
    return ret;
}

thrd_t jthread_start(Instance *ins) {//
    Runtime *runtime = runtime_create(NULL);
    jthread_init(ins, runtime);
    thrd_t pt;
    thrd_create(&pt, jtherad_run, ins);
    return pt;
}

__refer jthread_get_name_value(Instance *ins) {
    c8 *ptr = getInstanceFieldPtr(ins, jvm_runtime_cache.thread_name);
    return getFieldRefer(ptr);
}

__refer jthread_get_stackframe_value(Instance *ins) {
    c8 *ptr = getInstanceFieldPtr(ins, jvm_runtime_cache.thread_stackFrame);
    return (__refer) (intptr_t) getFieldLong(ptr);
}

void jthread_set_stackframe_value(Instance *ins, __refer val) {
    c8 *ptr = getInstanceFieldPtr(ins, jvm_runtime_cache.thread_stackFrame);
    setFieldLong(ptr, (s64) (intptr_t) val);
}

s32 jthread_get_daemon_value(Instance *ins, Runtime *runtime) {
    c8 *ptr = getFieldPtr_byName_c(ins, STR_CLASS_JAVA_LANG_THREAD, "daemon", "Z", runtime);
    if (ptr) {
        return getFieldByte(ptr);
    }
    return 0;
}

void jthreadlock_create(MemoryBlock *mb) {
    garbage_thread_lock();
    if (!mb->thread_lock) {
        ThreadLock *tl = jvm_calloc(sizeof(ThreadLock));
        thread_lock_init(tl);
        mb->thread_lock = tl;
    }
    garbage_thread_unlock();
}

void jthreadlock_destory(MemoryBlock *mb) {
    thread_lock_dispose(mb->thread_lock);
    if (mb->thread_lock) {
        jvm_free(mb->thread_lock);
        mb->thread_lock = NULL;
    }
}

s32 jthread_lock(MemoryBlock *mb, Runtime *runtime) { //可能会重入，同一个线程多次锁同一对象
    if (mb == NULL)return -1;
    if (!mb->thread_lock) {
        jthreadlock_create(mb);
    }
    ThreadLock *jtl = mb->thread_lock;
    //can pause when lock
    while (mtx_trylock(&jtl->mutex_lock) != thrd_success) {
        check_suspend_and_pause(runtime);
        jthread_yield(runtime);
    }

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("  lock: %llx   lock holder: %s \n", (s64) (intptr_t) (runtime->threadInfo->jthread),
               utf8_cstr(mb->clazz->name));
#endif
    return 0;
}

s32 jthread_unlock(MemoryBlock *mb, Runtime *runtime) {
    if (mb == NULL)return -1;
    if (!mb->thread_lock) {
        jthreadlock_create(mb);
    }
    ThreadLock *jtl = mb->thread_lock;
    mtx_unlock(&jtl->mutex_lock);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("unlock: %llx   lock holder: %s, \n", (s64) (intptr_t) (runtime->threadInfo->jthread),
               utf8_cstr(mb->clazz->name));
#endif
    return 0;
}

s32 jthread_notify(MemoryBlock *mb, Runtime *runtime) {
    if (mb == NULL)return -1;
    if (mb->thread_lock == NULL) {
        jthreadlock_create(mb);
    }
    cnd_signal(&mb->thread_lock->thread_cond);
    return 0;
}

s32 jthread_notifyAll(MemoryBlock *mb, Runtime *runtime) {
    if (mb == NULL)return -1;
    if (mb->thread_lock == NULL) {
        jthreadlock_create(mb);
    }
    cnd_broadcast(&mb->thread_lock->thread_cond);
    return 0;
}

s32 jthread_yield(Runtime *runtime) {
    thrd_yield();
    return 0;
}

s32 jthread_suspend(Runtime *runtime) {
    spin_lock(&runtime->threadInfo->lock);
//    MethodInfo *m = runtime->threadInfo->top_runtime->method;
//    jvm_printf("suspend %lx ,%s\n", runtime->threadInfo->jthread, m ? utf8_cstr(m->name) : "");
    runtime->threadInfo->suspend_count++;
    spin_unlock(&runtime->threadInfo->lock);
    return 0;
}

void jthread_block_enter(Runtime *runtime) {
    runtime->threadInfo->is_blocking = 1;
}

void jthread_block_exit(Runtime *runtime) {
    runtime->threadInfo->is_blocking = 0;
    check_suspend_and_pause(runtime);
}

s32 jthread_resume(Runtime *runtime) {
    spin_lock(&runtime->threadInfo->lock);
//    MethodInfo *m = runtime->threadInfo->top_runtime->method;
//    jvm_printf("resume %lx ,%s\n", runtime->threadInfo->jthread, m ? utf8_cstr(m->name) : "");
    if (runtime->threadInfo->suspend_count > 0)runtime->threadInfo->suspend_count--;
    spin_unlock(&runtime->threadInfo->lock);
    return 0;
}

s32 jthread_waitTime(MemoryBlock *mb, Runtime *runtime, s64 waitms) {
    if (mb == NULL)return -1;
    if (!mb->thread_lock) {
        jthreadlock_create(mb);
    }
    jthread_block_enter(runtime);
    runtime->threadInfo->curThreadLock = mb;
    runtime->threadInfo->thread_status = THREAD_STATUS_WAIT;
    if (waitms) {
        waitms += currentTimeMillis();
        struct timespec t;
        //clock_gettime(CLOCK_REALTIME, &t);
        t.tv_sec = waitms / 1000;
        t.tv_nsec = (waitms % 1000) * 1000000;
        cnd_timedwait(&mb->thread_lock->thread_cond, &mb->thread_lock->mutex_lock, &t);
    } else {
        cnd_wait(&mb->thread_lock->thread_cond, &mb->thread_lock->mutex_lock);
    }
    runtime->threadInfo->thread_status = THREAD_STATUS_RUNNING;
    runtime->threadInfo->curThreadLock = NULL;
    jthread_block_exit(runtime);
    return 0;
}

s32 jthread_sleep(Runtime *runtime, s64 ms) {
    jthread_block_enter(runtime);
    runtime->threadInfo->thread_status = THREAD_STATUS_SLEEPING;
    threadSleep(ms);
    runtime->threadInfo->thread_status = THREAD_STATUS_RUNNING;
    jthread_block_exit(runtime);
    return 0;
}

s32 check_suspend_and_pause(Runtime *runtime) {
    JavaThreadInfo *threadInfo = runtime->threadInfo;
    if (threadInfo->suspend_count && !threadInfo->no_pause) {
        threadInfo->is_suspend = 1;
        garbage_thread_lock();
        garbage_thread_notifyall();
        while (threadInfo->suspend_count) {
            garbage_thread_timedwait(10);
        }
        threadInfo->is_suspend = 0;
        //jvm_printf(".");
        garbage_thread_unlock();
    }
    return 0;
}

//===============================    实例化数组  ==================================
Instance *jarray_create_by_class(Runtime *runtime, s32 count, JClass *clazz) {
    //类型的 index
    s32 typeIdx = clazz->mb.arr_type_index;
    //取得对应类型所占内存大小
    s32 width = data_type_bytes[typeIdx];
    //为数组对象分配内存，大小为 元素大小 * 长度
    Instance *arr = jvm_calloc(sizeof(Instance) + (width * count));
    //配置 memblock 属性
    arr->mb.type = MEM_TYPE_ARR;
    arr->mb.clazz = clazz;
    arr->mb.arr_type_index = typeIdx;
    arr->arr_length = count;
    //指向数组体
    if (arr->arr_length)arr->arr_body = (c8 *) (&arr[1]);
    gc_refer_reg(runtime, arr);
    return arr;
}

Instance *jarray_create_by_type_index(Runtime *runtime, s32 count, s32 typeIdx) {
    JClass *clazz = NULL;
    clazz = array_class_get_by_index(runtime, typeIdx);
    Instance *arr = jarray_create_by_class(runtime, count, clazz);
    return arr;
}

Instance *jarray_create_by_type_name(Runtime *runtime, s32 count, Utf8String *name) {
    JClass *clazz = NULL;
    clazz = array_class_get_by_name(runtime, name);
    Instance *arr = jarray_create_by_class(runtime, count, clazz);
    return arr;
}


s32 jarray_destory(Instance *arr) {
    if (arr && arr->mb.type == MEM_TYPE_ARR) {
        jthreadlock_destory(&arr->mb);
        arr->mb.thread_lock = NULL;
        arr->arr_length = -1;
        jvm_free(arr);
    }
    return 0;
}

/**
 * create multi array
 * @param dim arrdim
 * @param pdesc desc
 * @return ins
 */
Instance *jarray_multi_create(Runtime *runtime, s32 *dim, s32 dim_size, Utf8String *pdesc, s32 deep) {
    s32 len = dim[dim_size - 1 - deep];
    if (len == -1) {
        return NULL;
    }
    JClass *cl = array_class_create_get(runtime, pdesc);

    //获取或者创建普通数组
    Instance *arr = jarray_create_by_class(runtime, len, cl);
    //维度 - 1
    Utf8String *desc = utf8_create_part(pdesc, 1, pdesc->length - 1);
    
    c8 ch = utf8_char_at(desc, 0);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("multi arr deep :%d  type(%c) arr[%x] size:%d\n", deep, ch, arr, len);
#endif
    //如果还有维度，则继续递归
    if (ch == '[') {
        int i;
        s64 val;
        for (i = 0; i < len; i++) {
            Instance *elem = jarray_multi_create(runtime, dim, dim_size, desc, deep + 1);
            val = (intptr_t) elem;
            jarray_set_field(arr, i, val);
        }
    }
    utf8_destory(desc);
    return arr;
}


void jarray_set_field(Instance *arr, s32 index, s64 val) {
    s32 idx = arr->mb.arr_type_index;
    s32 bytes = data_type_bytes[idx];
    if (isDataReferByIndex(idx)) {
        setFieldRefer((c8 *) ((__refer *) arr->arr_body + index), (__refer) (intptr_t) val);
    } else {
        switch (bytes) {
            case 1:
                setFieldByte((c8 *) (arr->arr_body + index), (s8) val);
                break;
            case 2:
                setFieldShort((c8 *) ((s16 *) arr->arr_body + index), (s16) val);
                break;
            case 4:
                setFieldInt((c8 *) ((s32 *) arr->arr_body + index), (s32) val);
                break;
            case 8:
                setFieldLong((c8 *) ((s64 *) arr->arr_body + index), val);
                break;
        }
    }
}

s64 jarray_get_field(Instance *arr, s32 index) {
    s32 idx = arr->mb.arr_type_index;
    s32 bytes = data_type_bytes[idx];
    s64 val = 0;
    if (isDataReferByIndex(idx)) {
        val = (intptr_t) getFieldRefer((c8 *) ((__refer *) arr->arr_body + index));
    } else {
        switch (bytes) {
            case 1:
                val = getFieldByte(arr->arr_body + index);
                break;
            case 2:
                if (idx == DATATYPE_JCHAR) {
                    val = (u16) getFieldShort((c8 *) ((u16 *) arr->arr_body + index));
                } else
                    val = getFieldShort((c8 *) ((s16 *) arr->arr_body + index));
                break;
            case 4:
                val = getFieldInt((c8 *) ((s32 *) arr->arr_body + index));
                break;
            case 8:
                val = getFieldLong((c8 *) ((s64 *) arr->arr_body + index));
                break;
        }
    }
    return val;
}

//===============================    实例化对象  ==================================
Instance *instance_create(Runtime *runtime, JClass *clazz) {
    //很简单，分配目标类中的成员变量所占内存大小的内存空间 + Instance 结构体本身所占空间 就可以了。
    Instance *ins = jvm_calloc(sizeof(Instance) + clazz->field_instance_len);
    ins->mb.type = MEM_TYPE_INS;
    ins->mb.clazz = clazz;
    //指向成员变量内存开始的地址
    ins->obj_fields = (c8 *) (&ins[1]);//jvm_calloc(clazz->field_instance_len);

    gc_refer_reg(runtime, ins);
    return ins;
}

//调用默认无参构造函数 及 初始化成员变量值
void instance_init(Instance *ins, Runtime *runtime) {
    instance_init_methodtype(ins, runtime, "()V", NULL);
}

void instance_init_methodtype(Instance *ins, Runtime *runtime, c8 *methodtype, RuntimeStack *para) {
    if (ins) {
        //构造构造函数签名
        Utf8String *methodName = utf8_create_c("<init>");
        Utf8String *methodType = utf8_create_c(methodtype);
        //找到函数
        MethodInfo *mi = find_methodInfo_by_name(ins->mb.clazz->name, methodName, methodType, runtime);
        push_ref(runtime->stack, (__refer) ins);
        if (para) {
            s32 i;
            for (i = 0; i < para->size; i++) {
                StackEntry entry;
                peek_entry(para, &entry, i);
                push_entry(runtime->stack, &entry);
            }
        }
        //执行
        s32 ret = execute_method_impl(mi, runtime, ins->mb.clazz);
        if (ret != RUNTIME_STATUS_NORMAL) {
            print_exception(runtime);
        }
        utf8_destory(methodName);
        utf8_destory(methodType);
    }
}

//调用 finalize
void instance_finalize(Instance *ins, Runtime *runtime) {
    if (ins) {
        MethodInfo *mi = ins->mb.clazz->finalizeMethod;
        if (mi) {
            push_ref(runtime->stack, ins);
            s32 ret = execute_method_impl(mi, runtime, mi->_this_class);
            if (ret != RUNTIME_STATUS_NORMAL) {
                print_exception(runtime);
            }
        }
    }
}

void instance_clear_refer(Instance *ins) {
    s32 i;
    JClass *clazz = ins->mb.clazz;
    while (clazz) {
        FieldPool *fp = &clazz->fieldPool;
        for (i = 0; i < fp->field_used; i++) {
            FieldInfo *fi = &fp->field[i];
            if ((fi->access_flags & ACC_STATIC) == 0 && fi->isrefer) {
                c8 *ptr = getInstanceFieldPtr(ins, fi);
                if (ptr) {
                    setFieldRefer(ptr, NULL);
                }
            }
        }
        clazz = getSuperClass(clazz);
    }
}

s32 instance_destory(Instance *ins) {
    jthreadlock_destory(&ins->mb);
    jvm_free(ins);

    return 0;
}

/**
 * for java string instance copy
 * deepth copy instance
 * deepth copy array
 *
 * @param src  source instance
 * @return  instance
 */
Instance *instance_copy(Runtime *runtime, Instance *src, s32 deep_copy) {
    Instance *dst = jvm_malloc(sizeof(Instance) + src->mb.clazz->field_instance_len);
//    Instance *dst = jvm_malloc(sizeof(Instance));
    memcpy(dst, src, sizeof(Instance));
    dst->mb.thread_lock = NULL;
    dst->mb.garbage_reg = 0;
    dst->mb.garbage_mark = 0;
    if (src->mb.type == MEM_TYPE_INS) {
        JClass *clazz = src->mb.clazz;
        s32 fileds_len = clazz->field_instance_len;
        if (fileds_len) {
            dst->obj_fields = (c8 *) dst + sizeof(Instance);//jvm_malloc(fileds_len);
//            dst->obj_fields = jvm_malloc(fileds_len);
            memcpy(dst->obj_fields, src->obj_fields, fileds_len);
            if (deep_copy) {
                s32 i, len;
                while (clazz) {
                    FieldPool *fp = &clazz->fieldPool;
                    for (i = 0, len = fp->field_used; i < len; i++) {
                        FieldInfo *fi = &fp->field[i];
                        if ((fi->access_flags & ACC_STATIC) == 0 && fi->isrefer) {
                            c8 *ptr = getInstanceFieldPtr(src, fi);
                            Instance *ins = (Instance *) getFieldRefer(ptr);
                            if (ins) {
                                Instance *new_ins = instance_copy(runtime, ins, deep_copy);
                                ptr = getInstanceFieldPtr(dst, fi);
                                setFieldRefer(ptr, new_ins);
                            }
                        }
                    }
                    clazz = getSuperClass(clazz);
                }
            }
        }
    } else if (src->mb.type == MEM_TYPE_ARR) {
        s32 size = src->arr_length * data_type_bytes[src->mb.arr_type_index];
        dst->arr_body = jvm_malloc(size);
        if (isDataReferByIndex(src->mb.arr_type_index) && deep_copy) {
            s32 i;
            s64 val;
            for (i = 0; i < dst->arr_length; i++) {
                val = jarray_get_field(src, i);
                if (val) {
                    val = (intptr_t) instance_copy(runtime, (Instance *) getFieldRefer((__refer) (intptr_t) val), deep_copy);
                    jarray_set_field(dst, i, val);
                }
            }
        } else {
            memcpy(dst->arr_body, src->arr_body, size);
        }
    }
    gc_refer_reg(runtime, dst);
    return dst;
}

//===============================    实例化 java.lang.Class  ==================================
/**
 *
 * 每个java 类有一个 java.lang.Class 的实例, 用于承载对相关java类的操作
 *
 * every java Class have a instance of java.lang.Class
 *
 * @param runtime
 * @param clazz
 * @return
 */

Instance *insOfJavaLangClass_create_get(Runtime *runtime, JClass *clazz) {
    JClass *java_lang_class = classes_load_get_c(STR_CLASS_JAVA_LANG_CLASS, runtime);
    if (java_lang_class) {
        if (clazz->ins_class) {
            return clazz->ins_class;
        } else {
            //New Class 对象
            Instance *ins = instance_create(runtime, java_lang_class);
            //加入 GC_Holder 防止被 GC
            gc_refer_hold(ins);
            //调用默认无参构造函数 及 初始化成员变量值
            instance_init(ins, runtime);
            //在 JClass 结构体中保存 Class 对象的指针
            clazz->ins_class = ins;
            //在 Class 对象中保存 JClass 结构体的指针
            insOfJavaLangClass_set_classHandle(ins, clazz);
            return ins;
        }
    }
    return NULL;
}


JClass *insOfJavaLangClass_get_classHandle(Instance *insOfJavaLangClass) {
    return (JClass *) (intptr_t) getFieldLong(getInstanceFieldPtr(insOfJavaLangClass, jvm_runtime_cache.class_classHandle));
}

void insOfJavaLangClass_set_classHandle(Instance *insOfJavaLangClass, JClass *handle) {
    setFieldLong(getInstanceFieldPtr(insOfJavaLangClass, jvm_runtime_cache.class_classHandle), (s64) (intptr_t) handle);
}

//===============================    实例化字符串  ==================================
Instance *jstring_create(Utf8String *src, Runtime *runtime) {
    if (!src)return NULL;
    Utf8String *clsName = utf8_create_c(STR_CLASS_JAVA_LANG_STRING);
    JClass *jstr_clazz = classes_load_get(clsName, runtime);
    Instance *jstring = instance_create(runtime, jstr_clazz);
    gc_refer_hold(jstring);//hold for no gc

    jstring->mb.clazz = jstr_clazz;
    instance_init(jstring, runtime);

    c8 *ptr = jstring_get_value_ptr(jstring);
    u16 *buf = jvm_calloc(src->length * data_type_bytes[DATATYPE_JCHAR]);
    //UTF-8 -> Unicode
    s32 len = utf8_2_unicode(src, buf);
    if (len >= 0) {//可能解析出错
        //填充 String 中的 char 数组
        Instance *arr = jarray_create_by_type_index(runtime, len, DATATYPE_JCHAR);//u16 type is 5
        setFieldRefer(ptr, (__refer) arr);//设置数组
        memcpy(arr->arr_body, buf, len * data_type_bytes[DATATYPE_JCHAR]);
    }
    jvm_free(buf);
    jstring_set_count(jstring, len);//设置长度
    utf8_destory(clsName);
    gc_refer_release(jstring);
    return jstring;
}

Instance *jstring_create_cstr(c8 *cstr, Runtime *runtime) {
    if (!cstr)return NULL;
    Utf8String *ustr = utf8_create_part_c(cstr, 0, strlen(cstr));
    Instance *jstr = jstring_create(ustr, runtime);
    utf8_destory(ustr);
    return jstr;
}

s32 jstring_get_count(Instance *jstr) {
    return getFieldInt(getInstanceFieldPtr(jstr, jvm_runtime_cache.string_count));
}

void jstring_set_count(Instance *jstr, s32 count) {
    setFieldInt(getInstanceFieldPtr(jstr, jvm_runtime_cache.string_count), count);
}

s32 jstring_get_offset(Instance *jstr) {
    return getFieldInt(getInstanceFieldPtr(jstr, jvm_runtime_cache.string_offset));
}

c8 *jstring_get_value_ptr(Instance *jstr) {
    return getInstanceFieldPtr(jstr, jvm_runtime_cache.string_value);
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
    if (ptr && ptr->arr_body && startAt >= 0) {
        u16 *jchar_arr = (u16 *) ptr->arr_body;
        s32 count = jstring_get_count(jstr);
        s32 offset = jstring_get_offset(jstr);
        s32 i;
        for (i = startAt; i < count; i++) {
            if (jchar_arr[i + offset] == ch) {
                return i;
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
    Instance *arr1 = jstring_get_value_array(jstr1);//取得 char[] value
    Instance *arr2 = jstring_get_value_array(jstr2);//取得 char[] value
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
    if (!jstr)return 0;
    Instance *arr = jstring_get_value_array(jstr);
    if (arr) {
        s32 count = jstring_get_count(jstr);
        s32 offset = jstring_get_offset(jstr);
        u16 *arrbody = (u16 *) arr->arr_body;
        if (arr->arr_body)unicode_2_utf8(&arrbody[offset], utf8, count);
    }
    return 0;
}
//===============================    New 异常  ==================================

Instance *exception_create(s32 exception_type, Runtime *runtime) {
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("create exception : %s\n", STRS_CLASS_EXCEPTION[exception_type]);
#endif
    Utf8String *clsName = utf8_create_c(STRS_CLASS_EXCEPTION[exception_type]);
    JClass *clazz = classes_load_get(clsName, runtime);
    utf8_destory(clsName);

    Instance *ins = instance_create(runtime, clazz);
    gc_refer_hold(ins);
    instance_init(ins, runtime);
    gc_refer_release(ins);
    return ins;
}

Instance *exception_create_str(s32 exception_type, Runtime *runtime, c8 *errmsg) {
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("create exception : %s\n", STRS_CLASS_EXCEPTION[exception_type]);
#endif
    //New String 对象用作异常提示
    Utf8String *uerrmsg = utf8_create_c(errmsg);
    Instance *jstr = jstring_create(uerrmsg, runtime);
    gc_refer_hold(jstr);
    utf8_destory(uerrmsg);
    //String 入参数栈
    RuntimeStack *para = stack_create(1);
    push_ref(para, jstr);
    gc_refer_release(jstr);
    //New 异常对象
    Utf8String *clsName = utf8_create_c(STRS_CLASS_EXCEPTION[exception_type]);
    JClass *clazz = classes_load_get(clsName, runtime);
    utf8_destory(clsName);
    Instance *ins = instance_create(runtime, clazz);
    gc_refer_hold(ins);
    //调用异常对象的带 String 参数的构造方法，传入 Msg
    instance_init_methodtype(ins, runtime, "(Ljava/lang/String;)V", para);
    gc_refer_release(ins);
    stack_destory(para);
    return ins;
}
//===============================    lambda  ==================================

//MethodType 包含一个 String 类型的方法描述
Instance *method_type_create(Runtime *runtime, Utf8String *desc) {
    JClass *cl = classes_load_get_c(STR_CLASS_JAVA_LANG_INVOKE_METHODTYPE, runtime);
    if (cl) {
        Instance *mt = instance_create(runtime, cl);
        gc_refer_hold(mt);
        Instance *jstr_desc = jstring_create(desc, runtime);
        RuntimeStack *para = stack_create(1);
        //String 参数入栈
        push_ref(para, jstr_desc);
        //调用构造方法
        instance_init_methodtype(mt, runtime, "(Ljava/lang/String;)V", para);
        stack_destory(para);
        gc_refer_release(mt);
        return mt;
    }
    return NULL;
}
//handler 较为复杂，包含了具体的方法信息，有4个参数
Instance *method_handle_create(Runtime *runtime, MethodInfo *mi, s32 kind) {
    JClass *cl = classes_load_get_c(STR_CLASS_JAVA_LANG_INVOKE_METHODHANDLE, runtime);
    if (cl) {
        Instance *mh = instance_create(runtime, cl);
        gc_refer_hold(mh);
        RuntimeStack *para = stack_create(4);
        //类型
        push_int(para, kind);
        //当前类名
        Instance *jstr_clsName = jstring_create(mi->_this_class->name, runtime);
        gc_refer_hold(jstr_clsName);
        push_ref(para, jstr_clsName);
        //lambda 方法名
        Instance *jstr_methodName = jstring_create(mi->name, runtime);
        push_ref(para, jstr_methodName);
        gc_refer_hold(jstr_methodName);
        //方法的描述
        Instance *jstr_methodDesc = jstring_create(mi->descriptor, runtime);
        push_ref(para, jstr_methodDesc);
        gc_refer_hold(jstr_methodDesc);
        instance_init_methodtype(mh, runtime, "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", para);
        stack_destory(para);
        gc_refer_release(mh);
        gc_refer_release(jstr_clsName);
        gc_refer_release(jstr_methodName);
        gc_refer_release(jstr_methodDesc);
        return mh;
    }
    return NULL;
}
//Lookup 包含 lambda 表达式方法所在的类
Instance *method_handles_lookup_create(Runtime *runtime, JClass *caller) {
    JClass *cl = classes_load_get_c(STR_CLASS_JAVA_LANG_INVOKE_METHODHANDLES_LOOKUP, runtime);
    if (cl) {
        Instance *lookup = instance_create(runtime, cl);
        gc_refer_hold(lookup);
        RuntimeStack *para = stack_create(1);

        push_ref(para, insOfJavaLangClass_create_get(runtime, caller));
        instance_init_methodtype(lookup, runtime, "(Ljava/lang/Class;)V", para);
        stack_destory(para);
        gc_refer_release(lookup);
        return lookup;
    }
    return NULL;
}
//===============================    实例操作  ==================================



c8 *getFieldPtr_byName_c(Instance *instance, c8 *pclassName, c8 *pfieldName, c8 *pfieldType, Runtime *runtime) {
    Utf8String *clsName = utf8_create_c(pclassName);
    //Class *clazz = classes_get(clsName);

    //set value
    Utf8String *fieldName = utf8_create_c(pfieldName);
    Utf8String *fieldType = utf8_create_c(pfieldType);
    c8 *ptr = getFieldPtr_byName(instance, clsName, fieldName, fieldType, runtime);
    utf8_destory(clsName);
    utf8_destory(fieldName);
    utf8_destory(fieldType);
    return ptr;
}


c8 *getFieldPtr_byName(Instance *instance, Utf8String *clsName, Utf8String *fieldName, Utf8String *fieldType, Runtime *runtime) {

    c8 *ptr = NULL;
    FieldInfo *fi = find_fieldInfo_by_name(clsName, fieldName, fieldType, runtime);

    if (fi) {
        if (fi->access_flags & ACC_STATIC) {
            ptr = getStaticFieldPtr(fi);
        } else {
            ptr = getInstanceFieldPtr(instance, fi);
        }
    }
    return ptr;
}

s32 getLineNumByIndex(CodeAttribute *ca, s32 offset) {
    s32 j;

    for (j = 0; j < ca->line_number_table_length; j++) {
        LineNumberTable *node = &(ca->line_number_table[j]);
        if (offset >= node->start_pc) {
            if (j + 1 < ca->line_number_table_length) {
                LineNumberTable *next_node = &(ca->line_number_table[j + 1]);

                if (offset < next_node->start_pc) {
                    return node->line_number;
                }
            } else {
                return node->line_number;
            }
        }
    }
    return -1;
}


void memoryblock_destory(__refer ref) {
    MemoryBlock *mb = (MemoryBlock *) ref;
    if (!mb)return;
//    if (utf8_equals_c(mb->clazz->name, "test/GuiTest$CallBack")) {
//        garbage_dump_runtime();
//        int debug = 1;
//    }
    if (mb->type == MEM_TYPE_INS) {
        instance_destory((Instance *) mb);
    } else if (mb->type == MEM_TYPE_ARR) {
        jarray_destory((Instance *) mb);
    } else if (mb->type == MEM_TYPE_CLASS) {
        class_destory((JClass *) mb);
    }
}

JavaThreadInfo *threadinfo_create() {
    JavaThreadInfo *threadInfo = jvm_calloc(sizeof(JavaThreadInfo));
    threadInfo->instance_holder = arraylist_create(0);
    return threadInfo;
}

void threadinfo_destory(JavaThreadInfo *threadInfo) {
    arraylist_destory(threadInfo->instance_holder);
    jvm_free(threadInfo);
}

s64 currentTimeMillis() {

    struct timespec tv;
    clock_gettime(CLOCK_REALTIME, &tv);
    return ((s64) tv.tv_sec) * MILL_2_SEC_SCALE + tv.tv_nsec / NANO_2_MILLS_SCALE;
}

s64 nanoTime() {

    struct timespec tv;
    clock_gettime(CLOCK_REALTIME, &tv);

    if (!NANO_START) {
        NANO_START = ((s64) tv.tv_sec) * NANO_2_SEC_SCALE + tv.tv_nsec;
    }
    s64 v = ((s64) tv.tv_sec) * NANO_2_SEC_SCALE + tv.tv_nsec;
    return v - NANO_START;
}

s64 threadSleep(s64 ms) {
    //wait time
    struct timespec req;
    clock_gettime(CLOCK_REALTIME, &req);
    req.tv_sec += ms / MILL_2_SEC_SCALE;
    req.tv_nsec += (ms % MILL_2_SEC_SCALE) * NANO_2_MILLS_SCALE;
    //if notify or notifyall ,the thread is active again, rem record remain wait time
    struct timespec rem;
    rem.tv_sec = 0;
    rem.tv_nsec = 0;
    thrd_sleep(&req, &rem);
    return (rem.tv_sec * MILL_2_SEC_SCALE + rem.tv_nsec / NANO_2_MILLS_SCALE);
}

void instance_hold_to_thread(Instance *ref, Runtime *runtime) {
    if (runtime && ref) {
        arraylist_push_back(runtime->threadInfo->instance_holder, ref);
    }
}

void instance_release_from_thread(Instance *ref, Runtime *runtime) {
    if (runtime && ref) {
        arraylist_remove(runtime->threadInfo->instance_holder, ref);
    }
}

CStringArr *cstringarr_create(Instance *jstr_arr) { //byte[][] to char**
    if (!jstr_arr)return NULL;
    CStringArr *cstr_arr = jvm_calloc(sizeof(CStringArr));
    cstr_arr->arr_length = jstr_arr->arr_length;
    cstr_arr->arr_body = jvm_calloc(jstr_arr->arr_length * sizeof(__refer));
    s32 i;
    for (i = 0; i < cstr_arr->arr_length; i++) {
        s64 val = jarray_get_field(jstr_arr, i);
        Instance *jbyte_arr = (__refer) (intptr_t) val;
        if (jbyte_arr) {
            cstr_arr->arr_body[i] = jbyte_arr->arr_body;
        }
    }
    return cstr_arr;
}

void cstringarr_destory(CStringArr *cstr_arr) {
    jvm_free(cstr_arr->arr_body);
    jvm_free(cstr_arr);
}

ReferArr *referarr_create(Instance *jobj_arr) {
    if (!jobj_arr)return NULL;
    CStringArr *ref_arr = jvm_calloc(sizeof(CStringArr));
    ref_arr->arr_length = jobj_arr->arr_length;
    ref_arr->arr_body = jvm_calloc(jobj_arr->arr_length * sizeof(__refer));
    s32 i;
    for (i = 0; i < ref_arr->arr_length; i++) {
        s64 val = jarray_get_field(jobj_arr, i);
        ref_arr->arr_body[i] = (__refer) (intptr_t) val;
    }
    return ref_arr;
}

void referarr_destory(CStringArr *ref_arr) {
    jvm_free(ref_arr->arr_body);
    jvm_free(ref_arr);
}

void referarr_2_jlongarr(ReferArr *ref_arr, Instance *jlong_arr) {
    s32 i;
    for (i = 0; i < ref_arr->arr_length && i < jlong_arr->arr_length; i++) {
        __refer ref = ref_arr->arr_body[i];
        jarray_set_field(jlong_arr, i, (intptr_t) ref);
    }
};


/**
 * load file less than 4G bytes
 */
s32 _loadFileContents(c8 *file, ByteBuf *buf) {

    FILE *pFile;
    long lSize;
    char *buffer;
    size_t result;

    /* 若要一个byte不漏地读入整个文件，只能采用二进制方式打开 */
    pFile = fopen(file, "rb");
    if (pFile == NULL) {
        //jvm_printf("File error");
        return -1;
    }

    /* 获取文件大小 */
    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);

    /* 分配内存存储整个文件 */
    buffer = jvm_malloc((u32) lSize);
    if (buffer == NULL) {
        //jvm_printf("Memory error");
        return -1;
    }

    /* 将文件拷贝到buffer中 */
    result = fread(buffer, 1, lSize, pFile);
    if (result != lSize) {
        //jvm_printf("Reading error");
        return -1;
    }
    /* 现在整个文件已经在buffer中，可由标准输出打印内容 */
    //printf("%s", buffer);

    /* 结束演示，关闭文件并释放内存 */
    fclose(pFile);
    bytebuf_write_batch(buf, buffer, (s32) lSize);
    jvm_free(buffer);

    return 0;
}

//根据path读字节码到内存中
ByteBuf *load_file_from_classpath(ClassLoader *loader, Utf8String *path) {
    ByteBuf *bytebuf = NULL;
    s32 i, iret;
    for (i = 0; i < loader->classpath->length; i++) {
        Utf8String *pClassPath = arraylist_get_value(loader->classpath, i);
        if (isDir(pClassPath)) { //form file
            Utf8String *filepath = utf8_create_copy(pClassPath);
            utf8_pushback(filepath, '/');
            utf8_append(filepath, path);

            bytebuf = bytebuf_create(16);
            //加载文件到 bytebuf
            iret = _loadFileContents(utf8_cstr(filepath), bytebuf);
            utf8_destory(filepath);
            //回收
            if (iret != 0) {
                bytebuf_destory(bytebuf);
                bytebuf = NULL;
            }
        } else { //from jar
            bytebuf = bytebuf_create(16);
            iret = zip_loadfile(utf8_cstr(pClassPath), utf8_cstr(path), bytebuf);
            //回收
            if (iret != 0) {
                bytebuf_destory(bytebuf);
                bytebuf = NULL;
            } else {
                break;
            }
        }
    }
    return bytebuf;
}

//初始化必要的 native 方法表（JNIENV）
void init_jni_func_table() {
    jnienv.data_type_bytes = (s32 *) &data_type_bytes;
    jnienv.native_reg_lib = native_reg_lib;
    jnienv.native_remove_lib = native_remove_lib;
    jnienv.push_entry = push_entry_jni;
    jnienv.push_int = push_int_jni;
    jnienv.push_long = push_long_jni;
    jnienv.push_double = push_double_jni;
    jnienv.push_float = push_float_jni;
    jnienv.push_ref = push_ref_jni;
    jnienv.pop_ref = pop_ref_jni;
    jnienv.pop_int = pop_int_jni;
    jnienv.pop_long = pop_long_jni;
    jnienv.pop_double = pop_double_jni;
    jnienv.pop_float = pop_float_jni;
    jnienv.pop_entry = pop_entry_jni;
    jnienv.pop_empty = pop_empty_jni;
    jnienv.entry_2_int = entry_2_int_jni;
    jnienv.peek_entry = peek_entry_jni;
    jnienv.entry_2_long = entry_2_long_jni;
    jnienv.entry_2_refer = entry_2_refer_jni;
    jnienv.localvar_setRefer = localvar_setRefer_jni;
    jnienv.localvar_setInt = localvar_setInt_jni;
    jnienv.localvar_getRefer = localvar_getRefer_jni;
    jnienv.localvar_getInt = localvar_getInt_jni;
    jnienv.localvar_getLong_2slot = localvar_getLong_2slot_jni;
    jnienv.localvar_setLong_2slot = localvar_setLong_2slot_jni;
    jnienv.jthread_block_enter = jthread_block_enter;
    jnienv.jthread_block_exit = jthread_block_exit;
    jnienv.utf8_create = utf8_create;
    jnienv.utf8_create_part_c = utf8_create_part_c;
    jnienv.utf8_cstr = utf8_cstr;
    jnienv.utf8_destory = utf8_destory;
    jnienv.jstring_2_utf8 = jstring_2_utf8;
    jnienv.jstring_create = jstring_create;
    jnienv.jstring_create_cstr = jstring_create_cstr;
    jnienv.cstringarr_create = cstringarr_create;
    jnienv.cstringarr_destory = cstringarr_destory;
    jnienv.referarr_create = referarr_create;
    jnienv.referarr_destory = referarr_destory;
    jnienv.referarr_2_jlongarr = referarr_2_jlongarr;
    jnienv.jvm_calloc = jvm_calloc;
    jnienv.jvm_malloc = jvm_malloc;
    jnienv.jvm_free = jvm_free;
    jnienv.jvm_realloc = jvm_realloc;
    jnienv.execute_method = execute_method;
    jnienv.find_methodInfo_by_name = find_methodInfo_by_name;
    jnienv.jarray_create_by_type_name = jarray_create_by_type_name;
    jnienv.jarray_create_by_type_index = jarray_create_by_type_index;
    jnienv.jarray_set_field = jarray_set_field;
    jnienv.jarray_get_field = jarray_get_field;
    jnienv.instance_hold_to_thread = instance_hold_to_thread;
    jnienv.instance_release_from_thread = instance_release_from_thread;
    jnienv.print_exception = print_exception;
    jnienv.garbage_refer_hold = gc_refer_hold;
    jnienv.garbage_refer_release = gc_refer_release;
}
