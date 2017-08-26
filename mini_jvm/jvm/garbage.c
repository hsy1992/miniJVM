
#include "jvm.h"
#include "garbage.h"
#include "../utils/arraylist.h"
#include "jvm_util.h"

s64 GARBAGE_PERIOD_MS = 1000;

s32 garbage_collector_create() {
    son_2_father = hashtable_create(_DEFAULT_HashtableHash, _DEFAULT_HashtableEquals);
    father_2_son = hashtable_create(_DEFAULT_HashtableHash, _DEFAULT_HashtableEquals);
    _garbage_thread = jvm_alloc(sizeof(pthread_t));

    pthread_mutexattr_init(&_garbage_attr);
    pthread_mutexattr_settype(&_garbage_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&_garbage_lock, &_garbage_attr);
    pthread_create(_garbage_thread, NULL, collect_thread_run, NULL);
    return 0;
}

void garbage_collector_destory() {
    hashtable_destory(son_2_father);
    hashtable_destory(father_2_son);
}

void *collect_thread_run(void *para) {
    while (!_garbage_thread_stop) {
        s64 freepre = MAX_HEAP_SIZE - heap_size;
        garbage_collect();
        printf("garbage collected , from %lld to %lld\n", freepre, MAX_HEAP_SIZE - heap_size);
        threadWait(GARBAGE_PERIOD_MS);
    }
    return NULL;
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

/**
 * 查找所有实例，如果发现没有被引用 list->length==0 时，
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

    for (; hashtable_iter_has_more(&hti);) {

        HashtableKey k = hashtable_iter_next_key(&hti);
        ArrayList *v = (ArrayList *) hashtable_get(son_2_father, k);
        //printf("%s,%s\n", k, v);
        if (v != HASH_TABLE_NULL) {
            if (v->length == 0) {
                garbage_derefer_all(k);
#if _JVM_DEBUG_GARBAGE_DUMP
                Instance *ins = (Instance *) k;
                printf("garbage collect instance [%x] of %s\n", k,
                       ins->type == MEM_TYPE_OBJ ? utf8_cstr(ins->obj_of_clazz->name) : "ARRAY");
#endif
                hashtable_remove(son_2_father, k, 0);
                arraylist_destory(v);
                instance_destory(k);

            }
        }
    }
    garbage_thread_unlock();

    return 0;
}

Utf8String *getMemBlockName(void *memblock) {
    Utf8String *name = utf8_create();
    MemoryBlock *mb = (MemoryBlock *) memblock;
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
        case MEM_TYPE_OBJ: {
            Instance *ins = (Instance *) mb;
            utf8_append_c(name, "L");
            utf8_append(name, ins->obj_of_clazz->name);
            break;
        }
        case MEM_TYPE_ARR: {
            Instance *arr = (Instance *) mb;
            printf("Array{%d}", data_type_bytes[arr->arr_data_type]);
            utf8_append_c(name, "Array{");
            utf8_insert(name, name->length, '0' + data_type_bytes[arr->arr_data_type]);
            utf8_append_c(name, "}");
            break;

        }
        default:
            utf8_append_c(name, "ERROR");
    }
    return name;
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
        printf("%s[%x] <-{", utf8_cstr(((Instance *) k)->obj_of_clazz->name), k);
        ArrayList *list = (ArrayList *) hashtable_get(son_2_father, k);
        if (list != HASH_TABLE_NULL) {
            int i;
            for (i = 0; i < list->length; i++) {
                if (list->length) {
                    MemoryBlock *parent = (MemoryBlock *) arraylist_get_value(list, i);
                    Utf8String *us = getMemBlockName(parent);
                    printf("%s[%x]; ", utf8_cstr(us), parent);
                    utf8_destory(us);
                }
            }
        }
        printf("}\n");
    }

    hashtable_iterate(father_2_son, &hti);
    printf("=========================father -> son :%d\n", hashtable_num_entries(father_2_son));
    for (; hashtable_iter_has_more(&hti);) {

        HashtableKey k = hashtable_iter_next_key(&hti);
        Utf8String *us = getMemBlockName(k);
        printf("%s[%x] ->{", utf8_cstr(us), k);
        utf8_destory(us);
        ArrayList *list = (ArrayList *) hashtable_get(son_2_father, k);
        if (list != HASH_TABLE_NULL) {
            int i;
            for (i = 0; i < list->length; i++) {
                if (list->length) {
                    Instance *son = (Instance *) arraylist_get_value(list, i);
                    printf("%s[%x]; ", utf8_cstr(son->obj_of_clazz->name), son);
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
#if _JVM_DEBUG_GARBAGE_DUMP
    Utf8String *us = getMemBlockName(parentPtr);
    printf("+: L%s[%x] <- %s[%x]\n", utf8_cstr(((Instance *) sonPtr)->obj_of_clazz->name), sonPtr, utf8_cstr(us),
           parentPtr);
    utf8_destory(us);
#endif
    garbage_thread_lock();
    //放入子引父
    ArrayListValue list = hashtable_get(son_2_father, sonPtr);
    if (list == HASH_TABLE_NULL) {
        list = arraylist_create(0);
        hashtable_put(son_2_father, sonPtr, list);
    }
    arraylist_append(list, parentPtr);

    //放入父引子
    list = hashtable_get(father_2_son, parentPtr);
    if (list == HASH_TABLE_NULL) {
        list = arraylist_create(0);
        hashtable_put(father_2_son, parentPtr, list);
    }
    arraylist_append(list, sonPtr);
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
    ArrayList *list;
    if (sonPtr && parentPtr) {
        //移除子引父
        list = hashtable_get(son_2_father, sonPtr);
        if (list != HASH_TABLE_NULL) {
            arraylist_remove_at(list, arraylist_index_of(list, arraylist_compare_int, parentPtr));
        }


        //移除父引子
        list = hashtable_get(father_2_son, parentPtr);
        if (list != HASH_TABLE_NULL) {
            arraylist_remove_at(list, arraylist_index_of(list, arraylist_compare_int, sonPtr));
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
    ArrayList *list;
    //移除父引用的所有子
    list = hashtable_get(father_2_son, parentPtr);
    if (list) {
        s32 i;
        while (list->length) {
            ArrayListValue son = arraylist_pop_end(list);
            garbage_derefer(son, parentPtr);
        }
        arraylist_destory(list);
    }
    hashtable_remove(father_2_son, parentPtr, 0);
    garbage_thread_unlock();
}

/**
 * 在分配的内存块前面加4个字节用于存放此块内存的长度
 * @param size
 * @return
 */
void *jvm_alloc(u32 size) {
    if (!size)return NULL;
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

void jvm_free(void *ptr) {
    if (ptr) {
        heap_size -= getFieldInt(ptr - 4);
//        if (getFieldInt(ptr - 4) > 1000) {
//            int debug = 1;
//        }
        free(ptr - 4);
    }
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