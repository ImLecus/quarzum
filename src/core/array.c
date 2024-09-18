#include "array.h"

Array* init_array(unsigned int size){
    Array* array = malloc(sizeof(Array));
    if(!array) return NULL;
    array->size = size;
    array->length = 0;
    array->content = malloc(sizeof(void*) * size);
    if(!array->content) free(array->content); return NULL;
    array->push = array_push;
}