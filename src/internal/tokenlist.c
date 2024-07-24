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

tlist_t* init_tlist(u_int64_t size){
    tlist_t* list = (tlist_t*) malloc(sizeof(TokenList));
    list->content = _create_array(Token*, size);
    list->size = 0;
    list->capacity = size;
    return list;
} 

inline void delete_tlist(tlist_t* list){
    free(list->content);
    free(list);
}

void push_tlist(tlist_t* list, Token* token){
    if(list->size < list->capacity){
        list->content[list->size++] = token;
        return;
    }
    // realloc
}

inline Token* get_token(tlist_t* list, u_int32_t i){
    return  i <= list->size? 
            list->content[i]: 
            &ERROR_TOKEN;
}
