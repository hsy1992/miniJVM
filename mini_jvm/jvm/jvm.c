

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jvm.h"
#include "../utils/utf8_string.h"
#include "garbage.h"
#include "jvm_util.h"
#include "java_native_std.h"
#include "jdwp.h"


void main_thread_create(Runtime *runtime) {

    Class *thread_clazz = classes_load_get_c("java/lang/Thread", runtime);
    //为主线程创建Thread实例
    main_thread = instance_create(thread_clazz);
    //pthread_t pthread = pthread_self();
    runtime->threadInfo->jthread = main_thread;
    runtime->threadInfo->thread_status = THREAD_STATUS_RUNNING;
    instance_init(main_thread, runtime);//必须放在最好，初始化时需要用到前面的赋值
    threadlist_add(runtime);
    jthread_set_threadq_value(main_thread, runtime);
}

void main_thread_destory() {
    threadlist_remove(main_runtime);
    main_runtime->threadInfo->is_suspend = 1;
    garbage_derefer_all(main_thread);
    garbage_refer(main_thread, NULL);//主线程实例被回收
}

ClassLoader *classloader_create(c8 *path) {
    ClassLoader *class_loader = jvm_alloc(sizeof(ClassLoader));
    class_loader->g_classpath = utf8_create_c(path);
    //创建类容器
    class_loader->classes = hashtable_create(UNICODE_STR_HASH_FUNC, UNICODE_STR_EQUALS_FUNC);
    //创建引用类
    class_loader->JVM_CLASS = class_create();
    class_loader->JVM_CLASS->name = utf8_create_c("ClassLoader");
    //创建数组类容器

    return class_loader;
}

void classloader_destory(ClassLoader *class_loader) {
//    utf8_destory(class_loader->JVM_CLASS->name);
//    class_loader->JVM_CLASS->name = NULL;
    garbage_derefer_all(class_loader->JVM_CLASS);
    garbage_refer(class_loader->JVM_CLASS, NULL);

    utf8_destory(class_loader->g_classpath);
    class_loader->g_classpath = NULL;
    hashtable_destory(class_loader->classes);
    class_loader->classes = NULL;
    jvm_free(class_loader);
}

s32 execute(c8 *p_classpath, c8 *p_mainclass, s32 argc, c8 **argv) {
#ifdef __MEM_LEAK_DETECT
    dbg_init(0);
    //dbg_catch_sigsegv();
#endif //__MEM_LEAK_DETECT
    //
    open_log();
#if _JVM_DEBUG_PROFILE
    instruct_profile = hashtable_create(DEFAULT_HASH_FUNC, DEFAULT_HASH_EQUALS_FUNC);
#endif
    thread_lock_init(&sys_lock);

    //为指令创建索引
    instructionsIndexies = instruct_indexies_create();

    sys_classloader = classloader_create(p_classpath);

    array_classloader = classloader_create("");

    //创建垃圾收集器
    MAX_HEAP_SIZE = 20 * 1024 * 1024;//if heapsize great than MAX, gc would be trigger
    garbage_collector_create();


    //创建线程容器
    thread_list = arraylist_create(0);
    //本地方法库
    native_libs = arraylist_create(0);
    reg_std_native_lib();
    reg_net_native_lib();
    reg_jdwp_native_lib();

    //创建运行时栈
    Runtime runtime;
    runtime_init(&runtime);
    main_runtime = &runtime;

    //开始装载类

    Utf8String *str_mainClsName = utf8_create_c(p_mainclass);

    //装入系统属性
    sys_properties_load(sys_classloader->g_classpath);

    load_class(sys_classloader->g_classpath, str_mainClsName, sys_classloader->classes);

    HashtableIterator hti;
    hashtable_iterate(sys_classloader->classes, &hti);
#if _JVM_DEBUG > 5
    jvm_printf("classes size:%d\n", hashtable_num_entries(sys_classloader->classes));
#endif
    for (; hashtable_iter_has_more(&hti);) {
        Utf8String *k = hashtable_iter_next_key(&hti);
        Class *clazz = hashtable_get(sys_classloader->classes, k);
#if _JVM_DEBUG > 5
        jvm_printf("classes entry : %s,%d\n", utf8_cstr(k), clazz);
#endif
        if (clazz->status != CLASS_STATUS_PREPARED)class_prepar(clazz);
    }
    hashtable_iterate(sys_classloader->classes, &hti);
    for (; hashtable_iter_has_more(&hti);) {
        Utf8String *k = hashtable_iter_next_key(&hti);
        Class *clazz = hashtable_get(sys_classloader->classes, k);
        if (clazz->status != CLASS_STATUS_CLINITED)class_clinit(clazz, &runtime);//初始化
    }

    Class *clazz = classes_get(str_mainClsName);

    s32 ret = 0;
    if (clazz) {
        Utf8String *methodName = utf8_create_c("main");
        Utf8String *methodType = utf8_create_c("([Ljava/lang/String;)V");

        MethodInfo *main = find_methodInfo_by_name(str_mainClsName, methodName, methodType);
        if (main) {
            main_thread_create(&runtime);
            //启动调试器
            //startJdwp(&runtime);
            jdwp_start_server();
            //启动垃圾回收
            garbage_thread_resume();

            //准备参数
            localvar_init(&runtime, main->para_count + 1);
            s32 count = argc;
            Long2Double l2d;
            s32 bytes = data_type_bytes[DATATYPE_REFERENCE];
            Utf8String *ustr = utf8_create_c("[java/lang/String;");
            Instance *arr = jarray_create(count, DATATYPE_REFERENCE, ustr);
            utf8_destory(ustr);
            int i;
            for (i = 0; i < argc; i++) {
                Utf8String *utfs = utf8_create_c(argv[i]);
                Instance *jstr = jstring_create(utfs, &runtime);
                l2d.r = jstr;
                jarray_set_field(arr, i, &l2d);
                utf8_destory(utfs);
            }
            push_ref(runtime.stack, arr);
            garbage_refer(arr, main_thread);
            s64 start = currentTimeMillis();
            jvm_printf("\n\n\n\n\n\n================================= main start ================================\n");
            //调用主方法
            ret = execute_method(main, &runtime, clazz);
            if (ret != RUNTIME_STATUS_NORMAL) {
                __refer ref = pop_ref(runtime.stack);
                Instance *ins = (Instance *) ref;
                jvm_printf("MAIN ERROR: %s\n", utf8_cstr(ins->mb.clazz->name));
            }
            runtime.threadInfo->is_blocking = 1;
            while ((thread_list->length) > 1) {//wait for other thread over ,
                check_suspend_and_pause(&runtime);
                threadSleep(100);
            }
            runtime.threadInfo->is_blocking = 0;
            jvm_printf("================================= main  end  ================================\n");
            jvm_printf("spent %lld\n", (currentTimeMillis() - start));

            localvar_dispose(&runtime);
#if _JVM_DEBUG_PROFILE
            hashtable_iterate(instruct_profile, &hti);
            for (; hashtable_iter_has_more(&hti);) {
                u8 instruct_code = (u8) (long) hashtable_iter_next_key(&hti);
                HashtableValue sum_v = hashtable_get(instruct_profile, (HashtableKey) (long) instruct_code);
                jvm_printf("%2x \t %lld\n", instruct_code, (s64) (long) sum_v);
            }
#endif

            jdwp_stop_server();

            main_thread_destory();
        }
        utf8_destory(methodName);
        utf8_destory(methodType);
    }


    //
    classloader_destory(sys_classloader);
    sys_classloader = NULL;
    classloader_destory(array_classloader);
    array_classloader = NULL;
    garbage_collector_destory();
    //
    utf8_destory(str_mainClsName);
    arraylist_destory(thread_list);
    instruct_indexies_destory(instructionsIndexies);
    instructionsIndexies = NULL;
    native_lib_destory();
    runtime_dispose(&runtime);
    sys_properties_dispose();
    thread_lock_dispose(&sys_lock);
    close_log();
    jvm_printf("over\n");

#ifdef __MEM_LEAK_DETECT
    dbg_heap_dump("");
    dbg_mem_stat();
#endif //__MEM_LEAK_DETECT
    return ret;
}
