/**
 * \file            expr.c
 * \brief           Expression parsing implementation file
 * \copyright       2024 Marcos González GPL3
 */
#include "parse.h"

static Node* call_expr(Lexer* const lexer, const char* id);
static Node* non_literal_expr(Lexer* const lexer);
static Node* index_expr(Lexer* const lexer, Node* array);

// Returns an expression with the form [expr].[expr]
static Node* member_expr(Lexer* const lexer, Node* parent){
    next(lexer);
    Node* member = init_node(2, N_MEMBER_EXPR, lexer->position);
    vector_push(member->children, parent);
    Node* child = non_literal_expr(lexer);

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
static Node* call_expr(Lexer* const lexer, const char* id){
    Node* expr = init_node(2, N_CALL_EXPR, lexer->position);
    vector_push(expr->children, id);
    next(lexer);
    while(lexer->tok->type != T_RIGHT_PAR){
        Node* arg = parse_expr(lexer);
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

static Node* literal_expr(Lexer* const lexer, Type* t){
    Node* lit_expr = init_node(2, N_LITERAL, lexer->position);
    Type* lit_type = malloc(sizeof(Type));
    vector_push(lit_expr->children, lexer->tok->value);
    memcpy(lit_type, t, sizeof(Type));
    vector_push(lit_expr->children, lit_type);
    next(lexer);
    return lit_expr;
}

static Node* paren_expr(Lexer* const lexer, Node* expr){
    Node* par_expr = init_node(1, N_PAREN_EXPR, lexer->position);
    vector_push(par_expr->children, expr);
    return par_expr;
}

static Node* non_literal_expr(Lexer* const lexer){
    const char* id = lexer->tok->value;
    next(lexer);
    if(lexer->tok->type == T_LEFT_PAR){
        return call_expr(lexer, id);
    } 
    Node* id_expr = init_node(1,N_IDENTIFIER, lexer->position);
    vector_push(id_expr->children, id);
    return id_expr;
}

static Node* cast_expr(Lexer* const lexer){
    Node* cast = init_node(2, N_CAST, lexer->position);
    Type* t = parse_type(lexer);
    vector_push(cast->children, t);
    expect(lexer->tok, T_RIGHT_SQUARE, "]");
    next(lexer);
    vector_push(cast->children, parse_expr(lexer));
    return cast;
}

static Node* index_expr(Lexer* const lexer, Node* array){
    Node* expr = init_node(2, N_INDEX_EXPR, lexer->position);
    vector_push(expr->children, array);
    next(lexer);
    Node* index = parse_expr(lexer);
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

static Node* stack_var_expr(Lexer* const lexer){
   
    return NULL;
}

static Node* parse_primary_expr(Lexer* const lexer){
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
        Node* expr = parse_expr(lexer);
        expect(lexer->tok, T_RIGHT_PAR, "')'");
        next(lexer);
        return paren_expr(lexer, expr);
    case T_LEFT_SQUARE:
        next(lexer);
        return cast_expr(lexer);
    
    case T_IDENTIFIER:
        // a non-literal can be an identifier or a function call
        Node* non_literal = non_literal_expr(lexer);
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

Node* parse_expr(Lexer* const lexer){
    Node* left = parse_primary_expr(lexer);
    if(!left) return NULL_EXPR(lexer->position);

    char op;
    switch (lexer->tok->type)
    {
    case T_ARITHMETIC_OP:
    case T_LOGICAL_OP:
    case T_COMPARATION_OP:
        op = lexer->tok->value[0];
        next(lexer);
        Node* right = parse_expr(lexer);

        Node* binary = init_node(3, N_BINARY_EXPR, lexer->position);
        vector_push(binary->children, left);
        vector_push(binary->children, right);
        vector_push(binary->children, &op);
        return binary;
    default:
        break;
    }

    if(lexer->tok->type == T_TERNARY_OP){
        next(lexer);
        Node* if_1 = parse_expr(lexer);
        
        expect(lexer->tok, T_COLON, "':'");
        next(lexer);
        Node* if_false = parse_expr(lexer);

        Node* ternary = init_node(3, N_TERNARY_EXPR, lexer->position);
        vector_push(ternary->children, left);
        vector_push(ternary->children, if_1);
        vector_push(ternary->children, if_false);
        return ternary;
    }

    return left;
}