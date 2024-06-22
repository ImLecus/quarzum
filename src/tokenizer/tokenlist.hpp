#pragma once
#include "token.hpp"
#include <vector>

class TokenList {
public:
    void add(const Token token){
        tokens.push_back(token);
        ++length;
    }

    inline std::vector<Token> getItems() const noexcept{
        return tokens;
    }

    inline size_t size() noexcept{
        return tokens.size();
    }

    inline const Token operator[](const size_t index){
        return tokens.at(index);
    }

    TokenList split(unsigned int from, const unsigned int to){
        TokenList newList = TokenList();
        for(; from < to; ++from){
            newList.add(this->tokens[from]);
        }
        return newList;
    }

    inline bool isEmpty() const noexcept{
        return tokens.empty();
    }

    inline void addLine(const size_t& index){
        lineJumps.push_back(index);
    }

    size_t getLine(const size_t& index) const{
        const u_int32_t size = lineJumps.size();
        for(u_int32_t i = 0; i < size; ++i){
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