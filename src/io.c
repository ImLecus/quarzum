#include "quarzum.h"

inline string* read_file(const char* filename){
    FILE* file = fopen(filename, "r");
    if(file == NULL){
        printf(RED"[ERROR]"RESET" (%s) File not found\n",filename);
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

inline void write_file(FILE* file, char* content){
    if(file == NULL){
        return;
    }
    fprintf(file,"%s",content);
    fflush(file);
}