/**
 * \file            string.c
 * \brief           String implementation for the Quarzum compiler
 * \copyright       2024 Marcos González GPL3
 */
#include "string.h"

inline String* const init_string(const uint32_t size){
    String* s = malloc(sizeof(String));
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

inline void free_string(String* const s){
    if(s == NULL) return;
    free(s->content);
    free(s);
}

inline void string_push(String* const s, const char c){
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

inline void string_pop(String* const s){
    if(s->length == 0) return;
    s->content[--s->length] = '\0';
}

inline void string_clear(String* const s){
    memset(s->content, '\0', s->size);
    s->length = 0;
}

inline const char* string_copy(const String* const s){
    char* result = malloc(s->length + 1);
    if(result == NULL) return NULL;
    strcpy(result, s->content);
    return result;
}

void string_append(String* const s, const char* c){
    if(c == NULL || s == NULL) return;
    const size_t len = strlen(c);
    for(unsigned int i = 0; i < len; ++i){
        string_push(s,c[i]);
    }
}

const char* delete_quotes(const char* c){
    const size_t len = strlen(c);
    char* new_str = malloc(len - 2);
    strcpy(new_str, c + 1);
    new_str[len - 2] = 0;
    return new_str;
}

const char* resolve_path(const char* path){
    if(path && path[0] == '@'){
        char* lib_path = malloc(LIB_PATH_LEN + strlen(path));
        strcpy(lib_path, LIB_PATH);
        strcat(lib_path, path + 1);
        return lib_path;
    }
    return path;
}