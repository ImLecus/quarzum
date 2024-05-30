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
        return (unsigned int)this->type >= 0xF0;
    }

    bool isKeyword(){
        return (unsigned int)this->type <= 0x2F;
    }
    bool isTypeKeyword(){
        return (unsigned int)this->type <= 0x05;
    }

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