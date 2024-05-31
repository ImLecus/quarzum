#pragma once
#include "tokentype.hpp"
#include <unordered_map>
#include <string>

class Token {
public:
    explicit Token(const TokenType type, const std::string value){
        this->type = type;
        this->value = value;
    }

    bool isSymbol(){
        return (u_int8_t)this->type >= 0x60 and (u_int8_t)this->type <= 0x7F;
    }

    bool isKeyword(){
        return (u_int8_t)this->type <= 0x2F;
    }
    bool isTypeKeyword(){
        return (u_int8_t)this->type <= 0x07 or this->type == identifier;
    }
    bool isOperator(){
        return (u_int8_t)this->type >= 0x30 and (u_int8_t)this->type <= 0x5F;
    }
    bool isLiteral(){
        return (u_int8_t)this->type >= 0x80 and (u_int8_t)this->type <= 0x9F;
    }

    u_int8_t getPriority(){
        switch (this->type)
        {
        case is_equal:
        case not_equal:
            return 0;
        case lower:
        case lower_eq:
        case greater:
        case greater_eq:
            return 1;
        case plus:
        case minus:
            return 2;
        case prod:
        case division:
        case mod:
            return 3;
        
        default:
            return __UINT8_MAX__;
        }
    }
    static const u_int8_t MAX_PRIORITY = 4;

    std::string getValue(){
        return this -> value;
    }

    TokenType getType(){
        return this -> type;
    }

private:
    TokenType type;
    std::string value;
};

const Token ERROR_TOKEN = Token(token_error, "");