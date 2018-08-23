
#include <errno.h>
#include "jvm.h"
#include "garbage.h"
#include "jvm_util.h"


void _dump_refer(void);

void _getMBName(void *memblock, Utf8String *name);

void _garbage_clear(void);

void _garbage_destory_memobj(MemoryBlock *k);

void _garbage_change_flag(void);

void _garbage_copy_refer(void);

s32 _garbage_big_search(void);

s32 _garbage_pause_the_world(void);

s32 _garbage_resume_the_world(void);

s32 _checkAndWaitThreadIsSuspend(Runtime *runtime);

void _garbage_mark_object(__refer ref);

s32 _garbage_copy_refer_thread(Runtime *pruntime);


/**
 * 创建垃圾收集线程，
 *
 * 收集方法如下：
 * 当对象被创建时，注册进垃圾收集器，纳入监管体系。
 *
 *
 * 注册的对象包括 Class 类， Instance 对象实例（包括数组对象）
 *
 * 在垃圾回收时，由垃圾收集线程来收集，收集 collector->header 链表中的对象，
 * 收集方法为： 当对象未被任一线程引用时，进行标记，直接销毁，释放内存
 *
 * 收集线程会暂停所有正在执行中的java线程 ，回收完之后，恢复线程执行
 *
 * jdwp调试线程中的运行时对象不可回收。
 *
 * collecting  step:
 *
 * 1. stop the world
 * 2. all reg/hold/release operation add to a temp linklist ,
 * 3. copy all runtime refer
 * 4. mark object when referenced by threads
 * 5. resume the world
 * 6. release unmarked object memory
 * 7. move temp linklist to main list
 *
 * @return errorcode
 */


s32 garbage_collector_create() {
    collector = jvm_calloc(sizeof(GcCollector));
    collector->objs_holder = hashset_create();

    collector->runtime_refer_copy = arraylist_create(256);

    collector->runtime = runtime_create(NULL);

    collector->_garbage_thread_status = GARBAGE_THREAD_PAUSE;
    thread_lock_init(&collector->garbagelock);
    //开始 GC 线程
    s32 rc = thrd_create(&collector->_garbage_thread, _collect_thread_run, NULL);
    if (rc != thrd_success) {
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
    _garbage_clear();
    //
    hashset_destory(collector->objs_holder);
    collector->objs_holder = NULL;

    arraylist_destory(collector->runtime_refer_copy);

    //
    runtime_destory(collector->runtime);
    thread_lock_dispose(&collector->garbagelock);
    jvm_free(collector);
    collector = NULL;
}


void _garbage_clear() {
    jvm_printf("[INFO]garbage clear start========================\n");

    //
    jvm_printf("[INFO]objs size :%lld\n", collector->obj_count);

    //解除所有引用关系后，回收全部对象
    while (garbage_collect());//collect instance

    //release class static field
    classloader_release_classs_static_field(sys_classloader);
    while (garbage_collect());//collect classes

    //release classes
    classloader_destory(sys_classloader);
    sys_classloader = NULL;
    while (garbage_collect());//collect classes

    //
    jvm_printf("[INFO]objs size :%lld\n", collector->obj_count);
    //dump_refer();
}


//===============================   inner  ====================================

s32 garbage_thread_trylock() {
    return mtx_trylock(&collector->garbagelock.mutex_lock);
}

void garbage_thread_lock() {
    mtx_lock(&collector->garbagelock.mutex_lock);
}

void garbage_thread_unlock() {
    mtx_unlock(&collector->garbagelock.mutex_lock);
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
    cnd_wait(&collector->garbagelock.thread_cond, &collector->garbagelock.mutex_lock);
}

void garbage_thread_timedwait(s64 ms) {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    t.tv_sec += ms / 1000;
    t.tv_nsec += (ms % 1000) * 1000000;
    s32 ret = cnd_timedwait(&collector->garbagelock.thread_cond, &collector->garbagelock.mutex_lock, &t);
//    if (ret == ETIMEDOUT) {
//        s32 debug = 1;
//    }
}

void garbage_thread_notify() {
    cnd_signal(&collector->garbagelock.thread_cond);
}

void garbage_thread_notifyall() {
    cnd_broadcast(&collector->garbagelock.thread_cond);
}
//=============================   debug ===================================

void _getMBName(void *memblock, Utf8String *name) {

    MemoryBlock *mb = (MemoryBlock *) memblock;
    if (!mb) {
        utf8_append_c(name, "NULL");
    } else {
        switch (mb->type) {
            case MEM_TYPE_CLASS: {
                utf8_append_c(name, "C");
                JClass *clazz = collector->_garbage_thread_status == GARBAGE_THREAD_NORMAL ? (JClass *) mb : NULL;
                if (clazz)
                    utf8_append(name, clazz->name);
                break;
            }
            case MEM_TYPE_INS: {
                Instance *ins = (Instance *) mb;
                utf8_append_c(name, "L");
                JClass *clazz = collector->_garbage_thread_status == GARBAGE_THREAD_NORMAL ? ins->mb.clazz : NULL;
                if (clazz)
                    utf8_append(name, clazz->name);
                utf8_append_c(name, ";");
                break;
            }
            case MEM_TYPE_ARR: {
                Instance *arr = (Instance *) mb;

                utf8_append_c(name, "Array{");
                JClass *clazz = collector->_garbage_thread_status == GARBAGE_THREAD_NORMAL ? arr->mb.clazz : NULL;
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


s32 _getMBSize(MemoryBlock *mb) {
    s32 size = 0;
    if (mb) {
        switch (mb->type) {
            case MEM_TYPE_INS: {
                size = sizeof(Instance) + mb->clazz->field_instance_len;
                break;
            }
            case MEM_TYPE_ARR: {
                Instance *arr = (Instance *) mb;
                size = sizeof(Instance) + data_type_bytes[mb->arr_type_index] * arr->arr_length;
                break;
            }
            case MEM_TYPE_CLASS: {
                size = sizeof(JClass) + ((JClass *) mb)->field_static_len;
                break;
            }
            default:
                jvm_printf("[ERROR] error memblock %llx.\n", (s64) (intptr_t) mb);
        }
    }

    return size;
}

/**
 * 调试用，打印所有引用信息
 */
void _dump_refer() {
    //jvm_printf("%d\n",sizeof(struct _Hashset));
    MemoryBlock *mb = collector->tmp_header;
    while (mb) {
        Utf8String *name = utf8_create();
        _getMBName(mb, name);
        jvm_printf("   %s[%llx] \n", utf8_cstr(name), (s64) (intptr_t) mb);
        utf8_destory(name);
        mb = mb->next;
    }
}

void garbage_dump_runtime() {
    s32 i;
    //jvm_printf("thread set size:%d\n", thread_list->length);
    Utf8String *name = utf8_create();
    for (i = 0; i < thread_list->length; i++) {
        Runtime *runtime = threadlist_get(i);

        jvm_printf("[INFO]============ runtime dump [%llx] ============\n", (s64) (intptr_t) runtime);
        s32 j;
        StackEntry entry;
        RuntimeStack *stack = runtime->stack;
        for (j = 0; j < stack->size; j++) {
            peek_entry(stack, &entry, j);
            if (is_ref(&entry)) {
                __refer ref = entry_2_refer(&entry);
                if (ref) {
                    utf8_clear(name);
                    _getMBName(ref, name);
                    jvm_printf("   %s[%llx] \n", utf8_cstr(name), (s64) (intptr_t) ref);
                    utf8_destory(name);
                }
            }
        }
        while (runtime) {
            for (j = 0; j < runtime->localvar_count; j++) {
                LocalVarItem *item = &runtime->localvar[j];
                if (item->type & STACK_ENTRY_REF) {
                    __refer ref = item->rvalue;
                    utf8_clear(name);
                    _getMBName(ref, name);
                    jvm_printf("   %s[%llx] \n", utf8_cstr(name), (s64) (intptr_t) ref);
                    utf8_destory(name);
                }
            }
            runtime = runtime->son;
        }
    }
}


void _garbage_put_in_holder(__refer ref) {
    hashset_put(collector->objs_holder, ref);

#if _JVM_DEBUG_GARBAGE_DUMP
    Utf8String *sus = utf8_create();
    _getMBName((MemoryBlock *) ref, sus);
    jvm_printf("+: %s[%llx]\n", utf8_cstr(sus), (s64) (intptr_t) ref);
    utf8_destory(sus);
#endif
}

void _garbage_remove_out_holder(__refer ref) {
    hashset_remove(collector->objs_holder, ref, 0);
#if _JVM_DEBUG_GARBAGE_DUMP
    Utf8String *sus = utf8_create();
    _getMBName((MemoryBlock *) ref, sus);
    jvm_printf("-: %s[%llx]\n", utf8_cstr(sus), (s64) (intptr_t) ref);
    utf8_destory(sus);
#endif
}
//==============================   thread_run() =====================================

// GC 线程
s32 _collect_thread_run(void *para) {
    s64 lastgc = currentTimeMillis();
    while (1) {
        threadSleep(10);

        if (collector->_garbage_thread_status == GARBAGE_THREAD_STOP) {
            break;
        }
        if (collector->_garbage_thread_status == GARBAGE_THREAD_PAUSE) {
            continue;
        }

        //至少间隔 1S 才做一次 GC
        if (currentTimeMillis() - lastgc < 1000) {// less than custom sec no gc
            continue;
        }
//        if (java_debug && jdwpserver.clients->length) {// less than 3 sec no gc
//            continue;
//        }

        //当堆占用超过 80% 才做 GC
        if (currentTimeMillis() - lastgc > GARBAGE_PERIOD_MS || heap_size > MAX_HEAP_SIZE * .8f) {
            //真正 GC
            garbage_collect();
            lastgc = currentTimeMillis();
        }

    }
    collector->_garbage_thread_status = GARBAGE_THREAD_DEAD;
    return 0;
}


/**
 * 查找所有实例，如果发现没有被引用时 mb->garbage_mark ，
 * 去除掉此对象对其他对象的引用，并销毁对象
 *
 * @return ret
 */
s64 garbage_collect() {
    collector->isgc = 1;
    s64 mem_total = 0, mem_free = 0;
    s64 del = 0;
    s64 time, start;

    start = time = currentTimeMillis();
    //prepar gc resource ,
    //GC 同步锁
    garbage_thread_lock();

    //Stop the world 暂停所有线程
    if (_garbage_pause_the_world() != 0) {
        _garbage_resume_the_world();
        return -1;
    }
//    jvm_printf("garbage_pause_the_world %lld\n", (currentTimeMillis() - time));
//    time = currentTimeMillis();
    if (collector->tmp_header) {
        collector->tmp_tailer->next = collector->header;//接起来
        collector->header = collector->tmp_header;
        collector->tmp_header = NULL;
        collector->tmp_tailer = NULL;
    }
//    jvm_printf("garbage_move_cache %lld\n", (currentTimeMillis() - time));
//    time = currentTimeMillis();
    //拷贝所有引用到 GC 链表中
    _garbage_copy_refer();
    //
//    jvm_printf("garbage_copy_refer %lld\n", (currentTimeMillis() - time));
//    time = currentTimeMillis();
    //real GC start
    //开始 GC，设置 FLAG
    _garbage_change_flag();
    //开始递归搜索已经拷贝的根引用
    _garbage_big_search();
    //
//    jvm_printf("garbage_big_search %lld\n", (currentTimeMillis() - time));
//    time = currentTimeMillis();

    //java 线程恢复运行，这时候已经标记了那些对象需要回收了，所以不再需要暂停线程
    _garbage_resume_the_world();
    garbage_thread_unlock();

//    jvm_printf("garbage_resume_the_world %lld\n", (currentTimeMillis() - time));

    s64 time_stopWorld = currentTimeMillis() - start;
    time = currentTimeMillis();
    //

    //处理 finalize 方法
    MemoryBlock *nextmb = collector->header;
    MemoryBlock *curmb, *prevmb = NULL;
    //finalize
    if (collector->_garbage_thread_status == GARBAGE_THREAD_NORMAL) {
        while (nextmb) {
            curmb = nextmb;
            nextmb = curmb->next;
            if (curmb->clazz->finalizeMethod) {// there is a method called finalize
                if (curmb->type == MEM_TYPE_INS && curmb->garbage_mark != collector->flag_refer) {
                    //这里回去调用每个对象的 finalize 方法
                    instance_finalize((Instance *) curmb, collector->runtime);
                }
            }
        }
    }

    //在 finalize 创建的对象，实在 gc 线程创建的对象，理论上没有用，需要被 GC
    gc_move_refer_thread_2_gc(collector->runtime);// maybe someone new object in finalize...

//    jvm_printf("garbage_finalize %lld\n", (currentTimeMillis() - time));
//    time = currentTimeMillis();
    //clear
    //开始释放垃圾对象的内存
    nextmb = collector->header;
    prevmb = NULL;
    s64 iter = 0;
    while (nextmb) {
        iter++;
        curmb = nextmb;
        nextmb = curmb->next;
        s32 size = _getMBSize(curmb);
        mem_total += size;
        if (curmb->garbage_mark != collector->flag_refer) {
            mem_free += size;
            //释放对象内存
            _garbage_destory_memobj(curmb);
            if (prevmb)prevmb->next = nextmb;
            else collector->header = nextmb;
            del++;
        } else {
            prevmb = curmb;
        }
    }

    //更新堆大小
    spin_lock(&collector->lock);
    collector->obj_count = iter - del;
    heap_size = mem_total - mem_free;
    spin_unlock(&collector->lock);

    s64 time_gc = currentTimeMillis() - time;
//    jvm_printf("[INFO]gc obj: %lld->%lld   heap : %lld -> %lld  stop_world: %lld  gc:%lld\n",
//               iter, collector->obj_count, mem_total, heap_size, time_stopWorld, time_gc);

#ifdef MEM_ALLOC_LTALLOC
    jvm_squeeze(0);
#endif
    collector->isgc = 0;
    return del;
}

void _garbage_change_flag() {
    collector->flag_refer++;
    if (collector->flag_refer == 0) {
        collector->flag_refer = 1;
    }
}


void _garbage_destory_memobj(MemoryBlock *mb) {
#if _JVM_DEBUG_GARBAGE_DUMP
    Utf8String *sus = utf8_create();
    _getMBName(mb, sus);
    jvm_printf("X: %s[%llx]\n", utf8_cstr(sus), (s64) (intptr_t) mb);
    utf8_destory(sus);
#endif
    memoryblock_destory((Instance *) mb);

}


/**
 * 各个线程把自己还需要使用的对象进行标注，表示不能回收
 */
void _list_iter_thread_pause(ArrayListValue value, void *para) {
    jthread_suspend((Runtime *) value);
}

//暂停所有 java 线程
s32 _garbage_pause_the_world(void) {
    s32 i;
    //jvm_printf("thread size:%d\n", thread_list->length);

    if (thread_list->length) {
        arraylist_iter_safe(thread_list, _list_iter_thread_pause, NULL);

        for (i = 0; i < thread_list->length; i++) {
            Runtime *runtime = arraylist_get_value(thread_list, i);
            if (_checkAndWaitThreadIsSuspend(runtime) == -1) {
                return -1;
            }
            gc_move_refer_thread_2_gc(runtime);

#if _JVM_DEBUG_GARBAGE_DUMP
            Utf8String *stack = utf8_create();
            getRuntimeStack(runtime, stack);
            jvm_printf("%s\n", utf8_cstr(stack));
            utf8_destory(stack);
#endif
        }

    }
    //调试线程
    if (java_debug) {
        Runtime *runtime = jdwpserver.runtime;
        if (runtime) {
            jthread_suspend(runtime);
            if (_checkAndWaitThreadIsSuspend(runtime) == -1) {
                return -1;
            }
            gc_move_refer_thread_2_gc(runtime);
        }
    }
    return 0;
}

//恢复所有线程
s32 _garbage_resume_the_world() {
    s32 i;
    for (i = 0; i < thread_list->length; i++) {
        Runtime *runtime = arraylist_get_value(thread_list, i);
        if (runtime) {
#if _JVM_DEBUG_GARBAGE_DUMP
            Utf8String *stack = utf8_create();
            getRuntimeStack(runtime, stack);
            jvm_printf("%s\n", utf8_cstr(stack));
            utf8_destory(stack);
#endif
            jthread_resume(runtime);
            garbage_thread_notifyall();
        }
    }

    //调试线程
    if (java_debug) {
        Runtime *runtime = jdwpserver.runtime;
        if (runtime) {
            jthread_resume(runtime);
        }
    }
    return 0;
}


s32 _checkAndWaitThreadIsSuspend(Runtime *runtime) {
    while (!(runtime->threadInfo->is_suspend) &&
           !(runtime->threadInfo->is_blocking)) { // if a native method blocking , must set thread status is wait before enter native method
        garbage_thread_timedwait(100);
        if (collector->_garbage_thread_status != GARBAGE_THREAD_NORMAL) {
            return -1;
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
s32 _garbage_big_search() {
    //遍历递归标记刚刚拷贝线程中的根引用
    s32 i, len;
    for (i = 0, len = collector->runtime_refer_copy->length; i < len; i++) {
        __refer r = arraylist_get_value(collector->runtime_refer_copy, i);
        _garbage_mark_object(r);
    }
    //遍历标记常量池
    HashsetIterator hi;
    hashset_iterate(collector->objs_holder, &hi);
    while (hashset_iter_has_more(&hi)) {
        HashsetKey k = hashset_iter_next_key(&hi);

        _garbage_mark_object(k);
    }

    return 0;
}

void _garbage_copy_refer() {
    arraylist_clear(collector->runtime_refer_copy);
    s32 i;
    //jvm_printf("thread set size:%d\n", thread_list->length);
    // 遍历所有线程的 Runtime，取出其中的引用
    for (i = 0; i < thread_list->length; i++) {
        // 根 Runtime，相当于根栈帧
        Runtime *runtime = threadlist_get(i);
        // 开始拷贝引用
        _garbage_copy_refer_thread(runtime);
    }
//    arraylist_iter_safe(thread_list, _list_iter_iter_copy, NULL);
    //调试线程
    if (java_debug) {
        Runtime *runtime = jdwpserver.runtime;
        if (runtime) {
            _garbage_copy_refer_thread(runtime);
        }
    }
}

/**
 * 判定某个对象是否被所有线程的runtime引用
 * 被运行时的栈或局部变量所引用，
 * 这两种情况下，对象是不能被释放的
 *
 * @param pruntime son of runtime
 * @return how many marked
 */

//可达性算法
//可注意到几个 GC Root
/**
 * 1.线程运行栈，类似寄存器所持有的引用，每个线程各持有一个
 * 2.该线程引用的其他实例的引用，是 Filed Get 操作带来的
 * 3.方法运行栈帧中的本地变量持有的引用
 **/
s32 _garbage_copy_refer_thread(Runtime *pruntime) {
    arraylist_push_back(collector->runtime_refer_copy, pruntime->threadInfo->jthread);

    s32 i, imax;
    //栈中元素(局部变量等)
    StackEntry entry;
    Runtime *runtime = pruntime;
    //该 JVM 是基于栈的，非基于寄存器，所以每个线程会持有一个运行栈来模拟 CPU CORE 的寄存器
    //取出根运行栈
    RuntimeStack *stack = runtime->stack;
    //遍历拷贝运行栈中的所有的引用，即寄存器的引用
    for (i = 0; i < stack->size; i++) {
        peek_entry(stack, &entry, i);
        if (is_ref(&entry)) {
            __refer ref = entry_2_refer(&entry);
            if (ref) {
                arraylist_push_back(collector->runtime_refer_copy, ref);
            }
        }
    }
    //遍历拷贝该线程引用的所有实例，类似 field get
    ArrayList *holder = runtime->threadInfo->instance_holder;
    for (i = 0, imax = holder->length; i < imax; i++) {
        __refer ref = arraylist_get_value(holder, i);
        arraylist_push_back(collector->runtime_refer_copy, ref);
    }
    // 遍历拷贝每个栈帧中的本地变量，即本地变量对实例的引用
    while (runtime) {
        for (i = 0; i < runtime->localvar_count; i++) {
            LocalVarItem *item = &runtime->localvar[i];
            if (item->type & STACK_ENTRY_REF) {
                __refer ref = item->rvalue;
                arraylist_push_back(collector->runtime_refer_copy, ref);
            }
        }
        runtime = runtime->son;
    }
    //jvm_printf("[%llx] notified\n", (s64) (intptr_t) pruntime->threadInfo->jthread);
    return 0;
}

//inline 方法方式栈溢出，因为对象引用关系复杂，层级可能非常深
static inline void _instance_mark_refer(Instance *ins) {
    s32 i, len;
    JClass *clazz = ins->mb.clazz;
    //遍历该对象的所有父类型，找出所有 Field
    while (clazz) {
        FieldPool *fp = &clazz->fieldPool;
        ArrayList *fiList = clazz->insFieldPtrIndex;
        for (i = 0, len = fiList->length; i < len; i++) {
            FieldInfo *fi = &fp->field[(s32) (intptr_t) arraylist_get_value_unsafe(fiList, i)];
            c8 *ptr = getInstanceFieldPtr(ins, fi);
            //如果该 Field 不为空
            if (ptr) {
                //取出该 Field 的引用
                __refer ref = getFieldRefer(ptr);
                //如果引用存在，则标记该引用可达，不会在接下来被回收
                if (ref)_garbage_mark_object(ref);
            }
        }
        clazz = getSuperClass(clazz);
    }
}

//数组对象
static inline void _jarray_mark_refer(Instance *arr) {
    if (arr && arr->mb.type == MEM_TYPE_ARR) {
//        if (utf8_equals_c(arr->mb.clazz->name, "[[D")) {
//            jvm_printf("check %llx\n", (s64) (intptr_t) arr);
//        }
        if (isDataReferByIndex(arr->mb.arr_type_index)) {
            s32 i;
            //遍历标记数组的每一个元素
            for (i = 0; i < arr->arr_length; i++) {//把所有引用去除，否则不会垃圾回收
                s64 val = jarray_get_field(arr, i);
                if (val)_garbage_mark_object((__refer) (intptr_t) val);
            }
        }
    }
    return;
}

//类对象
/**
 * mark class static field is used
 * @param clazz class
 */
static inline void _class_mark_refer(JClass *clazz) {
    s32 i, len;
    //类对象需要一同标记类中的静态变量，即常量池
    if (clazz->field_static) {
        FieldPool *fp = &clazz->fieldPool;
        ArrayList *fiList = clazz->staticFieldPtrIndex;
        for (i = 0, len = fiList->length; i < len; i++) {
            FieldInfo *fi = &fp->field[(s32) (intptr_t) arraylist_get_value_unsafe(fiList, i)];
            c8 *ptr = getStaticFieldPtr(fi);
            if (ptr) {
                __refer ref = getFieldRefer(ptr);
                _garbage_mark_object(ref);
            }
        }
    }
}


/**
 * 递归标注obj所有的子孙
 * @param ref addr
 */
//3种对象
/**
 *1.普通对象
 *2.数组对象
 *3.类对象 
 **/
void _garbage_mark_object(__refer ref) {
    if (ref) {
        MemoryBlock *mb = (MemoryBlock *) ref;
        if (collector->flag_refer != mb->garbage_mark) {
            mb->garbage_mark = collector->flag_refer;
            switch (mb->type) {
                case MEM_TYPE_INS:
                    //普通对象
                    _instance_mark_refer((Instance *) mb);
                    break;
                case MEM_TYPE_ARR:
                    //数组对象
                    _jarray_mark_refer((Instance *) mb);
                    break;
                case MEM_TYPE_CLASS:
                    //类对象
                    _class_mark_refer((JClass *) mb);
                    break;
            }
        }
    }
}
//=================================  reg unreg ==================================

MemoryBlock *gc_is_alive(__refer ref) {
    __refer result = hashset_get(collector->objs_holder, ref);
    spin_lock(&collector->lock);
    if (!result) {
        MemoryBlock *mb = collector->header;
        while (mb) {
            if (mb == ref) {
                result = mb;
                break;
            }
            mb = mb->next;
        }
    }
    if (!result) {
        MemoryBlock *mb = collector->tmp_header;
        while (mb) {
            if (mb == ref) {
                result = mb;
                break;
            }
            mb = mb->next;
        }
    }
    if (!result) {
        s32 i;
        for (i = 0; i < thread_list->length; i++) {
            Runtime *runtime = threadlist_get(i);
            MemoryBlock *mb = runtime->threadInfo->objs_header;
            while (mb) {
                if (mb == ref) {
                    result = mb;
                    break;
                }
                mb = mb->next;
            }
        }
    }
    if (!result) {
        if (java_debug) {
            Runtime *runtime = jdwpserver.runtime;
            if (runtime) {
                MemoryBlock *mb = runtime->threadInfo->objs_header;
                while (mb) {
                    if (mb == ref) {
                        result = mb;
                        break;
                    }
                    mb = mb->next;
                }
            }
        }
    }
    spin_unlock(&collector->lock);
    return (MemoryBlock *) result;
}

/**
 * 把对象放到线程的对象列表中
 * @param runtime
 * @param ref
 */
void gc_refer_reg(Runtime *runtime, __refer ref) {
    if (!ref)return;
    MemoryBlock *mb = (MemoryBlock *) ref;
    if (!mb->garbage_reg) {
        mb->garbage_reg = 1;
        JavaThreadInfo *ti = runtime->threadInfo;
        mb->next = ti->objs_header;
        ti->objs_header = ref;
        if (!ti->objs_tailer) {
            ti->objs_tailer = ref;
        }
#if _JVM_DEBUG_GARBAGE_DUMP
        Utf8String *sus = utf8_create();
        _getMBName(mb, sus);
        jvm_printf("R: %s[%llx]\n", utf8_cstr(sus), (s64) (intptr_t) mb);
        utf8_destory(sus);
#endif
    }
}

/**
 * 把线程持有的所有对象,链接到gc的链表中去
 * move thread 's jobject to garbage
 * @param runtime
 */
void gc_move_refer_thread_2_gc(Runtime *runtime) {
    if (runtime) {
        JavaThreadInfo *ti = runtime->threadInfo;

        if (ti->objs_header) {
            //lock
            spin_lock(&collector->lock);

#if _JVM_DEBUG_GARBAGE_DUMP
            MemoryBlock *mb = ti->objs_header;
            while (mb) {
                Utf8String *sus = utf8_create();
                _getMBName(mb, sus);
                jvm_printf("M: %s[%llx]\n", utf8_cstr(sus), (s64) (intptr_t) mb);
                utf8_destory(sus);
                mb = mb->next;
            }
#endif
            ti->objs_tailer->next = collector->tmp_header;
            if (!collector->tmp_tailer) {
                collector->tmp_tailer = ti->objs_tailer;
            }
            collector->tmp_header = ti->objs_header;
            spin_unlock(&collector->lock);

            ti->objs_header = NULL;
            ti->objs_tailer = NULL;
        }
    }
}


void gc_refer_hold(__refer ref) {
    if (ref) {
        _garbage_put_in_holder(ref);
    }
}

void gc_refer_release(__refer ref) {
    if (ref) {
        _garbage_remove_out_holder(ref);
    }
}

