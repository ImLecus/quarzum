/**
 * \file            hashmap.h
 * \brief           Hashmap include file
 * \copyright       2024 Marcos González GPL3
 */
#ifndef HASHMAP_H
#define HASHMAP_H
#include "core.h"

#define HASH_SEED 0x592AC62F

/**
 * \brief           Defines a key-value pair.
 */
typedef struct {
    char* key;
    void* value;
} pair;

/**
 * \brief           Defines a Hashmap.
 */
typedef struct {
    pair** content;
    uint32_t size, len;
} hashmap_t;

/**
 * \brief           Allocates a new `hashmap_t` on the heap.
 * \warning         Returns `NULL` if the hashmap can not be allocated.
 */
hashmap_t* init_hashmap(unsigned int size);

/**
 * \brief           Frees an allocated `hashmap_t`.
 */
void free_hashmap(hashmap_t* map);

/**
 * \brief           Returns an integer number result of hashing the key.
 * \returns         An integer, the hashed key.
 */
int hash(const char* key, int len, uint32_t seed);

/**
 * \brief           Returns the value linked to the passed key inside a
 *                  `hashmap_t`.
 * \returns         A pointer to any type value. `NULL` if the key-value
 *                  pair is not used or in case of error.
 */
void* hashmap_get(hashmap_t* map, const char* key);

/**
 * \brief           Adds a new element to a `hashmap_t`. 
 * In case of collision, the hashmap will try to add the element
 * in the first place possible, starting from the initial desired index.
 */
void hashmap_add(hashmap_t* map, const char* key, void* value);

#endif