/**
 * \file            expr.c
 * \brief           Expression parsing implementation file
 * \copyright       2024 Marcos González GPL3
 */
#include "parse.h"

static node_t* call_expr(lexer_t* lexer, char* id);
static node_t* non_literal_expr(lexer_t* lexer);
static node_t* index_expr(lexer_t* lexer, node_t* array);

// Returns an expression with the form [expr].[expr]
static node_t* member_expr(lexer_t* lexer, node_t* parent){
    next(lexer);
    node_t* member = init_node(2, N_MEMBER_EXPR, lexer->position);
    vector_push(member->children, parent);
    node_t* child = non_literal_expr(lexer);

    if(lexer->tok->type == T_DOT){
        vector_push(member->children, member_expr(lexer, child));
    }
    vector_push(member->children, child);
    if(lexer->tok->type == T_LEFT_SQUARE){
        return index_expr(lexer, member);
    }
    return member;
}

// Returns an expression with the form [id]([expr]? (,[expr])*)
static node_t* call_expr(lexer_t* lexer, char* id){
    node_t* expr = init_node(2, N_CALL_EXPR, lexer->position);
    vector_push(expr->children, id);
    next(lexer);
    while(lexer->tok->type != T_RIGHT_PAR){
        node_t* arg = parse_expr(lexer);
        vector_push(expr->children, arg);
        if(lexer->tok->type == T_RIGHT_PAR){
            break;
        }
        expect(lexer->tok, T_COMMA, "',' or ')'");
        next(lexer);
    }
    next(lexer);
    return expr;
}

static node_t* literal_expr(lexer_t* lexer, type* t){
    node_t* lit_expr = init_node(2, N_LITERAL, lexer->position);
    type* lit_type = malloc(sizeof(type));
    vector_push(lit_expr->children, lexer->tok->value);
    memcpy(lit_type, t, sizeof(type));
    vector_push(lit_expr->children, lit_type);
    next(lexer);
    return lit_expr;
}

static node_t* paren_expr(lexer_t* lexer, node_t* expr){
    node_t* par_expr = init_node(1, N_PAREN_EXPR, lexer->position);
    vector_push(par_expr->children, expr);
    return par_expr;
}

static node_t* non_literal_expr(lexer_t* lexer){
    char* id = lexer->tok->value;
    next(lexer);
    if(lexer->tok->type == T_LEFT_PAR){
        return call_expr(lexer, id);
    } 
    node_t* id_expr = init_node(1,N_IDENTIFIER, lexer->position);
    vector_push(id_expr->children, id);
    return id_expr;
}

static node_t* cast_expr(lexer_t* lexer){
    node_t* cast = init_node(2, N_CAST, lexer->position);
    type* t = parse_type(lexer);
    vector_push(cast->children, t);
    expect(lexer->tok, T_RIGHT_SQUARE, "]");
    next(lexer);
    vector_push(cast->children, parse_expr(lexer));
    return cast;
}

static node_t* index_expr(lexer_t* lexer, node_t* array){
    node_t* expr = init_node(2, N_INDEX_EXPR, lexer->position);
    vector_push(expr->children, array);
    next(lexer);
    node_t* index = parse_expr(lexer);
    expect(lexer->tok, T_RIGHT_SQUARE, "']");
    vector_push(expr->children, index);
    next(lexer);

    if(lexer->tok->type == T_DOT){
        return member_expr(lexer, expr);
    }
    if(lexer->tok->type == T_LEFT_SQUARE){
        return index_expr(lexer, expr);
    }
    return expr;
}

static node_t* stack_var_expr(lexer_t* lexer){
   
    return NULL;
}

static node_t* parse_primary_expr(lexer_t* lexer){
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
        next(lexer);
        return NULL_EXPR(lexer->position);
    case T_LEFT_PAR:
        next(lexer);
        node_t* expr = parse_expr(lexer);
        expect(lexer->tok, T_RIGHT_PAR, "')'");
        next(lexer);
        return paren_expr(lexer, expr);
    case T_LEFT_SQUARE:
        next(lexer);
        return cast_expr(lexer);
    
    case T_IDENTIFIER:
        // a non-literal can be an identifier or a function call
        node_t* non_literal = non_literal_expr(lexer);
        if(lexer->tok->type == T_DOT){
            return member_expr(lexer, non_literal);
        }
        if(lexer->tok->type == T_LEFT_SQUARE){
            return index_expr(lexer, non_literal);
        }
        return non_literal;
    case T_KEYWORD_NEW:
        return stack_var_expr(lexer);
    default:
        invalid_expr_err(lexer->position);
    }

    return NULL;
}

node_t* parse_expr(lexer_t* lexer){
    node_t* left = parse_primary_expr(lexer);
    if(!left) return NULL_EXPR(lexer->position);

    char op;
    switch (lexer->tok->type)
    {
    case T_ARITHMETIC_OP:
    case T_LOGICAL_OP:
    case T_COMPARATION_OP:
        op = lexer->tok->value[0];
        next(lexer);
        node_t* right = parse_expr(lexer);

        node_t* binary = init_node(3, N_BINARY_EXPR, lexer->position);
        vector_push(binary->children, left);
        vector_push(binary->children, right);
        vector_push(binary->children, &op);
        return binary;
    default:
        break;
    }

    if(lexer->tok->type == T_TERNARY_OP){
        next(lexer);
        node_t* if_1 = parse_expr(lexer);
        
        expect(lexer->tok, T_COLON, "':'");
        next(lexer);
        node_t* if_false = parse_expr(lexer);

        node_t* ternary = init_node(3, N_TERNARY_EXPR, lexer->position);
        vector_push(ternary->children, left);
        vector_push(ternary->children, if_1);
        vector_push(ternary->children, if_false);
        return ternary;
    }

    return left;
}