#include "quarzum.h"

hashmap* init_hashmap(uint32_t size){
    hashmap* map = malloc(sizeof(hashmap));
    map->size = size;
    map->len =0;
    map->content = malloc(size * sizeof(bucket*));
    for(uint32_t i = 0; i < map->size; ++i){
        map->content[i] = NULL;
    }
    return map;
}

void free_hashmap(hashmap* map){
    free(map->content);
    free(map);
}

int hash(char* key){
    unsigned long hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (unsigned long)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

void* hashmap_get(hashmap* map, char* key){
    int hashed_key = hash(key)%map->size;
    if(map->content[hashed_key] != NULL){
        if(strcmp(map->content[hashed_key]->key, key) != 0){
            for(uint32_t i = hashed_key; i < map->size; ++i){
                if(map->content[i] != NULL && strcmp(map->content[i]->key, key) == 0){
                    return map->content[i]->value;
                }
            }
            for(uint32_t i = 0; i < hashed_key; ++i){
                if(map->content[i] != NULL && strcmp(map->content[i]->key, key) == 0){
                    return map->content[i]->value;
                }
            }
            return NULL;
        }
        return map->content[hashed_key]->value;
    }
    return NULL;
}

void hashmap_add(hashmap* map, char* key, void* value){
    if(map->len >= map->size){
        bucket** new_content = realloc(map->content, sizeof(bucket*) * map->size * 2);
        if(!new_content){
            printf("Error in realloc");
            return;
        }
        map->size *= 2;
        map->content = new_content;
    }
    int hashed_key = hash(key)%map->size; 

    if(map->content[hashed_key] != NULL){
        int has_key = 0;
        for(uint32_t i = hashed_key; i < map->size; ++i){
            if(map->content[i] == NULL){
                hashed_key = i;
                has_key = 1;
                break;
            }
        }
        if(!has_key){
            for(uint32_t i = 0; i < hashed_key; ++i){
                if(map->content[i] == NULL){
                    hashed_key = i;
                    break;
                }
            }
        }
        
    }
    bucket* b = malloc(sizeof(bucket));
    b->key = key;
    b->value = value;
    map->content[hashed_key] = b;
    ++map->len;
    //printf("added %s on index %d\n\n", key, hashed_key);
}