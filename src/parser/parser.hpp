#include "../tokenizer/tokenlist.hpp"
#include "../ast/expressions.hpp"
#include "../ast/literals.hpp"
#include "../ast/nodes.hpp"
#include "../ast/declarations.hpp"
#include "../ast/statements.hpp"
#include "../error.hpp"
#include "identation.hpp"
#include <array>
#include <stack>

#define EXPECT_SEMICOLON if(get(i).getType() != semicolon){throwSyntaxError("Expected semicolon", tokens.getLine(i));}

using std::vector;


class Parser {
public:
    Parser(const TokenList& tokens): tokens(tokens) {}

    const RootNode parse(){
        RootNode root = RootNode();
        identation.open(&root);

        for(i = 0; i < tokens.size(); ++i){


            
            if(auto dec = parseTypeAndId()){
                if(get(i).getType() == semicolon){
                    if(dec->array and dec->sizeDefined){
                        identation.getLastLayer()->add(new ArrayDeclaration(dec->type, dec->id,{},dec->constant, dec->arrayLength));
                        continue;
                    }
                    identation.getLastLayer()->add(new VariableDeclaration(dec->type, dec->id, getNullValue(dec->type), dec->constant));
                    continue;
                }
                if(get(i).getType() == equal){
                    ++i;
                    if(dec->array and not dec->sizeDefined){
                        if(get(i).getType() != left_curly){
                            throwSyntaxError("Expected array elements", tokens.getLine(i));
                        }
                        ++i;
                        vector<ASTNode*> elements = parseAgumentsInCall();
                        if(get(i).getType() != right_curly){
                            throwSyntaxError("Expected array elements", tokens.getLine(i));  
                        }
                        ++i;
                        identation.getLastLayer()->add(new ArrayDeclaration(dec->type, dec->id,elements,dec->constant, new IntegerLiteral(std::to_string(elements.size()))));
                        continue;
                    }
                    identation.getLastLayer()->add(new VariableDeclaration(dec->type, dec->id, parseExpression(), dec->constant));
                    EXPECT_SEMICOLON
                    continue;
                        
                }
                if(get(i).getType() == left_par){
                    if(dec->array and dec->sizeDefined){
                        throwSyntaxError("Array-type functions must not have defined size", tokens.getLine(i));
                    }
                    ++i;
                    vector<ASTNode*> args = parseArguments();
                    if(get(i).getType() == right_par and get(++i).getType() == left_curly){
                        identation.open<FunctionContainer>(new FunctionContainer(dec->id,dec->type,args));
                        continue;
                    }
                    throwSyntaxError("Expected arguments or function body", tokens.getLine(i));
                }
                throwSyntaxError("Expected semicolon or expression", tokens.getLine(i));
            }




            if(get(i).getType() == import_keyword){
                ++i;
                bool valid = true;
                vector<ASTNode*> imports;
                while(valid){
                    if(get(i).getType() == identifier){
                        imports.push_back(new Identifier(get(i++).getValue()));
                        if(get(i).getType() == comma){
                            ++i;
                            continue;
                        }
                        valid = false;
                        continue;
                    }
                    valid = false;
                }
                if(imports.empty()){
                    throwSyntaxError("Invalid import statement", tokens.getLine(i));
                }
                std::string location;
                if(get(i).getType() != from_keyword or get(i + 1).getType() != string_literal){
                    throwSyntaxError("Missing import path", tokens.getLine(i));
                }
                location = get(++i).getValue();
                identation.getLastLayer()->add(new ImportStatement(imports, location));
                continue;
            }

            if(get(i).getType() == right_curly and identation.hasLayers()){
                identation.close();
                continue;
            }

            if(get(i).getType() == return_keyword){
                ++i;
                parseSimpleStatement(new Return(parseExpression()));
                continue;
            }

            if(get(i).getType() == exit_keyword){
                ++i;
                parseSimpleStatement(new Exit(parseExpression()));
                continue;
            }
            if(get(i).getType() == break_keyword){
                ++i;
                parseSimpleStatement(new Break());
                continue;
            }
            if(get(i).getType() == continue_keyword){
                ++i;
                parseSimpleStatement(new Continue());
                continue;
            }

            if(get(i).getType() == foreach_keyword){
                if(get(i + 1).getType() != left_par){
                    throwSyntaxError("Expected foreach statement", tokens.getLine(i));
                }
                i += 2;
                Symbol* symbol = parseTypeAndId();
                if(symbol == nullptr){
                    throwSyntaxError("Expected iterable declaration", tokens.getLine(i));
                }
                if(get(i).getType() != in_keyword){
                    throwSyntaxError("Expected 'in' keyword", tokens.getLine(i));
                }
                ++i;
                ASTNode* expression = parseExpression();
                if(instanceOf<NullExpression>(expression)){
                    throwSyntaxError("Expected iterable expression", tokens.getLine(i));
                }
                if(get(i).getType() != right_par or get(++i).getType() != left_curly){
                    throwSyntaxError("Expected ')'", tokens.getLine(i));
                }
                identation.open<ForeachContainer>(new ForeachContainer(symbol->id, expression, symbol->type));
            }

            if(get(i).getType() == identifier){
                Identifier* name = new Identifier(get(i).getValue());
                if(get(i +1).getType() == equal){
                    i += 2;
                    identation.getLastLayer()->add(new VariableRedeclaration(name, parseExpression()));
                    EXPECT_SEMICOLON
                    continue;
                }
                if(get(i +1).getType() == left_par){
                    i += 2;
                    vector<ASTNode*> args = parseAgumentsInCall();
                    if(get(i).getType() != right_par){
                        throwSyntaxError("Expected arguments or function call", tokens.getLine(i));                       
                    }
                    identation.getLastLayer()->add(new FunctionCall(name, args));
                    ++i;
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
            if(get(i).getType() == while_keyword){
                parseWhileStatement();
            }

            if(get(i).getType() == enum_keyword){
                if(get(i + 1).getType() != identifier){
                    throwSyntaxError("Expected identifier", tokens.getLine(i));
                }
                Identifier* name = new Identifier(get(i + 1).getValue());
                ASTNode* extend = nullptr;
                i += 2;
                if(get(i).getType() == arrow and get(i + 1).isTypeKeyword()){
                    extend = new Type(get(i + 1).getValue());
                    i += 2;
                }
                if(get(i).getType() != left_curly){
                    throwSyntaxError("Expected enumeration", tokens.getLine(i));
                }
                
                ++i;
                if(extend == nullptr){extend = new Type("int");}    
                vector<ASTNode*> elements = parseEnumElements();
                if(get(i).getType() != right_curly){
                    throwSyntaxError("Expected end of enumeration", tokens.getLine(i));
                }
                identation.getLastLayer()->add(new EnumStatement(elements, name, extend));
                continue;
                
            }
            
        }

        root.print();
        return root;
    }

private:
    TokenList tokens;
    size_t i;
    std::string lastType;
    IdentationManager identation;

    Token get(const size_t& index){
        if(index <= tokens.size()){
            return tokens[index];
        }
        return ERROR_TOKEN;
    }
    
    
    Literal* getNullValue(Type* type){
        if(type->type == "int"){
            return new IntegerLiteral("0");
        }
        if(type->type == "num"){
            return new NumericLiteral("0");
        }
        if(type->type == "bool"){
            return new BoolLiteral("false");
        }
        if(type->type == "var"){
            return new NullLiteral();
        }
        if(type->type == "char"){
            return new CharLiteral("''");
        }
        if(type->type == "string"){
            return new StringLiteral("\"\"");
        }
        return nullptr;
    }

    TokenList getExpressionTerms(){
        TokenList expressionTokens;
        while(true){
            if(get(i).isOperator() or get(i).isLiteral() or get(i).getType() == identifier){
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
        
        if(expressionTokens.size() == 0){
            return new NullExpression();
        }
        // Parsing literals
        if(expressionTokens.size() == 1){
            switch (expressionTokens[0].getType())
            {
            case int_literal:
                return new IntegerLiteral(expressionTokens[0].getValue());
            case num_literal:
                return new BoolLiteral(expressionTokens[0].getValue());
            case char_literal:
                return new CharLiteral(expressionTokens[0].getValue());
            case string_literal:
                return new StringLiteral(expressionTokens[0].getValue());
            case null_literal:
                return new NullLiteral();
            case identifier:
                return new Identifier(expressionTokens[0].getValue());
            default:
                throwSyntaxError("Invalid expression", tokens.getLine(i));
                break;
            }
        }
        // Parsing unary expressions
        if(expressionTokens.size() == 2){
            if(expressionTokens[0].getType() == not_op and expressionTokens[1].isLiteral()){
                return new UnaryExpression("not", parseExpression(expressionTokens.split(1,expressionTokens.size())));
            }
            if(expressionTokens[0].getType() == minus and expressionTokens[1].isLiteral()){
                return new UnaryExpression("-", parseExpression(expressionTokens.split(1,expressionTokens.size())));
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

        throwSyntaxError("Invalid expression", tokens.getLine(i));
        return nullptr;
    }

    //
    // PARSING RELATED
    //

    void parseIfStatement(){
        if(get(++i).getType() != left_par){
            throwSyntaxError("Expected condition", tokens.getLine(i));
        } 
        ++i;
        ASTNode* condition = parseExpression();
        if(get(i).getType() != right_par){
            throwSyntaxError("Expected condition", tokens.getLine(i));
        }
        if(get(i+ 1).getType() != left_curly){
            throwSyntaxError("Expected if body", tokens.getLine(i));
        }
        identation.open<IfContainer>(new IfContainer(condition));
    }

    void parseWhileStatement(){
        if(get(++i).getType() != left_par){
            throwSyntaxError("Expected condition", tokens.getLine(i));
        }
        ++i;
        ASTNode* condition = parseExpression();
        if(get(i).getType() != right_par){
            throwSyntaxError("Expected condition", tokens.getLine(i));
        }
        if(get(i+ 1).getType() != left_curly){
            throwSyntaxError("Expected while body", tokens.getLine(i));
        }
        identation.open<WhileContainer>(new WhileContainer(condition));
    }

    void parseModuleStatement(){
        ++i;
        bool moduleClass = false;
        if(get(i).getType() == class_keyword){
            moduleClass = true;
            ++i;
        }
        if(get(i).getType() != identifier){
           throwSyntaxError("Expected identifier", tokens.getLine(i));
        }
        ASTNode* identifier = new Identifier(get(i++).getValue());
        if(get(i).getType() != left_curly){
            throwSyntaxError("Expected module body", tokens.getLine(i));
        }
        identation.open<ModuleContainer>(new ModuleContainer(identifier, moduleClass));
    }

    vector<ASTNode*> parseArguments(){
        vector<ASTNode*> arguments;
        bool valid = true;
        while(valid){
            if(not get(i).isTypeKeyword() or get(i + 1).getType() != identifier){
                valid = false;
                continue;
            }
            ASTNode* type = new Type(get(i++).getValue());
            std::array<ASTNode*, 2> nameAndValue = parseIdWithOptionalValue();
            arguments.push_back(new Argument(type, nameAndValue[0], nameAndValue[1]));
            if(get(i).getType() == comma){
                ++i;
                continue;
            }
            valid = false;
        }
        return arguments;
    }

    vector<ASTNode*> parseEnumElements(){
        vector<ASTNode*> elements;
        bool valid = true;
        while(valid){
            if(get(i).getType() != identifier){
                valid = false;
            }
            std::array<ASTNode*, 2> nameAndValue = parseIdWithOptionalValue();
            elements.push_back(new EnumElement(nameAndValue[0], nameAndValue[1]));
            if(get(i).getType() != comma){
                valid = false;
                continue;
            }
            ++i;
        }
        return elements;
    }

    /*
    * Searches for the structure id = value |  id. 
    * Throws a syntax error if it can't be found.
    */
    std::array<ASTNode*, 2> parseIdWithOptionalValue(){
        Identifier* name;
        ASTNode* value = nullptr;
        if(get(i).getType() == identifier){
            name = new Identifier(get(i++).getValue());
            if(get(i).getType() == equal){
                ++i;
                value = parseExpression();
                if(instanceOf<NullExpression>(value)){
                    throwSyntaxError("Invalid default value assignation", tokens.getLine(i));
                }
            }
        }
        return {name,value};
    }

    vector<ASTNode*> parseAgumentsInCall(){
        vector<ASTNode*> arguments;
        bool valid = true;
        while(valid){
            ASTNode* expression = parseExpression();
            if(instanceOf<NullExpression>(expression)){
                valid = false;
                continue;
            }
            arguments.push_back(expression);
            if(get(i).getType() != comma){
                valid = false;
                continue;
            }
            ++i;
        }
        return arguments;
    }

    template<typename T, typename U>
    bool instanceOf(const U& object){
        if(T* type = dynamic_cast<T*>(object)){
            return true;
        }
        return false;
    }
      
    void parseSimpleStatement(ASTNode* node){
        identation.getLastLayer()->add(node);
        EXPECT_SEMICOLON;
    }

    /*
    * Searches for the sequence [type] [arrayDefinition?] [name]
    */
    Symbol* parseTypeAndId(){
        Identifier* name;
        Type* type;
        bool isConst = get(i - 1).getType() == const_keyword;
        bool isArray = false;
        bool sizeDefined = false;
        ASTNode* length = nullptr;

        if(not get(i).isTypeKeyword()){
            return nullptr;
        }
        type = new Type(get(i++).getValue());

        if(get(i).getType() == left_square){
            i++;
            isArray = true;
            ASTNode* expression = parseExpression();
            sizeDefined = not instanceOf<NullExpression>(expression);
            if(sizeDefined and get(i).getType() != right_square){
                throwSyntaxError("Expected array initializer", tokens.getLine(i));

            }
            length = expression;
            if(get(i++).getType() != right_square){
                throwSyntaxError("Expected array initializer", tokens.getLine(i));
            }
            type->type += "[]";
        }
        
        if(get(i).getType() != identifier){
           return nullptr;
        }
        name = new Identifier(get(i++).getValue());
        return new Symbol(type,name, isConst, isArray, sizeDefined, length);
    }
};