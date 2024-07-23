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

Buffer* readFile(const char* filename){
    FILE* file = fopen(filename, "r");
    if(file == NULL){
        err("No such file or directory.",1);
        return NULL;
    }
    Buffer* b = createBuffer(256);
    int c;
    while ((c = fgetc(file)) != EOF) {
        addToBuffer(b,c);
    }
    fclose(file);
    return b;
}

FILE* createFile(const char* filename){
    FILE* file = fopen(filename, "rw");
    return file;
}

void writeFile(FILE* file, char* content){
    if(file == NULL){
        return;
    }
    fprintf(file,"%s",content);
    fflush(file);
}