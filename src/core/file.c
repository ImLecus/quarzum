/**
 * \file            file.c
 * \brief           File and path util implementatons
 * \copyright       2024 Marcos González GPL3
 */
#include "file.h"

const char* get_extension(const char* file){
    for(uint32_t i = strlen(file); i > 0; --i){
        if(file[i] == '.'){
            return file+i;
        }
    }
    return NULL;
}

inline String* const read_file(const char* const filename){
    FILE* const file = fopen(filename, "r");
    if(file == NULL){
        file_not_found_err(filename);
        return NULL;
    }
    String* const result = init_string(512);
    int c;
    while ((c = fgetc(file)) != EOF) {
        string_push(result,c);
    }
    fclose(file);
    return result;
}

inline void write_file(FILE* file, char* const content){
    if(file == NULL) return;
    fprintf(file,"%s",content);
    fflush(file);
}