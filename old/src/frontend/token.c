/**
 * \file            token.c
 * \brief           Token implementation file
 * \copyright       2024 Marcos González GPL3
 */
#include "../lexer/include/token.h"

Token token_new(TokenType type, char* value){
    Token tok;
    tok.type = type;
    tok.value = strdup(value);
    return tok;
}

void print_token(Token t){
    printf("Token: [type: %d, value: '%s']\n",t.type, t.value ? t.value : "(null)");
}