//
// Created by gust on 2017/8/30.
//
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include "../jvm/garbage.h"
#include "hashset.h"

static s32 HASH_SET_DEFAULT_SIZE = 4;

/* Internal function used to allocate the table on hash table creation
 * and when enlarging the table */

static int hashset_allocate_table(Hashset *set, int size) {
    if (size) {
        set->table = jvm_alloc(size *
                               sizeof(HashtableEntry *));
        if (set->table)set->table_size = size;
    }

    return set->table != NULL;
}

/* Free an entry, calling the free functions if there are any registered */

static void hashset_free_entry(Hashset *set, HashsetEntry *entry) {
    /* If there is a function registered for freeing keys, use it to free
     * the key */

//    if (set->key_free_func != NULL) {
//        set->key_free_func(entry->key);
//    }

    /* Free the data structure */

    jvm_free(entry);
}

unsigned long _DEFAULT_HashsetHash(HashsetKey kmer) {
    return (unsigned long) kmer;
}


//Hashset *hashset_create(HashtableHashFunc hash_func,
//                        HashtableEqualFunc equal_func) {
Hashset *hashset_create() {
    Hashset *set;

    /* Allocate a new hash table structure */

    set = (Hashset *) jvm_alloc(sizeof(Hashset));

    if (set == NULL) {
        return NULL;
    }

//    set->hash_func = hash_func;
//    set->equal_func = equal_func;
//    set->key_free_func = NULL;
    set->entries = 0;

    /* Allocate the table */

    if (!hashset_allocate_table(set, HASH_SET_DEFAULT_SIZE)) {
        jvm_free(set);

        return NULL;
    }

    return set;
}

void hashset_destory(Hashset *set) {
    HashsetEntry *rover;
    HashsetEntry *next;
    unsigned long long int i;

    /* Free all entries in all chains */

    for (i = 0; i < set->table_size; ++i) {
        rover = set->table[i];
        while (rover != NULL) {
            next = rover->next;
            hashset_free_entry(set, rover);
            rover = next;
        }
    }

    /* Free the table */

    jvm_free(set->table);

    /* Free the hash table structure */

    jvm_free(set);
}


void hashset_clear(Hashset *set) {
    HashsetEntry *rover;
    HashsetEntry *next;
    unsigned long long int i;

    for (i = 0; i < set->table_size; ++i) {
        rover = set->table[i];
        while (rover != NULL) {
            next = rover->next;
            hashset_free_entry(set, rover);
            rover = next;
        }
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
//
//void hashset_register_free_functions(Hashset *set,
//                                     HashtableKeyFreeFunc key_free_func) {
//    set->key_free_func = key_free_func;
//}

int hashset_put(Hashset *set, HashsetKey key) {
    HashsetEntry *rover;
    HashsetEntry *newentry;
    unsigned long long int index;

    /* If there are too many items in the table with respect to the table
     * size, the number of hash collisions increases and performance
     * decreases. Enlarge the table size to prevent this happening */

    if ((set->entries << 1) >= set->table_size) {

        /* Table is more than 1/2 full */

        if (!hashset_resize(set, set->table_size << 1)) {

            /* Failed to enlarge the table */

            return 0;
        }
    }

    /* Generate the hash of the key and hence the index into the table */

    index = DEFAULT_HASH_FUNC(key) % set->table_size;

    /* Traverse the chain at this location and look for an existing
     * entry with the same key */

    rover = set->table[index];

    while (rover != NULL) {
        if (DEFAULT_HASH_EQUALS_FUNC(rover->key, key) != 0) {

            /* Same key: overwrite this entry with new data */
            /* Same with the key: use the new key value and free 
             * the old one */

//            if (set->key_free_func != NULL) {
//                set->key_free_func(rover->key);
//            }

            rover->key = key;

            /* Finished */

            return 1;
        }
        rover = rover->next;
    }

    /* Not in the hash table yet.  Create a new entry */

    newentry = (HashsetEntry *) jvm_alloc(sizeof(HashsetEntry));

    if (newentry == NULL) {
        return 0;
    }

    newentry->key = key;

    /* Link into the list */

    newentry->next = set->table[index];
    set->table[index] = newentry;

    /* Maintain the count of the number of entries */

    ++set->entries;

    /* Added successfully */

    return 1;
}

HashsetKey hashset_get(Hashset *set, HashsetKey key) {
    HashsetEntry *rover;
    unsigned long long int index;

    /* Generate the hash of the key and hence the index into the table */

    index = DEFAULT_HASH_FUNC(key) % set->table_size;

    /* Walk the chain at this index until the corresponding entry is
     * found */

    rover = set->table[index];

    while (rover != NULL) {
        if (DEFAULT_HASH_EQUALS_FUNC(key, rover->key) != 0) {

            /* Found the entry.  Return the data. */

            return key;
        }
        rover = rover->next;
    }

    /* Not found */

    return HASH_NULL;
}

int hashset_remove(Hashset *set, HashsetKey key, int resize) {
    HashsetEntry **rover;
    HashsetEntry *entry;
    unsigned long long int index;
    unsigned long long int result;

    /* If there are too few items in the table with respect to the table
     * size, the table is taking up too much space. 
     * Shrink table to improve space efficiency. */

    if (resize && (set->entries << 3) < set->table_size) {

        /* Table is less than 1/8 full */

        if (!hashset_resize(set, set->table_size >> 1)) {

            /* Failed to enlarge the table */

            return 0;
        }
    }

    /* Generate the hash of the key and hence the index into the table */

    index = DEFAULT_HASH_FUNC(key) % set->table_size;

    /* Rover points at the pointer which points at the current entry
     * in the chain being inspected.  ie. the entry in the table, or
     * the "next" pointer of the previous entry in the chain.  This
     * allows us to unlink the entry when we find it. */

    result = 0;
    rover = &set->table[index];

    while (*rover != NULL) {

        if (DEFAULT_HASH_EQUALS_FUNC(key, (*rover)->key) != 0) {

            /* This is the entry to remove */

            entry = *rover;

            /* Unlink from the list */

            *rover = entry->next;

            /* Destroy the entry structure */

            hashset_free_entry(set, entry);

            /* Track count of entries */

            --set->entries;

            result = 1;

            break;
        }

        /* Advance to the next entry */

        rover = &((*rover)->next);
    }

    return result;
}

unsigned long long int hashset_num_entries(Hashset *set) {
    return set->entries;
}

void hashset_iterate(Hashset *set, HashsetIterator *iterator) {
    unsigned long long int chain;

    iterator->set = set;

    /* Default value of next if no entries are found. */

    iterator->next_entry = NULL;

    /* Find the first entry */

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


HashsetKey hashset_iter_next_key(HashsetIterator *iterator) {
    HashsetEntry *current_entry;
    Hashset *set;
    HashsetKey result;
    unsigned long long int chain;

    set = iterator->set;

    /* No more entries? */

    if (iterator->next_entry == NULL) {
        return HASH_NULL;
    }

    /* Result is immediately available */

    current_entry = iterator->next_entry;
    result = current_entry->key;

    /* Find the next entry */

    if (current_entry->next != NULL) {

        /* Next entry in current chain */

        iterator->next_entry = current_entry->next;

    } else {

        /* None left in this chain, so advance to the next chain */

        chain = iterator->next_chain + 1;

        /* Default value if no next chain found */

        iterator->next_entry = NULL;

        while (chain < set->table_size) {

            /* Is there anything in this chain? */

            if (set->table[chain] != NULL) {
                iterator->next_entry = set->table[chain];
                break;
            }

            /* Try the next chain */

            ++chain;
        }

        iterator->next_chain = chain;
    }

    return result;
}

int hashset_resize(Hashset *set, int size) {
    HashsetEntry **old_table;
    unsigned long long int old_table_size;
    int old_prime_index;
    HashsetEntry *rover;
    HashsetEntry *next;
    unsigned long long int index;
    unsigned long long int i;

    if (size != 0) {
        /* Store a copy of the old table */
        old_table = set->table;
        old_table_size = set->table_size;

        if (!hashset_allocate_table(set, size)) {

            /* Failed to allocate the new table */

            printf("CRITICAL: FAILED TO ALLOCATE HASH TABLE!\n");

            set->table = old_table;
            set->table_size = old_table_size;

            return 0;
        }

        /* Link all entries from all chains into the new table */

        for (i = 0; i < old_table_size; ++i) {
            rover = old_table[i];

            while (rover != NULL) {
                next = rover->next;

                /* Find the index into the new table */

                index = DEFAULT_HASH_FUNC(rover->key) % set->table_size;

                /* Link this entry into the chain */

                rover->next = set->table[index];
                set->table[index] = rover;

                /* Advance to next in the chain */

                rover = next;
            }
        }

        /* Free the old table */

        jvm_free(old_table);
    }

    return 1;
}

