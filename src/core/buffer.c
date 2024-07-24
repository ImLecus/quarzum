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
#include "../include/core/buffer.h"
inline Buffer* init_buffer(const u_int64_t size){
    Buffer* b = (Buffer*) malloc(sizeof(Buffer));
    if(b == NULL){
        return NULL;
    }
    b->size = size;
    b->len = 0;
    b->value = (char*) malloc(size + 1);
    if(b->value == NULL){
        free(b);
        return NULL;
    }
    b->value[0] = '\0';
    return b;
}

inline void delete_buffer(Buffer* buffer){
    free(buffer->value);
    free(buffer);
}

inline void add_buffer(Buffer* b, const char c){
    if(b->len >= b->size){
        char* newBuffer = (char*) realloc(b->value,b->size * 2);
        if(newBuffer == NULL){
            return;
        }
        b->size *= 2;
        b->value = newBuffer;
    }
    b->value[b->len++] = c;
    b->value[b->len] = '\0';
}

inline void pop_buffer(Buffer* b){
    b->value[--b->len] = '\0';
}

inline void clear_buffer(Buffer* b){
    for(unsigned long i = 0; b->value[i]; ++i){
        b->value[i] = '\0';
    }
    b->len = 0;
}

inline char* get_buffer(const Buffer *b){
    char* result = (char *) malloc(b->len + 1);
    if(result == NULL){
        return NULL;
    }
    strcpy(result, b->value);
    return result;
}