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
#include "../sem_ir/symboltable.cpp"

struct Node {

};

struct Statement: public Node {};

struct Container: public Statement {
    std::vector<unique_ptr<Statement>> nodes;
};

struct RootNode : public Container {};

struct Type: public Node {
    const string name;
    explicit Type(const string& name): name(move(name)) {};
};

struct Expr: public Node {
    unique_ptr<BaseType> type;
    Expr(unique_ptr<BaseType> type): type(move(type)) {};
};

struct LiteralExpr: public Expr {
    const Token token;
    explicit LiteralExpr(const Token t): token(move(t)), Expr(nullptr) {
        // switch (t.type)
        // {
        // case int_literal:
        //     this->type = strToType("int");
        //     break;
        // case numeric_literal:
        //     this->type = strToType("num");
        //     break;
        // case char_literal:
        //     this->type = strToType("char");
        //     break;
        // case string_literal:
        //     this->type = strToType("string");
        //     break;
        // case null_literal:
        //     this->type = strToType("null");
        //     break;

        // default:
        //     break;
        // }
    };
};

struct IdentifierExpr: public Expr {
    const string value;
    explicit IdentifierExpr(const string& value): value(move(value)), Expr(nullptr)  {};
};

struct ParenExpr: public Expr{
    unique_ptr<Expr> expr;
    explicit ParenExpr(unique_ptr<Expr> expr): expr(move(expr)), Expr(nullptr)  {};
};

struct UnaryExpr : public Expr {
    const string op;
    unique_ptr<Expr> expr;
    bool isPrefix;
    explicit UnaryExpr(const string& op,unique_ptr<Expr> expr, const bool isPrefix = true): 
    op(op), expr(move(expr)), isPrefix(isPrefix), Expr(nullptr)  {};  
};

struct BinaryExpr : public Expr {
    const string op;
    unique_ptr<Expr> left;
    unique_ptr<Expr> right;
    explicit BinaryExpr(const string& op,unique_ptr<Expr> left, unique_ptr<Expr> right): 
    op(op), left(move(left)), right(move(right)), Expr(nullptr)  {}; 
};

struct TernaryExpr : public Expr {
    unique_ptr<Expr> condition;
    unique_ptr<Expr> ifTrue;
    unique_ptr<Expr> ifFalse;
    explicit TernaryExpr(unique_ptr<Expr> condition,unique_ptr<Expr> ifTrue, unique_ptr<Expr> ifFalse): 
    condition(move(condition)), ifTrue(move(ifTrue)), ifFalse(move(ifFalse)), Expr(nullptr)  {};  
};

struct Argument : public Node {
    unique_ptr<Type> type;
    const string name;
    explicit Argument(unique_ptr<Type> type, const string& name): type(move(type)), name(name) {};
};

struct FunctionCallExpr : public Expr {
    const string name;
    std::vector<unique_ptr<Expr>> args;
    explicit FunctionCallExpr(const string& name): name(name), args{}, Expr(nullptr)  {};
    explicit FunctionCallExpr(const string& name, std::vector<unique_ptr<Expr>> args): 
    name(name), args(move(args)), Expr(nullptr)  {};
};

struct FunctionCall : public Statement {
    const string name;
    std::vector<unique_ptr<Expr>> args;
    explicit FunctionCall(const string& name): name(move(name)), args{} {};
    explicit FunctionCall(const string& name, std::vector<unique_ptr<Expr>> args): 
    name(move(name)), args(move(args)) {};
};

struct ArrayIndexExpr : public Expr {
    const string name;
    unique_ptr<Expr> index;
    explicit ArrayIndexExpr(const string& name, unique_ptr<Expr> index): name(move(name)), index(move(index)), Expr(nullptr)  {};
};

struct VarDeclaration : public Statement {
    unique_ptr<Type> type;
    string id;
    unique_ptr<Expr> expr;
    string access = "none";
    bool constant = false;
};

struct VarRedec : public Statement {
    const string id;
    unique_ptr<Expr> expr;
    explicit VarRedec(const string& id): id(move(id)) {};
};

struct ModuleContainer : public Container {
    const string id;
    explicit ModuleContainer(const string& id): id(move(id)) {};
};
struct IfContainer : public Container {
    unique_ptr<Expr> condition;
    std::vector<unique_ptr<Statement>> elseContainer;
    explicit IfContainer(unique_ptr<Expr> condition): condition(move(condition)) {};
};
struct WhileContainer : public Container {
    unique_ptr<Expr> condition;
    explicit WhileContainer(unique_ptr<Expr> condition): condition(move(condition)) {};
};
struct DoWhileContainer : public Container {
    unique_ptr<Expr> condition;
};
struct ForeachContainer : public Container {
    string id;
    unique_ptr<Expr> expr;
};
struct ForContainer : public Container {
    unique_ptr<VarDeclaration> decl;
    unique_ptr<Expr> condition;
};
struct ClassContainer : public Container {
    const string name;
    unique_ptr<Type> type;
    explicit ClassContainer(const string& name): name(move(name)) {};
};
struct FunctionContainer : public Container {
    string name;
    std::vector<unique_ptr<Argument>> args;
    unique_ptr<Type> type;
};
struct ReturnStatement : public Statement {
    unique_ptr<Expr> expr;
    explicit ReturnStatement(unique_ptr<Expr> expr): expr(move(expr)) {};
};

struct BreakStatement : public Statement {};
struct ContinueStatement : public Statement {};

struct ImportStatement : public Statement {
    std::vector<string> objects;
    string path;
};

struct EnumElement : public Node {
    string name;
    std::vector<unique_ptr<Expr>> expr;
};

struct EnumStatement : public Statement {
    string name;
    unique_ptr<Type> extend;
    std::vector<unique_ptr<EnumElement>> items;
};

struct DeleteStatement : public Statement {
    const string identifier;
    explicit DeleteStatement(const string& identifier): identifier(move(identifier)) {};
};

struct ForeignStatement: public Statement {
    string name;
    std::vector<unique_ptr<Argument>> args;
    unique_ptr<Type> type;
    ForeignStatement() {
        //symbolTable.insert(name, {'f',name,type->name,"global",args.size()});
    }
};