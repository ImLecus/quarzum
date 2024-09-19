/**
 * \file            vector.c
 * \brief           Dynamic array implementation for the
 *                  Quarzum compiler
 * \copyright       2024 Marcos González GPL3
 */

#include "vector.h"

vector* init_vector(unsigned int size){
    vector* vec = malloc(sizeof(vector));
    if(vec == NULL) return NULL;
    vec->size = size;
    vec->len = 0;
    vec->value = malloc(sizeof(void*)*size);
    return vec;
}

void free_vector(vector* v){
    if(v == NULL) return;
    free(v->value);
    free(v);
}

void vector_push(vector* v, void* element){
    if(v == NULL) return;
    // Tries to reallocate the vector if it's full
    if(v->len >= v->size){
        unsigned int new_size = v->size*2;
        v->value = realloc(v->value,sizeof(void*)*new_size);
        if(!v->value){
            printf("ERROR: vector could not reallocate\n");
            return;
        }
        v->size = new_size;
    }
    if(element && v->value){
        v->value[v->len++] = element;
    }
}

void vector_pop(vector* v){
    if(v != NULL && v->len > 0){
        v->value[v->len--] = NULL;
    }
}

void* vector_get(vector* v, unsigned int index){
    return (v != NULL && index < v->len) ? v->value[index] : NULL;
}

int binary_search(const char* symbol, const char** list, unsigned int size){
    int left = 0;
    int right = size - 1;
    while(left <= right){
        int mid = left + (right - left)/2;
        if(mid >= size){
            return -1;
        }
        int cmp = strcmp(symbol, list[mid]);

        if(cmp == 0){
            return mid;
        }
        else if(cmp < 0){
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }
    return -1;
}