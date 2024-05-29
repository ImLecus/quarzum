#pragma once
#include "token.hpp"
#include <vector>

class TokenList {
public:
    TokenList(){}

    void add(Token token){
        this -> tokens.push_back(token);
    }

    std::vector<Token> getItems(){
        return this -> tokens;
    }

    size_t size(){
        return this->tokens.size();
    }

    Token get(size_t index){
        return this->tokens.at(index);
    }

    Token operator[](size_t index){
        return get(index);
    }

private:
    std::vector<Token> tokens;
};