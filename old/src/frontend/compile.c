/**
 * \file            compile.c
 * \brief           Quarzum Compiler front-end entry point.
 * \copyright       2024 Marcos González GPL3
 */
#include "compile.h"

int compile(const char* const file, AST* ast){
    ast = parse(file);
    if(error_count > 0){
        check_errors();
        return -1;
    }
    // check_ast(ast);
    if(error_count > 0){
        check_errors();
        return -1;
    }
    free(type_map);
    free(symbol_table);

}