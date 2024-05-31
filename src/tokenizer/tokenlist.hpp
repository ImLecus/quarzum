#pragma once
#include "token.hpp"
#include <vector>

class TokenList {
public:
    TokenList(){}

    void add(const Token token){
        this -> tokens.push_back(token);
        this -> length += 1;
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
        return this->tokens.empty();
    }

    void addLine(size_t index){
        lineJumps.push_back(index);
    }

    size_t getLine(size_t index){
        for(u_int32_t i = 0; i < lineJumps.size(); ++i){
            if(lineJumps[i] > index){
                return i + 1;
            }
        }
        return lineJumps.at(lineJumps.size());
    }

    size_t length;
private:
    std::vector<Token> tokens;
    std::vector<size_t> lineJumps;
    
};