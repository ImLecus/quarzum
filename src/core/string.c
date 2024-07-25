/*
 * Quarzum Compiler - buffer.c
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
#include "../include/core/string.h"
inline string* init_string(const unsigned long size){
    string* s = (struct string*) malloc(sizeof(string));
    if(s == NULL){
        return NULL;
    }
    s->size = size;
    s->len = 0;
    s->value = (char*) malloc(size + 1);
    if(s->value == NULL){
        free(s);
        return NULL;
    }
    s->value[0] = '\0';
    return s;
}

inline void free_string(string* s){
    free(s->value);
    free(s);
}

inline void string_push(string* s, const char c){
    if(s->len >= s->size){
        char* newBuffer = (char*) realloc(s->value,s->size * 2);
        if(newBuffer == NULL){
            return;
        }
        s->size *= 2;
        s->value = newBuffer;
    }
    s->value[s->len++] = c;
    s->value[s->len] = '\0';
}

inline void string_pop(string* s){
    s->value[--s->len] = '\0';
}

inline void string_clear(string* s){
    for(unsigned int i = 0; s->value[i]; ++i){
        s->value[i] = '\0';
    }
    s->len = 0;
}

inline char* string_copy(string* s){
    char* result = (char *) malloc(s->len + 1);
    if(result == NULL){
        return NULL;
    }
    strcpy(result, s->value);
    return result;
}