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

using std::vector;


class Parser {
public:
    Parser(const TokenList& tokens): tokens(tokens) {}

    const RootNode parse(){
        RootNode root = RootNode();
        identation.open(&root);

        for(i = 0; i < tokens.size(); ++i){

            if(auto dec = parseTypeAndId()){
                if(ask(semicolon)){
                    if(dec->array and dec->sizeDefined){
                        identation.addElement(new ArrayDeclaration(dec->type, dec->id,{},dec->constant, dec->arrayLength));
                        continue;
                    }
                    identation.addElement(new VariableDeclaration(dec, getNullValue(dec->type)));
                    continue;
                }
                if(ask(equal)){
                    ++i;
                    if(dec->array and not dec->sizeDefined){
                        expect(left_curly, "Expected array elements");
                        vector<ASTNode*> elements = parseAgumentsInCall();
                        expect(right_curly, "Expected array elements");
                        identation.addElement(new ArrayDeclaration(dec->type, dec->id,elements,dec->constant, new IntegerLiteral(std::to_string(elements.size()))));
                        continue;
                    }
                    identation.addElement(new VariableDeclaration(dec, parseExpression()));
                    expect(semicolon, "Expected semicolon");
                    continue;
                        
                }
                if(ask(left_par)){
                    if(dec->array and dec->sizeDefined){
                        throwSyntaxError("Array-type functions must not have defined size", tokens.getLine(i));
                    }
                    ++i;
                    vector<ASTNode*> args = parseArguments();
                    if(ask(right_par) and ask(left_curly, 1)){
                        ++i;
                        identation.open<FunctionContainer>(new FunctionContainer(dec->id,dec->type,args));
                        continue;
                    }
                    throwSyntaxError("Expected arguments or function body", tokens.getLine(i));
                }
                throwSyntaxError("Expected semicolon or expression", tokens.getLine(i));
            }

            


            else if(ask(import_keyword)){
                parseImport();
            }

            else if(ask(right_curly) and identation.hasLayers()){
                identation.close();
            }

            else if(ask(return_keyword)){
                ++i;
                parseSimpleStatement(new Return(parseExpression()));
            }

            else if(ask(exit_keyword)){
                ++i;
                parseSimpleStatement(new Exit(parseExpression()));
            }
            else if(ask(break_keyword)){
                ++i;
                parseSimpleStatement(new Break());
            }
            else if(ask(continue_keyword)){
                ++i;
                parseSimpleStatement(new Continue());
            }

            else if(ask(foreach_keyword)){
                ++i;
                expect(left_par, "Expected foreach statement");
                Symbol* symbol = parseTypeAndId();
                if(symbol == nullptr){
                    throwSyntaxError("Expected iterable declaration", tokens.getLine(i));
                }
                expect(in_keyword, "Expected 'in' keyword");
                ASTNode* expression = parseExpression();
                if(instanceOf<NullExpression>(expression)){
                    throwSyntaxError("Expected iterable expression", tokens.getLine(i));
                }
                if(not ask(right_par) or get(++i).getType() != left_curly){
                    throwSyntaxError("Expected ')'", tokens.getLine(i));
                }
                identation.open<ForeachContainer>(new ForeachContainer(symbol->id, expression, symbol->type));
            }

            else if(ask(identifier)){
                
                Identifier* name = new Identifier(get(i).getValue());
                if(ask(equal, 1)){
                    i += 2;
                    identation.addElement(new VariableRedeclaration(name, parseExpression()));
                    expect(semicolon, "Expected semicolon");
                    continue;
                }
                
                if(ask(left_par, 1)){
                    i += 2;
                    vector<ASTNode*> args = parseAgumentsInCall();
                    expect(right_par, "Expected arguments or function call");
                    identation.addElement(new FunctionCall(name, args));
                   
                    expect(semicolon, "Expected semicolon");
                    continue;
                }
            }

            else if(ask(module_keyword)){
                parseModuleStatement();
            }

            else if(ask(if_keyword)){
                parseIfStatement();
            }
            else if(ask(while_keyword)){
                parseWhileStatement();
            }

            else if(ask(enum_keyword)){
                parseEnum();
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

    bool ask(const TokenType type,const int8_t distance = 0){
        return get(i + distance).getType() == type;
    }

    void expect(const TokenType t, const char* description){
        if(get(i).getType() != t){
            throwSyntaxError(description, tokens.getLine(i));
            return;
        }
        ++i;
    }

    /**
     * EXPRESSION PARSING
    */
    
    Literal* getNullValue(Type* type){
        if(type->type.find("int") != std::string::npos ){
            return new IntegerLiteral("0");
        }
        if(type->type == "num" or type -> type == "num64" or type -> type == "decimal"){
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
        throwTypeError("Unable to find null value for type " + type->type);
        return nullptr;
    }

    TokenList getExpressionTerms(){
        TokenList expressionTokens;
        while(true){
            if(get(i).isOperator() or get(i).isLiteral() or ask(identifier)){
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

    /*
    * STATEMENT PARSING
    */
    void parseEnum(){
        ++i;
        expect(identifier, "Expected identifier");
        Identifier* name = new Identifier(get(i - 1).getValue());
        ASTNode* extend = nullptr;
        
        if(ask(arrow) and get(i + 1).isTypeKeyword()){
            extend = new Type(get(i + 1).getValue());
            i += 2;
        }
        expect(left_curly, "Expected enumeration");
        if(extend == nullptr){extend = new Type("int");}    
        vector<ASTNode*> elements = parseEnumElements();
        expect(right_curly, "Expected end of enumeration");
        identation.addElement(new EnumStatement(elements, name, extend));
    }

    void parseImport(){
        ++i;
        bool valid = true;
        vector<ASTNode*> imports;
        while(valid){
            if(ask(identifier)){
                imports.push_back(new Identifier(get(i++).getValue()));
                if(ask(comma)){
                    ++i;
                    continue;
                }
                valid = false;
            }
            valid = false;
        }
        if(imports.empty()){
            throwSyntaxError("Invalid import statement", tokens.getLine(i));
        }
        std::string location;
        if(not ask(from_keyword) or not ask(string_literal, 1)){
            throwSyntaxError("Missing import path", tokens.getLine(i));
        }
        location = get(++i).getValue();
        identation.addElement(new ImportStatement(imports, location));
    }

    void parseIfStatement(){
        ++i;
        expect(left_par, "Expected condition");
        ASTNode* condition = parseExpression();
        expect(right_par, "Expected condition");
        expect(left_curly, "Expected if body");
        identation.open<IfContainer>(new IfContainer(condition));
    }

    void parseWhileStatement(){
        ++i;
        expect(left_par, "Expected condition");
        ASTNode* condition = parseExpression();
        expect(right_par, "Expected condition");
        expect(left_curly, "Expected while body");
        identation.open<WhileContainer>(new WhileContainer(condition));
    }

    void parseModuleStatement(){
        ++i;
        bool moduleClass = false;
        if(ask(class_keyword)){
            moduleClass = true;
            ++i;
        }
        expect(identifier, "Expected identifier");
        ASTNode* identifier = new Identifier(get(i - 1).getValue());
        if(not ask(left_curly)){
            throwSyntaxError("Expected module body", tokens.getLine(i));
        }
        identation.open<ModuleContainer>(new ModuleContainer(identifier, moduleClass));
    }

    vector<ASTNode*> parseArguments(){
        vector<ASTNode*> arguments;
        bool valid = true;
        while(valid){
            Symbol* symbol = parseTypeAndId();
            if(symbol == nullptr){
                valid = false;
                continue;
            }
            ASTNode* value = parseExpression();
            if(instanceOf<NullExpression>(value)){
                arguments.push_back(new Argument(symbol));
            }
            else{
                arguments.push_back(new Argument(symbol, value));
            }
            
            if(ask(comma)){
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
            if(not ask(identifier)){
                valid = false;
            }
            std::array<ASTNode*, 2> nameAndValue = parseIdWithOptionalValue();
            elements.push_back(new EnumElement(nameAndValue[0], nameAndValue[1]));
            if(not ask(comma)){
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
        if(ask(identifier)){
            name = new Identifier(get(i++).getValue());
            if(ask(equal)){
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
            if(not ask(comma)){
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
        identation.addElement(node);
        expect(semicolon, "Expected semicolon");;
    }

    /*
    * Searches for the sequence [type] [arrayDefinition?] [name]
    */
    Symbol* parseTypeAndId(){
        Identifier* name;
        Type* type;
        bool isConst = ask(const_keyword, -1);
        bool isArray = false;
        bool sizeDefined = false;
        ASTNode* length = nullptr;

        if(not get(i).isTypeKeyword()){
            return nullptr;
        }
        type = new Type(get(i).getValue());
        i++;
        if(ask(left_square)){
            i++;
            isArray = true;
            ASTNode* expression = parseExpression();
            sizeDefined = not instanceOf<NullExpression>(expression);
            if(sizeDefined and not ask(right_square)){
                throwSyntaxError("Expected array initializer", tokens.getLine(i));
            }
            length = expression;
            if(get(i++).getType() != right_square){
                throwSyntaxError("Expected array initializer", tokens.getLine(i));
            }
            type->type += "[]";
        }
        
        if(not ask(identifier)){
            --i;
           return nullptr;
        }
        name = new Identifier(get(i++).getValue());
        return new Symbol(type,name, isConst, isArray, sizeDefined, length);
    }
};