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

Buffer* read(const char* filename){
    FILE* file = fopen(filename, "r");
    if(file == NULL){
        // err
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