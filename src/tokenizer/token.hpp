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


    std::string getValue(){
        return this -> value;
    }

    TokenType getType(){
        return this -> type;
    }

    const std::string toString(){

        static const std::unordered_map<TokenType, std::string> typeToString = {
            {identifier, "identifier"},
            {equal, "equal"},
            {int_keyword, "int_keyword"},
            {int_literal, "int_literal"},
            {semicolon, "semicolon"},
            {num_literal, "num_literal"}
        };
        try{
            return typeToString.at(this->type) + ": " + this->value;
        }
        catch(...){
            return "";
        }
        
    }

private:
    TokenType type;
    std::string value;
};

const Token ERROR_TOKEN = Token(token_error, "");