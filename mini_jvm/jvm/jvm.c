
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jvm.h"
#include "../utils/utf8_string.h"
#include "garbage.h"
#include "jvm_util.h"
#include "java_native_std.h"


void destoryAllClasses(hmap_t classes) {
    hashtable_destory(classes);
}

s32 execute(c8 *p_classpath, c8 *p_mainclass, s32 argc, c8 **argv) {

#if _JVM_DEBUG_PROFILE
    instruct_profile = hashtable_create(DEFAULT_HASH_FUNC, DEFAULT_HASH_EQUALS_FUNC);
#endif

    //为指令创建索引
    instructionsIndexies = createInstructIndexies();
    //创建类容器
    classes = hashtable_create(UNICODE_STR_HASH_FUNC, UNICODE_STR_EQUALS_FUNC);

    JVM_CLASS = class_create();
    JVM_CLASS->name = utf8_create_c("MINI_JVM");
    //创建垃圾收集器
    garbage_collector_create();
    //创建线程容器
    thread_list = arraylist_create(0);
    //本地方法库
    native_libs = arraylist_create(0);
    reg_std_native_lib();
    reg_net_native_lib();

    //创建运行时栈
    Runtime runtime;
    runtime_create(&runtime);

    //开始装载类
    Utf8String *classpath = utf8_create_c(p_classpath);
    Utf8String *mainclass = utf8_create_c(p_mainclass);

    //装入系统属性
    loadSysProperties(classpath);

    load_class(classpath, mainclass, classes);

    HashtableIterator hti;
    hashtable_iterate(classes, &hti);
#if _JVM_DEBUG
    printf("classes size:%d\n", hashtable_num_entries(classes));
#endif
    for (; hashtable_iter_has_more(&hti);) {
        Utf8String *k = hashtable_iter_next_key(&hti);
        Class *clazz = classes_get(k);
#if _JVM_DEBUG
        printf("classes entry : %s,%d\n", utf8_cstr(k), clazz);
#endif
        if (clazz->status != CLASS_STATUS_LINKED)class_link(clazz);
    }
    hashtable_iterate(classes, &hti);
    for (; hashtable_iter_has_more(&hti);) {
        Utf8String *k = hashtable_iter_next_key(&hti);
        Class *clazz = classes_get(k);
        if (clazz->status != CLASS_STATUS_CLINITED)class_clinit(clazz, &runtime);//初始化
    }

    Class *clazz = classes_get(mainclass);

    s32 ret = 0;
    if (clazz) {
        //printf("class: %s : %d\n", utf8_cstr(mname), obj_of_clazz);

#if _JVM_DEBUG
        printConstantPool(clazz);
        printMethodPool(clazz, &(clazz->methodPool));
        printFieldPool(clazz, &(clazz->fieldPool));
        printClassFileFormat(&(clazz->cff));
#endif

        Utf8String *methodName = utf8_create_c("main");
        Utf8String *methodType = utf8_create_c("([Ljava/lang/String;)V");
        Instance arr;
        memset(&arr, 0, sizeof(Instance));
        push_ref(runtime.stack, (__refer) &arr);//main(String[])参数

        MethodInfo *main = find_methodInfo_by_name(mainclass, methodName, methodType);
        if (main) {
            Class *thread_clazz = classes_load_get_c("java/lang/Thread", &runtime);
            //为主线程创建Thread实例
            Instance *main_thread = instance_create(thread_clazz);
            //pthread_t pthread = pthread_self();
            runtime.threadInfo->jthread = main_thread;
            runtime.threadInfo->thread_running = 1;
            instance_init(main_thread, &runtime);//必须放在最好，初始化时需要用到前面的赋值
            s64 start = currentTimeMillis();
            arraylist_append(thread_list, &runtime);
            jthread_set_threadq_value(main_thread, &runtime);

            _garbage_thread_pause = 0;
            //准备参数
            localvar_init(&runtime, main->paraType->length + 1);
            s32 count = argc;
            Long2Double l2d;
            s32 bytes = data_type_bytes[DATATYPE_REFERENCE];
            Instance *arr = jarray_create(count, DATATYPE_REFERENCE);
            int i;
            for (i = 0; i < argc; i++) {
                Utf8String *utfs = utf8_create_c(argv[i]);
                Instance *jstr = jstring_create(utfs, &runtime);
                l2d.r = jstr;
                jarray_set_field(arr, i, &l2d, bytes);
                utf8_destory(utfs);
            }
            push_ref(runtime.stack, arr);
            printf("\n\n\n\n\n\n================================= main start ================================\n");
            //调用主方法
            ret = execute_method(main, &runtime, clazz);
            printf("================================= main  end  ================================\n");
            printf("spent %lld\n", (currentTimeMillis() - start));

            //dump_refer();
#if _JVM_DEBUG_PROFILE
            hashtable_iterate(instruct_profile, &hti);
            for (; hashtable_iter_has_more(&hti);) {
                u8 instruct_code = (u8) (long) hashtable_iter_next_key(&hti);
                HashtableValue sum_v = hashtable_get(instruct_profile, (HashtableKey) (long) instruct_code);
                printf("%2x \t %lld\n", instruct_code, (s64) (long) sum_v);
            }
#endif
            garbage_thread_stop();
        }
        utf8_destory(methodName);
        utf8_destory(methodType);
    }

//    if (clazz) {
//#if _JVM_DEBUG
//        printConstantPool(&(obj_of_clazz->constantPool));
//        printMethodPool(&(obj_of_clazz->constantPool), &(obj_of_clazz->methodPool));
//        printClassFileFormat(&(obj_of_clazz->cff));
//#endif
//    }

    utf8_destory(classpath);

    utf8_destory(mainclass);
    destoryAllClasses(classes);
    utf8_destory(JVM_CLASS->name);
    class_destory(JVM_CLASS);
    runtime_destory(&runtime);
    printf("over\n");
    return ret;
}
