/**
 * \file            compile.c
 * \brief           Quarzum Compiler front-end entry point.
 * \copyright       2024 Marcos González GPL3
 */
#include "compile.h"

ast_t* compile(const char* file){
    ast_t* ast = parse(file);
    check_errors();
    // check_ast(ast);
    check_errors();
    free(type_map);
    free(symbol_table);
    return ast;
}