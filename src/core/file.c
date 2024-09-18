// File: file.c
// Description: File and path util implementations
#include "file.h"
#include <string.h>

char* get_extension(char* file){
    for(unsigned int i = strlen(file); i > 0; --i){
        if(file[i] == '.'){
            return file+i;
        }
    }
}

