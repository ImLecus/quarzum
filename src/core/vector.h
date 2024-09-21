/**
 * \file            vector.h
 * \brief           Dynamic array include file
 * \copyright       2024 Marcos González GPL3           
 */
#ifndef VECTOR_H
#define VECTOR_H
#include "core.h"

/**
 * \brief           Defines a vector, a dynamic array of any type.
 */
typedef struct {
    uint32_t size, len;
    void** value;
} vector_t;

/**
 * \brief           Allocates a `vector_t` with a determined size on 
 *                  the heap.
 * \returns         A pointer to a `vector_t` or `NULL` in case of error.
 */
vector_t* init_vector(uint32_t size);

/**
 * \brief           Frees an allocated `vector_t`.
 */
void free_vector(vector_t* v);

/**
 * \brief           Adds a new value at the end of an existing `vector_t`.
 */
void vector_push(vector_t* v, void* element);

/**
 * \brief           Deletes the last value of a `vector_t` without returning it.
 */
void vector_pop(vector_t* v);

/**
 * \brief           Gets the nth element of a `vector_t`.
 * \warning         Returns `NULL` if n is greater than the
 *                  vector length.
 */
void* vector_get(vector_t* v, uint32_t index);

/**
 * \brief           Searches a symbol inside a list with fixed size.
 * Uses the binary search algorithm to search inside a list of elements
 * at O(log2(n)), much faster than a linear search.
 * \note            The list must be sorted.
 * \returns         The index of the element inside the list.
 */
int binary_search(const char* symbol, const char** list, uint32_t size);

#endif
