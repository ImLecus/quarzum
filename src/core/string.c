/**
 * \file            string.c
 * \brief           String implementation for the Quarzum compiler
 * \copyright       2024 Marcos González GPL3
 */
#include "string.h"

inline string_t* init_string(const uint32_t size){
    string_t* s = malloc(sizeof(string_t));
    if(s == NULL) return NULL;
    s->size = size;
    s->length = 0;
    s->content = malloc(size + 1);
    if(s->content == NULL){
        free(s);
        return NULL;
    }
    s->content[0] = 0;
    return s;
}

inline void free_string(string_t* s){
    if(s == NULL) return;
    free(s->content);
    free(s);
}

inline void string_push(string_t* s, const char c){
    if(s->length >= s->size){
        char* newBuffer = realloc(s->content,s->size * 2);
        if(newBuffer == NULL){
            return;
        }
        s->size *= 2;
        s->content = newBuffer;
    }
    s->content[s->length++] = c;
    s->content[s->length] = '\0';
}

inline void string_pop(string_t* s){
    if(s->length == 0) return;
    s->content[--s->length] = '\0';
}

inline void string_clear(string_t* s){
    memset(s->content, '\0', s->size);
    s->length = 0;
}

inline char* string_copy(string_t* s){
    char* result = malloc(s->length + 1);
    if(result == NULL) return NULL;
    strcpy(result, s->content);
    return result;
}

void string_append(string_t* s, char* c){
    if(c == NULL || s == NULL) return;
    int len = strlen(c);
    for(unsigned int i = 0; i < len; ++i){
        string_push(s,c[i]);
    }
}

char* delete_quotes(char* c){
    int len = strlen(c);
    char* new_str = malloc(len - 2);
    strcpy(new_str, c + 1);
    new_str[len - 2] = 0;
    return new_str;
}

char* resolve_path(char* path){
    if(path && path[0] == '@'){
        char* lib_path = malloc(LIB_PATH_LEN + strlen(path));
        strcpy(lib_path, LIB_PATH);
        strcat(lib_path, path + 1);
        return lib_path;
    }
    return path;
}