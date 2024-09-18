/**
 * \file            lexer.h
 * \brief           Quarzum lexer include file
 */

// LICENSE

#ifndef LEXER_H
#define LEXER_H
#include "../core/string.h"

#define KEYWORDS_SIZE 55
#define SYMBOLS_SIZE 41

static const char* keywords[KEYWORDS_SIZE] = {
    "alloc","and","break","case",
    "char","class","const","constructor","continue","default",
    "delete","destructor","do","else","enum",
    "false","for","foreach","foreign","function",
    "if","import","in","int","int16","int32","int64","int8",
    "module","new","not","null","num","num16","num32","num64",
    "operator", "or","private","protected","public",
    "return","sizeof","struct","switch",
    "true","typedef","uint","uint16",
    "uint32","uint64","uint8","var","while","xor"
};

// There are 27 keywords in the Quarzum language, but internally,
// primitive type names are keywords.
static const int keyword_types[KEYWORDS_SIZE] = {
    T_KEYWORD_ALLOC ,T_LOGICAL_OP, T_KEYWORD_BREAK, T_KEYWORD_CASE,
    T_TYPE, T_KEYWORD_CLASS, T_SPECIFIER, T_KEYWORD_CONSTRUCTOR, T_KEYWORD_CONTINUE, T_KEYWORD_DEFAULT,
    T_KEYWORD_DELETE,T_KEYWORD_DESTRUCTOR, T_KEYWORD_DO, T_KEYWORD_ELSE, T_KEYWORD_ENUM, T_KEYWORD_FALSE, T_KEYWORD_FOR, T_KEYWORD_FOREACH, T_SPECIFIER,
    T_TYPE, T_KEYWORD_IF, T_KEYWORD_IMPORT, T_KEYWORD_IN,
    T_TYPE, T_TYPE, T_TYPE, T_TYPE, T_TYPE, T_KEYWORD_MODULE, T_KEYWORD_NEW,
    T_UNARY, T_NULL_LITERAL, T_TYPE, T_TYPE, T_TYPE, T_TYPE,
    T_KEYWORD_OPERATOR, T_LOGICAL_OP, T_ACCESS, T_ACCESS, T_ACCESS, T_KEYWORD_RETURN,
    T_KEYWORD_SIZEOF, T_KEYWORD_STRUCT, T_KEYWORD_SWITCH, T_KEYWORD_TRUE,
    T_KEYWORD_TYPEDEF, T_TYPE, T_TYPE, T_TYPE, T_TYPE,
    T_TYPE, T_TYPE, T_KEYWORD_WHILE, T_LOGICAL_OP

};

static const char* symbols[SYMBOLS_SIZE] = {
    "!","!=","#","#=","%","%=","&","&=","(",")","*","*=","+","++","+=",",","-","--","-=",
    ".","/","/=",":","::",";","<","<=","=","==","=>",">",">=","?","[","]","^","^=","{","|","|=","}"
};

static const int symbol_types[SYMBOLS_SIZE] = {
    T_BITWISE_OP, T_ASSIGN_OP, T_BITWISE_OP, T_ASSIGN_OP, T_ARITHMETIC_OP,
    T_ASSIGN_OP, T_BITWISE_OP, T_ASSIGN_OP, T_LEFT_PAR, T_RIGHT_PAR, T_ARITHMETIC_OP,
    T_ASSIGN_OP, T_ARITHMETIC_OP, T_UNARY, T_ASSIGN_OP, T_COMMA, T_ARITHMETIC_OP,
    T_UNARY, T_ASSIGN_OP, T_DOT, T_ARITHMETIC_OP, T_ASSIGN_OP, T_COLON,T_TYPE_EXTENSION_OP, T_SEMICOLON,
    T_COMPARATION_OP, T_COMPARATION_OP, T_EQUAL, T_COMPARATION_OP,T_ARROW ,T_COMPARATION_OP,
    T_COMPARATION_OP, T_TERNARY_OP, T_LEFT_SQUARE, T_RIGHT_SQUARE, T_ARITHMETIC_OP,
    T_ASSIGN_OP, T_LEFT_CURLY, T_BITWISE_OP, T_ASSIGN_OP, T_RIGHT_CURLY

};


/**
 * \brief           Token type enum
 */
typedef enum {
    T_TOKEN_ERROR,
    T_LOGICAL_OP,
    T_SPECIFIER,
    T_UNARY,
    T_ACCESS,
    T_TYPE,
    T_BITWISE_OP,
    T_ARITHMETIC_OP,
    T_ASSIGN_OP,
    T_IDENTIFIER,
    T_EOF,
    T_SYMBOL,
    T_NUMERIC_LITERAL,
    T_STRING_LITERAL,
    T_CHAR_LITERAL,
    T_INT_LITERAL,
    T_NULL_LITERAL,
    T_KEYWORD_ALLOC,
    T_KEYWORD_BREAK,
    T_KEYWORD_CASE,
    T_KEYWORD_CLASS,
    T_KEYWORD_CONTINUE,
    T_KEYWORD_DEFAULT,
    T_KEYWORD_DELETE,
    T_KEYWORD_DO,
    T_KEYWORD_ELSE,
    T_KEYWORD_ENUM,
    T_KEYWORD_FALSE,
    T_KEYWORD_FOR,
    T_KEYWORD_FOREACH,
    T_KEYWORD_IF,
    T_KEYWORD_IMPORT,
    T_KEYWORD_IN,
    T_KEYWORD_MODULE,
    T_KEYWORD_NEW,
    T_KEYWORD_RETURN,
    T_KEYWORD_SIZEOF,
    T_KEYWORD_STRUCT,
    T_KEYWORD_SWITCH,
    T_KEYWORD_TRUE,
    T_KEYWORD_TYPEDEF,
    T_KEYWORD_WHILE,
    T_KEYWORD_CONSTRUCTOR,
    T_KEYWORD_DESTRUCTOR,
    T_LEFT_PAR,
    T_RIGHT_PAR,
    T_COMMA,
    T_DOT,
    T_COLON,
    T_SEMICOLON,
    T_COMPARATION_OP,
    T_TERNARY_OP,
    T_LEFT_SQUARE,
    T_RIGHT_SQUARE,
    T_LEFT_CURLY,
    T_RIGHT_CURLY,
    T_ARROW,
    T_EQUAL,
    T_KEYWORD_OPERATOR,
    T_TYPE_EXTENSION_OP
} token_type_t;

/**
 * \brief           Struct to store an element position
 *                  inside a file during the compilation
 */
typedef struct {
    unsigned int file, column;
    char* file;
} pos_t;

/**
 * \brief           Basic lexer result unit
 */
typedef struct {
    pos_t position;
    token_type_t type;
    char* value;
} token_t;

/**
 * \brief           Lexer struct. Contains the result of the
 *                  last iteration and its position
 */
typedef struct {
    pos_t position;
    token_t* tok;
    char* input;
    string_t* buffer;
    unsigned int pos;
} lexer_t;

lexer_t* init_lexer(char* filename);

void next_token(lexer_t* lexer);


#endif /* LEXER_H */