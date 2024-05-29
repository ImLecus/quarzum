#include "statement.hpp"
#include "../tokenizer/tokenlist.hpp"
#include <vector>

class Parser {
    public:
        static std::vector<Statement> parse(TokenList tokens){

            std::vector<Statement> statements;
            size_t index = 0;

            for(;index < tokens.size(); ++index){

                if(tokens[index].getType() == int_keyword and tokens[index + 1].getType() == identifier){
                    if(tokens[index + 2].getType() == semicolon){

                        statements.push_back(VarDeclarationStatement(tokens[index + 1].getValue(), "int", Expression()));
                    }
                    if(tokens[index + 2].getType() == equal and tokens[index + 3].getType() == int_literal){
                        statements.push_back(VarDeclarationStatement(tokens[index + 1].getValue(), "int", Expression()));
                    }
                }
            }


            return statements;
        }

    private:
};