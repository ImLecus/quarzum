/**
 * \file            error.c
 * \brief           Error handling implementation for the
 *                  Quarzum compiler
 * \copyright       2024 Marcos González GPL3
 */
#include "error.h"

error_t ERROR_LIST[MAX_ERRORS];
int error_count;
error_t WARNING_LIST[MAX_ERRORS];
int warning_count;



void throw_error(const Position pos, const char* const message){
    if(error_count >= MAX_ERRORS){
        return;
    }
    if(ERROR_LIST[error_count - 1].line == pos.line){
        return;
    }
    ERROR_LIST[error_count].line = pos.line;
    if(pos.line >= 0){
        sprintf(ERROR_LIST[error_count++].message, "\e[1;31m[ERROR] \e[0m(%s) %s at line %d\n",pos.file, message, pos.line);
        return;
    }
    sprintf(ERROR_LIST[error_count++].message, "\e[1;31m[ERROR] \e[0m(%s) %s\n",pos.file, message);

}

void throw_warning(const Position pos, const char* const message){
    if(warning_count >= MAX_ERRORS){
        return;
    }
    if(WARNING_LIST[warning_count - 1].line == pos.line){
        return;
    }
    WARNING_LIST[warning_count].line = pos.line;
    sprintf(WARNING_LIST[warning_count++].message, "\e[1;38;5;214m[WARNING] \e[0m(%s) %s at line %d\n",pos.file, message, pos.line);
}

void print_errors(){
    for(unsigned int i = 0; i < error_count; ++i){
        if(i < warning_count){
            printf(WARNING_LIST[i].message);
        }
        printf(ERROR_LIST[i].message);
    }
    warning_count = 0;
}

inline void check_errors(){
    print_errors();
    if(error_count > 0){
        printf("Process failed with %d errors\n", error_count);
        exit(-1);
    }
}

inline void unexpected_token_err(const Position pos,const char* const buffer){
    char message[256];
    sprintf(message, "Unexpected token '%s'", buffer);
    throw_error(pos, message);   
}

inline void invalid_decimal_err(Position pos){
    throw_error(pos, "Too many decimal points on numeric literal");
}

inline void unclosed_quotes_err(Position pos){
    throw_error(pos, "Unclosed quotes");
}

inline void expected_token_err(Position pos, const char* const expected){
    char message[256];
    sprintf(message, "Expected %s", expected);
    throw_error(pos, message);   
}

inline void duplicated_type_err(Position pos, const char* const typename){
    char message[256];
    sprintf(message, "Type '%s' has been already defined", typename);
    throw_error(pos, message);   
}

inline void undefined_type_err(Position pos, const char* const typename){
    char message[256];
    sprintf(message, "Undefined type '%s'", typename);
    throw_error(pos, message);  
}

inline void duplicated_flag_warning(Position pos, const char* const flag){
    char message[256];
    sprintf(message, "Duplicated flag '%s'", flag);
    throw_error(pos, message); 
}

inline void duplicated_symbol_err(Position pos, const char* const symbol){
    char message[256];
    sprintf(message, "Symbol '%s' already exists", symbol);
    throw_error(pos, message); 
}

inline void file_not_found_err(const char* const filename){
    throw_error((Position){-1, 0, filename}, "File not found");
}

inline void invalid_expr_err(Position pos){
    throw_error(pos, "Invalid expression");
}
