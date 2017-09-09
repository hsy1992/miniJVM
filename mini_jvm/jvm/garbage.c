
#include "jvm.h"
#include "garbage.h"
#include "jvm_util.h"


s32 garbage_collector_create() {
    son_2_father = hashtable_create(DEFAULT_HASH_FUNC, DEFAULT_HASH_EQUALS_FUNC);
    father_2_son = hashtable_create(DEFAULT_HASH_FUNC, DEFAULT_HASH_EQUALS_FUNC);

    _garbage_refer_set_pool = arraylist_create(4);
    _garbage_thread_pause = 1;
    _garbage_thread = jvm_alloc(sizeof(pthread_t));
    _garbageCond = PTHREAD_COND_INITIALIZER;
    pthread_cond_init(&_garbageCond, NULL);
    pthread_mutexattr_init(&_garbage_attr);
    pthread_mutexattr_settype(&_garbage_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&_garbage_lock, &_garbage_attr);
    pthread_create(_garbage_thread, NULL, collect_thread_run, NULL);
    return 0;
}

void garbage_collector_destory() {
    pthread_exit(_garbage_thread);
    jvm_free(_garbage_attr);
    jvm_free(_garbage_lock);
    jvm_free(_garbage_thread);
    hashtable_destory(son_2_father);
    hashtable_destory(father_2_son);
}

void *collect_thread_run(void *para) {
    while (!_garbage_thread_stop) {
        //_garbage_thread_pause=1;
        if (!_garbage_thread_pause) {
            garbage_collect();
        }
        threadSleep(GARBAGE_PERIOD_MS);
    }
    return NULL;
}

s32 garbage_thread_trylock() {
    return pthread_mutex_trylock(&_garbage_lock);
}

void garbage_thread_lock() {
    pthread_mutex_lock(&_garbage_lock);
}

void garbage_thread_unlock() {
    pthread_mutex_unlock(&_garbage_lock);
}

void garbage_thread_stop() {
    _garbage_thread_stop = 1;
}

void garbage_thread_wait() {
    pthread_cond_wait(&_garbageCond, &_garbage_lock);
}

void garbage_thread_notify() {
    pthread_cond_signal(&_garbageCond);
}

Hashset *_garbage_get_set() {
    if (!_garbage_refer_set_pool->length) {
        Hashset *set = hashset_create();
        return set;
    }
    return arraylist_pop_back(_garbage_refer_set_pool);
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
            if (_garbage_refer_set_pool->length > 100000) {
                hashset_destory(set);
                return;
            }
            hashset_clear(set);
            arraylist_append(_garbage_refer_set_pool, set);
            break;
        case 2:
            //it would be big mem use
            hashset_clear(set);
            arraylist_append(_garbage_refer_set_pool, set);
            break;
    }
}

//===================================================================
/**
 * 查找所有实例，如果发现没有被引用 set->length==0 时，
 * 去除掉此对象对其他对象的引用，
 * 由此引发递归回收，也可以更改回收策略，不进行递归回收，分成很多次回收，减少单次回收时间
 *
 * @return
 */
s32 garbage_collect() {
    garbage_thread_lock();
#if _JVM_DEBUG_GARBAGE_DUMP
    dump_refer();
#endif

    HashtableIterator hti;
    hashtable_iterate(son_2_father, &hti);
    //printf("hmap_t:%d\n", hashtable_num_entries(son_2_father));
    s64 obj_count = hashtable_num_entries(son_2_father);
    s64 mem1 = heap_size;


    for (; hashtable_iter_has_more(&hti);) {
        MemoryBlock *mb = (MemoryBlock *) hashtable_iter_next_key(&hti);
        mb->garbage_mark = GARBAGE_MARK_NO_REFERED;
    }
    //轮询所有线程，这些对象是否被引用过了
    //in this sub proc ,in wait maybe other thread insert new obj into son_2_father ,
    // so new inserted obj must be garbage_mark==GARBAGE_MARK_UNDEF  ,otherwise new obj would be collected.
    garbage_check_by_all_thread();

    //真正删除
    s32 i = 0;
    hashtable_iterate(son_2_father, &hti);
    for (; hashtable_iter_has_more(&hti);) {

        HashtableKey k = hashtable_iter_next_key(&hti);
        MemoryBlock *mb = (MemoryBlock *) k;
        Hashset *v = (Hashset *) hashtable_get(son_2_father, k);
//        Utf8String *str = utf8_create();
//        getMemBlockName((void*)k, str);
//        if (utf8_equals_c(str, "Array{C}") == 0 && v->entries == 1) {
//            int debug = 1;
//        }
//        utf8_destory(str);
        if (v != HASH_NULL) {
            if (v->entries == 0 && mb->garbage_mark == GARBAGE_MARK_NO_REFERED) {
                garbage_collect_memobj(k);
                i++;
            }
        }
    }
    printf("garbage cllected OBJ: %lld -> %lld    MEM : %lld -> %lld\n", obj_count, hashtable_num_entries(son_2_father),
           mem1, heap_size);

    if (_garbage_count++ % 5 == 0) {//每n秒resize一次
        hashtable_remove(son_2_father, NULL, 1);
        hashtable_remove(father_2_son, NULL, 1);
    }
    garbage_thread_unlock();

    return 0;
}

/**
 * destory a instance
 * @param k
 */
void garbage_collect_memobj(__refer k) {
    garbage_derefer_all(k);
//    if (((MemoryBlock *) k)->type == MEM_TYPE_CLASS) {
//        int debug = 1;
//    }
#if _JVM_DEBUG_GARBAGE_DUMP
    Instance *ins = (Instance *) k;
    printf("garbage collect instance [%x] of %s\n", k,
           ins->type == MEM_TYPE_INS ? utf8_cstr(ins->obj_of_clazz->name) : "ARRAY");
#endif
    Hashset *v = (Hashset *) hashtable_get(son_2_father, k);
    _garbage_put_set(v);
    hashtable_remove(son_2_father, k, 0);
    instance_destory(k);

}

/**
 * 各个线程把自己还需要使用的对象从引用列表中移除，表示不能回收
 * @param son
 * @return
 */
s32 garbage_check_by_all_thread() {
    s32 i;
    //printf("thread set size:%d\n", thread_list->length);
    for (i = 0; i < thread_list->length; i++) {
        Runtime *runtime = (Runtime *) arraylist_get_value(thread_list, i);
        /**
         * 此处有两种方式：
         * 一种是 回收线程 进行收集，收集完之后通知java工作线程
         * 第二是 java线程 进行收集，收集完之后通知回收线程
         *
         * 第一种方式主要是因为，在某些时候，java线程处于blocking io 状态，或在sleep,
         * 此时如果垃圾回收线程等待其自己回收是一件不确定的事情
         *
         */

        if (runtime->threadInfo->thread_running) {//让线程自己来标注，有的线程在等IO，等不起
            runtime->threadInfo->garbage_collect_mark_task = 1;
            //printf("thread wait\n");
            while (runtime->threadInfo->garbage_collect_mark_task == 1)
                garbage_thread_wait();//等待处理完成
        } else {//线程没有run的时候（wait sleep），自己来做，但线程恢复run时，会试图获取锁，此进正在做下面的操作
            garbage_mark_refered_obj(runtime);
            garbage_thread_notify();
        }
        //printf("thread marked refered , [%llx]\n", (s64) (long) runtime->threadInfo->jthread);
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
    garbage_thread_lock();
    s32 i;
    StackEntry entry;
    Runtime *runtime = pruntime;
    StackFrame *stack = runtime->stack;
    for (i = 0; i < stack->size; i++) {
        peek_entry(stack, &entry, i);
        if (is_ref(&entry)) {
            __refer ref = entry_2_refer(&entry);
            if (ref) {
                //printf("mark:[%llx]   ",(s64)(long)ref);
                ((MemoryBlock *) ref)->garbage_mark = GARBAGE_MARK_REFERED;
            }
        }
    }
    while (runtime) {
        for (i = 0; i < runtime->localvar_count; i++) {
            __refer ref = (runtime->localVariables + i)->refer;
            if (ref) {
                //printf("mark:[%llx]   ",(s64)(long)ref);
                ((MemoryBlock *) ref)->garbage_mark = GARBAGE_MARK_REFERED;
            }
        }
        runtime = runtime->son;
    }
    //printf("[%llx] notified\n", (s64) (long) pruntime->threadInfo->jthread);
    garbage_thread_notify();
    garbage_thread_unlock();
    return 0;
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
            case MEM_TYPE_RUNTIME: {
                Runtime *runtime = (Runtime *) mb;
                utf8_append_c(name, "Runtime ");
                utf8_append(name, runtime->clazz->name);
                utf8_append_c(name, ".");
                utf8_append(name, runtime->methodInfo->name);
                //utf8_append(name, runtime->methodInfo->descriptor);
                break;
            }
            case MEM_TYPE_INS: {
                Instance *ins = (Instance *) mb;
                utf8_append_c(name, "L");
                utf8_append(name, ins->mb.obj_of_clazz->name);
                break;
            }
            case MEM_TYPE_ARR: {
                Instance *arr = (Instance *) mb;
                //printf("Array{%d}", data_type_bytes[arr->arr_data_type]);
                utf8_append_c(name, "Array{");
                utf8_insert(name, name->length,
                            getDataTypeFlag(arr->arr_data_type));//'0' + data_type_bytes[arr->arr_data_type]);
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
    //printf("%d\n",sizeof(struct _Hashtable));
    HashtableIterator hti;
    hashtable_iterate(son_2_father, &hti);
    printf("=========================son <- father :%d\n", hashtable_num_entries(son_2_father));
    for (; hashtable_iter_has_more(&hti);) {

        HashtableKey k = hashtable_iter_next_key(&hti);
        Utf8String *name = utf8_create();
        getMemBlockName(k, name);
        printf("%s[%llx] <-{", utf8_cstr(name), (s64) (long) k);
        utf8_destory(name);
        Hashset *set = (Hashset *) hashtable_get(son_2_father, k);
        if (set != HASH_NULL) {
            HashsetIterator hti;
            hashset_iterate(set, &hti);

            for (; hashset_iter_has_more(&hti);) {
                HashsetKey key = hashset_iter_next_key(&hti);
                if (key) {
                    MemoryBlock *parent = (MemoryBlock *) key;
                    Utf8String *name = utf8_create();
                    getMemBlockName(parent, name);
                    printf("%s[%llx]; ", utf8_cstr(name), (s64) (long) parent);
                    utf8_destory(name);
                }
            }
        }
        printf("}\n");
    }

    hashtable_iterate(father_2_son, &hti);
    printf("=========================father -> son :%d\n", hashtable_num_entries(father_2_son));
    for (; hashtable_iter_has_more(&hti);) {

        HashtableKey k = hashtable_iter_next_key(&hti);
        Utf8String *name = utf8_create();
        getMemBlockName(k, name);
        printf("%s[%llx] ->{", utf8_cstr(name), (s64) (long) k);
        utf8_destory(name);
        Hashset *set = (Hashset *) hashtable_get(father_2_son, k);
        if (set != HASH_NULL) {
            HashsetIterator hti;
            hashset_iterate(set, &hti);

            for (; hashset_iter_has_more(&hti);) {
                HashsetKey key = hashset_iter_next_key(&hti);
                if (key) {
                    Instance *son = (Instance *) key;
                    Utf8String *name = utf8_create();
                    getMemBlockName(son, name);
                    printf("%s[%llx]; ", utf8_cstr(name), (s64) (long) son);
                    utf8_destory(name);
                }
            }
        }
        printf("}\n");
    }
}

/**
 *    建立引用列表
 *    1.putfield 时需要创建引用，并把原来的引用从列表删除
 *    2. aastore 时需要创建引用，并把原来的引用从列表删除
 *    3. new 新对象时创建引用，其父为 Runtime 对象，因为此对象可能还在栈空间，Runtime 销毁时，删除对其的引用
 *    4. newarray 新数组创建引用，其父为 Runtime 对象，因为此对象可能还在栈空间，Runtime 销毁时，删除对其的引用
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
#if _JVM_DEBUG_GARBAGE_DUMP
    Utf8String *us = getMemBlockName(parentPtr);
    printf("+: L%s[%x] <- %s[%x]\n", utf8_cstr(((Instance *) sonPtr)->obj_of_clazz->name), sonPtr, utf8_cstr(us),
           parentPtr);
    utf8_destory(us);
#endif
    garbage_thread_lock();
    //放入子引父
    Hashset *set = (Hashset *) hashtable_get(son_2_father, sonPtr);
    if (set == HASH_NULL) {
        set = _garbage_get_set();
        hashtable_put(son_2_father, sonPtr, set);
    }
    if (parentPtr) {
        hashset_put(set, parentPtr);
        //放入父引子
        set = hashtable_get(father_2_son, parentPtr);
        if (set == HASH_NULL) {
            set = _garbage_get_set();
            hashtable_put(father_2_son, parentPtr, set);
        }
        hashset_put(set, sonPtr);
    }
    garbage_thread_unlock();
    return sonPtr;
}

void garbage_derefer(__refer sonPtr, __refer parentPtr) {

#if _JVM_DEBUG_GARBAGE_DUMP
    Utf8String *us = getMemBlockName(parentPtr);
    printf("-: L%s[%x] <- %s[%x]\n", utf8_cstr(((Instance *) sonPtr)->obj_of_clazz->name), sonPtr, utf8_cstr(us),
           parentPtr);
    utf8_destory(us);
#endif
    garbage_thread_lock();
    Hashset *set;
    if (sonPtr && parentPtr) {
        //移除子引父
        set = hashtable_get(son_2_father, sonPtr);
        if (set != HASH_NULL) {
            hashset_remove(set, parentPtr, 0);
        }


        //移除父引子
        set = hashtable_get(father_2_son, parentPtr);
        if (set != HASH_NULL) {
            hashset_remove(set, sonPtr, 0);
        }
    }
    garbage_thread_unlock();
}

void garbage_derefer_all(__refer parentPtr) {
#if _JVM_DEBUG_GARBAGE_DUMP
    Utf8String *us = getMemBlockName(parentPtr);
    printf("X:  %s[%x]\n", utf8_cstr(us), parentPtr);
    utf8_destory(us);
#endif
    garbage_thread_lock();
    Hashset *set;
    //移除父引用的所有子
    set = hashtable_get(father_2_son, parentPtr);
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
    hashtable_remove(father_2_son, parentPtr, 0);
    garbage_thread_unlock();

}

s32 garbage_is_refer_by(__refer sonPtr, __refer parentPtr) {
    garbage_thread_lock();
    Hashset *set;
    s32 ret = 0;
    if (sonPtr && parentPtr) {
        //移除子引父
        set = hashtable_get(son_2_father, sonPtr);
        if (set != HASH_NULL) {
            ret = hashset_get(set, parentPtr) != HASH_NULL;
        }
    }
    garbage_thread_unlock();
    return ret;
}

/**
 * 在分配的内存块前面加4个字节用于存放此块内存的长度
 * @param size
 * @return
 */
void *jvm_alloc(u32 size) {
    if (!size)return NULL;
    if (size > 5000) {
        int debug = 1;
    }
    size += 4;
    void *ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
        heap_size += size;
        setFieldInt(ptr, size);
        return ptr + 4;
    }
    return NULL;
}

s32 jvm_free(void *ptr) {
    if (ptr) {
        s32 size = getFieldInt(ptr - 4);
        heap_size -= size;
//        if (size == 176) {
//            int debug = 1;
//        }
        free(ptr - 4);
        return size;
    }
    return 0;
}

void *jvm_realloc(void *pPtr, u32 size) {
    if (!pPtr)return NULL;
    if (!size)return NULL;
    size += 4;
    heap_size -= getFieldInt(pPtr - 4);
    void *ptr = realloc(pPtr - 4, size);
    if (ptr) {
        heap_size += size;
        setFieldInt(ptr, size);
        return ptr + 4;
    }
    return NULL;
}