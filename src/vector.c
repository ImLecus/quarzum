#include "quarzum.h"

vector* init_vector(uint32_t size){
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
    if(v->len >= v->size){
        uint32_t new_size = v->size*VECTOR_SIZE_INCREMENT;
        v->value = realloc(v->value,sizeof(void*)*new_size);
        if(!v->value){
            printf("ERROR: vector could not reallocate\n");
        }
        v->size = new_size;
    }
    if(element && v->value){
        v->value[v->len++] = element;
    }
}

inline void vector_pop(vector* v){
    if(v->len > 0){
        v->value[v->len--] = NULL;
    }
}

inline void* vector_get(vector* v, uint32_t index){
    return (index < v->len) ? v->value[index] : NULL;
}