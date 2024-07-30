#include "quarzum.h"

static node* literal_expr(lexer* lexer, type* type){
    node* lit_expr = init_node(2, N_LITERAL);
    vector_push(lit_expr->children, lexer->tok->value);
    // int8 is the second child
    vector_push(lit_expr->children, type);
    return lit_expr;
}

static node* parse_primary_expr(lexer* lexer){
    switch (lexer->tok->type)
    {
    case T_INT_LITERAL:
        return literal_expr(lexer, ty_int32);
    case T_NUMERIC_LITERAL:
        return literal_expr(lexer, ty_num32);
    case T_CHAR_LITERAL:
        return literal_expr(lexer, ty_char);
    case T_STRING_LITERAL:
        return literal_expr(lexer, ty_string);
    case T_KEYWORD_TRUE:
    case T_KEYWORD_FALSE:
        return literal_expr(lexer, ty_bool);
    case T_NULL_LITERAL:
        return literal_expr(lexer, ty_null);
    
    default:
        printf(ERROR_MSG("Invalid expression"));
        break;
    }

    return NULL;
}

node* parse_expr(lexer* lexer){
    node* left = parse_primary_expr(lexer);
    return left;
}