#include "quarzum.h"

vector* init_vector(unsigned int size){
    vector* vec = malloc(sizeof(vector));
    vec->size = size;
    vec->len = 0;
    vec->value = malloc(sizeof(void*)*size);
    return vec;
}

inline void free_vector(vector* v){
    free(v->value);
    free(v);
}

inline void vector_push(vector* v, void* element){
    if(v->len + 1 >= v->size){
        v->value = (void**)realloc(v->value,sizeof(void*)*v->size*VECTOR_SIZE_INCREMENT);
        v->size *= 2;
    }
    v->value[v->len++] = element;
}

inline void vector_pop(vector* v){
    v->value[v->len--] = NULL;
}

inline void* vector_get(vector* v, unsigned int index){
    return index <= v->len ? v->value[index] : NULL;
}