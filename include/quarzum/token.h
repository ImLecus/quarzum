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
#include "../core/defines.h"

/**
 * @brief Defines a Token type.
 */
enum {
    // Special tokens
    TokenError,
    TypeKeyword,
    AccessSpecifier,
    Identifier,
    Eof,
    // Keywords
    Const,
    Return,
    Module,
    Class,
    If,
    While,
    Import,
    From,
    Continue,
    Break,
    Enum,
    Foreach,
    In,

    Setup,
    Destroy,
    Do,
    For,
    Else,
    New,
    Delete,
    Persist,
    Typedef,
    Foreign,
    // Operators
    AssignOperator,
    UnaryOperator,
    ArithmeticOperator,
    ComparationOperator,
    TernaryOperator,
    TernarySeparator,

    // Symbols
    Equal,
    Semicolon,
    LeftPar,
    RightPar,
    LeftCurly,
    RightCurly,
    LeftSquare,
    RightSquare,
    Comma,
    Arrow,
    Point,
    // literals

    NumericLiteral,
    StringLiteral,
    CharLiteral,
    BoolLiteral,
    NullLiteral,
    IntLiteral
};
typedef u_int8_t TokenType;

/**
 * @brief Defines the extra information that contains a Token. It
 * includes the line, column and file name where the Token was
 * created.
 */
typedef struct {
    u_int32_t line, column;
    char* file;
} TokenInfo;

/**
 * @brief Defines a Token. Used as basic interpretation structure 
 * by the compiler. 
 */
typedef struct {
    TokenType type;
    char* value;
    TokenInfo* info;
} Token;

/**
 * @brief Defines an error as a Token. 
 * Used to not interrumpt by force the toolchain.
 */
static Token ERROR_TOKEN = {TokenError, NULL, &(TokenInfo){0,0,NULL}};
#endif