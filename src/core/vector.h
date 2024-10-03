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
typedef struct Vector {
    uint32_t size, len;
    void** value;
} Vector;

/**
 * \brief           Allocates a `Vector` with a determined size on 
 *                  the heap.
 * \returns         A pointer to a `Vector` or `NULL` in case of error.
 */
Vector* const init_vector(const uint32_t size);

/**
 * \brief           Frees an allocated `Vector`.
 */
void free_vector(Vector* const v);

/**
 * \brief           Adds a new value at the end of an existing `Vector`.
 */
void vector_push(Vector* v, const void* element);

/**
 * \brief           Deletes the last value of a `Vector` without returning it.
 */
void vector_pop(Vector* const const v);

/**
 * \brief           Gets the nth element of a `Vector`.
 * \warning         Returns `NULL` if n is greater than the
 *                  vector length.
 */
void* vector_get(const Vector* const v,const uint32_t index);

/**
 * \brief           Searches a symbol inside a list with fixed size.
 * Uses the binary search algorithm to search inside a list of elements
 * at O(log2(n)), much faster than a linear search.
 * \note            The list must be sorted.
 * \returns         The index of the element inside the list.
 */
const int binary_search(const char* symbol, const char* const * list, uint32_t size);

#endif
