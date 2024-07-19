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

tlist_t createTokenList(unsigned long size){
    tlist_t list = (tlist_t) malloc(sizeof(TokenList));
    list->content = (Token*) malloc(size * sizeof(Token));
    list->size = 0;
    list->capacity = size;
    return list;
} 

inline void deleteTokenList(tlist_t list){
    free(list->content);
    free(list);
}

void addToTokenList(tlist_t list, Token token){
    if(list->size < list->capacity){
        list->content[list->size++] = token;
        return;
    }
    // realloc
}

inline Token getToken(tlist_t list, unsigned int i){
    return  i <= list->size? 
            list->content[i]: 
            ERROR_TOKEN;
}
