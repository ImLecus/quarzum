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

Buffer* createBuffer(const long size){
    Buffer* b = (Buffer*) malloc(sizeof(Buffer));
    b->size = size;
    b->len = 0;
    b->value = (char*) malloc(size + 1);
    b->value[0] = '\0';
    return b;
}


void deleteBuffer(Buffer* b){
    free(b->value);
    free(b);
}

unsigned char resizeBuffer(Buffer* b, const long newSize){
    char* newBuffer = (char*) realloc(b->value,newSize);
    if(newBuffer == NULL){
        return -1;
    }
    long size = b->size;
    for(unsigned int i = 0; i < size; ++i){
        newBuffer[i] = b->value[i];
    }
    free(b->value);
    b->size = newSize;
    b->value = newBuffer;
    return 0;
}

void addToBuffer(Buffer* b,char c){
    if(b->len >= b->size){
        resizeBuffer(b, b->size * 2);
    }
    b->value[b->len] = c;
    ++(b->len);
    b->value[b->len] = '\0';
}

char popFromBuffer(Buffer* b){
    char c = b->value[b->len - 1];
    b->value[b->len - 1] = '\0';
    --(b->len);
    return c;
}

void clearBuffer(Buffer* b){
    for(unsigned long i = 0; b->value[i]; ++i){
        b->value[i] = '\0';
    }
    b->len = 0;
}

const char* getBuffer(const Buffer *b){
    char* result = (char *) malloc(b->len + 1);
    if(result == NULL){
        // err
        
    }
    strcpy(result, b->value);
    return result;
}