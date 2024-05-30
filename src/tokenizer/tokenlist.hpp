#pragma once
#include "token.hpp"
#include <vector>

class TokenList {
public:
    TokenList(){}

    void add(const Token token){
        this -> tokens.push_back(token);
    }

    std::vector<Token> getItems(){
        return this -> tokens;
    }

    size_t size(){
        return this->tokens.size();
    }

    Token operator[](const size_t index){
        return this->tokens.at(index);
    }

    TokenList split(unsigned int from, const unsigned int to){
        TokenList newList = TokenList();
        for(; from < to; ++from){
            newList.add(this->tokens[from]);
        }
        return newList;
    }

    bool isEmpty(){
        return this->tokens.size() == 0;
    }

private:
    std::vector<Token> tokens;
};