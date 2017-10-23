/*

Copyright (c) 2005-2008, Simon Howard

Permission to use, copy, modify, and/or distribute this software 
for any purpose with or without fee is hereby granted, provided 
that the above copyright notice and this permission notice appear 
in all copies. 

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL 
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE 
AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR 
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM 
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, 
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN      
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 

 */

/* Hash table implementation */

#include <stdlib.h>
#include <stdio.h>
#include "phashtable.h"
#include "math.h"


static int HASH_TABLE_DEFAULT_SIZE = 16;

/* Internal function used to allocate the table on hash table creation
 * and when enlarging the table */

static int hash_table_allocate_table(PHashtable *hash_table, unsigned int size) {
    //unsigned int new_table_size = getPHashtableSizeWithIncrement(hash_table, increment);

    /* Allocate the table and initialise to NULL for all entries */
    if (size) {
        hash_table->table = calloc(size *
                                   sizeof(PHashtableEntry *), 1);
        if (hash_table->table)hash_table->table_size = size;
    }

    return hash_table->table != NULL;
}

/* Free an entry, calling the free functions if there are any registered */

static void hash_table_free_entry(PHashtable *hash_table, PHashtableEntry *entry) {
    /* If there is a function registered for freeing keys, use it to free
     * the key */

    if (hash_table->key_free_func != NULL) {
        hash_table->key_free_func(entry->key);
    }

    /* Likewise with the value */

    if (hash_table->value_free_func != NULL) {
        hash_table->value_free_func(entry->value);
    }

    /* Free the data structure */
    free(entry);
}

unsigned long P_DEFAULT_HASH_FUNC(PHashtableKey kmer) {
    return (unsigned long) kmer;
}

int P_DEFAULT_HASH_EQUALS_FUNC(PHashtableValue value1, PHashtableValue value2) {
    return (value1 == value2);
}

PHashtable *phashtable_create(PHashtableHashFunc hash_func,
                              PHashtableEqualFunc equal_func) {
    PHashtable *hash_table;

    /* Allocate a new hash table structure */

    hash_table = (PHashtable *) calloc(sizeof(PHashtable), 1);

    if (hash_table == NULL) {
        return NULL;
    }

    hash_table->hash_func = hash_func;
    hash_table->equal_func = equal_func;
    hash_table->key_free_func = NULL;
    hash_table->value_free_func = NULL;
    hash_table->entries = 0;

    /* Allocate the table */

    if (!hash_table_allocate_table(hash_table, HASH_TABLE_DEFAULT_SIZE)) {
        free(hash_table);

        return NULL;
    }

    return hash_table;
}

void phashtable_destory(PHashtable *hash_table) {
    PHashtableEntry *rover;
    PHashtableEntry *next;
    unsigned int i;

    /* Free all entries in all chains */

    for (i = 0; i < hash_table->table_size; ++i) {
        rover = hash_table->table[i];
        while (rover != NULL) {
            next = rover->next;
            hash_table_free_entry(hash_table, rover);
            rover = next;
        }
    }

    /* Free the table */

    free(hash_table->table);

    /* Free the hash table structure */

    free(hash_table);
}


void phashtable_clear(PHashtable *hash_table) {
    PHashtableEntry *rover;
    PHashtableEntry *next;
    unsigned int i;

    for (i = 0; i < hash_table->table_size; ++i) {
        rover = hash_table->table[i];
        while (rover != NULL) {
            next = rover->next;
            hash_table_free_entry(hash_table, rover);
            rover = next;
        }
        hash_table->table[i] = NULL;
    }
    hash_table->entries = 0;
    if (hash_table->table_size > HASH_TABLE_DEFAULT_SIZE) {
        free(hash_table->table);
        hash_table->table = NULL;
        hash_table->table_size = 0;
        if (!hash_table_allocate_table(hash_table, HASH_TABLE_DEFAULT_SIZE)) {
            free(hash_table);
        }
    }
}

void phashtable_register_free_functions(PHashtable *hash_table,
                                        PHashtableKeyFreeFunc key_free_func,
                                        PHashtableValueFreeFunc value_free_func) {
    hash_table->key_free_func = key_free_func;
    hash_table->value_free_func = value_free_func;
}

int phashtable_put(PHashtable *hash_table, PHashtableKey key, PHashtableValue value) {
    PHashtableEntry *rover;
    PHashtableEntry *newentry;
    unsigned int index;

    /* If there are too many items in the table with respect to the table
     * size, the number of hash collisions increases and performance
     * decreases. Enlarge the table size to prevent this happening */

    if ((hash_table->entries << 1) >= hash_table->table_size) {
        /* Table is more than 1/2 full */
        if (!phashtable_resize(hash_table, hash_table->table_size << 1)) {

            /* Failed to enlarge the table */
            return 0;
        }
    }

    /* Generate the hash of the key and hence the index into the table */

    index = hash_table->hash_func(key) % hash_table->table_size;

    /* Traverse the chain at this location and look for an existing
     * entry with the same key */

    rover = hash_table->table[index];

    while (rover != NULL) {
        if (hash_table->equal_func(rover->key, key) != 0) {

            /* Same key: overwrite this entry with new data */

            /* If there is a value free function, free the old data
             * before adding in the new data */

            if (hash_table->value_free_func != NULL) {
                hash_table->value_free_func(rover->value);
            }

            /* Same with the key: use the new key value and free 
             * the old one */

            if (hash_table->key_free_func != NULL) {
                hash_table->key_free_func(rover->key);
            }

            rover->key = key;
            rover->value = value;

            /* Finished */

            return 1;
        }
        rover = rover->next;
    }

    /* Not in the hash table yet.  Create a new entry */

    newentry = (PHashtableEntry *) calloc(sizeof(PHashtableEntry), 1);

    if (newentry == NULL) {
        return 0;
    }

    newentry->key = key;
    newentry->value = value;

    /* Link into the list */

    newentry->next = hash_table->table[index];
    hash_table->table[index] = newentry;

    /* Maintain the count of the number of entries */

    ++hash_table->entries;

    /* Added successfully */

    return 1;
}

PHashtableValue phashtable_get(PHashtable *hash_table, PHashtableKey key) {
    PHashtableEntry *rover;
    unsigned int index;

    /* Generate the hash of the key and hence the index into the table */

    index = hash_table->hash_func(key) % hash_table->table_size;

    /* Walk the chain at this index until the corresponding entry is
     * found */

    rover = hash_table->table[index];

    while (rover != NULL) {
        if (hash_table->equal_func(key, rover->key) != 0) {

            /* Found the entry.  Return the data. */

            return rover->value;
        }
        rover = rover->next;
    }

    /* Not found */

    return HASH_NULL;
}

int phashtable_remove(PHashtable *hash_table, PHashtableKey key, int resize) {
    PHashtableEntry *rover;
    PHashtableEntry *pre;
    PHashtableEntry *next;
    unsigned int index;
    unsigned int result;

    if (resize && (hash_table->entries << 3) < hash_table->table_size) {
        if (!phashtable_resize(hash_table, hash_table->table_size >> 1)) {
            return 0;
        }
    }
    index = hash_table->hash_func(key) % hash_table->table_size;

    result = 0;
    rover = hash_table->table[index];
    pre = rover;

    while (rover != NULL) {
        next = rover->next;
        if (hash_table->equal_func(key, rover->key) != 0) {
            if (pre == rover)hash_table->table[index] = next;
            else pre->next = next;
            hash_table_free_entry(hash_table, rover);
            --hash_table->entries;
            result = 1;
            break;
        }
        pre = rover;
        rover = next;
    }
    return result;
}


unsigned int phashtable_num_entries(PHashtable *hash_table) {
    return hash_table->entries;
}

void phashtable_iterate(PHashtable *hash_table, PHashtableIterator *iterator) {
    unsigned int chain;

    iterator->hash_table = hash_table;

    /* Default value of next if no entries are found. */

    iterator->next_entry = NULL;

    /* Find the first entry */

    for (chain = 0; chain < hash_table->table_size; ++chain) {

        if (hash_table->table[chain] != NULL) {
            iterator->next_entry = hash_table->table[chain];
            iterator->next_chain = chain;
            break;
        }
    }
}

int phashtable_iter_has_more(PHashtableIterator *iterator) {
    return iterator->next_entry != NULL;
}

PHashtableValue phashtable_iter_next(PHashtableIterator *iterator) {
    PHashtableEntry *current_entry;
    PHashtable *hash_table;
    PHashtableValue result;
    unsigned int chain;

    hash_table = iterator->hash_table;

    /* No more entries? */

    if (iterator->next_entry == NULL) {
        return HASH_NULL;
    }

    /* Result is immediately available */

    current_entry = iterator->next_entry;
    result = current_entry->value;

    /* Find the next entry */

    if (current_entry->next != NULL) {

        /* Next entry in current chain */

        iterator->next_entry = current_entry->next;

    } else {

        /* None left in this chain, so advance to the next chain */

        chain = iterator->next_chain + 1;

        /* Default value if no next chain found */

        iterator->next_entry = NULL;

        while (chain < hash_table->table_size) {

            /* Is there anything in this chain? */

            if (hash_table->table[chain] != NULL) {
                iterator->next_entry = hash_table->table[chain];
                break;
            }

            /* Try the next chain */

            ++chain;
        }

        iterator->next_chain = chain;
    }

    return result;
}

PHashtableKey phashtable_iter_next_key(PHashtableIterator *iterator) {
    PHashtableEntry *current_entry;
    PHashtable *hash_table;
    PHashtableKey result;
    unsigned int chain;

    hash_table = iterator->hash_table;

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

        while (chain < hash_table->table_size) {

            /* Is there anything in this chain? */

            if (hash_table->table[chain] != NULL) {
                iterator->next_entry = hash_table->table[chain];
                break;
            }

            /* Try the next chain */

            ++chain;
        }

        iterator->next_chain = chain;
    }

    return result;
}

int phashtable_resize(PHashtable *hash_table, unsigned int size) {
    PHashtableEntry **old_table;
    unsigned int old_table_size;
    PHashtableEntry *rover;
    PHashtableEntry *next;
    unsigned int index;
    unsigned int i;


    if (size) {
        /* Store a copy of the old table */
        old_table = hash_table->table;
        old_table_size = hash_table->table_size;

        if (!hash_table_allocate_table(hash_table, size)) {

            /* Failed to allocate the new table */

            printf("CRITICAL: FAILED TO ALLOCATE HASH TABLE!\n");

            hash_table->table = old_table;
            hash_table->table_size = old_table_size;

            return 0;
        }

        /* Link all entries from all chains into the new table */

        for (i = 0; i < old_table_size; ++i) {
            rover = old_table[i];

            while (rover != NULL) {
                next = rover->next;

                /* Find the index into the new table */

                index = hash_table->hash_func(rover->key) % hash_table->table_size;

                /* Link this entry into the chain */

                rover->next = hash_table->table[index];
                hash_table->table[index] = rover;

                /* Advance to next in the chain */

                rover = next;
            }
        }

        /* Free the old table */

        free(old_table);
    }

    return 1;
}

