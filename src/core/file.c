/**
 * \file            file.c
 * \brief           File and path util implementatons
 * \copyright       2024 Marcos González GPL3
 */
#include "file.h"

char* get_extension(char* file){
    for(unsigned int i = strlen(file); i > 0; --i){
        if(file[i] == '.'){
            return file+i;
        }
    }
}

inline string_t* read_file(char* filename){
    FILE* file = fopen(filename, "r");
    if(file == NULL){
        file_not_found_err(filename);
        return NULL;
    }
    string_t* result = init_string(512);
    int c;
    while ((c = fgetc(file)) != EOF) {
        string_push(result,c);
    }
    fclose(file);
    return result;
}

inline void write_file(FILE* file, char* content){
    if(file == NULL) return;
    fprintf(file,"%s",content);
    fflush(file);
}