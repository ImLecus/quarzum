#include "parser.hpp"
using std::vector;

template<typename T, typename U>
bool instanceOf(const U& object){
    if(T* type = dynamic_cast<T*>(object)){
        return true;
    }
    return false;
}

Literal* getNullValue(GenericType* type){
    if(type->name.find("int") != std::string::npos ){
        return new Literal(new Integer(),"0");
    }
    if(type->name == "num" or type -> name == "num64" or type -> name == "decimal"){
        return new Literal(new Number(),"0");
    }
    if(type->name == "bool"){
        return new Literal(new Boolean(),"false");
    }
    if(type->name == "var"){
        return new Literal(new NullType(), "");
    }
    if(type->name == "char"){
        return new Literal(new Character(),"''");
    }
    if(type->name == "string"){
        return new Literal(new String(),"\"\"");
    }
    throwTypeError("Unable to find null value for type " + type->name);
    return nullptr;
}

Parser::Parser(const TokenList& tokens): tokens(tokens) {}

const RootNode Parser::parse(){
    RootNode root = RootNode();
    identation.open(&root);

    for(i = 0; i < tokens.size(); ++i){
        if(auto decl = parseVar()){
            identation.addElement(decl);
            --i;

        }
        else if(auto func = parseFunction()){
            identation.open<FunctionContainer>(func);
            --i;
        }
        else if(get(i).isAccessKeyword()){
            Access access = Access(get(i).getType() - TokenType::public_keyword);
            ++i;
            if(auto decl = parseVar()){
                identation.addElement(new AtributeDeclaration(access,decl));
                --i;
            }
            else if(auto func = parseFunction()){
                identation.open<MethodContainer>(new MethodContainer(access,func));
                --i;
            }
        }

        else if(consume(for_keyword)){
            expect(left_par, "Expected '('");
            VariableDeclaration* decl;
            if(not (decl = parseVar())){
                expect(semicolon, "Expected variable declaration or semicolon");
            }
            Expression* condition = parseExpression();
            expect(semicolon, "Expected condition or semicolon");
            VariableRedeclaration* redec = parseRedec();
            expect(right_par, "Expected ')'");
            expect(left_curly, "Expected for body");
            identation.open<ForContainer>(new ForContainer(decl,condition,redec));
            --i;
        }

        else if(consume(import_keyword)){
            parseImport();
        }

        else if(ask(right_curly) and identation.hasLayers()){
            identation.close();
        }

        else if(consume(return_keyword)){
            parseSimpleStatement(new ReturnStatement(parseExpression()));
        }

        else if(consume(exit_keyword)){
            parseSimpleStatement(new ExitStatement(parseExpression()));
        }
        else if(consume(break_keyword)){
            parseSimpleStatement(new BreakStatement());
        }
        else if(consume(continue_keyword)){
            parseSimpleStatement(new ContinueStatement());
        }

        else if(consume(foreach_keyword)){
            expect(left_par, "Expected foreach statement");
            GenericType* type = parseType();
            Identifier* id = getIdentifier();
            expect(in_keyword, "Expected 'in' keyword");
            Expression* expression = parseExpression();
            if(instanceOf<NullExpression>(expression)){
               throwSyntaxError("Expected iterable expression", tokens.getLine(i));
            }
            if(not ask(right_par) or get(++i).getType() != left_curly){
              throwSyntaxError("Expected ')'", tokens.getLine(i));
            }
            identation.open<ForeachContainer>(new ForeachContainer(id, expression, type));
        }

        else if(consume(module_keyword)){
            bool moduleClass = consume(class_keyword);
            Identifier* identifier = getIdentifier();
            expect(left_curly, "Expected module body");
            identation.open<ModuleContainer>(new ModuleContainer(identifier, moduleClass));
            --i;
        }

        else if(consume(class_keyword)){
            Identifier* id = getIdentifier();
            GenericType* type = parseInheritance();
            expect(left_curly, "Expected class body");
            identation.open<ClassContainer>(new ClassContainer(id,type));
            --i;
        }

        else if(consume(do_keyword)){
            expect(left_curly, "Expected '{'");
            identation.open<DoContainer>(new DoContainer());
            --i;
        }

        else if(consume(if_keyword)){
            expect(left_par, "Expected condition");
            Expression* condition = parseExpression();
            expect(right_par, "Expected condition");
            expect(left_curly, "Expected if body");
            identation.open<IfContainer>(new IfContainer(condition));
            --i;
        }
        else if(consume(while_keyword)){
            expect(left_par, "Expected condition");
            Expression* condition = parseExpression();
            expect(right_par, "Expected condition");
            if(ask(semicolon)){
                identation.addElement(new WhileStatement(condition));
                continue;
            }
            expect(left_curly, "Expected while body");
            identation.open<WhileContainer>(new WhileContainer(condition));
            --i;
        }

        else if(consume(enum_keyword)){
            parseEnum();
        }  

        else if(auto redec = parseRedec()){
            identation.addElement(redec);
            expect(semicolon, "Expected semicolon");
            --i;
        }

        else if(consume(identifier)){
            
            Identifier* name = new Identifier(get(i-1).getValue());
            
            if(consume(left_par)){
                vector<ASTNode*> args = parseAgumentsInCall();
                expect(right_par, "Expected arguments or function call");
                identation.addElement(new FunctionCall(name, args));
                expect(semicolon, "Expected semicolon");
                --i;
            }
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

bool Parser::consume(const TokenType type,const int8_t distance){
    if(ask(type,distance)){
        ++i;
        return true;
    }
    return false;
}

void Parser::expect(const TokenType t, const char* description){
    if(not consume(t)){
        throwSyntaxError(description, tokens.getLine(i));
    }  
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
            return new Literal(new Integer(), expressionTokens[0].getValue());
        case num_literal:
            return new Literal(new Number(), expressionTokens[0].getValue());
        case char_literal:
            return new Literal(new Character(),expressionTokens[0].getValue());
        case string_literal:
            return new Literal(new String(),expressionTokens[0].getValue());
        case null_literal:
            return new Literal(new NullType(), "");
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
    expect(identifier, "Expected identifier");
    return new Identifier(get(i - 1).getValue());
}

/*
* STATEMENT PARSING
*/
void Parser::parseEnum(){
    Identifier* name = getIdentifier();
    GenericType* extend = parseInheritance();
    expect(left_curly, "Expected enumeration");
    if(extend == nullptr){extend = new Integer();}    
    vector<Element*> elements = parseEnumElements();
    expect(right_curly, "Expected end of enumeration");
    identation.addElement(new EnumStatement(elements, name, extend));
    --i;
}

void Parser::parseImport(){
    bool valid = true;
    vector<Identifier*> imports;
    while(valid){
        if(ask(identifier)){
            imports.push_back(new Identifier(get(i++).getValue()));
            valid = consume(comma);
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

GenericType* Parser::parseInheritance(){
    if(consume(arrow)){
        if(get(i).isTypeKeyword()){
            return getTypeByName(get(i++).getValue());
        }
        expect(identifier, "Expected inheritance");
    }
    return nullptr;
}

vector<ASTNode*> Parser::parseArguments(){
    vector<ASTNode*> arguments;
    bool valid = true;
    while(valid){
        if(not get(i).isTypeKeyword()){valid = false; continue;}
        GenericType* type = parseType();
        Identifier* id = getIdentifier();
        if(consume(equal)){
            Expression* value = parseExpression();
            if(instanceOf<NullExpression>(value)){
                throwSyntaxError("Expected expression", tokens.getLine(i));
            }
            arguments.push_back(new Argument(type,id, value));
        }
        else{
            arguments.push_back(new Argument(type, id));
        }
        valid = consume(comma);
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

        valid = consume(comma);
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
    if(consume(identifier)){
        name = new Identifier(get(i-1).getValue());
        if(consume(equal)){
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
        valid = consume(comma);
    }
    return arguments;
}

void Parser::parseSimpleStatement(Statement* node){
    identation.addElement(node);
    expect(semicolon, "Expected semicolon");
    --i;
}

GenericType* Parser::parseType(){
    GenericType* t = getTypeByName(get(i).getValue(), ask(const_keyword, -1));
    ++i;
    if(consume(left_square,1)){
        // ARRAY PARSING
    }
    return t;
}

VariableDeclaration* Parser::parseVar(){
    
    size_t initialPos = i;
    consume(const_keyword);
    if(get(i).isTypeKeyword()){
        GenericType* type = parseType();

        if(not ask(identifier)){
            i = initialPos;
            return nullptr;
        }

        Identifier* id = getIdentifier();

        if(consume(equal)){
            Expression* expr = parseExpression();
            expect(semicolon, "Expected semicolon");
            return new VariableDeclaration(type,id,expr);
        }
        if(consume(left_par)){
            i = initialPos;
            return nullptr;
        }
        expect(semicolon, "Expected expression or semicolon");
        return new VariableDeclaration(type, id, getNullValue(type));
    }
    return nullptr;
}

FunctionContainer* Parser::parseFunction(){
    size_t initialPos = i;
    if(consume(const_keyword)){}
    if(get(i).isTypeKeyword()){
        GenericType* type = parseType();
        if(not ask(identifier)){
            i = initialPos;
            return nullptr;
        }
        Identifier* id = getIdentifier();
        expect(left_par, "Expected function arguments");
        std::vector<ASTNode*> args = parseArguments();
        expect(right_par, "Expected end of arguments");
        expect(left_curly, "Expected function body");
        return new FunctionContainer(id, type, args);
    }
    return nullptr;
}

VariableRedeclaration* Parser::parseRedec(){

    if(get(i).isUnaryOperator() and ask(identifier, 1) or ask(identifier) and get(i+1).isUnaryOperator()){
        Identifier* id = new Identifier(ask(identifier)?get(i).getValue():get(i+1).getValue());
        std::string op = ask(identifier)? get(i + 1).getValue() : get(i).getValue();
        i += 2;
        return new VariableRedeclaration(id, new UnaryExpression(op,id));
    }
    if(ask(identifier) and get(i + 1).isAssignOperator()){
        Identifier* id = new Identifier(get(i).getValue());
        Token op = get(i +1);
        i += 2;
        Expression* expr = parseExpression();
        if(instanceOf<NullExpression>(expr)){
            // err
        }
        if(op.getType() == equal){
            return new VariableRedeclaration(id, expr);
        }
        return new VariableRedeclaration(id,new BinaryExpression(op.getValue().substr(0,1) ,id,expr));
    }
    return nullptr;
}