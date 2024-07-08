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
#include <stddef.h>
/**
 * @brief Defines a Token type as an uint8 (byte).
 */
typedef enum {
    // Special tokens
    TokenError,
    TypeKeyword,
    AccessSpecifier,
    Identifier,
    Comment,
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
} TokenType;

/**
 * @brief Defines a Token. Used as basic interpretation structure by the compiler.
 */
typedef struct {
    TokenType type;
    const char* value;
    unsigned int line;
    unsigned int column;
} Token;

/**
 * @brief Defines an error as a Token. 
 * Used to not interrumpt by force the toolchain.
 */
#define ERROR_TOKEN = {TokenError, NULL, 0, 0};
#endif