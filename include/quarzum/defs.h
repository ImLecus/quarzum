/*
 * Quarzum Compiler - defs.h
 * Version 1.0, 07/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#ifndef DEFS_H
#define DEFS_H
#include "token.h"
#include "../core/bsearch.h"


#define KEYWORDS_SIZE 54
#define SYMBOLS_SIZE 39
/**
 * @brief Searches in the `keywords` array using binary search.
 * If the element is not a keyword, it will return an `Identifier` type.
 */
const int keywordToType(const char* keyword);


/**
 * @brief Converts a symbol string to a TokenType using binary search.
 * If the symbol is not found, it will return a `TokenError`.
 */
const int symbolToType(const char* symbol);

/**
 * @brief Defines the keywords in the Quarzum language.
 * The array must be ordered, because the algorithm that
 * tries to search a keyword uses binary search.
 */
static const char* keywords[KEYWORDS_SIZE] = {
    "and",
    "bool",
    "break",
    "catch",
    "char",
    "class",
    "const",
    "continue",
    "decimal",
    "delete",
    "destroy",
    "do",
    "else",
    "enum",
    "false",
    "for",
    "foreach",
    "foreign",
    "from",
    "function",
    "if",
    "import",
    "in",
    "int",
    "int16",
    "int32",
    "int64",
    "int8",
    "module",
    "new",
    "not",
    "null",
    "num",
    "num16",
    "num32",
    "num64",
    "or",
    "persist",
    "private",
    "protected",
    "public",
    "return",
    "setup",
    "string",
    "true",
    "try",
    "typedef",
    "uint",
    "uint16",
    "uint32",
    "uint64",
    "uint8",
    "while",
    "xor"
};

static const int keywordTypes[KEYWORDS_SIZE] = {
    T_COMPARATION_OPERATOR,
    T_TYPE_KEYWORD,
    T_BREAK,
    T_TOKEN_ERROR, // NOT IMPLEMENTED YET
    T_TYPE_KEYWORD,
    T_CLASS,
    T_CONST,
    T_CONTINUE,
    T_TYPE_KEYWORD,
    T_DELETE,
    T_DESTROY,
    T_DO,
    T_ELSE,
    T_ENUM,
    T_BOOL_LITERAL,
    T_FOR,
    T_FOREACH,
    T_FOREIGN,
    T_FROM,
    T_TYPE_KEYWORD,
    T_IF,
    T_IMPORT,
    T_IN,
    T_TYPE_KEYWORD,
    T_TYPE_KEYWORD,
    T_TYPE_KEYWORD,
    T_TYPE_KEYWORD,
    T_TYPE_KEYWORD,
    T_MODULE,
    T_NEW,
    T_COMPARATION_OPERATOR,
    T_NULL_LITERAL,
    T_TYPE_KEYWORD,
    T_TYPE_KEYWORD,
    T_TYPE_KEYWORD,
    T_TYPE_KEYWORD,
    T_COMPARATION_OPERATOR,
    T_PERSIST,
    T_ACCESS_SPECIFIER,
    T_ACCESS_SPECIFIER,
    T_ACCESS_SPECIFIER,
    T_RETURN,
    T_SETUP,
    T_TYPE_KEYWORD,
    T_BOOL_LITERAL,
    T_TOKEN_ERROR, // NOT IMPLEMENTED YET
    T_TYPEDEF,
    T_TYPE_KEYWORD,
    T_TYPE_KEYWORD,
    T_TYPE_KEYWORD,
    T_TYPE_KEYWORD,
    T_TYPE_KEYWORD,
    T_WHILE,
    T_COMPARATION_OPERATOR

};
/**
 * @brief Defines the symbols in the Quarzum language.
 * It must be ordered by the ASCII code.
 * See more: https://www.asciitable.com/
 */
static const char* symbols[SYMBOLS_SIZE] = {
    "!",
    "!=",
    "#",
    "#=",
    "%",
    "%=",
    "&",
    "&=",
    "(",
    ")",
    "*",
    "*=",
    "+",
    "++",
    "+=",
    ",",
    "-",
    "--",
    "-=",
    ".",
    "/",
    "/=",
    ":",
    ";",
    "<",
    "<=",
    "=",
    "==",
    ">",
    ">=",
    "?",
    "[",
    "]",
    "^",
    "^=",
    "{",
    "|",
    "|=",
    "}"
};
static const int symbolTypes[SYMBOLS_SIZE] = {
    T_ARITHMETIC_OPERATOR,
    T_COMPARATION_OPERATOR,
    T_ARITHMETIC_OPERATOR,
    T_ASSIGN_OPERATOR,
    T_ARITHMETIC_OPERATOR,
    T_ASSIGN_OPERATOR,
    T_ARITHMETIC_OPERATOR,
    T_ASSIGN_OPERATOR,
    T_LEFT_PAR,
    T_RIGHT_PAR,
    T_ARITHMETIC_OPERATOR,
    T_ASSIGN_OPERATOR,
    T_ARITHMETIC_OPERATOR,
    T_ARITHMETIC_OPERATOR,
    T_ASSIGN_OPERATOR,
    T_COMMA,
    T_ARITHMETIC_OPERATOR,
    T_ARITHMETIC_OPERATOR,
    T_ASSIGN_OPERATOR,
    T_POINT,
    T_ARITHMETIC_OPERATOR,
    T_ASSIGN_OPERATOR,
    T_TERNARY_SEPARATOR,
    T_SEMICOLON,
    T_COMPARATION_OPERATOR,
    T_COMPARATION_OPERATOR,
    T_EQUAL,
    T_COMPARATION_OPERATOR,
    T_COMPARATION_OPERATOR,
    T_COMPARATION_OPERATOR,
    T_TERNARY_OPERATOR,
    T_LEFT_SQUARE,
    T_RIGHT_SQUARE,
    T_ARITHMETIC_OPERATOR,
    T_ASSIGN_OPERATOR,
    T_LEFT_CURLY,
    T_ARITHMETIC_OPERATOR,
    T_ASSIGN_OPERATOR,
    T_RIGHT_CURLY

};

#endif