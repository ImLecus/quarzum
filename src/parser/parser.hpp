#include "../tokenizer/tokenlist.hpp"
#include "../ast/expressions.hpp"
#include "../ast/literals.hpp"
#include "../ast/nodes.hpp"
#include "../ast/declarations.hpp"
#include "../ast/statements.hpp"
#include "../error.hpp"
#include <vector>

#define EXPECT_SEMICOLON if(get(i).getType() != semicolon){throwSyntaxError("Expected semicolon");}

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
                        EXPECT_SEMICOLON
                        continue;
                    }

                    throwSyntaxError("Expected semicolon or expression");

                }
            }

            if(get(i).getType() == identifier){
                ASTNode* name = new Identifier(get(i).getValue());
                if(get(i +1).getType() == equal){
                    i += 2;
                    root.add(new VariableRedeclaration(name, parseExpression()));
                    EXPECT_SEMICOLON
                    continue;
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
            return new IntegerLiteral("0");
        }
        if(lastType == "num"){
            return new NumericLiteral("0");
        }
        if(lastType == "bool"){
            return new BoolLiteral("false");
        }
        if(lastType == "var"){
            return new NullLiteral();
        }
        return nullptr;
    }

    TokenList getExpressionTerms(){
        TokenList expressionTokens;
        TokenType validTerms[] = {
            int_literal, 
            num_literal, 
            true_literal, 
            false_literal, 
            char_literal, 
            string_literal, 
            null_literal,
            plus,
            minus
        };
        while(true){
            for(TokenType term : validTerms){
                if(get(i).getType() == term){
                    expressionTokens.add(get(i++));
                    continue;
                }
            }
            return expressionTokens;
        }
    }

    ASTNode* parseExpression(TokenList expressionTokens = TokenList()){
        if(expressionTokens.isEmpty()) {
            expressionTokens = getExpressionTerms();
        }
        
        // Parsing literals
        if(expressionTokens.size() == 1){
            switch (expressionTokens[0].getType())
            {
            case int_literal:
                return new IntegerLiteral(expressionTokens[0].getValue());
            case num_literal:
                return new NumericLiteral(expressionTokens[0].getValue());
            case true_literal:
            case false_literal:
                return new BoolLiteral(expressionTokens[0].getValue());
            case char_literal:
                return new CharLiteral(expressionTokens[0].getValue());
            case string_literal:
                return new StringLiteral(expressionTokens[0].getValue());
            case null_literal:
                return new NullLiteral();
            default:
                throwSyntaxError("Invalid expression");
                break;
            }
        }
        // Parsing composite expressions
        for(unsigned int i = 0; i <= expressionTokens.size(); ++i){
            if(expressionTokens[i].getType() == plus){
                return new BinaryExpression(
                    "+",
                    parseExpression(expressionTokens.split(0,i)),
                    parseExpression(expressionTokens.split(i + 1, expressionTokens.size()))
                    
                );
            }
        }


        
        return nullptr;
    }
};