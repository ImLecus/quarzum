/*
 * Quarzum Compiler - token.h
 * Version 1.0, 04/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#pragma once

/**
 * @brief Defines a Token type as an uint8 (byte).
 */
enum class TokenType : uint8_t {
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
};

/**
 * @brief Defines a Token. Used as basic interpretation structure by the compiler.
 */
struct Token {
    inline bool isOperator() const noexcept {
        return this->type == TokenType::ArithmeticOperator or this->type == TokenType::ComparationOperator;
    };
    const TokenType type;
    const char* value;
    const uint32_t line;
    const uint32_t column;
};

/**
 * @brief Defines an error as a Token. 
 * Used to not interrumpt by force the toolchain.
 */
constexpr Token ERROR_TOKEN = {TokenType::TokenError, nullptr, 0, 0};