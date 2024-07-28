#include "quarzum.h"

inline string* read_file(const char* filename){
    FILE* file = fopen(filename, "r");
    if(file == NULL){
        printf(ERROR_MSG("No such file or directory."));
        return NULL;
    }
    string* b = init_string(256);
    int c;
    while ((c = fgetc(file)) != EOF) {
        string_push(b,c);
    }
    fclose(file);
    return b;
}

inline FILE* create_file(const char* filename){
    FILE* file = fopen(filename, "rw");
    return file;
}

inline void write_file(FILE* file, char* content){
    if(file == NULL){
        return;
    }
    fprintf(file,"%s",content);
    fflush(file);
}