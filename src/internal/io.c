/*
 * Quarzum Compiler - io.c
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
#include "../../include/quarzum/io.h"

inline string* read_file(const char* filename){
    FILE* file = fopen(filename, "r");
    if(file == NULL){
        err("No such file or directory.",1);
        return NULL;
    }
    struct string* b = init_string(256);
    int c;
    while ((c = fgetc(file)) != EOF) {
        string_push(b,c);
    }
    fclose(file);
    return b;
}

inline FILE* createFile(const char* filename){
    FILE* file = fopen(filename, "rw");
    return file;
}

inline void writeFile(FILE* file, char* content){
    if(file == NULL){
        return;
    }
    fprintf(file,"%s",content);
    fflush(file);
}