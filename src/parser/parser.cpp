#include "parser.hpp"
using std::vector;

template<typename T, typename U>
bool instanceOf(const U& object){
    if(T* type = dynamic_cast<T*>(object)){
        return true;
    }
    return false;
}

Literal* getNullValue(GenericType*& type){
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

Parser::Parser(TokenList& tokens): tokens(tokens) {}

const RootNode Parser::parse(){
    RootNode root = RootNode();
    identation.open(&root);
    const size_t size = tokens.size();
    for(i = 0; i < size; ++i){
        if(auto decl = parseVar()){
            identation.addElement(decl);
            --i;

        }
        else if(auto func = parseFunction()){
            identation.open<FunctionContainer>(func);
            --i;
        }
        else if(ask(ACCESS_SPECIFIER)){
            Access access = Access(get(i).value == "public"? 0: get(i).value == "private"? 1: 2);
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
            if(auto e = dynamic_cast<ElseContainer*>(identation.getLastLayer()->getLastObject())){
                identation.getLastLayer()->deleteLastObject();
                if(auto ic = dynamic_cast<IfContainer*>(identation.getLastLayer()->getLastObject())){
                    ic->elseContainer = e;
                    continue;
                }
                throwSyntaxError("Expected 'if' statement before 'else'",tokens.getLine(i));
            }
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
            Identifier* id = getIdentifier(true);
            expect(in_keyword, "Expected 'in' keyword");
            Expression* expression = parseExpression();
            if(instanceOf<NullExpression>(expression)){
               throwSyntaxError("Expected iterable expression", tokens.getLine(i));
            }
            if(not ask(right_par) or get(++i).type != left_curly){
              throwSyntaxError("Expected ')'", tokens.getLine(i));
            }
            identation.open<ForeachContainer>(new ForeachContainer(id, expression, type));
        }

        else if(consume(module_keyword)){
            Identifier* identifier = getIdentifier(true);
            expect(left_curly, "Expected module body");
            identation.open<ModuleContainer>(new ModuleContainer(identifier));
            --i;
        }

        else if(consume(class_keyword)){
            Identifier* id = getIdentifier(true);
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

        else if(consume(else_keyword)){
            expect(left_curly, "Expected '{'");
            identation.open<ElseContainer>(new ElseContainer());
            --i;
        }

        else if(auto redec = parseRedec()){
            identation.addElement(redec);
            expect(semicolon, "Expected semicolon");
            --i;
        }

        else if(auto call = parseFunctionCall()){
            identation.addElement(call);
            expect(semicolon, "Expected semicolon");
            --i;
        }
    }

    return root;
}

FunctionCall* Parser::parseFunctionCall() {
    size_t firstIndex = i;
    Identifier* name = getIdentifier();    
    if(consume(left_par)){
        vector<Expression*> args = parseAgumentsInCall();
        expect(right_par, "Expected arguments or function call");
        return new FunctionCall(name, args);
    }
    i = firstIndex;
    return nullptr;
}

inline Token Parser::get(const size_t& index){
    if(index <= tokens.size()){
        return tokens[index];
    }
    return ERROR_TOKEN;
}
inline bool Parser::ask(const TokenType& type,const int8_t& distance){
    return get(i + distance).type == type;
}

bool Parser::consume(const TokenType& type,const int8_t& distance){
    if(ask(type,distance)){
        ++i;
        return true;
    }
    return false;
}

void Parser::expect(const TokenType& t, const char* description){
    if(not consume(t)){
        throwSyntaxError(description, tokens.getLine(i));
    }  
}

Expression* Parser::parseExpression(TokenList expressionTokens){
    if(expressionTokens.isEmpty()) {
        while(true){
            // TO-DO: Change this function to accept function calls
            if(get(i).isOperator() or get(i).isLiteral() or ask(identifier)){
                expressionTokens.add(get(i++));
                continue;
            }
            // if(auto call = parseFunctionCall()){
                
            // }
            break;
        }
    }
    
    if(expressionTokens.size() == 0){
        return new NullExpression();
    }
    // Parsing literals
    if(expressionTokens.size() == 1){
        switch (expressionTokens[0].type)
        {
        case int_literal:
            return new Literal(new Integer(), expressionTokens[0].value);
        case num_literal:
            return new Literal(new Number(), expressionTokens[0].value);
        case char_literal:
            return new Literal(new Character(),expressionTokens[0].value);
        case string_literal:
            return new Literal(new String(),expressionTokens[0].value);
        case null_literal:
            return new Literal(new NullType(), "");
        case identifier:
            return new Identifier(expressionTokens[0].value);
        case true_literal:
        case false_literal:
            return new Literal(new Boolean(), expressionTokens[0].value);
        default:
            throwSyntaxError("Invalid expression", tokens.getLine(i));
            break;
        }
    }
    // Parsing unary expressions
    if(expressionTokens.size() == 2){
        if(expressionTokens[0].type == not_op and expressionTokens[1].isLiteral()){
            return new UnaryExpression("not", parseExpression(expressionTokens.split(1,expressionTokens.size())));
        }
        if(expressionTokens[0].type == minus and expressionTokens[1].isLiteral()){
            return new UnaryExpression("-", parseExpression(expressionTokens.split(1,expressionTokens.size())));
        }
    }
    // Parsing composite expressions, in reverse priority order
    for(u_int8_t priority = 0; priority < Token::MAX_PRIORITY; ++priority){
        u_int16_t size = expressionTokens.size();
        for(u_int16_t i = 0; i < size; ++i){
            if(expressionTokens[i].getPriority() == priority){
                return new BinaryExpression(
                    expressionTokens[i].value,
                    parseExpression(expressionTokens.split(0,i)),
                    parseExpression(expressionTokens.split(i + 1, expressionTokens.size()))
                );
            }
        }
    }

    throwSyntaxError("Invalid expression", tokens.getLine(i));
    return nullptr;
}

Identifier* Parser::getIdentifier(const bool& noScope){
    expect(identifier, "Expected identifier");
    Identifier* id = new Identifier(get(i -1).value);
    while(ask(point) and ask(identifier,1)){
        if(noScope){
            throwSyntaxError("Invalid scope declaration", tokens.getLine(i));
        }
        id->withScope = true;
        id->value += "." + get(i + 1).value;
        i += 2;
    }
    return id;
}

/*
* STATEMENT PARSING
*/
void Parser::parseEnum(){
    Identifier* name = getIdentifier(true);
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

            imports.push_back(getIdentifier());
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
    location = get(++i).value;
    identation.addElement(new ImportStatement(imports, location));
}

GenericType* Parser::parseInheritance(){
    if(consume(arrow)){
        if(get(i).type == TYPE_KEYWORD){
            return getTypeByName(get(i++).value);
        }
        expect(identifier, "Expected inheritance");
    }
    return nullptr;
}

vector<Argument*> Parser::parseArguments(){
    vector<Argument*> arguments;
    bool valid = true;
    while(valid){
        if(not get(i).type == TYPE_KEYWORD){valid = false; continue;}
        GenericType* type = parseType();
        Identifier* id = getIdentifier(true);
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
        name = new Identifier(get(i-1).value);
        if(consume(equal)){
            value = parseExpression();
            if(instanceOf<NullExpression>(value)){
                throwSyntaxError("Invalid default value assignation", tokens.getLine(i));
            }
        }
    }
    return new Element(name,value);
}

vector<Expression*> Parser::parseAgumentsInCall(){
    vector<Expression*> arguments;
    bool valid = true;
    while(valid){
        Expression* expression = parseExpression();
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
    GenericType* t = getTypeByName(get(i).value, ask(const_keyword, -1));
    ++i;
    if(consume(left_square,1)){
        // ARRAY PARSING
    }
    return t;
}

VariableDeclaration* Parser::parseVar(){
    
    size_t initialPos = i;
    consume(const_keyword);
    if(get(i).type == TYPE_KEYWORD){
        GenericType* type = parseType();

        if(not ask(identifier)){
            i = initialPos;
            return nullptr;
        }

        Identifier* id = getIdentifier(true);

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
    if(get(i).type == TYPE_KEYWORD){
        GenericType* type = parseType();
        if(not ask(identifier)){
            i = initialPos;
            return nullptr;
        }
        Identifier* id = getIdentifier(true);
        expect(left_par, "Expected function arguments");
        std::vector<Argument*> args = parseArguments();
        expect(right_par, "Expected end of arguments");
        expect(left_curly, "Expected function body");
        return new FunctionContainer(id, type, args);
    }
    return nullptr;
}

VariableRedeclaration* Parser::parseRedec(){

    if(get(i).isUnaryOperator() and ask(identifier, 1) or ask(identifier) and get(i+1).isUnaryOperator()){
        Identifier* id = new Identifier(ask(identifier)?get(i).value:get(i+1).value);
        std::string op = ask(identifier)? get(i + 1).value : get(i).value;
        i += 2;
        return new VariableRedeclaration(id, new UnaryExpression(op,id));
    }
    if(ask(identifier) and get(i + 1).type == ASSIGN_OPERATOR or get(i +1).type == equal){
        Identifier* id = new Identifier(get(i).value);
        Token op = get(i +1);
        i += 2;
        Expression* expr = parseExpression();
        if(instanceOf<NullExpression>(expr)){
            // err
        }
        if(op.type == equal){
            return new VariableRedeclaration(id, expr);
        }
        return new VariableRedeclaration(id,new BinaryExpression(op.value.substr(0,1) ,id,expr));
    }
    return nullptr;
}