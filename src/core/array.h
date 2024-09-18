// File: array.h
// Description: Dynamic array implementation for the Quarzum compiler
#ifndef ARRAY_H
#define ARRAY_H
#include <memory.h>
typedef struct {
    unsigned int length;
    unsigned int size;
    void** content;

    void (*push)(void* element);
    void (*pop)();
    void* (*get)(unsigned int index);
} Array;

Array* init_array(unsigned int size);

void array_push(void* element);

#endif