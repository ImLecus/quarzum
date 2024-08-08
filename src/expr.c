#include "quarzum.h"

static node* literal_expr(lexer* lexer, type* type){
    node* lit_expr = init_node(2, N_LITERAL);
    vector_push(lit_expr->children, lexer->tok->value);
    vector_push(lit_expr->children, type);
    return lit_expr;
}

static node* paren_expr(node* expr){
    node* par_expr = init_node(1, N_PAREN_EXPR);
    vector_push(par_expr->children, expr);
    return par_expr;
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
    case T_LEFT_PAR:
        read_next(lexer);
        node* expr = parse_expr(lexer);
        expect(lexer->tok, T_RIGHT_PAR, "')'");
        return paren_expr(expr);
    
    case T_IDENTIFIER:
        node* id_expr = init_node(1,N_IDENTIFIER);
        vector_push(id_expr->children, lexer->tok->value);
        return id_expr;

    default:
        printf(ERROR_MSG("Invalid expression"));
        break;
    }

    return NULL;
}

node* parse_expr(lexer* lexer){
    node* left = parse_primary_expr(lexer);
    read_next(lexer);
    if(!left){
        printf(ERROR_MSG("Invalid expression")); 
        return NULL;
    }

    char* op;
    switch (lexer->tok->type)
    {
    case T_ARITHMETIC_OP:
    case T_LOGICAL_OP:
    case T_COMPARATION_OP:
        op = lexer->tok->value;
        read_next(lexer);
        node* right = parse_expr(lexer);
        break;
    
    default:
        break;
    }
    return left;
}