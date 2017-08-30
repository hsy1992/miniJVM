//
// Created by gust on 2017/8/30.
//
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include "../jvm/garbage.h"
#include "hashset.h"


/* This is a set of good hash table prime numbers, from:
 *   http://planetmath.org/encyclopedia/GoodHashsetPrimes.html
 * Each prime is roughly double the previous value, and as far as
 * possible from the nearest powers of two.
 *
 * Note: Primes after 1610612741 are added by finding the nearest prime to
 *       previous prime multiplied by 2.
 */

static const unsigned long long int hashset_primes[] = {
        4UL, 8UL, 16UL, 32UL, 64UL, 128UL, 256UL, 389UL, 769UL, 1543UL, 3079UL, 6151UL, 12289UL,
        24593UL, 49157UL, 98317UL, 196613UL, 393241UL, 786433UL, 1572869UL, 3145739UL,
        6291469UL, 12582917UL, 25165843UL, 50331653UL, 100663319UL, 201326611UL,
        402653189UL, 805306457UL, 1610612741UL, 3221225479UL, 6442450967UL,
        12884901947UL, 25769803897UL, 51539607793UL, 103079215583UL, 206158431161UL,
};

static const int hash_table_num_primes
        = sizeof(hashset_primes) / sizeof(unsigned long long int);

unsigned long long int getHashsetSizeWithIncrement(Hashset *set, int increment) {
    unsigned long long int size = hashset_primes[0];

    // Target size is within range:
    if (set->prime_index + increment < hash_table_num_primes) {
        size = hashset_primes[set->prime_index + increment];
    }        // Target size not in range:
    else {
        // Current in range, target not in range:
        if (set->prime_index < hash_table_num_primes) {
            int diff = set->prime_index + increment - (hash_table_num_primes - 1);

            size = hashset_primes[hash_table_num_primes - 1];
            size = size * pow(2, diff);
        }            // Current is outside, target is outside; current < target:
        else if (set->prime_index < set->prime_index + increment) {
            size = size * pow(2, increment);
        }            // Current is outside, target is outside; current > target:
        else if (set->prime_index > set->prime_index + increment) {
            size = size / pow(2, increment);
        }
    }

    return size;
}

/* Internal function used to allocate the table on hash table creation
 * and when enlarging the table */

static int hash_table_allocate_table(Hashset *set, int increment) {
    unsigned long long int new_table_size = getHashsetSizeWithIncrement(set, increment);

    // Update:
    set->table_size = new_table_size;
    set->prime_index = set->prime_index + increment;

    /* Allocate the table and initialise to NULL for all entries */
    set->table = jvm_alloc(set->table_size *
                           sizeof(HashsetEntry *));

    return set->table != NULL;
}

/* Free an entry, calling the free functions if there are any registered */

static void hash_table_free_entry(Hashset *set, HashsetEntry *entry) {
    /* If there is a function registered for freeing keys, use it to free
     * the key */

    if (set->key_free_func != NULL) {
        set->key_free_func(entry->key);
    }

    /* Free the data structure */

    jvm_free(entry);
}

unsigned long _DEFAULT_HashsetHash(HashsetKey kmer) {
    return (unsigned long) kmer;
}


Hashset *hashset_create(HashtableHashFunc hash_func,
                        HashtableEqualFunc equal_func) {
    Hashset *set;

    /* Allocate a new hash table structure */

    set = (Hashset *) jvm_alloc(sizeof(Hashset));

    if (set == NULL) {
        return NULL;
    }

    set->hash_func = hash_func;
    set->equal_func = equal_func;
    set->key_free_func = NULL;
    set->entries = 0;
    set->prime_index = 0;

    /* Allocate the table */

    if (!hash_table_allocate_table(set, 0)) {
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
            hash_table_free_entry(set, rover);
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
            hash_table_free_entry(set, rover);
            rover = next;
        }
    }
    set->entries = 0;
    set->prime_index = 0;
    if (set->table_size > hashset_primes[set->prime_index]) {
        jvm_free(set->table);
        if (!hash_table_allocate_table(set, 0)) {
            jvm_free(set);
        }
    }
}

void hashset_register_free_functions(Hashset *set,
                                     HashtableKeyFreeFunc key_free_func) {
    set->key_free_func = key_free_func;
}

int hashset_put(Hashset *set, HashsetKey key) {
    HashsetEntry *rover;
    HashsetEntry *newentry;
    unsigned long long int index;

    /* If there are too many items in the table with respect to the table
     * size, the number of hash collisions increases and performance
     * decreases. Enlarge the table size to prevent this happening */

    if ((set->entries * 2) / set->table_size > 0) {

        /* Table is more than 1/2 full */

        if (!hashset_resize(set)) {

            /* Failed to enlarge the table */

            return 0;
        }
    }

    /* Generate the hash of the key and hence the index into the table */

    index = set->hash_func(key) % set->table_size;

    /* Traverse the chain at this location and look for an existing
     * entry with the same key */

    rover = set->table[index];

    while (rover != NULL) {
        if (set->equal_func(rover->key, key) != 0) {

            /* Same key: overwrite this entry with new data */
            /* Same with the key: use the new key value and free 
             * the old one */

            if (set->key_free_func != NULL) {
                set->key_free_func(rover->key);
            }

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

    index = set->hash_func(key) % set->table_size;

    /* Walk the chain at this index until the corresponding entry is
     * found */

    rover = set->table[index];

    while (rover != NULL) {
        if (set->equal_func(key, rover->key) != 0) {

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

    if (resize && (set->entries * 8) / set->table_size <= 0) {

        /* Table is less than 1/8 full */

        if (!hashset_resize(set)) {

            /* Failed to enlarge the table */

            return 0;
        }
    }

    /* Generate the hash of the key and hence the index into the table */

    index = set->hash_func(key) % set->table_size;

    /* Rover points at the pointer which points at the current entry
     * in the chain being inspected.  ie. the entry in the table, or
     * the "next" pointer of the previous entry in the chain.  This
     * allows us to unlink the entry when we find it. */

    result = 0;
    rover = &set->table[index];

    while (*rover != NULL) {

        if (set->equal_func(key, (*rover)->key) != 0) {

            /* This is the entry to remove */

            entry = *rover;

            /* Unlink from the list */

            *rover = entry->next;

            /* Destroy the entry structure */

            hash_table_free_entry(set, entry);

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

int hashset_resize(Hashset *set) {
    HashsetEntry **old_table;
    unsigned long long int old_table_size;
    int old_prime_index;
    HashsetEntry *rover;
    HashsetEntry *next;
    unsigned long long int index;
    unsigned long long int i;

    int increment = 0;

    // GROW
    if ((set->entries * 2) / set->table_size > 0) {
        do {
            /* Table is more than 1/2 full */
            increment++;

        } while ((set->entries * 2) / getHashsetSizeWithIncrement(set, increment) > 0);
    }
        // SHRINK
    else if ((set->entries * 8) / set->table_size == 0) {
        do {
            // Are we already as small as possible?
            if (getHashsetSizeWithIncrement(set, increment) <= hashset_primes[0]) {
                break;
            }

            /* Table is less than 1/8 full */
            increment--;
        } while ((set->entries * 8) / getHashsetSizeWithIncrement(set, increment) <= 0);
    }

    if (increment != 0) {
        /* Store a copy of the old table */
        old_table = set->table;
        old_table_size = set->table_size;
        old_prime_index = set->prime_index;

        if (!hash_table_allocate_table(set, increment)) {

            /* Failed to allocate the new table */

            printf("CRITICAL: FAILED TO ALLOCATE HASH TABLE!\n");

            set->table = old_table;
            set->table_size = old_table_size;
            set->prime_index = old_prime_index;

            return 0;
        }

        /* Link all entries from all chains into the new table */

        for (i = 0; i < old_table_size; ++i) {
            rover = old_table[i];

            while (rover != NULL) {
                next = rover->next;

                /* Find the index into the new table */

                index = set->hash_func(rover->key) % set->table_size;

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

