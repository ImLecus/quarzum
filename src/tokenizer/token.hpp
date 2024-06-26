#pragma once
#include "tokentype.hpp"

namespace Quarzum::Lexer {
struct Token {
    Token(): type(TokenType::token_error), value(""), line(0), column(0) {}
    Token(const TokenType& type, const std::string& value, const uint32_t line, const uint32_t column): 
    type(type), value(value), line(line), column(column){}

    inline bool isSymbol() const noexcept {
        return this->type >= 0x60 and this->type <= 0x7F;
    }

    inline bool isKeyword() const noexcept {
        return this->type <= 0x2F;
    }

    inline bool isOperator() const noexcept {
        return this->type == ARITHMETIC_OPERATOR or this->type == COMPARATION_OPERATOR;
    }

    const TokenType type;
    const std::string value;
    const uint32_t line;
    const uint32_t column;
};

}