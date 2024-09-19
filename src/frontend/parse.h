/**
 * \file            parse.h
 * \brief           Parse include file
 * \copyright       2024 Marcos González GPL3
 */
#ifndef PARSE_H
#define PARSE_H
#include "type.h"
#include "tokenize.h"

enum {
    N_ROOT,
    N_IF,
    N_WHILE,
    N_VAR,
    N_MODULE,
    N_BREAK,
    N_CONTINUE,
    N_FUNCTION,
    N_IDENTIFIER,
    N_TYPE,
    N_RETURN,
    N_LAMBDA,
    N_ENUM,
    // Expression nodes
    N_UNARY_EXPR,
    N_BINARY_EXPR,
    N_PAREN_EXPR,
    N_TERNARY_EXPR,
    N_MEMBER_EXPR,
    N_CALL_EXPR,
    N_NULL_EXPR,
    N_CAST,
    N_INDEX_EXPR,

    N_LITERAL,
    N_CLASS,
    N_INIT_LIST,
    N_CLASS_STMT
};

typedef struct {
    uint8_t type;
    vector* children;
    uint32_t line, column;
} node;

void* n_get(node* n, uint32_t index);

node* init_node(uint32_t children, uint8_t type, pos_t pos);
void expect(token_t* t, uint8_t type, char* what);
type* parse_type(lexer_t* lexer);
node* parse(char* file);


#ifndef NULL_EXPR
#define NULL_EXPR init_node(0, N_NULL_EXPR, (pos_t){0,0,""})
#endif 

node* parse_expr(lexer_t* lexer);

#endif