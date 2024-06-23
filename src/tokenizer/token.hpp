#pragma once
#include "tokentype.hpp"
#include <string>

class Token {
public:
    explicit Token(const TokenType& type, const std::string& value): type(type), value(value){}

    inline bool isSymbol() const noexcept {
        return this->type >= 0x60 and this->type <= 0x7F;
    }

    inline bool isKeyword() const noexcept {
        return this->type <= 0x2F;
    }
    inline bool isTypeKeyword() const noexcept {
        return this->type <= 0x0F or this->type == identifier;
    }
    inline bool isOperator() const noexcept {
        return this->type >= 0x30 and this->type <= 0x5F;
    }

    inline bool isAssignOperator() const noexcept {
        return this->type == equal or this->type == greater_eq or this->type == lower_eq or (this->type >= 70 and this->type < 77);
    }

    inline bool isUnaryOperator() const noexcept {
        return this->type == plus_unary or this->type == minus_unary;
    }

    inline bool isLiteral() const noexcept {
        return this->type >= 0x80 and this->type <= 0x9F ;
    }

    u_int8_t getPriority() const noexcept {
        switch (this->type)
        {
        case or_op:
            return 0;
        case and_op:
            return 1;
        case bit_or:
            return 2;
        case bit_xor:
            return 3;
        case bit_and:
            return 4;
        case is_equal:
        case not_equal:
            return 5;
        case lower:
        case lower_eq:
        case greater:
        case greater_eq:
            return 6;
        case plus:
        case converge_sum:
        case minus:
            return 7;
        case prod:
        case division:
        case mod:
            return 8;
        case not_op:
        case bit_not:
            return 9;

        
        default:
            return __UINT8_MAX__;
        }
    }
    static const u_int8_t MAX_PRIORITY = 9;

    inline std::string getValue() const noexcept {
        return this -> value;
    }

    inline TokenType getType() const noexcept {
        return this -> type;
    }

private:
    const TokenType type;
    const std::string value;
};

const Token ERROR_TOKEN = Token(token_error, "");