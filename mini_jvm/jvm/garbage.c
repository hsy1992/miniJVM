
#include <errno.h>
#include "jvm.h"
#include "garbage.h"
#include "jvm_util.h"

void getMemBlockName(void *memblock, Utf8String *name);

void __garbage_clear(void);

Hashset *_garbage_get_set(void);

void _garbage_put_set(Hashset *set);

void garbage_mark_son(__refer r);

s32 checkAndWaitThreadIsSuspend(Runtime *runtime);

void _garbage_derefer(__refer sonPtr, __refer parentPtr, s32 clearAll);

void _garbage_refer(__refer sonPtr, __refer parentPtr);

void _garbage_derefer_all(__refer parentPtr);

s32 garbage_stop_the_world();

void _garbage_buf_2_table();

/**
 * 创建垃圾收集线程，
 *
 * 收集方法如下：
 * 所有新创建的对象，向垃圾收集器注册，表示这个对象将来可能会被回收，纳入监管体系。
 * 同时向 son_2_father，father_2_son 注册，
 * son_2_father 记录了子对象对父对象的引用，同时在father_2_son中记录父对象对子对象的引用。
 *
 * 建立和解除引用关系，比如：
 * Class Foo{
 *     Object var;
 *
 *     void m(Object obj1, Object obj2){
 *         var= obj1;  //建立 Foo实例 与 obj1 的引用关系
 *         var= obj2;  //解除 Foo实例 与 obj1 的引用关系， 建立 Foo实例 与 obj2 的引用关系
 *     }
 * }
 *
 * 注册的对象包括 Class 类， Instance 对象实例（包括数组对象）
 *
 * 在垃圾回收时，由单独的线程来异步收集，主要收集 son_2_father 中的对象，
 * 其中在 son_2_father 中尚存在引用关系的不收集，
 * key为一个对象实例，值为一个set集合，其中包括所有其他引用了key的对象，set 中0个对象时，表示系统中没有别的对象对其引用。
 * 不过，有一些对象并没有被别的对象引用，但他仍然在运行程序中被使用，也不可回收，
 * 这些对象存在于 Runtime中的堆栈 Runtime->stack或者方法method的局部变量表中Runtime->localvar。
 *
 * 因此在收集时，收集线程会暂停正在执行中的java线程 ，然后比对这个线程中所有的 stack 和 localvar ,
 * 如果有对象正在栈或局部变量中，则标记为不可回收。
 *
 * 还有一个jdwp调试线程中的运行时对象不可回收。
 *
 * 检查完这几方面之后，那些 （1）引用对象为0个，（2）未被其他线程运行时使用的对象，将被回收。
 *
 * @return errorcode
 */
s32 garbage_collector_create() {
    collector = jvm_alloc(sizeof(Collector));
    collector->objs = hashtable_create(DEFAULT_HASH_FUNC, DEFAULT_HASH_EQUALS_FUNC);

    collector->buffer = linkedlist_create();
    thread_lock_init(&collector->bufferlock);

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

    //
    hashtable_destory(collector->objs);
    collector->objs = NULL;

    //
    linkedlist_destory(collector->buffer);
    thread_lock_dispose(&collector->bufferlock);
    //
    thread_lock_dispose(&collector->garbagelock);
    jvm_free(collector);
    collector = NULL;
}

void __garbage_clear() {
    //
    HashtableIterator hti;
    //
    jvm_printf("objs size :%lld\n", collector->objs->entries);
    //解除所有引用关系后，回收全部对象
    while (garbage_collect()) {

    }
    //
    jvm_printf("objs size :%lld\n", collector->objs->entries);
    //
    hashtable_iterate(collector->objs, &hti);
    for (; hashtable_iter_has_more(&hti);) {
        HashtableKey k = hashtable_iter_next_key(&hti);
        Hashset *v = (Hashset *) hashtable_get(collector->objs, k);
        if (v != HASH_NULL) {
            hashset_destory(v);
            hashtable_put(collector->objs, k, NULL);
        }
    }

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
                Class *clazz = (Class *) mb;
                utf8_append(name, clazz->name);
                break;
            }
            case MEM_TYPE_INS: {
                Instance *ins = (Instance *) mb;
                utf8_append_c(name, "L");
                if (sys_classloader)
                    utf8_append(name, ins->mb.clazz->name);
                utf8_append_c(name, ";");
                break;
            }
            case MEM_TYPE_ARR: {
                Instance *arr = (Instance *) mb;

                utf8_append_c(name, "Array{");
                if (array_classloader)
                    utf8_append(name, arr->mb.clazz->name);
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
        jvm_printf("%s[%llx] :{ %d }\n", utf8_cstr(name), (s64) (long) k, ((MemoryBlock *) k)->refer_count);
        utf8_destory(name);
    }

}

//==============================   gc() =====================================

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

#if _JVM_DEBUG_GARBAGE_DUMP
    //dump_refer();
#endif
    s64 obj_count = (collector->objs->entries);

    HashtableIterator hti;
    s64 mem1 = heap_size;
    s64 del = 0;

    if (garbage_stop_the_world(thread_list) != 0) {
        return -1;
    }

    garbage_thread_lock();
    //

    hashtable_iterate(collector->objs, &hti);
    for (; hashtable_iter_has_more(&hti);) {

        HashtableKey k = hashtable_iter_next_key(&hti);
        MemoryBlock *mb = (MemoryBlock *) k;

        if (mb->refer_count == 0) {
            //memoryblock_destory((Instance *) mb);
            del++;
            hashtable_remove(collector->objs, mb, 0);
        }
    }

    jvm_printf("garbage cllected OBJ: %lld -> %lld    MEM : %lld -> %lld\n",
               obj_count, hashtable_num_entries(collector->objs), mem1, heap_size);

    if (collector->_garbage_count++ % 5 == 0) {//每n秒resize一次
        hashtable_remove(collector->objs, NULL, 1);
    }
    garbage_thread_unlock();

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

/**
 * 各个线程把自己还需要使用的对象进行标注，表示不能回收
 * @return ret
 */
s32 garbage_stop_the_world() {
    s32 i;
    //jvm_printf("thread size:%d\n", thread_list->length);
    garbage_thread_lock();
    s32 all_paused = 0;
    while (all_paused == thread_list->length) {//停掉所有线程
        all_paused = 0;
        for (i = 0; i < thread_list->length; i++) {
            Runtime *runtime = arraylist_get_value(thread_list, i);
            jthread_suspend(runtime);
            if (runtime->threadInfo->is_suspend ||
                runtime->threadInfo->is_blocking ||
                runtime->threadInfo->thread_status == THREAD_STATUS_ZOMBIE) {
                all_paused++;
                continue;
            }
            garbage_thread_timedwait(10);//让出锁
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

s32 garbage_refer_count_inc(__refer ref) {
    MemoryBlock *mb = (MemoryBlock *) ref;
    garbage_thread_lock();
    mb->refer_count++;
    hashtable_put(collector->objs, mb, NULL);
    garbage_thread_unlock();
    return 0;
}

s32 garbage_refer_count_dec(__refer ref) {
    MemoryBlock *mb = (MemoryBlock *) ref;
    if (mb) {
        garbage_thread_lock();
        mb->refer_count--;
        if (hashtable_get(collector->objs, mb) == NULL) {
            jvm_printf("garbage detect error: instance not found %llx\n", (s64) (long) mb);
        }
        garbage_thread_unlock();
    }
    return 0;
}

s32 garbage_reg(__refer ref) {
    MemoryBlock *mb = (MemoryBlock *) ref;
    garbage_thread_lock();
    hashtable_put(collector->objs, mb, NULL);
    garbage_thread_unlock();
    return 0;
}