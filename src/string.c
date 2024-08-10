#include "quarzum.h"
inline string* init_string(const uint32_t size){
    string* s = malloc(sizeof(string));
    if(s == NULL){
        return NULL;
    }
    s->size = size;
    s->len = 0;
    s->value = malloc(size + 1);
    if(s->value == NULL){
        free(s);
        return NULL;
    }
    s->value[0] = '\0';
    return s;
}

inline void free_string(string* s){
    free(s->value);
    free(s);
}

inline void string_push(string* s, const char c){
    if(s->len >= s->size){
        char* newBuffer = realloc(s->value,s->size * 2);
        if(newBuffer == NULL){
            return;
        }
        s->size *= 2;
        s->value = newBuffer;
    }
    s->value[s->len++] = c;
    s->value[s->len] = '\0';
}

inline void string_pop(string* s){
    s->value[--s->len] = '\0';
}

inline void string_clear(string* s){
    for(uint32_t i = 0; s->value[i]; ++i){
        s->value[i] = '\0';
    }
    s->len = 0;
}

inline char* string_copy(string* s){
    char* result = malloc(s->len + 1);
    if(result == NULL){
        return NULL;
    }
    strcpy(result, s->value);
    return result;
}

void string_append(string* s, char* c){
    int len = strlen(c);
    for(uint32_t i = 0; i < len; ++i){
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