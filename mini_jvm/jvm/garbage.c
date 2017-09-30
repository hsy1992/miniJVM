
#include "jvm.h"
#include "garbage.h"
#include "jvm_util.h"

void getMemBlockName(void *memblock, Utf8String *name);

void __garbage_clear();

Hashset *_garbage_get_set();

void _garbage_put_set(Hashset *set);

void garbage_mark_son(__refer r);

s32 garbage_mark_by_classloader(ClassLoader *classLoader);

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
 * @return
 */
s32 garbage_collector_create() {
    collector = jvm_alloc(sizeof(Collector));
    collector->son_2_father = hashtable_create(DEFAULT_HASH_FUNC, DEFAULT_HASH_EQUALS_FUNC);
    collector->father_2_son = hashtable_create(DEFAULT_HASH_FUNC, DEFAULT_HASH_EQUALS_FUNC);
    collector->_garbage_refer_set_pool = arraylist_create(4);

    collector->_garbage_thread_pause = 1;
    collector->_garbage_thread_stoped = 0;
    collector->_garbageCond = PTHREAD_COND_INITIALIZER;
    pthread_cond_init(&collector->_garbageCond, NULL);
    pthread_mutexattr_init(&collector->_garbage_attr);
    pthread_mutexattr_settype(&collector->_garbage_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&collector->_garbage_lock, &collector->_garbage_attr);
    pthread_create(&collector->_garbage_thread, NULL, collect_thread_run, NULL);
    return 0;
}

void garbage_collector_destory() {
    garbage_thread_lock();
    collector->_garbage_thread_stop = 1;
    while (!collector->_garbage_thread_stoped) {
        garbage_thread_timedwait(50);
    }
    garbage_thread_unlock();
    //
    __garbage_clear();
    //
    Hashset *set;
    s32 i;
    for (i = 0; i < collector->_garbage_refer_set_pool->length; i++) {
        set = arraylist_get_value(collector->_garbage_refer_set_pool, i);
        if (set)hashset_destory(set);
    }
    arraylist_destory(collector->_garbage_refer_set_pool);
    collector->_garbage_refer_set_pool = NULL;
    //
    hashtable_destory(collector->son_2_father);
    collector->son_2_father = NULL;
    hashtable_destory(collector->father_2_son);
    collector->father_2_son = NULL;
    //
//    pthread_exit(&collector->_garbage_thread);
    pthread_mutexattr_destroy(&collector->_garbage_attr);
    pthread_mutex_destroy(&collector->_garbage_lock);
    pthread_cond_destroy(&collector->_garbageCond);
    jvm_free(collector);
    collector = NULL;
}

void __garbage_clear() {
    //
    HashtableIterator hti;
    //
    jvm_printf("son_2_father.size: %lld , father_2_son.size:%lld\n",
               collector->son_2_father->entries, collector->father_2_son->entries);
    //解除所有引用关系后，回收全部对象
    while (garbage_collect()) {

    }
    //
    jvm_printf("son_2_father.size: %lld , father_2_son.size:%lld\n",
               collector->son_2_father->entries, collector->father_2_son->entries);
    //
    hashtable_iterate(collector->son_2_father, &hti);
    for (; hashtable_iter_has_more(&hti);) {
        HashtableKey k = hashtable_iter_next_key(&hti);
        Hashset *v = (Hashset *) hashtable_get(collector->son_2_father, k);
        if (v != HASH_NULL) {
            hashset_destory(v);
            hashtable_put(collector->son_2_father, k, NULL);
        }
    }
    //
    hashtable_iterate(collector->father_2_son, &hti);
    for (; hashtable_iter_has_more(&hti);) {
        HashtableKey k = hashtable_iter_next_key(&hti);
        Hashset *v = (Hashset *) hashtable_get(collector->father_2_son, k);
        if (v != HASH_NULL) {
            hashset_destory(v);
            hashtable_put(collector->father_2_son, k, NULL);
        }
    }
}


void *collect_thread_run(void *para) {
    while (!collector->_garbage_thread_stop) {
        //_garbage_thread_pause=1;
        if (!collector->_garbage_thread_pause) {
            //dump_refer();
            garbage_collect();
        }
        threadSleep(GARBAGE_PERIOD_MS);
    }
    collector->_garbage_thread_stoped = 1;
    return NULL;
}

s32 garbage_thread_trylock() {
    return pthread_mutex_trylock(&collector->_garbage_lock);
}

void garbage_thread_lock() {
    pthread_mutex_lock(&collector->_garbage_lock);
}

void garbage_thread_unlock() {
    pthread_mutex_unlock(&collector->_garbage_lock);
}

void garbage_thread_stop() {
    collector->_garbage_thread_stop = 1;
}

void garbage_thread_wait() {
    pthread_cond_wait(&collector->_garbageCond, &collector->_garbage_lock);
}

void garbage_thread_timedwait(s64 ms) {
    ms += currentTimeMillis();
    struct timespec t;
    t.tv_sec = ms / 1000;
    t.tv_nsec = (ms % 1000) * 1000000;
    pthread_cond_timedwait(&collector->_garbageCond, &collector->_garbage_lock, &t);
}

void garbage_thread_notify() {
    pthread_cond_signal(&collector->_garbageCond);
}

Hashset *_garbage_get_set() {
    if (!collector->_garbage_refer_set_pool->length) {
        Hashset *set = hashset_create();
        return set;
    }
    return arraylist_pop_back(collector->_garbage_refer_set_pool);
}

/**
 * 3 mode to mem or perfomence
 * @param set
 */
void _garbage_put_set(Hashset *set) {
    s32 mode = 0;
    switch (mode) {
        case 0:
            //no cache
            hashset_destory(set);
            break;
        case 1:
            //cache limited hashset
            if (collector->_garbage_refer_set_pool->length > 100000) {
                hashset_destory(set);
                return;
            }
            hashset_clear(set);
            arraylist_append(collector->_garbage_refer_set_pool, set);
            break;
        case 2:
            //it would be big mem use
            hashset_clear(set);
            arraylist_append(collector->_garbage_refer_set_pool, set);
            break;
    }
}

//===================================================================
/**
 * 查找所有实例，如果发现没有被引用 set->length==0 时，也不在运行时runtime的 stack 和 局部变量表中
 * 去除掉此对象对其他对象的引用，并销毁对象
 *
 * @return
 */
s32 garbage_collect() {
    garbage_thread_lock();
#if _JVM_DEBUG_GARBAGE_DUMP
    //dump_refer();
#endif
    s64 obj_count = (collector->son_2_father->entries);

    HashtableIterator hti;
    s64 mem1 = heap_size;

    hashtable_iterate(collector->father_2_son, &hti);
    //把所有对象标记为可回收
    for (; hashtable_iter_has_more(&hti);) {
        MemoryBlock *mb = (MemoryBlock *) hashtable_iter_next_key(&hti);
        mb->garbage_mark = GARBAGE_MARK_NO_REFERED;
    }
    hashtable_iterate(collector->son_2_father, &hti);
    //把所有对象标记为可回收
    for (; hashtable_iter_has_more(&hti);) {
        MemoryBlock *mb = (MemoryBlock *) hashtable_iter_next_key(&hti);
        mb->garbage_mark = GARBAGE_MARK_NO_REFERED;
    }
    //所有类标记他们的静态成员
    if (sys_classloader) {
        garbage_mark_son(sys_classloader->JVM_CLASS);
        //printf("\n");
    }
    if (sys_classloader) {
        garbage_mark_son(array_classloader->JVM_CLASS);
        //printf("\n");
    }
    //轮询所有线程，标记被引用的对象
    //in this sub proc ,in wait maybe other thread insert new obj into son_2_father ,
    // so new inserted obj must be garbage_mark==GARBAGE_MARK_UNDEF  ,otherwise new obj would be collected.
    if (garbage_mark_by_threads() != 0) {
        return 1;
    }

    if (sys_classloader)garbage_mark_son(sys_classloader->JVM_CLASS);
    if (array_classloader)garbage_mark_son(array_classloader->JVM_CLASS);

    //如果没有被标记为引用，则回收掉
    s32 i = 0;
    hashtable_iterate(collector->son_2_father, &hti);
    for (; hashtable_iter_has_more(&hti);) {

        HashtableKey k = hashtable_iter_next_key(&hti);
        MemoryBlock *mb = (MemoryBlock *) k;
        Hashset *v = (Hashset *) hashtable_get(collector->son_2_father, k);

        if (v != HASH_NULL) {
            if (mb->garbage_mark == GARBAGE_MARK_NO_REFERED) {
                garbage_destory_memobj(k);
                i++;
            }
        }
    }
    jvm_printf("garbage cllected OBJ: %lld -> %lld    MEM : %lld -> %lld\n",
               obj_count, hashtable_num_entries(collector->son_2_father), mem1, heap_size);

    if (collector->_garbage_count++ % 5 == 0) {//每n秒resize一次
        hashtable_remove(collector->son_2_father, NULL, 1);
        hashtable_remove(collector->father_2_son, NULL, 1);
    }
    garbage_thread_unlock();

    return i;
}

/**
 * destory a instance
 * @param k
 */
void garbage_destory_memobj(__refer k) {
    garbage_derefer_all(k);
//#if _JVM_DEBUG_GARBAGE_DUMP
    if (((MemoryBlock *) k)->type == MEM_TYPE_CLASS) {
        Utf8String *pus = utf8_create();
        getMemBlockName(k, pus);
        jvm_printf("garbage collect  %s [%x]\n", utf8_cstr(pus), k);
        utf8_destory(pus);
    }
//#endif
    Hashset *v = (Hashset *) hashtable_get(collector->son_2_father, k);
    _garbage_put_set(v);
    hashtable_remove(collector->son_2_father, k, 0);
    instance_destory(k);

}

/**
 * 各个线程把自己还需要使用的对象进行标注，表示不能回收
 * @param son
 * @return
 */
s32 garbage_mark_by_threads() {
    s32 i;
    //jvm_printf("thread set size:%d\n", thread_list->length);
    for (i = 0; i < thread_list->length; i++) {
        Runtime *runtime = threadlist_get(i);
        /**
         *  回收线程 进行收集，先暂停线程，收集完之后恢复java工作线程
         *
         */
        if (runtime->threadInfo->thread_status != THREAD_STATUS_ZOMBIE) {
            jthread_suspend(runtime);
            while (!runtime->threadInfo->is_suspend) {
                garbage_thread_timedwait(50);
                if (collector->_garbage_thread_stop || collector->_garbage_thread_pause) {
                    return 1;
                }
            }
            garbage_mark_refered_obj(runtime);
            jthread_resume(runtime);
        }
        //jvm_printf("thread marked refered , [%llx]\n", (s64) (long) runtime->threadInfo->jthread);
    }
    //调试线程
    if (java_debug) {
        Runtime *runtime = jdwpserver.runtime;
        if (runtime) {
            jthread_suspend(runtime);
            garbage_mark_refered_obj(runtime);
            jthread_resume(runtime);
        }
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
 * @param son
 * @param jthread
 * @return
 */


s32 garbage_mark_refered_obj(Runtime *pruntime) {
    garbage_mark_son(pruntime->threadInfo->jthread);

    s32 i;
    StackEntry entry;
    Runtime *runtime = pruntime;
    RuntimeStack *stack = runtime->stack;
    for (i = 0; i < stack->size; i++) {
        peek_entry(stack, &entry, i);
        if (is_ref(&entry)) {
            __refer ref = entry_2_refer(&entry);
            if (ref) {
                garbage_mark_son(ref);
                //printf("\n");
                //jvm_printf("mark:[%llx]   ", (s64) (long) ref);
                garbage_mark_son(ref);
                //((MemoryBlock *) ref)->garbage_mark = GARBAGE_MARK_REFERED;
            }
        }
    }
    while (runtime) {
        for (i = 0; i < runtime->localvar_count; i++) {
            __refer ref = runtime->localVariables[i].refer;
            if (ref) {
                garbage_mark_son(ref);
                //printf("\n");
                //jvm_printf("mark:[%llx]   ", (s64) (long) ref);
                garbage_mark_son(ref);

                //((MemoryBlock *) ref)->garbage_mark = GARBAGE_MARK_REFERED;
            }
        }
        runtime = runtime->son;
    }
    //jvm_printf("[%llx] notified\n", (s64) (long) pruntime->threadInfo->jthread);
    return 0;
}

/**
 * 递归标注obj所有的子孙
 * @param obj
 */
void garbage_mark_son(__refer r) {
    MemoryBlock *obj = (MemoryBlock *) r;

    //printf("%s [", utf8_cstr(obj->clazz->name));
    if (obj && GARBAGE_MARK_REFERED != obj->garbage_mark) {
        obj->garbage_mark = GARBAGE_MARK_REFERED;
        Hashset *set = hashtable_get(collector->father_2_son, obj);
        if (set != HASH_NULL) {
            //printf("%d |", (set->entries));

            HashsetIterator hti;
            hashset_iterate(set, &hti);

            for (; hashset_iter_has_more(&hti);) {
                HashsetKey key = hashset_iter_next_key(&hti);
                if (key) {
                    MemoryBlock *son = (MemoryBlock *) key;
                    garbage_mark_son(son);
                }
            }
        }
    }
    //printf("]");
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
                utf8_append(name, ins->mb.clazz->name);
                utf8_append_c(name, ";");
                break;
            }
            case MEM_TYPE_ARR: {
                Instance *arr = (Instance *) mb;
                //jvm_printf("Array{%d}", data_type_bytes[arr->arr_data_type]);
                utf8_append_c(name, "Array{");
                utf8_append(name, arr->mb.clazz->name);//'0' + data_type_bytes[arr->arr_data_type]);
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
    hashtable_iterate(collector->son_2_father, &hti);
    jvm_printf("=========================son <- father :%lld\n", (collector->son_2_father->entries));
    for (; hashtable_iter_has_more(&hti);) {

        HashtableKey k = hashtable_iter_next_key(&hti);
        Utf8String *name = utf8_create();
        getMemBlockName(k, name);
        jvm_printf("%s[%llx] <-{", utf8_cstr(name), (s64) (long) k);
        utf8_destory(name);
        Hashset *set = (Hashset *) hashtable_get(collector->son_2_father, k);
        if (set != HASH_NULL) {
            HashsetIterator hti;
            hashset_iterate(set, &hti);

            for (; hashset_iter_has_more(&hti);) {
                HashsetKey key = hashset_iter_next_key(&hti);
                if (key) {
                    MemoryBlock *parent = (MemoryBlock *) key;
                    Utf8String *name = utf8_create();
                    getMemBlockName(parent, name);
                    jvm_printf("%s[%llx]; ", utf8_cstr(name), (s64) (long) parent);
                    utf8_destory(name);
                }
            }
        }
        jvm_printf("}\n");
    }

    hashtable_iterate(collector->father_2_son, &hti);
    jvm_printf("=========================father -> son :%lld\n", (collector->father_2_son->entries));
    for (; hashtable_iter_has_more(&hti);) {

        HashtableKey k = hashtable_iter_next_key(&hti);
        Utf8String *name = utf8_create();
        getMemBlockName(k, name);
        jvm_printf("%s[%llx] ->{", utf8_cstr(name), (s64) (long) k);
        utf8_destory(name);
        Hashset *set = (Hashset *) hashtable_get(collector->father_2_son, k);
        if (set != HASH_NULL) {
            HashsetIterator hti;
            hashset_iterate(set, &hti);

            for (; hashset_iter_has_more(&hti);) {
                HashsetKey key = hashset_iter_next_key(&hti);
                if (key) {
                    Instance *son = (Instance *) key;
                    Utf8String *name = utf8_create();
                    getMemBlockName(son, name);
                    jvm_printf("%s[%llx]; ", utf8_cstr(name), (s64) (long) son);
                    utf8_destory(name);
                }
            }
        }
        jvm_printf("}\n");
    }
}

/**
 *    建立引用列表
 *    1.putfield 时需要创建引用，并把原对象的引用从列表删除
 *    2. aastore 时需要创建引用，并把原对象的引用从列表删除
 *    3. new 新对象时创建引用，其父为 NULL 空值， 此时这个对象在 栈或局部变量中
 *    4. newarray 新数组创建引用，其父为 NULL 空值， 此时这个对象在 栈或局部变量中
 *
 * @param sonPtr
 * @param parentPtr
 * @return
 */

void *garbage_refer(__refer sonPtr, __refer parentPtr) {
    if (sonPtr == NULL)return sonPtr;
    if (sonPtr == parentPtr) {
        return sonPtr;
    }
    garbage_thread_lock();
#if _JVM_DEBUG_GARBAGE_DUMP
    Utf8String *pus = utf8_create();
    Utf8String *sus = utf8_create();
    getMemBlockName(parentPtr, pus);
    getMemBlockName(sonPtr, sus);
    jvm_printf("+: %s[%x] <- %s[%x]\n", utf8_cstr(sus), sonPtr, utf8_cstr(pus),
               parentPtr);
    if (utf8_equals_c(pus, "java/lang/System")) {
        s32 debug = 1;
    }
    utf8_destory(pus);
    utf8_destory(sus);
#endif
    //放入子引父
    Hashset *set = (Hashset *) hashtable_get(collector->son_2_father, sonPtr);
    if (set == HASH_NULL) {
        set = _garbage_get_set();
        hashtable_put(collector->son_2_father, sonPtr, set);
    }
    if (parentPtr) {
        hashset_put(set, parentPtr);
        //放入父引子
        set = hashtable_get(collector->father_2_son, parentPtr);
        if (set == HASH_NULL) {
            set = _garbage_get_set();
            hashtable_put(collector->father_2_son, parentPtr, set);
        }
        hashset_put(set, sonPtr);
    }
    garbage_thread_unlock();
    return sonPtr;
}

void garbage_derefer(__refer sonPtr, __refer parentPtr) {
    garbage_thread_lock();

#if _JVM_DEBUG_GARBAGE_DUMP
    Utf8String *pus = utf8_create();
    Utf8String *sus = utf8_create();
    getMemBlockName(parentPtr, pus);
    getMemBlockName(sonPtr, sus);
//    jvm_printf("-: %s[%x] <- %s[%x]\n", utf8_cstr(((Instance *) sonPtr)->mb.clazz->name), sonPtr, utf8_cstr(pus),
//               parentPtr);
    jvm_printf("-: [%x] <- [%x]\n", sonPtr, utf8_cstr(pus),
               parentPtr);
    utf8_destory(sus);
    utf8_destory(pus);
#endif
    Hashset *set;
    if (sonPtr && parentPtr) {
        //移除子引父
        set = hashtable_get(collector->son_2_father, sonPtr);
        if (set != HASH_NULL) {
            hashset_remove(set, parentPtr, 0);
        }


        //移除父引子
        set = hashtable_get(collector->father_2_son, parentPtr);
        if (set != HASH_NULL) {
            hashset_remove(set, sonPtr, 0);
        }
    }
    garbage_thread_unlock();
}

void garbage_derefer_all(__refer parentPtr) {
    garbage_thread_lock();
#if _JVM_DEBUG_GARBAGE_DUMP
    Utf8String *us = utf8_create();
    getMemBlockName(parentPtr, us);
    jvm_printf("X:  %s[%x]\n", utf8_cstr(us), parentPtr);
    utf8_destory(us);
#endif
    Hashset *set;
    //移除父引用的所有子
    set = hashtable_get(collector->father_2_son, parentPtr);
    if (set) {
        HashsetIterator hti;
        hashset_iterate(set, &hti);

        for (; hashset_iter_has_more(&hti);) {
            HashsetKey key = hashset_iter_next_key(&hti);
            if (key) {
                garbage_derefer(key, parentPtr);
            }
        }
        _garbage_put_set(set);
    }
    hashtable_remove(collector->father_2_son, parentPtr, 0);
    garbage_thread_unlock();

}

s32 garbage_is_refer_by(__refer sonPtr, __refer parentPtr) {
    garbage_thread_lock();
    Hashset *set;
    s32 ret = 0;
    if (sonPtr && parentPtr) {
        //移除子引父
        set = hashtable_get(collector->son_2_father, sonPtr);
        if (set != HASH_NULL) {
            ret = hashset_get(set, parentPtr) != HASH_NULL;
        }
    }
    garbage_thread_unlock();
    return ret;
}
