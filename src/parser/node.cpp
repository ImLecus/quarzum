/*
 * Quarzum Compiler - node.cpp
 * Version 1.0, 02/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#pragma once
#include "node.h"
#include "../sem_ir/types.cpp"
#include "../sem_ir/irinstruction.hpp"
#include "../sem_ir/symboltable.hpp"

struct Node {};

struct Statement: public Node {};

struct Container: public Statement {
    std::vector<std::unique_ptr<Statement>> nodes;
};

struct RootNode : public Container {};

struct Type: public Node {
    uint8_t branch;
    uint8_t height;

    const std::string name;
    explicit Type(const std::string& name): name(std::move(name)) {
        if(name == "char" or name == "string"){
            branch = 1;
        }
        else{
            branch = 2;
        }
    };
};

struct Expr: public Node {
    std::unique_ptr<Type> type;
};

struct LiteralExpr: public Expr {
    const std::string value;
    explicit LiteralExpr(const std::string& value): value(std::move(value)) {};
};

struct IdentifierExpr: public Expr {
    const std::string value;
    explicit IdentifierExpr(const std::string& value): value(std::move(value)) {};
};

struct ParenExpr: public Expr{
    std::unique_ptr<Expr> expr;
    explicit ParenExpr(std::unique_ptr<Expr> expr): expr(std::move(expr)) {};
};

struct UnaryExpr : public Expr {
    const std::string op;
    std::unique_ptr<Expr> expr;
    bool isPrefix;
    explicit UnaryExpr(const std::string& op,std::unique_ptr<Expr> expr, const bool isPrefix = true): 
    op(op), expr(std::move(expr)), isPrefix(isPrefix) {};  
};

struct BinaryExpr : public Expr {
    const std::string op;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    explicit BinaryExpr(const std::string& op,std::unique_ptr<Expr> left, std::unique_ptr<Expr> right): 
    op(op), left(std::move(left)), right(std::move(right)) {}; 
};

struct TernaryExpr : public Expr {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Expr> ifTrue;
    std::unique_ptr<Expr> ifFalse;
    explicit TernaryExpr(std::unique_ptr<Expr> condition,std::unique_ptr<Expr> ifTrue, std::unique_ptr<Expr> ifFalse): 
    condition(std::move(condition)), ifTrue(std::move(ifTrue)), ifFalse(std::move(ifFalse)) {};  
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
};

struct FunctionCall : public Statement {
    const std::string name;
    std::vector<std::unique_ptr<Expr>> args;
    explicit FunctionCall(const std::string& name): name(std::move(name)), args{} {};
    explicit FunctionCall(const std::string& name, std::vector<std::unique_ptr<Expr>> args): 
    name(std::move(name)), args(std::move(args)) {};
};

struct ArrayIndexExpr : public Expr {
    const std::string name;
    std::unique_ptr<Expr> index;
    explicit ArrayIndexExpr(const std::string& name, std::unique_ptr<Expr> index): name(std::move(name)), index(std::move(index)) {};
};

struct VarDeclaration : public Statement {
    std::unique_ptr<Type> type;
    std::string id;
    std::unique_ptr<Expr> expr;
    std::string access = "none";
    bool constant = false;
};

struct VarRedec : public Statement {
    const std::string id;
    std::unique_ptr<Expr> expr;
    explicit VarRedec(const std::string& id): id(std::move(id)) {};
};

struct ModuleContainer : public Container {
    const std::string id;
    explicit ModuleContainer(const std::string& id): id(std::move(id)) {};
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
    const std::string name;
    std::unique_ptr<Type> type;
    explicit ClassContainer(const std::string& name): name(std::move(name)) {};
};
struct FunctionContainer : public Container {
    std::string name;
    std::vector<std::unique_ptr<Argument>> args;
    std::unique_ptr<Type> type;
};
struct ReturnStatement : public Statement {
    std::unique_ptr<Expr> expr;
    explicit ReturnStatement(std::unique_ptr<Expr> expr): expr(std::move(expr)) {};
};

struct BreakStatement : public Statement {};
struct ContinueStatement : public Statement {};

struct ImportStatement : public Statement {
    std::vector<std::string> objects;
    std::string path;
};

struct EnumElement : public Node {
    std::string name;
    std::vector<std::unique_ptr<Expr>> expr;
};

struct EnumStatement : public Statement {
    std::string name;
    std::unique_ptr<Type> extend;
    std::vector<std::unique_ptr<EnumElement>> items;
};

struct DeleteStatement : public Statement {
    const std::string identifier;
    explicit DeleteStatement(const std::string& identifier): identifier(std::move(identifier)) {};
};

struct ForeignStatement: public Statement {
    std::string name;
    std::vector<std::unique_ptr<Argument>> args;
    std::unique_ptr<Type> type;
};