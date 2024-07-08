/*
 * Quarzum Compiler - defs.c
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
#include "../../include/quarzum/defs.h"
/**
 * @brief Defines the keywords in the Quarzum language.
 * The array must be ordered, because the algorithm that
 * tries to search a keyword uses binary search.
 */
const char* keywords[KEYWORDS_SIZE] = {
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

const TokenType keywordTypes[KEYWORDS_SIZE] = {
    ComparationOperator,
    TypeKeyword,
    Break,
    TokenError, // NOT IMPLEMENTED YET
    TypeKeyword,
    Class,
    Const,
    Continue,
    TypeKeyword,
    Delete,
    Destroy,
    Do,
    Else,
    Enum,
    BoolLiteral,
    For,
    Foreach,
    Foreign,
    From,
    TypeKeyword,
    If,
    Import,
    In,
    TypeKeyword,
    TypeKeyword,
    TypeKeyword,
    TypeKeyword,
    TypeKeyword,
    Module,
    New,
    ComparationOperator,
    NullLiteral,
    TypeKeyword,
    TypeKeyword,
    TypeKeyword,
    TypeKeyword,
    ComparationOperator,
    Persist,
    AccessSpecifier,
    AccessSpecifier,
    AccessSpecifier,
    Return,
    Setup,
    TypeKeyword,
    BoolLiteral,
    TokenError, // NOT IMPLEMENTED YET
    Typedef,
    TypeKeyword,
    TypeKeyword,
    TypeKeyword,
    TypeKeyword,
    TypeKeyword,
    While,
    ComparationOperator
};
/**
 * @brief Defines the symbols in the Quarzum language.
 * It must be ordered by the ASCII code.
 * See more: https://www.asciitable.com/
 */
const char* symbols[SYMBOLS_SIZE] = {
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
const TokenType symbolTypes[SYMBOLS_SIZE] = {
    ArithmeticOperator,
    ComparationOperator,
    ArithmeticOperator,
    AssignOperator,
    ArithmeticOperator,
    AssignOperator,
    ArithmeticOperator,
    AssignOperator,
    LeftPar,
    RightPar,
    ArithmeticOperator,
    AssignOperator,
    ArithmeticOperator,
    ArithmeticOperator,
    AssignOperator,
    Comma,
    ArithmeticOperator,
    ArithmeticOperator,
    AssignOperator,
    Point,
    ArithmeticOperator,
    AssignOperator,
    TernarySeparator,
    Semicolon,
    ComparationOperator,
    ComparationOperator,
    Equal,
    ComparationOperator,
    ComparationOperator,
    ComparationOperator,
    TernaryOperator,
    LeftSquare,
    RightSquare,
    ArithmeticOperator,
    AssignOperator,
    LeftCurly,
    ArithmeticOperator,
    AssignOperator,
    RightCurly
};

#include <stdio.h>

const TokenType keywordToType(const char* keyword){
    int search = binarySearch(keyword, keywords, KEYWORDS_SIZE);
    if(search == -1){
        return Identifier;
    }
    return keywordTypes[search];
}

const TokenType symbolToType(const char* symbol){
    int search = binarySearch(symbol, symbols, SYMBOLS_SIZE);
    if(search == -1){
        return TokenError;
    }
    return symbolTypes[search];
}