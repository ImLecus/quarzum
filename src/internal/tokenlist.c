/*
 * Quarzum Compiler - tokenlist.c
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
#include "../../include/quarzum/tokenlist.h"

TokenList* init_tlist(unsigned int size){
    TokenList* list = (TokenList*) malloc(sizeof(TokenList));
    list->content = (struct token**)malloc(sizeof(struct token*) * size);
    list->size = 0;
    list->capacity = size;
    return list;
} 

inline void free_tlist(TokenList* list){
    free(list->content);
    free(list);
}

void push_tlist(TokenList* list, struct token* t){
    if(list->size < list->capacity){
        list->content[list->size++] = t;
        return;
    }
    // realloc
}

inline struct token* get_token(TokenList* list, unsigned int i){
    return  i <= list->size? 
            list->content[i]: 
            NULL;
}
