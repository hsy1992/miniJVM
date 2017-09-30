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

/**
 * @file hash-table.h
 *
 * @brief Hash table.
 *
 * A hash table stores a set of values which can be addressed by a 
 * key.  Given the key, the corresponding value can be looked up
 * quickly.
 *
 * To create a hash table, use @ref hash_table_new.  To destroy a 
 * hash table, use @ref hash_table_free.
 *
 * To insert a value into a hash table, use @ref hash_table_insert.
 *
 * To remove a value from a hash table, use @ref hash_table_remove.
 *
 * To look up a value by its key, use @ref hash_table_lookup.
 *
 * To iterate over all values in a hash table, use 
 * @ref hash_table_iterate to initialise a @ref PHashtableIterator
 * structure.  Each value can then be read in turn using 
 * @ref hash_table_iter_next and @ref hash_table_iter_has_more.
 */

#ifndef ALGORITHM_HASH_TABLE_H
#define ALGORITHM_HASH_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * A hash table structure.
 */

typedef struct _PHashtable PHashtable;

typedef PHashtable *hmap_t;
/**
 * Structure used to iterate over a hash table.
 */

typedef struct _PHashtableIterator PHashtableIterator;

/**
 * Internal structure representing an entry in a hash table.
 */

typedef struct _PHashtableEntry PHashtableEntry;

/**
 * A key to look up a value in a @ref PHashtable.
 */

typedef void *PHashtableKey;

/**
 * A value stored in a @ref PHashtable.
 */

typedef void *PHashtableValue;

/**
 * Definition of a @ref PHashtableIterator.
 */

struct _PHashtableIterator {
    PHashtable *hash_table;
    PHashtableEntry *next_entry;
    int next_chain;
};

/**
 * A null @ref PHashtableValue.
 */
#ifndef HASH_NULL
#define HASH_NULL ((void *) 0)
#endif

/**
 * Hash function used to generate hash values for keys used in a hash
 * table.
 *
 * @param value  The value to generate a hash value for.
 * @return       The hash value.
 */

typedef unsigned long (*PHashtableHashFunc)(PHashtableKey value);

/**
 * Function used to compare two keys for equality.
 *
 * @return   Non-zero if the two keys are equal, zero if the keys are
 *           not equal.
 */

typedef int (*PHashtableEqualFunc)(PHashtableKey value1, PHashtableKey value2);

/**
 * Type of function used to free keys when entries are removed from a
 * hash table.
 */

typedef void (*PHashtableKeyFreeFunc)(PHashtableKey value);

/**
 * Type of function used to free values when entries are removed from a
 * hash table.
 */

typedef void (*PHashtableValueFreeFunc)(PHashtableValue value);

struct _PHashtableEntry {
    PHashtableKey key;
    PHashtableValue value;
    PHashtableEntry *next;
};

struct _PHashtable {
    PHashtableEntry **table;
    unsigned long long int table_size;
    PHashtableHashFunc hash_func;
    PHashtableEqualFunc equal_func;
    PHashtableKeyFreeFunc key_free_func;
    PHashtableValueFreeFunc value_free_func;
    unsigned long long int entries;
};

/**
 * Create a new hash table.
 *
 * @param hash_func            Function used to generate hash keys for the
 *                             keys used in the table.
 * @param equal_func           Function used to test keys used in the table
 *                             for equality.
 * @return                     A new hash table structure, or NULL if it
 *                             was not possible to allocate the new hash
 *                             table.
 */

PHashtable *phashtable_create(PHashtableHashFunc hash_func,
                            PHashtableEqualFunc equal_func);


unsigned long P_DEFAULT_HASH_FUNC(PHashtableKey kmer);

int P_DEFAULT_HASH_EQUALS_FUNC(PHashtableValue value1, PHashtableValue value2);

/**
 * Destroy a hash table.
 *
 * @param hash_table           The hash table to destroy.
 */

void phashtable_destory(PHashtable *hash_table);

void phashtable_clear(PHashtable *hash_table);

/**
 * Register functions used to free the key and value when an entry is
 * removed from a hash table.
 *
 * @param hash_table           The hash table.
 * @param key_free_func        Function used to free keys.
 * @param value_free_func      Function used to free values.
 */

void phashtable_register_free_functions(PHashtable *hash_table,
                                       PHashtableKeyFreeFunc key_free_func,
                                       PHashtableValueFreeFunc value_free_func);

/**
 * Insert a value into a hash table, overwriting any existing entry
 * using the same key.
 *
 * @param hash_table           The hash table.
 * @param key                  The key for the new value.
 * @param value                The value to insert.
 * @return                     Non-zero if the value was added successfully,
 *                             or zero if it was not possible to allocate
 *                             memory for the new entry.
 */

int phashtable_put(PHashtable *hash_table,
                  PHashtableKey key,
                  PHashtableValue value);

/**
 * Look up a value in a hash table by key.
 *
 * @param hash_table          The hash table.
 * @param key                 The key of the value to look up.
 * @return                    The value, or @ref HASH_TABLE_NULL if there
 *                            is no value with that key in the hash table.
 */

PHashtableValue phashtable_get(PHashtable *hash_table,
                             PHashtableKey key);

/**
 * Remove a value from a hash table.
 *
 * @param hash_table          The hash table.
 * @param key                 The key of the value to remove.
 * @return                    Non-zero if a key was removed, or zero if the
 *                            specified key was not found in the hash table.
 */

int phashtable_remove(PHashtable *hash_table, PHashtableKey key, int resize);

/**
 * Retrieve the number of entries in a hash table.
 *
 * @param hash_table          The hash table.
 * @return                    The number of entries in the hash table.
 */

unsigned long long int phashtable_num_entries(PHashtable *hash_table);

/**
 * Initialise a @ref PHashtableIterator to iterate over a hash table.
 *
 * @param hash_table          The hash table.
 * @param iter                Pointer to an iterator structure to
 *                            initialise.
 */

void phashtable_iterate(PHashtable *hash_table, PHashtableIterator *iter);

/**
 * Determine if there are more keys in the hash table to iterate
 * over.
 *
 * @param iterator            The hash table iterator.
 * @return                    Zero if there are no more values to iterate
 *                            over, non-zero if there are more values to
 *                            iterate over.
 */

int phashtable_iter_has_more(PHashtableIterator *iterator);

/**
 * Using a hash table iterator, retrieve the next key.
 *
 * @param iterator            The hash table iterator.
 * @return                    The next key from the hash table, or
 *                            @ref HASH_TABLE_NULL if there are no more
 *                            keys to iterate over.
 */

PHashtableValue phashtable_iter_next(PHashtableIterator *iterator);

/**
 * (EXTENSION!)
 *
 * Using a hash table iterator, retrieve the next key.
 *
 * @param iterator            The hash table iterator.
 * @return                    The next key from the hash table, or
 *                            @ref HASH_TABLE_NULL if there are no more
 *                            keys to iterate over.
 */

PHashtableKey phashtable_iter_next_key(PHashtableIterator *iterator);

int phashtable_resize(PHashtable *hash_table, unsigned long long int size);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ALGORITHM_HASH_TABLE_H */

