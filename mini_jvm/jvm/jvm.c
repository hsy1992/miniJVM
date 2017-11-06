

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "jvm.h"
#include "../utils/utf8_string.h"
#include "garbage.h"
#include "jvm_util.h"
#include "java_native_std.h"
#include "jdwp.h"

void _on_jvm_sig(s32 signo) {
    int debug = 1;
}

void main_thread_create(Runtime *runtime) {

    Class *thread_clazz = classes_load_get_c("java/lang/Thread", runtime);
    //为主线程创建Thread实例
    main_thread = instance_create(thread_clazz);
    runtime->threadInfo->jthread = main_thread;//Thread.init currentThread() need this
    instance_init(main_thread, runtime);
    jthread_init_with_runtime(main_thread, runtime);

    garbage_refer_reg(main_thread);
}

void main_thread_destory() {

    main_runtime->threadInfo->is_suspend = 1;
    //主线程实例被回收
    jthread_dispose(main_thread);
    main_thread = NULL;
}

void print_exception(Runtime *runtime) {
    __refer ref = pop_ref(runtime->stack);
    Instance *ins = (Instance *) ref;
    Utf8String *getStackFrame_name = utf8_create_c("getCodeStack");
    Utf8String *getStackFrame_type = utf8_create_c("()Ljava/lang/String;");
    MethodInfo *getStackFrame = find_methodInfo_by_name(ins->mb.clazz->name, getStackFrame_name,
                                                        getStackFrame_type);
    utf8_destory(getStackFrame_name);
    utf8_destory(getStackFrame_type);
    if (getStackFrame) {
        push_ref(runtime->stack, ins);
        execute_method(getStackFrame, runtime, getStackFrame->_this_class);
        ins = (Instance *) pop_ref(runtime->stack);
        Utf8String *str = utf8_create();
        jstring_2_utf8(ins, str);
        printf("%s\n", utf8_cstr(str));
        utf8_destory(str);
    } else {
        printf("MAIN ERROR: %s\n", utf8_cstr(ins->mb.clazz->name));
    }
}

ClassLoader *classloader_create(c8 *path) {
    ClassLoader *class_loader = jvm_alloc(sizeof(ClassLoader));
    class_loader->g_classpath = utf8_create_c(path);
    //创建类容器
    class_loader->classes = hashtable_create(UNICODE_STR_HASH_FUNC, UNICODE_STR_EQUALS_FUNC);
    //创建引用类
    class_loader->JVM_CLASS = class_create();
    class_loader->JVM_CLASS->name = utf8_create_c("ClassLoader");
    garbage_refer_hold(class_loader->JVM_CLASS);
    garbage_refer_reg(class_loader->JVM_CLASS);
    return class_loader;
}

void classloader_destory(ClassLoader *class_loader) {
    HashtableIterator hti;
    hashtable_iterate(class_loader->classes, &hti);
    for (; hashtable_iter_has_more(&hti);) {
        HashtableValue v = hashtable_iter_next_value(&hti);
        garbage_refer_release(v);
    }
    garbage_refer_release(class_loader->JVM_CLASS);

    hashtable_clear(class_loader->classes);
    utf8_destory(class_loader->g_classpath);
    class_loader->g_classpath = NULL;
    hashtable_destory(class_loader->classes);
    class_loader->classes = NULL;
    jvm_free(class_loader);
}

void classloader_classstatic_clear(ClassLoader *class_loader) {
    HashtableIterator hti;
    hashtable_iterate(class_loader->classes, &hti);
    for (; hashtable_iter_has_more(&hti);) {
        HashtableValue v = hashtable_iter_next_value(&hti);
        Class *clazz = (Class *) (v);
        class_clear_refer(clazz);
    }
}

s32 execute(c8 *p_classpath, c8 *p_mainclass, s32 argc, c8 **argv) {
#ifdef __MEM_LEAK_DETECT
    dbg_init(0);
    //dbg_catch_sigsegv();
#endif //__MEM_LEAK_DETECT
    //
    open_log();

    //signal(SIGPIPE, _on_jvm_sig);

#if _JVM_DEBUG_PROFILE
    instruct_profile = hashtable_create(DEFAULT_HASH_FUNC, DEFAULT_HASH_EQUALS_FUNC);
#endif

    //为指令创建索引
    instructionsIndexies = instruct_indexies_create();
    //创建垃圾收集器
    garbage_collector_create();

    sys_classloader = classloader_create(p_classpath);

    array_classloader = classloader_create("");




    //创建线程容器
    thread_list = arraylist_create(0);
    thread_lock_init(&threadlist_lock);
    //本地方法库
    native_libs = arraylist_create(0);
    reg_std_native_lib();
    reg_net_native_lib();
    reg_jdwp_native_lib();

    //创建运行时栈
    Runtime *runtime = runtime_create(NULL);
    main_runtime = runtime;

    //开始装载类

    Utf8String *str_mainClsName = utf8_create_c(p_mainclass);

    //装入系统属性
    sys_properties_load(sys_classloader->g_classpath);

    load_class(sys_classloader->g_classpath, str_mainClsName, sys_classloader->classes);

    HashtableIterator hti;
    hashtable_iterate(sys_classloader->classes, &hti);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    jvm_printf("classes size:%d\n", hashtable_num_entries(sys_classloader->classes));
#endif
    for (; hashtable_iter_has_more(&hti);) {
        Utf8String *k = hashtable_iter_next_key(&hti);
        Class *clazz = hashtable_get(sys_classloader->classes, k);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
        jvm_printf("classes entry : %s,%d\n", utf8_cstr(k), clazz);
#endif
        if (clazz->status != CLASS_STATUS_PREPARED)class_prepar(clazz);
    }
    hashtable_iterate(sys_classloader->classes, &hti);
    for (; hashtable_iter_has_more(&hti);) {
        Utf8String *k = hashtable_iter_next_key(&hti);
        Class *clazz = hashtable_get(sys_classloader->classes, k);
        if (clazz->status != CLASS_STATUS_CLINITED)class_clinit(clazz, runtime);//初始化
    }

    Class *clazz = classes_get(str_mainClsName);

    s32 ret = 0;
    if (clazz) {
        Utf8String *methodName = utf8_create_c("main");
        Utf8String *methodType = utf8_create_c("([Ljava/lang/String;)V");

        MethodInfo *main = find_methodInfo_by_name(str_mainClsName, methodName, methodType);
        if (main) {
            main_thread_create(runtime);
            //启动调试器
            //startJdwp(runtime);
            jdwp_start_server();

            //准备参数
            localvar_dispose(runtime);
            localvar_init(runtime, main->para_count + 1);
            s32 count = argc;
            Long2Double l2d;
            Utf8String *ustr = utf8_create_c("[java/lang/String;");
            Instance *arr = jarray_create(count, DATATYPE_REFERENCE, ustr);
            utf8_destory(ustr);
            int i;
            for (i = 0; i < argc; i++) {
                Utf8String *utfs = utf8_create_c(argv[i]);
                Instance *jstr = jstring_create(utfs, runtime);
                l2d.r = jstr;
                jarray_set_field(arr, i, &l2d);
                utf8_destory(utfs);
            }
            push_ref(runtime->stack, arr);
            //启动垃圾回收
            garbage_thread_resume();

            s64 start = currentTimeMillis();
            jvm_printf("\n\n\n\n\n\n================================= main start ================================\n");
            //调用主方法
            //if (java_debug)jthread_suspend(runtime);//jdwp 会启动调试器
            ret = execute_method(main, runtime, clazz);
            if (ret != RUNTIME_STATUS_NORMAL) {
                print_exception(runtime);
            }
            runtime->threadInfo->is_blocking = 1;
            while ((thread_list->length) > 1) {//wait for other thread over ,
                check_suspend_and_pause(runtime);
                threadSleep(100);
            }
            runtime->threadInfo->is_blocking = 0;
            jvm_printf("================================= main  end  ================================\n");
            jvm_printf("spent %lld\n", (currentTimeMillis() - start));

#if _JVM_DEBUG_PROFILE
            hashtable_iterate(instruct_profile, &hti);
            for (; hashtable_iter_has_more(&hti);) {
                u8 instruct_code = (u8) (long) hashtable_iter_next_key(&hti);
                HashtableValue sum_v = hashtable_get(instruct_profile, (HashtableKey) (long) instruct_code);
                jvm_printf("%2x \t %lld\n", instruct_code, (s64) (long) sum_v);
            }
#endif

            main_thread_destory();
            jdwp_stop_server();
        }
        utf8_destory(methodName);
        utf8_destory(methodType);
    }
    //
    garbage_collector_destory();
    //
    utf8_destory(str_mainClsName);
    arraylist_destory(thread_list);
    thread_lock_dispose(&threadlist_lock);
    instruct_indexies_destory(instructionsIndexies);
    instructionsIndexies = NULL;
    native_lib_destory();
    sys_properties_dispose();
    close_log();
    jvm_printf("over ,heap:%lld\n", heap_size);

#ifdef __MEM_LEAK_DETECT
    dbg_heap_dump("");
    dbg_mem_stat();
#endif //__MEM_LEAK_DETECT
    return ret;
}
