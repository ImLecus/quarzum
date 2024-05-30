#include "../tokenizer/tokenlist.hpp"
#include "../ast/expressions.hpp"
#include "../ast/literals.hpp"
#include "../ast/nodes.hpp"
#include "../ast/declarations.hpp"
#include "../ast/statements.hpp"
#include "../error.hpp"

#define EXPECT_SEMICOLON if(get(i).getType() != semicolon){throwSyntaxError("Expected semicolon");}

class Parser {
public:
    Parser(TokenList tokens): tokens(tokens) {}

    RootNode parse(){
        RootNode root = RootNode();
        openIdentation(&root);

        for(i = 0; i < tokens.size(); ++i){

            if(get(i).getType() == right_curly and identationLayer > 1){
                closeIdentation();
                continue;
            }

            if(get(i).getType() == return_keyword){
                ++i;
                getLastLayer()->add(new Return(parseExpression()));
                EXPECT_SEMICOLON;
                continue;
            }

            if(get(i).isTypeKeyword()){
                bool constant = get(i - 1).getType() == const_keyword;
                lastType = get(i).getValue();
                ASTNode* type = new Type(lastType);

                if(get(i + 1).getType() == identifier){
                    ASTNode* name = new Identifier(get(i + 1).getValue());
                    i += 2;
                    if(get(i).getType() == left_par){
                        // TO-DO: parse arguments
                        if(get(++i).getType() == right_par and get(++i).getType() == left_curly){
                            openIdentation<FunctionContainer>(new FunctionContainer(name,type));
                            continue;
                        }
                        continue;
                    }
                    if(get(i).getType() == semicolon){
                        getLastLayer()->add(new VariableDeclaration(type, name, getNullValue(), constant));
                        continue;
                    }
                    if(get(i).getType() == equal){
                        ++i;
                        getLastLayer()->add(new VariableDeclaration(type, name, parseExpression(), constant));
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
                    getLastLayer()->add(new VariableRedeclaration(name, parseExpression()));
                    EXPECT_SEMICOLON
                    continue;
                }
                if(get(i +1).getType() == left_par and get(i + 2).getType() == right_par){
                    i += 3;
                    getLastLayer()->add(new FunctionCall(name));
                    EXPECT_SEMICOLON
                    continue;
                }
            }

            if(get(i).getType() == module_keyword){
                parseModuleStatement();
            }

            if(get(i).getType() == if_keyword){
                parseIfStatement();
            }
            
        }

        root.print();
        return root;
    }

private:
    TokenList tokens;
    size_t i;
    std::string lastType;

    Token get(size_t index){
        if(index <= tokens.size()){
            return tokens[index];
        }
        return ERROR_TOKEN;
    }
    
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
        if(lastType == "char"){
            return new CharLiteral("''");
        }
        if(lastType == "string"){
            return new StringLiteral("\"\"");
        }
        return nullptr;
    }

    TokenList getExpressionTerms(){
        TokenList expressionTokens;
        while(true){
            if(get(i).isOperator() or get(i).isLiteral()){
                expressionTokens.add(get(i++));
                continue;
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
        // Parsing composite expressions, in reverse priority order
        for(u_int8_t priority = 0; priority < Token::MAX_PRIORITY; ++priority){
            for(u_int16_t i = 0; i < expressionTokens.size(); ++i){
                if(expressionTokens[i].getPriority() == priority){
                    return new BinaryExpression(
                        expressionTokens[i].getValue(),
                        parseExpression(expressionTokens.split(0,i)),
                        parseExpression(expressionTokens.split(i + 1, expressionTokens.size()))
                    );
                }
            }
        }

        throwSyntaxError("Invalid expression");
        return nullptr;
    }

    u_int16_t identationLayer; 
    std::vector<Container*> layers;

    Container* getLastLayer(){
        return layers.at(layers.size() - 1);
    }

    template<typename T>
    void openIdentation(T* layer){
        ++identationLayer;
        layers.push_back(layer);
    }

    void closeIdentation(){
        --identationLayer;
        Container* lastLayer = getLastLayer();
        layers.pop_back();
        getLastLayer()->add(lastLayer);
    }

    void parseIfStatement(){
        if(get(++i).getType() == left_par){
            ++i;
            ASTNode* condition = parseExpression();
            if(get(i).getType() == right_par){
                if(get(i+ 1).getType() == left_curly){
                    openIdentation<IfContainer>(new IfContainer(condition));
                    return;
                }
                throwSyntaxError("Expected if body");
            }
            throwSyntaxError("Expected condition");
        }
        throwSyntaxError("Expected condition");
    }

    void parseModuleStatement(){
        bool moduleClass = false;
        if(get(i + 1).getType() == class_keyword){
            moduleClass = true;
            ++i;
        }
        if(get(i + 1).getType() == identifier){
            ASTNode* identifier = new Identifier(get(i + 1).getValue());
            if(get(i + 2).getType() == left_curly){
                openIdentation<ModuleContainer>(new ModuleContainer(identifier, moduleClass));
                return;
            }
            throwSyntaxError("Expected module body");
        }
        throwSyntaxError("Expected identifier");
    }
};