/*
 * Quarzum Compiler - token.h
 * Version 1.0, 06/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#ifndef TOKEN_H
#define TOKEN_H

enum {
    T_TOKEN_ERROR,
    T_TYPE_KEYWORD,
    T_ACCESS_SPECIFIER,
    T_IDENTIFIER,
    T_EOF,
    T_CONST,
    T_RETURN,
    T_MODULE,
    T_CLASS,
    T_IF,
    T_WHILE,
    T_IMPORT,
    T_FROM,
    T_CONTINUE,
    T_BREAK,
    T_ENUM,
    T_FOREACH,
    T_IN,
    T_SETUP,
    T_DESTROY,
    T_DO,
    T_FOR,
    T_ELSE,
    T_NEW,
    T_DELETE,
    T_PERSIST,
    T_TYPEDEF,
    T_FOREIGN,
    T_ASSIGN_OPERATOR,
    T_UNARY_OPERATOR,
    T_ARITHMETIC_OPERATOR,
    T_COMPARATION_OPERATOR,
    T_TERNARY_OPERATOR,
    T_TERNARY_SEPARATOR,
    T_EQUAL,
    T_SEMICOLON,
    T_LEFT_PAR,
    T_RIGHT_PAR,
    T_LEFT_CURLY,
    T_RIGHT_CURLY,
    T_LEFT_SQUARE,
    T_RIGHT_SQUARE,
    T_COMMA,
    T_ARROW,
    T_POINT,
    T_NUMERIC_LITERAL,
    T_STRING_LITERAL,
    T_CHAR_LITERAL,
    T_BOOL_LITERAL,
    T_NULL_LITERAL,
    T_INT_LITERAL
};

struct token_info {
    unsigned int line, column;
    char* file;
};

struct token {
    int type;
    char* value;
    struct token_info* info;
};

#endif