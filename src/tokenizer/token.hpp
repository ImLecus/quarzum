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
            {num_literal, "num_literal"},
            {plus, "plus"},
            {minus, "minus"},
            {prod, "product"},
            {division, "division"},
            {mod, "module"},
            {left_par, "left_par"},
            {right_par, "right_par"},
            {out_keyword, "out_keyword"},
            {num_keyword, "num_keyword"},
            {char_keyword, "char_keyword"},
            {string_keyword, "string_keyword"},
            {var_keyword, "var_keyword"},
            {null_literal, "null_literal"},
            {char_literal, "char_literal"},
            {string_literal, "string_literal"},
            {true_literal, "true_literal"},
            {false_literal, "false_literal"},
            {or_op, "or"},
            {and_op, "and"},
            {not_op, "not"},
            {xor_op, "xor"}
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