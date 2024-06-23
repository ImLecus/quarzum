#pragma once
#include "token.hpp"
#include <vector>
namespace quarzum::lexer {
class TokenList {
public:
    void add(const Token token){
        tokens.push_back(token);
        ++length;
    }

    inline std::vector<Token> getItems() const noexcept{
        return tokens;
    }

    inline uint64_t size() noexcept{
        return tokens.size();
    }

    inline const Token operator[](const uint64_t index){
        return tokens.at(index);
    }

    TokenList split(uint32_t from, const uint32_t to){
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
        const uint32_t size = lineJumps.size();
        for(uint32_t i = 0; i < size; ++i){
            if(lineJumps[i] > index){
                return i + 1;
            }
        }
        return lineJumps.at(lineJumps.size());
    }

    uint64_t length;
private:
    std::vector<Token> tokens;
    std::vector<size_t> lineJumps;
};
}