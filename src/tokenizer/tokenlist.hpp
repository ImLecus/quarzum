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

    private:
        std::vector<Token> tokens;
};