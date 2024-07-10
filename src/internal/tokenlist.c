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

TokenList* createTokenList(unsigned long size){
    TokenList* list = (TokenList*) malloc(sizeof(TokenList));
    list->content = (Token*) malloc(size * sizeof(Token));
    list->size = 0;
    list->capacity = size;
    return list;
} 

void deleteTokenList(TokenList* list){
    free(list->content);
    free(list);
}

void addToTokenList(TokenList* list, Token token){
    if(list->size < list->capacity){
        list->content[list->size++] = token;
        return;
    }
    
}

Token getToken(TokenList* list, unsigned int i){
    return i <= list->size? 
        list->content[i]: ERROR_TOKEN;
}
