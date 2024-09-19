/**
 * \file            hashmap.c
 * \brief           Hashmap implementation for the Quarzum compiler
 * \copyright       2024 Marcos González GPL3
 */
#include "hashmap.h"

hashmap_t* init_hashmap(unsigned int size){
    hashmap_t* map = malloc(sizeof(hashmap_t));
    if(map == NULL) return NULL;
    map->size = size;
    map->len =0;
    map->content = malloc(size * sizeof(pair*));
    for(unsigned int i = 0; i < map->size; ++i){
        map->content[i] = NULL;
    }
    return map;
}

void free_hashmap(hashmap_t* map){
    if(map == NULL) return;
    free(map->content);
    free(map);
}

/**
 * \details         This function uses the MurmurHash2 hash
 *                  algorithm.
 */
int hash(char* key, int len, uint32_t seed){
    const uint32_t m = 0x5bd1e995;
    const int r = 24;
    uint32_t h = seed ^ len;
    const unsigned char *data = (const unsigned char *)key;

    while (len >= 4) {
        uint32_t k = *(uint32_t *)data;
        k *= m;
        k ^= k >> r;
        k *= m;
        h *= m;
        h ^= k;
        data += 4;
        len -= 4;
    }

    switch (len) {
        case 3: h ^= data[2] << 16;
        case 2: h ^= data[1] << 8;
        case 1: h ^= data[0];
                h *= m;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}

void* hashmap_get(hashmap_t* map, char* key){
    int hashed_key = hash(key, strlen(key), HASH_SEED)%map->size;
    if(map->content[hashed_key] != NULL){
        if(strcmp(map->content[hashed_key]->key, key) != 0){
            for(unsigned int i = hashed_key; i < map->size; ++i){
                if(map->content[i] != NULL && strcmp(map->content[i]->key, key) == 0){
                    return map->content[i]->value;
                }
            }
            for(unsigned int i = 0; i < hashed_key; ++i){
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

void hashmap_add(hashmap_t* map, char* key, void* value){
    if(map->len >= map->size){
        pair** new_content = realloc(map->content, sizeof(pair*) * map->size * 2);
        if(!new_content){
            printf("Error in realloc");
            return;
        }
        map->size *= 2;
        map->content = new_content;
        for(unsigned int i = map->size/2; i < map->size; ++i){
            map->content[i] = NULL;
        }   
    }
    int hashed_key = hash(key, strlen(key), HASH_SEED)%map->size; 

    if(map->content[hashed_key] != NULL){
        uint32_t start = hashed_key;
        do{
            hashed_key = (hashed_key + 1)%map->size;
        } while(map->content[hashed_key] && hashed_key != start);
    }
    pair* b = malloc(sizeof(pair));
    b->key = key;
    b->value = value;
    map->content[hashed_key] = b;
    ++map->len;
}