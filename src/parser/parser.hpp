#include "../tokenizer/tokenlist.hpp"
#include "../ast/expressions.hpp"
#include "../ast/literals.hpp"
#include <vector>

class Parser {
public:
    static std::vector<Token> parse(TokenList tokens){

        std::vector<Token> statements;
        size_t index = 0;

        for(;index < tokens.size(); ++index){

            if(tokens[index].getType() == int_keyword and tokens[index + 1].getType() == identifier){
                if(tokens[index + 2].getType() == semicolon){

                    
                }
                if(tokens[index + 2].getType() == equal and tokens[index + 3].getType() == int_literal){
                    
                }
            }
        }


        return statements;
    }

private:
};