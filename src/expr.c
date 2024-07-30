#include "quarzum.h"

static node* parse_primary_expr(lexer* lexer){
    switch (lexer->tok->type)
    {
    case T_INT_LITERAL:
    case T_NUMERIC_LITERAL:
    case T_CHAR_LITERAL:
    case T_STRING_LITERAL:
    case T_KEYWORD_TRUE:
    case T_KEYWORD_FALSE:
    case T_NULL_LITERAL:
        node* literal_expr = init_node(2,N_LITERAL);

        // the literal value is the first child
        vector_push(literal_expr->children, lexer->tok->value);
        // int8 is the second child
        vector_push(literal_expr->children, &(type){TY_INT, 1, 1});
        
        return literal_expr;
    
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