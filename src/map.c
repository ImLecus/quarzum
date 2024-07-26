#include "quarzum.h"

map* init_map(unsigned int size){
    map* m = (map*)malloc(sizeof(map));
    m->keys = (char**)malloc(size * sizeof(char*));
    m->values = (int*)malloc(size * sizeof(int));

    m->size = size;
    return m;
}

