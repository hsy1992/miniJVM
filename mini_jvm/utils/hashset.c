//
// Created by gust on 2017/8/30.
//
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include "hashset.h"
#include "d_type.h"
#include "hashtable.h"

static s32 HASH_SET_DEFAULT_SIZE = 4;

static int hashset_allocate_table(Hashset *set, int size) {
    if (size) {
        set->table = jvm_alloc(size *
                               sizeof(HashsetEntry *));
        if (set->table)set->table_size = size;
    }

    return set->table != NULL;
}


static void hashset_free_entry(Hashset *set, HashsetEntry *entry) {

    jvm_free(entry);
}

unsigned long _DEFAULT_HashsetHash(HashsetKey kmer) {
    return (unsigned long) kmer;
}

Hashset *hashset_create() {
    Hashset *set;

    set = (Hashset *) jvm_alloc(sizeof(Hashset));

    if (set == NULL) {
        return NULL;
    }

    set->entries = 0;

    if (!hashset_allocate_table(set, HASH_SET_DEFAULT_SIZE)) {
        jvm_free(set);

        return NULL;
    }

    return set;
}

void hashset_destory(Hashset *set) {
    if (!set)return;
    HashsetEntry *rover;
    HashsetEntry *next;
    unsigned int i;

    for (i = 0; i < set->table_size; ++i) {
        rover = set->table[i];
        while (rover != NULL) {
            next = rover->next;
            hashset_free_entry(set, rover);
            rover = next;
        }
    }
    jvm_free(set->table);
    jvm_free(set);
}


void hashset_clear(Hashset *set) {
    HashsetEntry *rover;
    HashsetEntry *next;
    unsigned int i;

    for (i = 0; i < set->table_size; ++i) {
        rover = set->table[i];
        while (rover != NULL) {
            next = rover->next;
            hashset_free_entry(set, rover);
            rover = next;
        }
        set->table[i] = NULL;
    }
    set->entries = 0;
    if (set->table_size > HASH_SET_DEFAULT_SIZE) {
        jvm_free(set->table);
        set->table = NULL;
        set->table_size = 0;
        if (!hashset_allocate_table(set, HASH_SET_DEFAULT_SIZE)) {
            jvm_free(set);
        }
    }
}


int hashset_put(Hashset *set, HashsetKey key) {
    HashsetEntry *rover;
    HashsetEntry *newentry;
    unsigned int index;


    if ((set->entries << 1) >= set->table_size) {

        if (!hashset_resize(set, set->table_size << 1)) {

            return 0;
        }
    }

    index = DEFAULT_HASH_FUNC(key) % set->table_size;

    rover = set->table[index];

    while (rover != NULL) {
        if (DEFAULT_HASH_EQUALS_FUNC(rover->key, key) != 0) {
            rover->key = key;
            return 1;
        }
        rover = rover->next;
    }

    newentry = (HashsetEntry *) jvm_alloc(sizeof(HashsetEntry));

    if (newentry == NULL) {
        return 0;
    }

    newentry->key = key;
    newentry->val = 0;

    newentry->next = set->table[index];
    set->table[index] = newentry;

    ++set->entries;

    return 1;
}

HashsetKey hashset_get(Hashset *set, HashsetKey key) {
    HashsetEntry *rover = hashset_get_entry(set, key);
    if (!rover)return rover->key;
    return HASH_NULL;
}

HashsetEntry *hashset_get_entry(Hashset *set, HashsetKey key) {
    HashsetEntry *rover;
    unsigned int index;
    index = DEFAULT_HASH_FUNC(key) % set->table_size;
    rover = set->table[index];

    while (rover != NULL) {
        if (DEFAULT_HASH_EQUALS_FUNC(key, rover->key) != 0) {
            return rover;
        }
        rover = rover->next;
    }
    return HASH_NULL;
}

int hashset_remove(Hashset *set, HashsetKey key, int resize) {
    HashsetEntry *rover;
    HashsetEntry *pre;
    HashsetEntry *next;
    unsigned int index;
    unsigned int result;


    if (resize && (set->entries << 3) < set->table_size) {
        if (!hashset_resize(set, set->table_size >> 1)) {
            return 0;
        }
    }

    index = DEFAULT_HASH_FUNC(key) % set->table_size;

    result = 0;
    rover = set->table[index];
    pre = rover;

    while (rover != NULL) {
        next = rover->next;
        if (DEFAULT_HASH_EQUALS_FUNC(key, rover->key) != 0) {
            if (pre == rover)set->table[index] = next;
            else pre->next = next;

            hashset_free_entry(set, rover);
            --set->entries;
            result = 1;
            break;
        }
        pre = rover;
        rover = next;
    }

    return result;
}

unsigned int hashset_num_entries(Hashset *set) {
    return set->entries;
}

void hashset_iterate(Hashset *set, HashsetIterator *iterator) {
    unsigned int chain;

    iterator->set = set;
    iterator->next_entry = NULL;
    for (chain = 0; chain < set->table_size; ++chain) {

        if (set->table[chain] != NULL) {
            iterator->next_entry = set->table[chain];
            iterator->next_chain = chain;
            break;
        }
    }
}

int hashset_iter_has_more(HashsetIterator *iterator) {
    return iterator->next_entry != NULL;
}


HashsetEntry *hashset_iter_next_entry(HashsetIterator *iterator) {
    HashsetEntry *current_entry;
    Hashset *set;
    unsigned int chain;

    set = iterator->set;
    if (iterator->next_entry == NULL) {
        return HASH_NULL;
    }
    current_entry = iterator->next_entry;

    if (current_entry->next != NULL) {
        iterator->next_entry = current_entry->next;
    } else {
        chain = iterator->next_chain + 1;
        iterator->next_entry = NULL;
        while (chain < set->table_size) {
            if (set->table[chain] != NULL) {
                iterator->next_entry = set->table[chain];
                break;
            }
            ++chain;
        }

        iterator->next_chain = chain;
    }
    return current_entry;
}

HashsetKey hashset_iter_next_key(HashsetIterator *iterator) {
    HashsetEntry *current_entry = hashset_iter_next_entry(iterator);
    if (current_entry)return current_entry->key;
    return HASH_NULL;
}

int hashset_resize(Hashset *set, int size) {
    HashsetEntry **old_table;
    unsigned int old_table_size;
    HashsetEntry *rover;
    HashsetEntry *next;
    unsigned int index;
    unsigned int i;

    if (size != 0) {
        old_table = set->table;
        old_table_size = set->table_size;

        if (!hashset_allocate_table(set, size)) {
            printf("CRITICAL: FAILED TO ALLOCATE HASH TABLE!\n");

            set->table = old_table;
            set->table_size = old_table_size;

            return 0;
        }
        for (i = 0; i < old_table_size; ++i) {
            rover = old_table[i];

            while (rover != NULL) {
                next = rover->next;
                index = DEFAULT_HASH_FUNC(rover->key) % set->table_size;
                rover->next = set->table[index];
                set->table[index] = rover;
                rover = next;
            }
        }
        jvm_free(old_table);
    }

    return 1;
}

