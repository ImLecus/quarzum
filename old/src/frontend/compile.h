/**
 * \file            compile.h
 * \brief           Quarzum Compiler front-end entry point.
 * \copyright       2024 Marcos González GPL3
 */
#ifndef COMPILE_H
#define COMPILE_H
#include "check.h"

/**
 * \brief           Compiles a file and returns the AST.
 * \returns         An AST pointer, `NULL` in case of error.
 */
int compile(const char * const file, AST* ast);

#endif /* COMPILE_H */