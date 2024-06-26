#pragma once
#include "token.hpp"
#include <vector>
namespace Quarzum::Lexer {
class TokenList {
public:
    TokenList(std::deque<Token> t): tokens(t) {}
    void add(const Token token){
        tokens.push_back(token);
        ++length;
    }

    inline std::deque<Token> getItems() const noexcept{
        return tokens;
    }

    inline uint64_t size() noexcept{
        return tokens.size();
    }

    inline const Token operator[](const uint64_t index){
        return tokens.at(index);
    }

    TokenList split(uint32_t from, const uint32_t to){
        TokenList newList = TokenList({});
        for(; from < to; ++from){
            newList.add(this->tokens[from]);
        }
        return newList;
    }

    inline bool isEmpty() const noexcept{
        return tokens.empty();
    }

    uint64_t length;
private:
    std::deque<Token> tokens;
};
}