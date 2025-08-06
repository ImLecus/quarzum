/**
 * \file            token.h
 * \brief           Token include file
 * \copyright       2024 Marcos González GPL3
 */
#ifndef TOKEN_H
#define TOKEN_H
#include "../core/core.h"

/**
 * \brief           Token type enum
 */
typedef enum {
    TK_ERROR,
    TK_SPECIFIER,
    TK_UNARY,
    TK_ACCESS,
    TK_IDENTIFIER,
    TK_EOF,
    TK_SYMBOL,
    TK_NUMERIC_LITERAL,
    TK_STRING_LITERAL,
    TK_CHAR_LITERAL,
    TK_INT_LITERAL,
    TK_NULL_LITERAL,
    TK_ALLOC,
    TK_ASYNC,
    TK_AWAIT,
    TK_BREAK,
    TK_CASE,
    TK_CLASS,
    TK_CONTINUE,
    TK_DEFAULT,
    TK_DELETE,
    TK_DO,
    TK_ELSE,
    TK_ENUM,
    TK_FALSE,
    TK_FOR,
    TK_FOREACH,
    TK_IF,
    TK_IMPORT,
    TK_IN,
    TK_NEW,
    TK_RETURN,
    TK_SIZEOF,
    TK_STRUCT,
    TK_SWITCH,
    TK_THIS,
    TK_TRUE,
    TK_TYPEDEF,
    TK_WHILE,
    TK_CONSTRUCTOR,
    TK_DESTRUCTOR,
    TK_LEFT_PAR,
    TK_RIGHT_PAR,
    TK_COMMA,
    TK_DOT,
    TK_COLON,
    TK_SEMICOLON,
    TK_OPERATOR,
    TK_TYPE_EXTENSION_OP,
    TK_TERNARY_OP,
    TK_LEFT_SQUARE,
    TK_RIGHT_SQUARE,
    TK_LEFT_CURLY,
    TK_RIGHT_CURLY,
    TK_ARROW,
    TK_EQUAL = 0xf0,
    TK_COMPARATION_OP = 0xf1,
    TK_LOGICAL_OP = 0xf2,
    TK_BITWISE_OP = 0xf3,
    TK_ARITHMETIC_OP = 0xf4,
    TK_ASSIGN_OP = 0xf5
} TokenType;
#define IS_OPERATOR(t) (uint8_t)(t) >= 0xf0

/**
 * \brief           Basic lexer result unit
 */
typedef struct Token {
    TokenType type;
    const char* value;
} Token;

/**
 * \brief           Initializes a new `Token`
 *
 * \param           type : The new Token type
 * \param           value : The new Token value
 */
Token token_new(TokenType type, char* value);

/**
 * \brief           Prints a Token information in console
 * \note            Does not print the Token position
 */
void print_token(Token t);


#endif /* TOKEN_H */