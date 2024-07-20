/*
 * Quarzum Compiler - path.c
 * Version 1.0, 19/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#include "../../include/core/path.h"

char* getCWD(){
    if(getcwd(PATH,sizeof(PATH)) != NULL){
        return PATH;
    }
    return NULL;
}

char* resolvePath(const char* relativePath){
    char* basePath = getCWD();
    strcat(basePath, "/");
    strcat(basePath, relativePath);
    return basePath;
}