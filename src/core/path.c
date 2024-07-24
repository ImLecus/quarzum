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

char* get_cwd(){
    if(getcwd(PATH,sizeof(PATH)) != NULL){
        return PATH;
    }
    return NULL;
}

char* delete_quotes(char* string){
    char* newString = (char*)malloc(sizeof(char) *  (strlen(string) - 2));
    for(unsigned int i = 0; i < strlen(string) - 2; ++i){
       newString[i] = string[i + 1];
    }
    return newString;
}

char* resolve_path(char* _path){
    char* newPath;
    char* path = delete_quotes(_path);
    if(path[0] == '@'){
        // newPath has the length of the path, deleting the @ and
        // the quotes, and adding the LIBRARY_PATH length.
        newPath = (char*)malloc(sizeof(char) * (strlen(path) - 1 + LIB_PATH_LEN));
        strcat(newPath, LIBRARY_PATH);
        // for(unsigned int i = 1; i < strlen(path) - 1; ++i){
        //     newPath[LIB_PATH_LEN + i - 1] = path[i];
        // }
    }
    else{
        newPath = path;
    }
    return newPath;
}