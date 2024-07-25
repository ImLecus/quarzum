/*
 * Quarzum Compiler - buffer.h
 * Version 1.0, 06/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#ifndef STRING_H
#define STRING_H
#include <string.h>
#include <stdlib.h>

typedef struct {
    unsigned long size;
    unsigned long len;
    char* value;
} string;

string* init_string(const unsigned long size);

void free_string(string* s);

void string_push(string* s, const char c);

void string_pop(string* s);

void string_clear(string* s);

char* string_copy(string* s);


#endif