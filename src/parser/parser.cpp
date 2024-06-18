#include "parser.hpp"
using std::vector;

template<typename T, typename U>
bool instanceOf(const U& object){
    if(T* type = dynamic_cast<T*>(object)){
        return true;
    }
    return false;
}

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


Parser::Parser(const TokenList& tokens): tokens(tokens) {}

const RootNode Parser::parse(){
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
            parseSimpleStatement(new ReturnStatement(parseExpression()));
        }

        else if(ask(exit_keyword)){
            ++i;
            parseSimpleStatement(new ExitStatement(parseExpression()));
        }
        else if(ask(break_keyword)){
            ++i;
            parseSimpleStatement(new BreakStatement());
        }
        else if(ask(continue_keyword)){
            ++i;
            parseSimpleStatement(new ContinueStatement());
        }

        else if(ask(foreach_keyword)){
            ++i;
            expect(left_par, "Expected foreach statement");
            Symbol* symbol = parseTypeAndId();
            if(symbol == nullptr){
                throwSyntaxError("Expected iterable declaration", tokens.getLine(i));
            }
            expect(in_keyword, "Expected 'in' keyword");
            Expression* expression = parseExpression();
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
            bool moduleClass = false;
            if(ask(class_keyword, 1)){
                moduleClass = true;
                ++i;
            }
            Identifier* identifier = getIdentifier();
            expect(left_curly, "Expected module body");
            identation.open<ModuleContainer>(new ModuleContainer(identifier, moduleClass));
            --i;
        }

        else if(ask(class_keyword)){
            Identifier* id = getIdentifier();
            Type* type = parseInheritance();
            expect(left_curly, "Expected class body");
            identation.open<ClassContainer>(new ClassContainer(id,type));
            --i;
        }

        else if(ask(do_keyword)){
            ++i;
            expect(left_curly, "Expected '{'");
            identation.open<DoContainer>(new DoContainer());
            --i;
        }

        else if(ask(if_keyword)){
            ++i;
            expect(left_par, "Expected condition");
            Expression* condition = parseExpression();
            expect(right_par, "Expected condition");
            expect(left_curly, "Expected if body");
            identation.open<IfContainer>(new IfContainer(condition));
            --i;
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


Token Parser::get(const size_t& index){
    if(index <= tokens.size()){
        return tokens[index];
    }
    return ERROR_TOKEN;
}
bool Parser::ask(const TokenType type,const int8_t distance){
    return get(i + distance).getType() == type;
}

void Parser::expect(const TokenType t, const char* description){
    if(not ask(t)){
        throwSyntaxError(description, tokens.getLine(i));
        return;
    }
    ++i;
}

Expression* Parser::parseExpression(TokenList expressionTokens){
    if(expressionTokens.isEmpty()) {
        while(true){
            if(get(i).isOperator() or get(i).isLiteral() or ask(identifier)){
                expressionTokens.add(get(i++));
                continue;
            }
            break;
        }
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

Identifier* Parser::getIdentifier(){
    ++i; 
    expect(identifier, "Expected identifier");
    return new Identifier(get(i - 1).getValue());
}

/*
* STATEMENT PARSING
*/
void Parser::parseEnum(){
    Identifier* name = getIdentifier();
    Type* extend = parseInheritance();
    expect(left_curly, "Expected enumeration");
    if(extend == nullptr){extend = new Type("int");}    
    vector<Element*> elements = parseEnumElements();
    expect(right_curly, "Expected end of enumeration");
    identation.addElement(new EnumStatement(elements, name, extend));
    --i;
}

void Parser::parseImport(){
    ++i;
    bool valid = true;
    vector<Identifier*> imports;
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

Type* Parser::parseInheritance(){
    if(ask(arrow)){
        if(get(i + 1).isTypeKeyword()){
            i += 2;
            return new Type(get(i - 1).getValue());
        }
        expect(identifier, "Expected inheritance");
    }
    return nullptr;
}

void Parser::parseWhileStatement(){
    ++i;
    expect(left_par, "Expected condition");
    Expression* condition = parseExpression();
    expect(right_par, "Expected condition");
    if(ask(semicolon)){
        identation.addElement(new WhileStatement(condition));
        return;
    }
    expect(left_curly, "Expected while body");
    identation.open<WhileContainer>(new WhileContainer(condition));
    --i;
}

vector<ASTNode*> Parser::parseArguments(){
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

vector<Element*> Parser::parseEnumElements(){
    vector<Element*> elements;
    bool valid = true;
    while(valid){
        valid = ask(identifier);
        if(not valid){break;}
        elements.push_back(parseIdWithOptionalValue());
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
Element* Parser::parseIdWithOptionalValue(){
    Identifier* name;
    Expression* value = nullptr;
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
    return new Element(name,value);
}

vector<ASTNode*> Parser::parseAgumentsInCall(){
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


    
void Parser::parseSimpleStatement(Statement* node){
    identation.addElement(node);
    expect(semicolon, "Expected semicolon");
    --i;
}

/*
* Searches for the sequence [type] [arrayDefinition?] [name]
*/
Symbol* Parser::parseTypeAndId(){
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
