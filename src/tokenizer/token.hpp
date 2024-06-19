#pragma once
#include "tokentype.hpp"
#include <unordered_map>
#include <string>

class Token {
public:
    explicit Token(const TokenType type, const std::string& value){
        this->type = type;
        this->value = value;
    }

    bool isSymbol() const {
        return (u_int8_t)this->type >= 0x60 and (u_int8_t)this->type <= 0x7F;
    }

    bool isKeyword() const {
        return (u_int8_t)this->type <= 0x2F;
    }
    bool isTypeKeyword() const {
        return (u_int8_t)this->type <= 0x0F or this->type == identifier;
    }
    bool isOperator() const {
        return (u_int8_t)this->type >= 0x30 and (u_int8_t)this->type <= 0x5F;
    }

    bool isAssignOperator() const {
        return this->type == equal or this->type == greater_eq or this->type == lower_eq or (this->type >= 70 and this->type < 77);
    }

    bool isUnaryOperator() const {
        return this->type == plus_unary or this->type == minus_unary;
    }

    bool isAccessKeyword() const {
        return this->type == public_keyword or this->type == private_keyword or this->type == protected_keyword;
    }

    bool isLiteral() const {
        return (u_int8_t)this->type >= 0x80 and (u_int8_t)this->type <= 0x9F;
    }

    u_int8_t getPriority() const {
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

    std::string getValue() const{
        return this -> value;
    }

    TokenType getType() const{
        return this -> type;
    }

private:
    TokenType type;
    std::string value;
};

const Token ERROR_TOKEN = Token(token_error, "");