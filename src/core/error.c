// File: error.c
// Description: Quarzum error handling implementation
#include "error.h"
#include <stdio.h>

error_t ERROR_LIST[MAX_ERRORS];
int error_count;

void throw_error(uint32_t line, uint32_t column, char* message){
    if(error_count >= MAX_ERRORS){
        return;
    }
    if(ERROR_LIST[error_count - 1].line == line){
        return;
    }
    ERROR_LIST[error_count].line = line;
    sprintf(ERROR_LIST[error_count++].message, "\e[1;31m[ERROR] \e[0m%s at line %d\n", message,line);
}

void print_errors(){
    for(unsigned int i = 0; i < error_count; ++i){
        printf(ERROR_LIST[i].message);
    }
}
