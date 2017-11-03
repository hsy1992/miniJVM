
#include <errno.h>
#include "jvm.h"
#include "garbage.h"
#include "jvm_util.h"

void getMemBlockName(void *memblock, Utf8String *name);

void __garbage_clear(void);


s32 checkAndWaitThreadIsSuspend(Runtime *runtime);


s32 garbage_big_search();

s32 garbage_pause_the_world();

s32 garbage_resume_the_world();

void class_mark_refer(Class *clazz);

s32 jarray_mark_refer(Instance *arr);

void instance_mark_refer(Instance *ins);

void garbage_mark_object(__refer r);

s32 garbage_mark_thread(Runtime *pruntime);


/**
 * 创建垃圾收集线程，
 *
 * 收集方法如下：
 * 当对象被引用时，会引用计数+1，同时注册进垃圾收集器，纳入监管体系。
 *
 *
 * 注册的对象包括 Class 类， Instance 对象实例（包括数组对象）
 *
 * 在垃圾回收时，由垃圾收集线程来收集，收集 collector->objs 中的对象，
 * 收集所有引用计数<=0的对象，直接销毁，释放内存
 *
 * 收集线程会暂停所有正在执行中的java线程 ，回收完之后，恢复线程执行
 *
 * jdwp调试线程中的运行时对象不可回收。
 *
 *
 * @return errorcode
 */
s32 garbage_collector_create() {
    collector = jvm_alloc(sizeof(Collector));
    collector->objs = hashtable_create(DEFAULT_HASH_FUNC, DEFAULT_HASH_EQUALS_FUNC);
    collector->objs_holder = hashset_create();

    collector->_garbage_thread_status = GARBAGE_THREAD_PAUSE;
    thread_lock_init(&collector->garbagelock);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    s32 rc = pthread_create(&collector->_garbage_thread, &attr, collect_thread_run, NULL);
    pthread_attr_destroy(&attr);
    if (rc) {
        jvm_printf("ERROR: garbage thread can't create is %d\n", rc);
    }
    return 0;
}

void garbage_collector_destory() {
    garbage_thread_lock();
    garbage_thread_stop();
    while (collector->_garbage_thread_status != GARBAGE_THREAD_DEAD) {
        garbage_thread_timedwait(50);
    }
    garbage_thread_unlock();
    //
    __garbage_clear();
    //
    hashset_destory(collector->objs_holder);
    collector->objs_holder = NULL;
    //
    hashtable_destory(collector->objs);
    collector->objs = NULL;

    //
    thread_lock_dispose(&collector->garbagelock);
    jvm_free(collector);
    collector = NULL;
}

void __garbage_clear() {
    jvm_printf("garbage clear start========================\n");
    //
    HashtableIterator hti;
    //
    jvm_printf("objs size :%lld\n", collector->objs->entries);

    //解除所有引用关系后，回收全部对象
    while (garbage_collect());//collect instance

    //release class static field
    classloader_classstatic_clear(sys_classloader);
    classloader_classstatic_clear(array_classloader);
    while (garbage_collect());//collect classes

    //release classes
    classloader_destory(sys_classloader);
    sys_classloader = NULL;
    classloader_destory(array_classloader);
    array_classloader = NULL;
    while (garbage_collect());//collect classes
    //
    jvm_printf("objs size :%lld\n", collector->objs->entries);

    //dump_refer();
    //
//    hashtable_iterate(collector->objs, &hti);
//    for (; hashtable_iter_has_more(&hti);) {
//        HashtableKey k = hashtable_iter_next_key(&hti);
//        garbage_destory_memobj((MemoryBlock *) k);
//    }

}


//===============================   inner  ====================================

s32 garbage_thread_trylock() {
    return pthread_mutex_trylock(&collector->garbagelock.mutex_lock);
}

void garbage_thread_lock() {
    pthread_mutex_lock(&collector->garbagelock.mutex_lock);
}

void garbage_thread_unlock() {
    pthread_mutex_unlock(&collector->garbagelock.mutex_lock);
}

void garbage_thread_pause() {
    collector->_garbage_thread_status = GARBAGE_THREAD_PAUSE;
}

void garbage_thread_resume() {
    collector->_garbage_thread_status = GARBAGE_THREAD_NORMAL;
}

void garbage_thread_stop() {
    collector->_garbage_thread_status = GARBAGE_THREAD_STOP;
}

void garbage_thread_wait() {
    pthread_cond_wait(&collector->garbagelock.thread_cond, &collector->garbagelock.mutex_lock);
}

void garbage_thread_timedwait(s64 ms) {
    ms += currentTimeMillis();
    struct timespec t;
    t.tv_sec = ms / 1000;
    t.tv_nsec = (ms % 1000) * 1000000;
    s32 ret = pthread_cond_timedwait(&collector->garbagelock.thread_cond, &collector->garbagelock.mutex_lock, &t);
//    if (ret == ETIMEDOUT) {
//        s32 debug = 1;
//    }
}

void garbage_thread_notify() {
    pthread_cond_signal(&collector->garbagelock.thread_cond);
}

void getMemBlockName(void *memblock, Utf8String *name) {

    MemoryBlock *mb = (MemoryBlock *) memblock;
    if (!mb) {
        utf8_append_c(name, "NULL");
    } else {
        switch (mb->type) {
            case MEM_TYPE_CLASS: {
                utf8_append_c(name, "C");
                Class *clazz = hashtable_get(collector->objs, mb);
                if (clazz)
                    utf8_append(name, clazz->name);
                break;
            }
            case MEM_TYPE_INS: {
                Instance *ins = (Instance *) mb;
                utf8_append_c(name, "L");
                Class *clazz = hashtable_get(collector->objs, ins->mb.clazz);
                if (clazz)
                    utf8_append(name, clazz->name);
                utf8_append_c(name, ";");
                break;
            }
            case MEM_TYPE_ARR: {
                Instance *arr = (Instance *) mb;

                utf8_append_c(name, "Array{");
                Class *clazz = hashtable_get(collector->objs, arr->mb.clazz);
                if (clazz)
                    utf8_append(name, clazz->name);
                utf8_append_c(name, "}");
                break;
            }
            default:
                utf8_append_c(name, "ERROR");
        }
    }
}

/**
 * 调试用，打印所有引用信息
 */
void dump_refer() {
    //jvm_printf("%d\n",sizeof(struct _Hashtable));
    HashtableIterator hti;
    hashtable_iterate(collector->objs, &hti);
    jvm_printf("=========================objs :%lld\n", (collector->objs->entries));
    for (; hashtable_iter_has_more(&hti);) {

        HashtableKey k = hashtable_iter_next_key(&hti);
        Utf8String *name = utf8_create();
        getMemBlockName(k, name);
        jvm_printf("   %s[%llx] count:%d\n", utf8_cstr(name), (s64) (long) k, ((MemoryBlock *) k)->refer_count);
        utf8_destory(name);
    }

}

//==============================   thread_run() =====================================

void *collect_thread_run(void *para) {
    s64 startAt;
    while (1) {
        startAt = currentTimeMillis();
        while (currentTimeMillis() - startAt < GARBAGE_PERIOD_MS
               && collector->_garbage_thread_status == GARBAGE_THREAD_NORMAL
               && heap_size < MAX_HEAP_SIZE
                ) {
            threadSleep(20);
        }
        if (collector->_garbage_thread_status == GARBAGE_THREAD_STOP) {
            break;
        }
        if (collector->_garbage_thread_status == GARBAGE_THREAD_PAUSE) {
            continue;
        }

        garbage_collect();
    }
    collector->_garbage_thread_status = GARBAGE_THREAD_DEAD;
    return NULL;
}

/**
 * 查找所有实例，如果发现没有被引用 set->length==0 时，也不在运行时runtime的 stack 和 局部变量表中
 * 去除掉此对象对其他对象的引用，并销毁对象
 *
 * @return ret
 */
s32 garbage_collect() {

    s64 obj_count = (collector->objs->entries);

    HashtableIterator hti;
    s64 mem1 = heap_size;
    s64 del = 0;

    if (garbage_pause_the_world(thread_list) != 0) {
        return -1;
    }

    garbage_thread_lock();
    //
    collector->flag_refer++;
    garbage_big_search();

    hashtable_iterate(collector->objs, &hti);
    for (; hashtable_iter_has_more(&hti);) {

        HashtableKey k = hashtable_iter_next_key(&hti);
        MemoryBlock *mb = (MemoryBlock *) k;

//        if (mb->refer_count <= 0) {
//            hashtable_remove(collector->objs, mb, 0);
//            garbage_destory_memobj(mb);
//            del++;
//        }
        if (mb->garbage_mark != collector->flag_refer) {
            hashtable_remove(collector->objs, mb, 0);
            garbage_destory_memobj(mb);
            del++;
        }

    }

    jvm_printf("garbage cllected OBJ: %lld -> %lld    MEM : %lld -> %lld\n",
               obj_count, hashtable_num_entries(collector->objs), mem1, heap_size);

    if (collector->_garbage_count++ % 5 == 0) {//每n秒resize一次
        hashtable_remove(collector->objs, NULL, 1);
    }
    garbage_thread_unlock();
    garbage_resume_the_world();

    return del;
}

s32 checkAndWaitThreadIsSuspend(Runtime *runtime) {
    while (!runtime->threadInfo->is_suspend
           &&
           !runtime->threadInfo->is_blocking) { // if a native method blocking , must set thread status is wait before enter native method
        garbage_thread_timedwait(20);
        if (collector->_garbage_thread_status != GARBAGE_THREAD_NORMAL) {
            return -1;
        }
    }
    return 0;
}

void garbage_destory_memobj(MemoryBlock *mb) {
#if _JVM_DEBUG_GARBAGE_DUMP
    if (mb->garbage_mark != collector->flag_refer) {
        jvm_printf("?");
    }
    Utf8String *sus = utf8_create();
    getMemBlockName(mb, sus);
    jvm_printf("X: %s[%llx] count:%d\n", utf8_cstr(sus), (s64) (long) mb, mb->refer_count);
    utf8_destory(sus);
#endif
    memoryblock_destory((Instance *) mb);
}

/**
 * 各个线程把自己还需要使用的对象进行标注，表示不能回收
 * @return ret
 */
s32 garbage_pause_the_world() {
    s32 i;
    //jvm_printf("thread size:%d\n", thread_list->length);
    garbage_thread_lock();
    if (thread_list->length) {
        for (i = 0; i < thread_list->length; i++) {
            Runtime *runtime = arraylist_get_value(thread_list, i);
            jthread_suspend(runtime);
        }

        for (i = 0; i < thread_list->length; i++) {
            Runtime *runtime = arraylist_get_value(thread_list, i);

            while (!(runtime->threadInfo->is_suspend ||
                     runtime->threadInfo->is_blocking ||
                     runtime->threadInfo->thread_status == THREAD_STATUS_ZOMBIE)) {
                garbage_thread_timedwait(10);//让出锁
            }
#if _JVM_DEBUG_GARBAGE_DUMP
            Runtime *last = getLastSon(runtime);
            jvm_printf("PAUSE thread[%llx] %s.%s, op:%d\n",
                       (s64) (long) runtime,
                       utf8_cstr(last->methodInfo->_this_class->name),
                       utf8_cstr(last->methodInfo->name),
                       (last->pc) - (last->ca ? last->ca->code : 0)
            );
#endif
        }

    }
    //调试线程
    if (java_debug) {
        Runtime *runtime = jdwpserver.runtime;
        if (runtime) {
            jthread_suspend(runtime);
            if (checkAndWaitThreadIsSuspend(runtime) == -1) {
                return -1;
            }
        }
    }
    return 0;
}

s32 garbage_resume_the_world() {
    s32 i;
    for (i = 0; i < thread_list->length; i++) {
        Runtime *runtime = arraylist_get_value(thread_list, i);
        if (runtime) {
            jthread_resume(runtime);
        }
    }
    garbage_thread_unlock();
    //调试线程
    if (java_debug) {
        Runtime *runtime = jdwpserver.runtime;
        if (runtime) {
            jthread_resume(runtime);
        }
    }
    return 0;
}
//=================================  big_search ==================================

/**
 * on all threads stoped ,
 * mark thread's localvar and stack refered obj and deep search
 * @return ret
 */

s32 garbage_big_search() {
    s32 i;
    //jvm_printf("thread set size:%d\n", thread_list->length);
    for (i = 0; i < thread_list->length; i++) {
        Runtime *runtime = threadlist_get(i);
        garbage_mark_thread(runtime);
        jthread_resume(runtime);
    }
    //调试线程
    if (java_debug) {
        Runtime *runtime = jdwpserver.runtime;
        if (runtime) {
            jthread_suspend(runtime);
            garbage_mark_thread(runtime);
            jthread_resume(runtime);
        }
    }

    HashsetIterator hi;
    hashset_iterate(collector->objs_holder, &hi);
    while (hashset_iter_has_more(&hi)) {
        HashsetKey k = hashset_iter_next_key(&hi);
        garbage_mark_object(k);
    }

    return 0;
}


/**
 * 判定某个对象是否被所有线程的runtime引用
 * 引用有两种情况：
 * 一种是被其他实例所引用，会putfield或putstatic
 * 另一种是被运行时的栈或局部变量所引用，
 * 这两种情况下，对象是不能被释放的
 *
 * @param pruntime son of runtime
 * @return how many marked
 */


s32 garbage_mark_thread(Runtime *pruntime) {
    garbage_mark_object(pruntime->threadInfo->jthread);

    s32 i;
    StackEntry entry;
    Runtime *runtime = pruntime;
    RuntimeStack *stack = runtime->stack;
    for (i = 0; i < stack->size; i++) {
        peek_entry(stack, &entry, i);
        if (is_ref(&entry)) {
            __refer ref = entry_2_refer(&entry);
            if (ref) {
                garbage_mark_object(ref);
            }
        }
    }
    while (runtime) {
        for (i = 0; i < runtime->localvar_count; i++) {
            __refer ref = runtime->localVariables[i].refer;
            if (ref) {
                garbage_mark_object(ref);
            }
        }
        runtime = runtime->son;
    }
    //jvm_printf("[%llx] notified\n", (s64) (long) pruntime->threadInfo->jthread);
    return 0;
}

/**
 * 递归标注obj所有的子孙
 * @param r addr
 */

void garbage_mark_object(__refer r) {
    if (r) {
        MemoryBlock *obj = (MemoryBlock *) r;

        //printf("%s [", utf8_cstr(obj->clazz->name));
        if (collector->flag_refer != obj->garbage_mark) {
            obj->garbage_mark = collector->flag_refer;;
            switch (obj->type) {
                case MEM_TYPE_INS:
                    instance_mark_refer((Instance *) obj);
                    break;
                case MEM_TYPE_ARR:
                    jarray_mark_refer((Instance *) obj);
                    break;
                case MEM_TYPE_CLASS:
                    class_mark_refer((Class *) obj);
                    break;
            }
        }
    }
    //printf("]");
}

void instance_mark_refer(Instance *ins) {
    s32 i;
    Class *clazz = ins->mb.clazz;
    while (clazz) {
        FieldPool *fp = &clazz->fieldPool;
        for (i = 0; i < fp->field_used; i++) {
            FieldInfo *fi = &fp->field[i];
            if ((fi->access_flags & ACC_STATIC) == 0 && isDataReferByIndex(fi->datatype_idx)) {
                c8 *ptr = getInstanceFieldPtr(ins, fi);
                if (ptr) {
                    __refer ref = getFieldRefer(ptr);
                    garbage_mark_object(ref);
                }
            }
        }
        clazz = getSuperClass(clazz);
    }
}


s32 jarray_mark_refer(Instance *arr) {
    if (arr && arr->mb.type == MEM_TYPE_ARR) {
        if (isDataReferByIndex(arr->mb.arr_type_index)) {
            s32 i;
            Long2Double l2d;
            for (i = 0; i < arr->arr_length; i++) {//把所有引用去除，否则不会垃圾回收
                l2d.l = 0;
                jarray_get_field(arr, i, &l2d);
                garbage_mark_object(l2d.r);
            }
        }

    }
    return 0;
}

void class_mark_refer(Class *clazz) {
    s32 i;
    if (clazz->field_static) {
        FieldPool *fp = &clazz->fieldPool;
        for (i = 0; i < fp->field_used; i++) {
            FieldInfo *fi = &fp->field[i];
            if ((fi->access_flags & ACC_STATIC) != 0 && isDataReferByIndex(fi->datatype_idx)) {
                c8 *ptr = getStaticFieldPtr(fi);
                if (ptr) {
                    __refer ref = getFieldRefer(ptr);
                    garbage_mark_object(ref);
                }
            }
        }

    }
}
//=================================  reg unreg ==================================




s32 garbage_is_alive(__refer sonPtr) {
    garbage_thread_lock();
    Hashset *set;
    s32 ret = 0;
    if (sonPtr) {
        //移除子引父
        set = hashtable_get(collector->objs, sonPtr);
        if (set != HASH_NULL) {
            ret = 1;
        }
    }
    garbage_thread_unlock();
    return ret;
}

s32 garbage_refer_reg(__refer ref) {
    if (ref) {
        MemoryBlock *mb = (MemoryBlock *) ref;
        garbage_thread_lock();
        if (!mb->garbage_reg) {
            hashtable_put(collector->objs, mb, mb);
            mb->garbage_reg = 1;
        }
        garbage_thread_unlock();

#if _JVM_DEBUG_GARBAGE_DUMP
        Utf8String *sus = utf8_create();
        getMemBlockName(mb, sus);
        jvm_printf("+: %s[%llx] count:%d\n", utf8_cstr(sus), (s64) (long) mb, mb->refer_count);
        utf8_destory(sus);
#endif
    }
    return 0;
}


void garbage_refer_hold(__refer ref) {
    if (ref) {
        garbage_thread_lock();
        hashset_put(collector->objs_holder, ref);
        garbage_thread_unlock();

#if _JVM_DEBUG_GARBAGE_DUMP
        Utf8String *sus = utf8_create();
        getMemBlockName(mb, sus);
        jvm_printf("<: %s[%llx] count:%d\n", utf8_cstr(sus), (s64) (long) mb, mb->refer_count);
        utf8_destory(sus);
#endif
    }
}

void garbage_refer_release(__refer ref) {
    if (ref) {
        MemoryBlock *mb = (MemoryBlock *) ref;
        garbage_thread_lock();
        hashset_remove(collector->objs_holder, ref, 0);
        garbage_thread_unlock();
#if _JVM_DEBUG_GARBAGE_DUMP
        Utf8String *sus = utf8_create();
        getMemBlockName(mb, sus);
        jvm_printf(">: %s[%llx] count:%d\n", utf8_cstr(sus), (s64) (long) mb, mb->refer_count);
        utf8_destory(sus);
#endif
    }
}
