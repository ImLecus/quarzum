#include "quarzum.h"

static node* call_expr(lexer_t* lexer, char* id);
static node* non_literal_expr(lexer_t* lexer);
static node* index_expr(lexer_t* lexer, node* array);

// Returns an expression with the form [expr].[expr]
static node* member_expr(lexer_t* lexer, node* parent){
    read_next(lexer);
    node* member = init_node(2, N_MEMBER_EXPR, lexer->position);
    vector_push(member->children, parent);
    node* child = non_literal_expr(lexer);

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
static node* call_expr(lexer_t* lexer, char* id){
    node* expr = init_node(2, N_CALL_EXPR, lexer->position);
    vector_push(expr->children, id);
    read_next(lexer);
    while(lexer->tok->type != T_RIGHT_PAR){
        node* arg = parse_expr(lexer);
        vector_push(expr->children, arg);
        if(lexer->tok->type == T_RIGHT_PAR){
            break;
        }
        expect(lexer->tok, T_COMMA, "',' or ')'");
        read_next(lexer);
    }
    read_next(lexer);
    return expr;
}

static node* literal_expr(lexer_t* lexer, type* t){
    node* lit_expr = init_node(2, N_LITERAL, lexer->position);
    type* lit_type = malloc(sizeof(type));
    vector_push(lit_expr->children, lexer->tok->value);
    memcpy(lit_type, t, sizeof(type));
    vector_push(lit_expr->children, lit_type);
    read_next(lexer);
    return lit_expr;
}

static node* paren_expr(lexer_t* lexer, node* expr){
    node* par_expr = init_node(1, N_PAREN_EXPR, lexer->position);
    vector_push(par_expr->children, expr);
    return par_expr;
}

static node* non_literal_expr(lexer_t* lexer){
    char* id = lexer->tok->value;
    read_next(lexer);
    if(lexer->tok->type == T_LEFT_PAR){
        return call_expr(lexer, id);
    } 
    node* id_expr = init_node(1,N_IDENTIFIER, lexer->position);
    vector_push(id_expr->children, id);
    return id_expr;
}

static node* cast_expr(lexer_t* lexer){
    node* cast = init_node(2, N_CAST, lexer->position);
    type* t = parse_type(lexer);
    vector_push(cast->children, t);
    expect(lexer->tok, T_RIGHT_SQUARE, "]");
    read_next(lexer);
    vector_push(cast->children, parse_expr(lexer));
    return cast;
}

static node* index_expr(lexer_t* lexer, node* array){
    node* expr = init_node(2, N_INDEX_EXPR, lexer->position);
    vector_push(expr->children, array);
    read_next(lexer);
    node* index = parse_expr(lexer);
    expect(lexer->tok, T_RIGHT_SQUARE, "']");
    vector_push(expr->children, index);
    read_next(lexer);

    if(lexer->tok->type == T_DOT){
        return member_expr(lexer, expr);
    }
    if(lexer->tok->type == T_LEFT_SQUARE){
        return index_expr(lexer, expr);
    }
    return expr;
}

static node* stack_var_expr(lexer_t* lexer){
   
    return NULL;
}

static node* parse_primary_expr(lexer_t* lexer){
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
        read_next(lexer);
        return NULL_EXPR;
    case T_LEFT_PAR:
        read_next(lexer);
        node* expr = parse_expr(lexer);
        expect(lexer->tok, T_RIGHT_PAR, "')'");
        read_next(lexer);
        return paren_expr(lexer, expr);
    case T_LEFT_SQUARE:
        read_next(lexer);
        return cast_expr(lexer);
    
    case T_IDENTIFIER:
        // a non-literal can be an identifier or a function call
        node* non_literal = non_literal_expr(lexer);
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
        printf(RED"[ERROR]"RESET" (%s) Invalid expression at line %d\n", lexer->position.file, lexer->position.line); 
        break;
    }

    return NULL;
}

node* parse_expr(lexer_t* lexer){
    node* left = parse_primary_expr(lexer);
    if(!left){
        return NULL_EXPR;
    }

    char op;
    switch (lexer->tok->type)
    {
    case T_ARITHMETIC_OP:
    case T_LOGICAL_OP:
    case T_COMPARATION_OP:
        op = lexer->tok->value[0];
        read_next(lexer);
        node* right = parse_expr(lexer);

        node* binary = init_node(3, N_BINARY_EXPR, lexer->position);
        vector_push(binary->children, left);
        vector_push(binary->children, right);
        vector_push(binary->children, &op);
        return binary;
    default:
        break;
    }

    if(lexer->tok->type == T_TERNARY_OP){
        read_next(lexer);
        node* if_1 = parse_expr(lexer);
        
        expect(lexer->tok, T_COLON, "':'");
        read_next(lexer);
        node* if_false = parse_expr(lexer);

        node* ternary = init_node(3, N_TERNARY_EXPR, lexer->position);
        vector_push(ternary->children, left);
        vector_push(ternary->children, if_1);
        vector_push(ternary->children, if_false);
        return ternary;
    }

    return left;
}