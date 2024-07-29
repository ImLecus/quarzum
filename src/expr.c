#include "quarzum.h"

static node* parse_primary_expr(lexer* lexer){
    switch (lexer->tok->type)
    {
    case T_INT_LITERAL:
        node* literal_expr = init_node(2,N_LITERAL);
        // int8 is the first child
        vector_push(literal_expr->children, &(type){TY_INT, 1, 1});
        // the literal value is the second child
        vector_push(literal_expr->children, lexer->tok->value);
        return literal_expr;
    
    default:
        break;
    }

    return NULL;
}

node* parse_expr(lexer* lexer){
    node* left = parse_primary_expr(lexer);
    return left;
}