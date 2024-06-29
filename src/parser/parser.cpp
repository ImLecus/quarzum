#include "parser.hpp"
#include "expr.cpp"
using std::unique_ptr;
using std::make_unique;

Parser::Parser(std::deque<Token> tokens): tokens(tokens) {}

const unique_ptr<RootNode> Parser::parse(){
    unique_ptr<RootNode> root = make_unique<RootNode>();
    while(not tokens.empty()){
        if(auto s = parseStatement()){
            root->nodes.push_back(std::move(s));
        }
    }
    return root;    
}

inline const Token Parser::pop(){
    if(tokens.empty()){
        return Token();
    }
    Token t = tokens.front();
    tokens.pop_front();
    return t;
}

inline const bool Parser::ask(TokenType t){
    return (not tokens.empty() and tokens.front().type == t);
}

unique_ptr<Statement> Parser::parseStatement() {
    Token t = tokens.front();
    switch (t.type)
    {
    case access_specifier:
        tokens.pop_front();
        return parseVar(false,t.value);
    case type_keyword:
        return parseVar();
    case const_keyword:
        tokens.pop_front();
        return parseVar(true);
    case module_keyword:
        tokens.pop_front();
        return parseModule();
    case return_keyword:
        tokens.pop_front();
        return parseReturn();
    case foreign_keyword:
        tokens.pop_front();
        return parseForeign();
    case break_keyword:
        tokens.pop_front();
        return parseBreak();
    case continue_keyword:
        tokens.pop_front();
        return parseContinue();
    case if_keyword:
        tokens.pop_front();
        return parseIf();
    case while_keyword:
        tokens.pop_front();
        return parseWhile();
    case do_keyword:
        tokens.pop_front();
        return parseDoWhile();
    case foreach_keyword:
        tokens.pop_front();
        return parseForeach();
    case class_keyword:
        tokens.pop_front();
        return parseClass();
    case for_keyword:
        tokens.pop_front();
        return parseFor();
    case enum_keyword:
        tokens.pop_front();
        return parseEnum();
    case delete_keyword:
        tokens.pop_front();
        return parseDelete();
    case import_keyword:
        tokens.pop_front();
        return parseImport();
    case identifier:
        tokens.pop_front();
        if(not tokens.empty() and tokens.front().type == left_par){
            tokens.pop_front();
            return parseFunctionCall(t);
        }
        if(not tokens.empty() and 
            (tokens.front().type == ASSIGN_OPERATOR or 
            tokens.front().value == "++" or 
            tokens.front().value == "--")){
                auto redec = parseRedec(t);
                EXPECT(pop(), semicolon, "Expected semicolon")
                return redec;
        }
        return nullptr;
    default:
        tokens.pop_front();
        return nullptr;
    }
}

unique_ptr<ForeignStatement> Parser::parseForeign(){
    Token typeToken = pop();
    EXPECT(typeToken, type_keyword, "Expected type keyword");
    Token id = pop();
    EXPECT(id, identifier, "Expected identifier");
    EXPECT(pop(), left_par, "Expected '('")
    auto fStmt = make_unique<ForeignStatement>();
    if(ask(right_par)){
        tokens.pop_front();
    }
    else{
        while(not tokens.empty()){
            Token t = pop();
            EXPECT(t,type_keyword,"Expected parameter type")
            Token i = pop();
            EXPECT(i, identifier, "Expected parameter name")
            // DEFAULT VALUES
            fStmt->args.push_back(std::move(
                make_unique<Argument>( make_unique<Type>(t.value), i.value )
            ));
            if(ask(comma)){
                tokens.pop_front();
                continue;
            }
            EXPECT(pop(), right_par, "Expected ')'")
            break;
        }
    }
    fStmt->name = id.value;
    fStmt->type = make_unique<Type>(typeToken.value);
    EXPECT(pop(), semicolon, "Expected semicolon");
    return fStmt;
}

unique_ptr<FunctionContainer> Parser::parseFunction(){
    auto fCont = make_unique<FunctionContainer>();
    Token typeToken = pop();
    EXPECT(typeToken, type_keyword, "Expected type keyword");
    Token id = pop();
    EXPECT(id, identifier, "Expected identifier")
    EXPECT(pop(), left_par, "Expected '('")
    if(ask(right_par)){
        tokens.pop_front();
    }
    else{
        while(not tokens.empty()){
            Token t = pop();
            EXPECT(t,type_keyword,"Expected parameter type")
            Token i = pop();
            EXPECT(i, identifier, "Expected parameter name")
            // DEFAULT VALUES
            fCont->args.push_back(std::move(
                make_unique<Argument>( make_unique<Type>(t.value), i.value )
            ));
            if(ask(comma)){
                tokens.pop_front();
                continue;
            }
            EXPECT(pop(), right_par, "Expected ')'")
            break;
        }
    }
    EXPECT(pop(), left_curly, "Expected function body");
    fCont->name = id.value;
    fCont->type = make_unique<Type>(typeToken.value);
    IDENTATION(fCont)
    return fCont;
}

unique_ptr<ImportStatement> Parser::parseImport(){
    auto importStmt = make_unique<ImportStatement>();
    while(not tokens.empty()){
        Token id = pop();
        EXPECT(id, identifier, "Expected identifier");
        importStmt->objects.push_back(id.value);
        if(ask(comma)){
            tokens.pop_front();
            continue;
        }
        EXPECT(pop(), from_keyword, "Expected 'from' keyword");
        break;
    }
    Token location = pop();
    EXPECT(location, LITERAL, "Expected location") // improve to detect string literal or check()
    importStmt->path = location.value;
    EXPECT(pop(), semicolon, "Expected semicolon")
    return importStmt;
}

unique_ptr<DeleteStatement> Parser::parseDelete(){
    Token id = pop();
    EXPECT(id, identifier, "Expected identifier");
    EXPECT(pop(), semicolon, "Expected semicolon");
    return make_unique<DeleteStatement>(id.value);
}

unique_ptr<EnumStatement> Parser::parseEnum(){
    auto enumStmt = make_unique<EnumStatement>();
    Token id = pop();
    EXPECT(id, identifier, "Expected identifier")
    enumStmt->name = id.value;

    if(ask(arrow)){
        tokens.pop_front();
        Token t = pop();
        EXPECT(t,type_keyword,"Expected type keyword");
        enumStmt->extend = std::move(make_unique<Type>(t.value));
    }

    EXPECT(pop(), left_curly, "Expected 'enum' body");
    while(not tokens.empty()){
        // enum elements
        if(tokens.front().type == right_curly){
            tokens.pop_front();
            break;
        }
    }
    return enumStmt;
}



unique_ptr<VarRedec> Parser::parseRedec(Token id, const bool isPrefix){
    auto redec = make_unique<VarRedec>(id.value);
    auto identifier = make_unique<IdentifierExpr>(id.value);
    // ++ and --
    if(tokens.front().type != ASSIGN_OPERATOR){
        redec->expr = make_unique<UnaryExpr>(pop().value,std::move(identifier),isPrefix);
        return redec;
    }
    tokens.pop_front();
    auto expr = parseExpr();
    char op = id.value[0];
    switch (op){
        case '|':
            redec->expr = make_unique<BinaryExpr>("or",std::move(identifier), std::move(expr));
        case '&':
            redec->expr = make_unique<BinaryExpr>("and",std::move(identifier),std::move(expr));
        case '^':
            redec->expr = make_unique<BinaryExpr>("xor",std::move(identifier),std::move(expr));
        default:
            redec->expr = make_unique<BinaryExpr>(std::to_string(op),std::move(identifier),std::move(expr));
    }
    return redec;
}

unique_ptr<FunctionCall> Parser::parseFunctionCall(Token id){
    auto call = std::make_unique<FunctionCall>(id.value);
    Token t = tokens.front();
    if(t.type == right_par){
        tokens.pop_front();
        return call;
    }
    while(not tokens.empty()){
        auto expr = parseExpr();
        call->args.push_back(std::move(expr));
        Token next = pop();
        if(next.type == comma){
            continue;
        }
        if(next.type == right_par){
            break;
        }
        
        throwError("Expected ',' or ')'", next);
        return nullptr;
    }
    EXPECT(pop(), semicolon, "Expected semicolon");
    return call;
}

unique_ptr<ClassContainer> Parser::parseClass(){
    Token id = pop();
    EXPECT(id, identifier, "Expect identifier")
    auto c = make_unique<ClassContainer>(id.value);
    if(ask(arrow)){
        tokens.pop_front();
        Token t = pop();
        EXPECT(t,type_keyword,"Expected type keyword");
        c->type = std::move(make_unique<Type>(t.value));
    }
    
    EXPECT(pop(), left_curly, "Expected 'class' body");
    IDENTATION(c)
    return c;
}

unique_ptr<ModuleContainer> Parser::parseModule(){
    Token id = pop();
    EXPECT(id, identifier, "Expect identifier")
    auto mod = make_unique<ModuleContainer>(id.value);
    EXPECT(pop(), left_curly, "Expected 'module' body");
    IDENTATION(mod)
    return mod;
}

unique_ptr<IfContainer> Parser::parseIf(){
    EXPECT(pop(), left_par, "Expected '('")
    auto expr = parseExpr();
    auto ifCont = make_unique<IfContainer>(std::move(expr));
    EXPECT(pop(), right_par, "Expected ')'")
    EXPECT(pop(), left_curly, "Expected 'if' body");
    IDENTATION(ifCont)
    if(ask(else_keyword)){
        tokens.pop_front();
        EXPECT(pop(), left_curly, "Expected 'else' body");
        // SECURITY LEAK
        while(tokens.front().type != right_curly){
            if(tokens.empty()){
                err("Mismatched '{'");
                return nullptr;
            }
            ifCont->elseContainer.push_back(parseStatement());
        }
        tokens.pop_front();
    }
    return ifCont;
}

unique_ptr<WhileContainer> Parser::parseWhile(){
    EXPECT(pop(), left_par, "Expected '('")
    auto expr = parseExpr();
    auto wCont = make_unique<WhileContainer>(std::move(expr));
    EXPECT(pop(), right_par, "Expected ')'")
    EXPECT(pop(), left_curly, "Expected 'while' body");
    IDENTATION(wCont)
    return wCont;
}

unique_ptr<DoWhileContainer> Parser::parseDoWhile(){
    // check size
    auto dwCont = make_unique<DoWhileContainer>();
    EXPECT(pop(), left_curly, "Expected 'do' body");
    IDENTATION(dwCont)
    EXPECT(pop(), while_keyword, "Expected 'while' keyword")
    EXPECT(pop(), left_par, "Expected '('")
    dwCont->condition = parseExpr();
    EXPECT(pop(), right_par, "Expected ')'")
    EXPECT(pop(), semicolon, "Expected semicolon")
    return dwCont;
}

unique_ptr<ForeachContainer> Parser::parseForeach(){
    auto feCont = make_unique<ForeachContainer>();
    EXPECT(pop(), left_par, "Expected '('")
    Token typeToken = pop();
    EXPECT(typeToken, type_keyword, "Expected type keyword")
    Token id = pop();
    EXPECT(id, identifier, "Expected identifier")
    feCont->id = id.value;
    EXPECT(pop(), in_keyword, "Expected 'in' keyword")
    
    feCont->expr = std::move(parseExpr());
    EXPECT(pop(), right_par, "Expected ')'")
    EXPECT(pop(), left_curly, "Expected 'foreach' body");
    IDENTATION(feCont)
    return feCont;

}

unique_ptr<Statement> Parser::parseVar(const bool isConst,const std::string& access){
    auto result = make_unique<VarDeclaration>();
    result->constant = isConst;
    // type
    Token typeToken = tokens[0];
    EXPECT(typeToken, type_keyword, "Expected type keyword")
    result->type = make_unique<Type>(typeToken.value);
    // id
    Token idToken = tokens[1];
    EXPECT(idToken, identifier, "Expected identifier")
    result->id = idToken.value;
    // assign
    if(tokens[2].type == left_par){
        return parseFunction();
    }
    tokens.pop_front();
    tokens.pop_front();
    if(ask(equal)){
        tokens.pop_front();
        result->expr = parseExpr();
    }

    // semicolon
    EXPECT(pop(), semicolon, "Expected semicolon")
    return result;
}

unique_ptr<ReturnStatement> Parser::parseReturn(){
    if(not tokens.empty()){
        if(tokens.front().type == semicolon){
            return make_unique<ReturnStatement>(nullptr);
        }
        auto expr = parseExpr();
        EXPECT(pop(), semicolon, "Expected semicolon")
        return make_unique<ReturnStatement>(std::move(expr));
    }
    return nullptr;
}

unique_ptr<BreakStatement> Parser::parseBreak(){
    EXPECT(pop(), semicolon, "Expected semicolon")
    return make_unique<BreakStatement>();
}
unique_ptr<ContinueStatement> Parser::parseContinue(){
    EXPECT(pop(), semicolon, "Expected semicolon")
    return make_unique<ContinueStatement>();
}

std::unique_ptr<ForContainer> Parser::parseFor(){
    EXPECT(pop(), left_par, "Expected '('")
    auto var = parseVar(); // parseVar already includes semicolon at the end
    auto condition = parseExpr(); 
    EXPECT(pop(), semicolon, "Expected semicolon")
    unique_ptr<VarRedec> redec;
    if(ask(identifier)){
        tokens.pop_front();
        redec = parseRedec(tokens.front(), false);
    }// to-do: fix function args
    EXPECT(pop(), right_par, "Expected ')'")
    EXPECT(pop(), left_curly, "Expected 'for' body")
    auto fCont = make_unique<ForContainer>();
    IDENTATION(fCont)
    return fCont;
}