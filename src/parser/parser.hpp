#include "../tokenizer/tokenlist.hpp"
#include "../ast/expressions.hpp"
#include "../ast/literals.hpp"
#include "../ast/nodes.hpp"
#include "../ast/declarations.hpp"
#include <vector>

class Parser {
public:
    Parser(TokenList tokens): tokens(tokens) {}

    RootNode parse(){
        RootNode root = RootNode();

        for(i = 0; i < tokens.size(); ++i){
            if(get(i).isKeyword()){
                lastType = get(i).getValue();
                ASTNode* type = new Type(lastType);

                if(get(i + 1).getType() == identifier){
                    ASTNode* name = new Identifier(get(i + 1).getValue());

                    if(get(i + 2).getType() == semicolon){
                        i += 2;
                        root.add(new VariableDeclaration(type, name, getNullValue()));
                        continue;
                    }

                    if(get(i + 2).getType() == equal){
                        i += 3;
                        root.add(new VariableDeclaration(type, name, parseExpression()));
                        continue;
                    }

                    throwSyntaxError("Expected semicolon or expression");

                }
            }
            
        }

        root.print();
        return root;
    }

private:
    TokenList tokens;
    size_t i;
    Token get(size_t index){
        if(index <= tokens.size()){
            return tokens[index];
        }
        return ERROR_TOKEN;
    }
    std::string lastType;
    ASTNode* getNullValue(){
        if(lastType == "int"){
            return new IntegerLiteral(0);
        }
    }

    ASTNode* parseExpression(){
        if(get(i).getType() == int_literal){
            return new IntegerLiteral(std::stoi(get(i).getValue()));
        }
        return new IntegerLiteral(0);
    }
};