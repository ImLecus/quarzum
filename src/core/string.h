/**
 * \file            string.h
 * \brief           String include file
 * \copyright       2024 Marcos González, GPL3
 */

#ifndef STRING_H
#define STRING_H
#include "core.h"

/**
 * \brief           String struct as a dynamic array
 */
typedef struct String {
    uint32_t length, size;
    char* content;
} String;


/**
 * \brief           Allocates a new `String` on the heap.
 * \param           size: the initial size of the string.
 * \returns         A pointer to the allocated `String`.
 */
String* const init_string(const uint32_t size);

/**
 * \brief           Frees an allocated `String`.
 * \note            The function does nothing if s is `NULL`.
 */
void free_string(String* const s);

/**
 * \brief           Adds a character at the end of the string.
 * \note            If the string is full, it will resize and
 *                  duplicate its size.
 */
void string_push(String* const s, const char c);

/**
 * \brief           Eliminates the last character of a string.
 * \note            The function does nothing if s is `NULL`.
 */
void string_pop(String* const s);

/**
 * \brief           Clears the content of a string.
 */
void string_clear(String* const s);

/**
 * \brief           Copies the content of a string.
 * \returns         A pointer to a copy of the string content.
 * \note            The result of this function will not be
 *                  deleted if the original string is freed.
 */
const char* string_copy(const String* const s);

/**
 * \brief           Adds one or more characters to a string.
 */
void string_append(String* const s, const char* c);

/**
 * \brief           Deletes the quotes around a string literal.
 * \warning         The string literal must be sorrounded by double
 *                  quotes ("").
 * \returns         A new char* with the string content without the quotes.
 */
const char* delete_quotes(const char* c);

// Temporal library path
#define LIB_PATH "./lib/"
#define LIB_PATH_LEN 6

/**
 * \brief       Converts a library path into a relative path.
 * A library path starts with a `@`, and can be used as a 
 * substitute of the compiler library path.
 * \return      The complete path using the real library path.
 * \warning     The path must be a libary path (starting with `@`)
 */
const char* resolve_path(const char* c);

#endif /* STRING_H */