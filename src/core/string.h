/**
 * \file            string.h
 * \brief           String include file
 */

// LICENSE

#ifndef STRING_H
#define STRING_H

/**
 * \brief           String struct as a dynamic array
 */
typedef struct {
    unsigned int length, size;
    char* content;
} string_t;

string_t* init_string(const unsigned int size);
void free_string(string_t* s);
void string_push(string_t* s, const char c);
void string_pop(string_t* s);
void string_clear(string_t* s);
char* string_copy(string_t* s);
void string_append(string_t* s, char* c);
char* delete_quotes(char* c);

#endif