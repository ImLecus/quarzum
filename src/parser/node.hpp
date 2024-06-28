#pragma once
#include <iostream>
#include "../sem_ir/types.cpp"
#include "../sem_ir/irinstruction.hpp"
#include "../sem_ir/symboltable.hpp"
#include <vector>

struct Node {
    virtual void generateIR(){};
};

struct Statement: public Node {
    virtual inline void check() = 0;
};

struct Container: public Statement {
    std::vector<std::unique_ptr<Statement>> nodes;

    void check() override {
        for(auto& node: nodes){
            node->check();
        }
    }
};

struct RootNode : public Container {};

struct Type: public Node {
    uint8_t branch;
    uint8_t height;

    std::string name;
    explicit Type(const std::string& name): name(name) {
        if(name == "char" or name == "string"){
            branch = 1;
        }
        else{
            branch = 2;
        }
    };
};

struct Expr: public Node {
    virtual inline void check() = 0;
    std::unique_ptr<Type> type;
};

struct LiteralExpr: public Expr {
    const std::string value;
    explicit LiteralExpr(const std::string& value): value(value) {};
    void check() {
        // convert the value to a literal type
    }
};

struct IdentifierExpr: public Expr {
    const std::string value;
    explicit IdentifierExpr(const std::string& value): value(value) {};
    void check(){
        // ask the symboltable about the symbol and type
    }
};

struct ParenExpr: public Expr{
    std::unique_ptr<Expr> expr;
    explicit ParenExpr(std::unique_ptr<Expr> expr): expr(std::move(expr)) {};
    void check(){
        expr->check();
    }
};

struct UnaryExpr : public Expr {
    const std::string op;
    std::unique_ptr<Expr> expr;
    bool isPrefix;
    explicit UnaryExpr(const std::string& op,std::unique_ptr<Expr> expr, const bool isPrefix = true): 
    op(op), expr(std::move(expr)), isPrefix(isPrefix) {};  
    void check(){
        expr->check();
        // apply operator to type
    }
};

struct BinaryExpr : public Expr {
    const std::string op;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    explicit BinaryExpr(const std::string& op,std::unique_ptr<Expr> left, std::unique_ptr<Expr> right): 
    op(op), left(std::move(left)), right(std::move(right)) {}; 
    void check() {
        left->check();
        right->check();
        // apply the binary operator
    }
};

struct TernaryExpr : public Expr {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Expr> ifTrue;
    std::unique_ptr<Expr> ifFalse;
    explicit TernaryExpr(std::unique_ptr<Expr> condition,std::unique_ptr<Expr> ifTrue, std::unique_ptr<Expr> ifFalse): 
    condition(std::move(condition)), ifTrue(std::move(ifTrue)), ifFalse(std::move(ifFalse)) {};  

    void check(){
        //check if the condition is a bool and ifTrue and ifFalse have the same type
    }
};

struct Argument : public Node {
    std::unique_ptr<Type> type;
    const std::string name;
    explicit Argument(std::unique_ptr<Type> type, const std::string& name): type(std::move(type)), name(name) {};
};

struct FunctionCallExpr : public Expr {
    const std::string name;
    std::vector<std::unique_ptr<Expr>> args;
    explicit FunctionCallExpr(const std::string& name): name(name), args{} {};
    explicit FunctionCallExpr(const std::string& name, std::vector<std::unique_ptr<Expr>> args): 
    name(name), args(std::move(args)) {};

    void check(){
        // apply the type to the return type of the function, check the function exists
    }
};

struct FunctionCall : public Statement {
    const std::string name;
    std::vector<std::unique_ptr<Expr>> args;
    explicit FunctionCall(const std::string& name): name(name), args{} {};
    explicit FunctionCall(const std::string& name, std::vector<std::unique_ptr<Expr>> args): 
    name(name), args(std::move(args)) {};
    void check(){}
};

struct ArrayIndexExpr : public Expr {
    const std::string name;
    std::unique_ptr<Expr> index;
    explicit ArrayIndexExpr(const std::string& name, std::unique_ptr<Expr> index): name(name), index(std::move(index)) {};
    void check(){
        // check if its an array and the symbol table name and type
    }
};

struct VarDeclaration : public Statement {
    std::unique_ptr<Type> type;
    std::string id;
    std::unique_ptr<Expr> expr;
    std::string access = "none";
    bool constant = false;

    void check(){
        expr->check();
        // check if the type and the expr type are the same
    }
};

struct VarRedec : public Statement {
    std::string id;
    std::unique_ptr<Expr> expr;
    explicit VarRedec(const std::string& id): id(id) {};
    void check(){}
};

struct ModuleContainer : public Container {
    std::string id;
    explicit ModuleContainer(const std::string& id): id(id) {};
};
struct IfContainer : public Container {
    std::unique_ptr<Expr> condition;
    std::vector<std::unique_ptr<Statement>> elseContainer;
    explicit IfContainer(std::unique_ptr<Expr> condition): condition(std::move(condition)) {};
};
struct WhileContainer : public Container {
    std::unique_ptr<Expr> condition;
    explicit WhileContainer(std::unique_ptr<Expr> condition): condition(std::move(condition)) {};
};
struct DoWhileContainer : public Container {
    std::unique_ptr<Expr> condition;
};
struct ForeachContainer : public Container {
    std::string id;
    std::unique_ptr<Expr> expr;
};
struct ForContainer : public Container {
    std::unique_ptr<VarDeclaration> decl;
    std::unique_ptr<Expr> condition;
};
struct ClassContainer : public Container {
    std::string name;
    std::unique_ptr<Type> type;
    explicit ClassContainer(const std::string& name): name(name) {};
};
struct FunctionContainer : public Container {
    std::string name;
    std::vector<std::unique_ptr<Argument>> args;
    std::unique_ptr<Type> type;
};
struct ReturnStatement : public Statement {
    std::unique_ptr<Expr> expr;
    explicit ReturnStatement(std::unique_ptr<Expr> expr): expr(std::move(expr)) {};
    void check(){}
};
struct ExitStatement : public Statement {
    std::unique_ptr<Expr> expr;
    explicit ExitStatement(std::unique_ptr<Expr> expr): expr(std::move(expr)) {};
    void check(){}
};

struct BreakStatement : public Statement {
    void check(){}
};
struct ContinueStatement : public Statement {
    void check(){}
};

struct ImportStatement : public Statement {
    std::vector<std::string> objects;
    std::string path;
    void check(){}
};

struct EnumElement : public Node {
    std::string name;
    std::vector<std::unique_ptr<Expr>> expr;
};

struct EnumStatement : public Statement {
    std::string name;
    std::unique_ptr<Type> extend;
    std::vector<std::unique_ptr<EnumElement>> items;
    void check(){}
};

struct DeleteStatement : public Statement {
    std::string identifier;
    explicit DeleteStatement(const std::string& identifier): identifier(identifier) {};
    void check(){}
};