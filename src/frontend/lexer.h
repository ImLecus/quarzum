/**
 * \file            lexer.h
 * \brief           Quarzum lexer include file
 * \copyright       2024 Marcos González, GPL3
 */
#ifndef LEXER_H
#define LEXER_H
#include "../core/file.h"
#include "../core/vector.h"
#include "token.h"

/**
 * Keywords and symbols
 * --------------------
 * 
 * Primitive types are not keywords. 
 * Both string arrays are sorted.
 */
#define KEYWORDS_SIZE 40
#define SYMBOLS_SIZE 41

static const char* const keywords[KEYWORDS_SIZE] = {
    "alloc","and","async","await","break","case","class","constructor","continue","default",
    "delete","destructor","do","else","enum",
    "false","for","foreach",
    "if","import","in","mut","new","not","null",
    "operator", "or","private","protected","public",
    "return","sizeof","struct","switch", "this",
    "true","typedef","while","xor"
};

static const int keyword_types[KEYWORDS_SIZE] = {
    TK_ALLOC ,TK_LOGICAL_OP,TK_ASYNC, TK_AWAIT, TK_BREAK, TK_CASE, TK_CLASS, TK_CONSTRUCTOR, 
    TK_CONTINUE, TK_DEFAULT, TK_DELETE, TK_DESTRUCTOR, TK_DO, TK_ELSE,
    TK_ENUM, TK_FALSE, TK_FOR, TK_FOREACH, 
    TK_IF, TK_IMPORT, TK_IN,TK_SPECIFIER, TK_NEW,
    TK_UNARY, TK_NULL_LITERAL, TK_OPERATOR, TK_LOGICAL_OP, TK_ACCESS, TK_ACCESS, TK_ACCESS, TK_RETURN,
    TK_SIZEOF, TK_STRUCT, TK_SWITCH, TK_THIS, TK_TRUE,
    TK_TYPEDEF, TK_WHILE, TK_LOGICAL_OP

};

static const char* symbols[SYMBOLS_SIZE] = {
    "!","!=","#","#=","%","%=","&","&=","(",")","*","*=","+","++","+=",",","-","--","-=",
    ".","/","/=",":","::",";","<","<=","=","==","=>",">",">=","?","[","]","^","^=","{","|","|=","}"
};

static const uint8_t symbol_types[SYMBOLS_SIZE] = {
    TK_BITWISE_OP, TK_ASSIGN_OP, TK_BITWISE_OP, TK_ASSIGN_OP, TK_ARITHMETIC_OP,
    TK_ASSIGN_OP, TK_BITWISE_OP, TK_ASSIGN_OP, TK_LEFT_PAR, TK_RIGHT_PAR, TK_ARITHMETIC_OP,
    TK_ASSIGN_OP, TK_ARITHMETIC_OP, TK_UNARY, TK_ASSIGN_OP, TK_COMMA, TK_ARITHMETIC_OP,
    TK_UNARY, TK_ASSIGN_OP, TK_DOT, TK_ARITHMETIC_OP, TK_ASSIGN_OP, TK_COLON,TK_TYPE_EXTENSION_OP, TK_SEMICOLON,
    TK_COMPARATION_OP, TK_COMPARATION_OP, TK_EQUAL, TK_COMPARATION_OP,TK_ARROW ,TK_COMPARATION_OP,
    TK_COMPARATION_OP, TK_TERNARY_OP, TK_LEFT_SQUARE, TK_RIGHT_SQUARE, TK_ARITHMETIC_OP,
    TK_ASSIGN_OP, TK_LEFT_CURLY, TK_BITWISE_OP, TK_ASSIGN_OP, TK_RIGHT_CURLY

};


/**
 * \brief           Lexer struct. Contains the result of the
 *                  last iteration and its position
 */
typedef struct Lexer {
    Position position;
    const Token* tok;
    char* input;
    String* buffer;
} Lexer;

/**
 * \brief           Allocates a `Lexer` on the heap.
 * \returns         A pointer to the allocated `Lexer`
 * \warning         The function will return `NULL` if the
 *                  input does not exist or if there is not
 *                  enough memory to allocate the lexer.
 */
Lexer* const init_lexer(const char* filename, char* input);

static inline const char peek(Lexer* lexer);

static inline const char consume(Lexer* lexer);

const Position get_position(Lexer* lexer);

static const Token* const new_token(const TokenType type, Lexer* const lexer);

static void read_escape_char(Lexer* const lexer);

static void read_char_literal(Lexer* const lexer);

static void read_string_literal(Lexer* const lexer);

static inline void read_digit_chain(Lexer* const lexer);

static const int read_numeric_literal(Lexer* const lexer);

static inline const int read_id_or_keyword(Lexer* const lexer);

static const int read_symbol(Lexer* const lexer);

static void ignore_multi_comment(Lexer* const lexer);

static const bool check_comment(Lexer* const lexer);

/**
 * \brief           Analyzes the text to find the next token.
 * \returns         A pointer to `Token`, the next found token.
 * \note            The function will ever return an EOF token once
 *                  the text has been arrived to the end.
 */
const Token* const next_token(Lexer* const lexer);

/**
 * \brief           Tells the lexer to read the next token
 *                  and stores it in `lexer->tok`
 */
void next(Lexer* const lexer);


#define DEFAULTK_TOKENIZER_BUFFER_SIZE 10

#endif /* LEXER_H */