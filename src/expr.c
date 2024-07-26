#include "quarzum.h"

static node* parse_primary_expr(lexer* lexer){
    token* next = next_token(lexer);
    return NULL;
}

node* parse_expr(lexer* lexer){
    node* left = parse_primary_expr(lexer);
    return left;
}