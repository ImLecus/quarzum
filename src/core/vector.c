/**
 * \file            vector.c
 * \brief           Dynamic array implementation for the
 *                  Quarzum compiler
 * \copyright       2024 Marcos González GPL3
 */

#include "vector.h"

Vector* const init_vector(const uint32_t size){
    Vector* vec = malloc(sizeof(Vector));
    if(vec == NULL) return NULL;
    vec->size = size;
    vec->len = 0;
    vec->value = malloc(sizeof(void*)*size);
    return vec;
}

void free_vector(Vector* const v){
    if(v == NULL) return;
    free(v->value);
    free(v);
}

void vector_push(Vector* v,const void* element){
    if(v == NULL) return;
    // Tries to reallocate the Vector if it's full
    if(v->len >= v->size){
        const unsigned int new_size = v->size*2;
        v->value = realloc(v->value,sizeof(void*)*new_size);
        if(!v->value){
            printf("ERROR: Vector could not reallocate\n");
            return;
        }
        v->size = new_size;
    }
    if(element && v->value){
        v->value[v->len++] = element;
    }
}

void vector_pop(Vector* const v){
    if(v != NULL && v->len > 0){
        v->value[v->len--] = NULL;
    }
}

void* vector_get(const Vector* const v, const uint32_t index){
    return (v != NULL && index < v->len) ? v->value[index] : NULL;
}

const int binary_search(const char* symbol, const char* const * list, const uint32_t size){
    unsigned int left = 0;
    unsigned int right = size - 1;
    while(left <= right){
        const int mid = left + (right - left)/2;
        if(mid >= size){
            return -1;
        }
        const int cmp = strcmp(symbol, list[mid]);

        if(cmp == 0){
            return mid;
        }
        if(cmp < 0){
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }
    return -1;
}