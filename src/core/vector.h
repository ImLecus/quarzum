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
    unsigned int size, len;
    void** value;
} vector;

/**
 * \brief           Allocates a `vector_t` with a determined size on 
 *                  the heap.
 * \returns         A pointer to a `vector_t` or `NULL` in case of error.
 */
vector* init_vector(unsigned int size);

/**
 * \brief           Frees an allocated `vector_t`.
 */
void free_vector(vector* v);

/**
 * \brief           Adds a new value at the end of an existing `vector_t`.
 */
void vector_push(vector* v, void* element);

/**
 * \brief           Deletes the last value of a `vector_t` without returning it.
 */
void vector_pop(vector* v);

/**
 * \brief           Gets the nth element of a `vector_t`.
 * \warning         Returns `NULL` if n is greater than the
 *                  vector length.
 */
void* vector_get(vector* v, unsigned int index);

/**
 * \brief           Searches a symbol inside a list with fixed size.
 * Uses the binary search algorithm to search inside a list of elements
 * at O(log2(n)), much faster than a linear search.
 * \note            The list must be sorted.
 * \returns         The index of the element inside the list.
 */
int binary_search(const char* symbol, const char** list, unsigned int size);

#endif
