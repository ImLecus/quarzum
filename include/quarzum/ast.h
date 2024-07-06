/*
 * Quarzum Compiler - ast.h
 * Version 1.0, 04/07/2024
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
#include <vector>
#include <string>
#include "token.h"

enum class NodeType: uint8_t {

};

struct Node {
    const NodeType nodeType;
};

struct Statement: public Node {};

struct Container: public Statement {
    std::vector<Statement*> nodes;
};

struct RootNode : public Container {};

struct Type: public Node {
    const std::string name;
    explicit Type(const std::string& name): name(move(name)) {};
};

struct Expr: public Node {
    BaseType* type;
    Expr(BaseType* type): type(move(type)) {};
};

struct LiteralExpr: public Expr {
    const Token token;
    explicit LiteralExpr(const Token t): token(move(t)), Expr(nullptr) {};
};

struct IdentifierExpr: public Expr {
    const std::string value;
    explicit IdentifierExpr(const std::string& value): value(move(value)), Expr(nullptr)  {};
};

struct ParenExpr: public Expr{
    Expr* expr;
    explicit ParenExpr(Expr* expr): expr(move(expr)), Expr(nullptr)  {};
};

struct UnaryExpr : public Expr {
    const std::string op;
    Expr* expr;
    bool isPrefix;
    explicit UnaryExpr(const string& op,Expr* expr, const bool isPrefix = true): 
    op(op), expr(move(expr)), isPrefix(isPrefix), Expr(nullptr)  {};  
};

struct BinaryExpr : public Expr {
    const std::string op;
    Expr* left;
    Expr* right;
    explicit BinaryExpr(const string& op,Expr* left,Expr* right): 
    op(op), left(move(left)), right(move(right)), Expr(nullptr)  {}; 
};

struct TernaryExpr : public Expr {
    Expr* condition;
    Expr* ifTrue;
    Expr* ifFalse;
    explicit TernaryExpr(Expr* condition,Expr* ifTrue, Expr* ifFalse): 
    condition(move(condition)), ifTrue(move(ifTrue)), ifFalse(move(ifFalse)), Expr(nullptr)  {};  
};

struct Argument : public Node {
    Type* type;
    const std::string name;
    explicit Argument(Type* type, const std::string& name): type(move(type)), name(name) {};
};

struct FunctionCallExpr : public Expr {
    const std::string name;
    std::vector<Expr*> args;
    explicit FunctionCallExpr(const std::string& name): name(name), args{}, Expr(nullptr)  {};
    explicit FunctionCallExpr(const std::string& name, std::vector<Expr*> args): 
    name(name), args(move(args)), Expr(nullptr)  {};
};

struct FunctionCall : public Statement {
    const std::string name;
    std::vector<Expr*> args;
    explicit FunctionCall(const std::string& name): name(move(name)), args{} {};
    explicit FunctionCall(const std::string& name, std::vector<Expr*> args): 
    name(move(name)), args(move(args)) {};
};

struct ArrayIndexExpr : public Expr {
    const std::string name;
    Expr* index;
    explicit ArrayIndexExpr(const std::string& name, Expr* index): name(move(name)), index(move(index)), Expr(nullptr)  {};
};

struct VarDeclaration : public Statement {
    Type* type;
    std::string id;
    Expr* expr;
    std::string access = "none";
    bool constant = false;
};

struct VarRedec : public Statement {
    const std::string id;
    Expr* expr;
    explicit VarRedec(const std::string& id): id(move(id)) {};
};

struct ModuleContainer : public Container {
    const std::string id;
    explicit ModuleContainer(const std::string& id): id(move(id)) {};
};
struct IfContainer : public Container {
    Expr* condition;
    std::vector<Statement*> elseContainer;
    explicit IfContainer(Expr* condition): condition(move(condition)) {};
};
struct WhileContainer : public Container {
    Expr* condition;
    explicit WhileContainer(Expr* condition): condition(move(condition)) {};
};
struct DoWhileContainer : public Container {
    Expr* condition;
};
struct ForeachContainer : public Container {
    std::string id;
    Expr* expr;
};
struct ForContainer : public Container {
    VarDeclaration* decl;
    Expr* condition;
};
struct ClassContainer : public Container {
    const std::string name;
    Type* type;
    explicit ClassContainer(const std::string& name): name(move(name)) {};
};
struct FunctionContainer : public Container {
    std::string name;
    std::vector<Argument*> args;
    Type* type;
};
struct ReturnStatement : public Statement {
    Expr* expr;
    explicit ReturnStatement(Expr* expr): expr(move(expr)) {};
};

struct BreakStatement : public Statement {};
struct ContinueStatement : public Statement {};

struct ImportStatement : public Statement {
    std::vector<std::string> objects;
    std::string path;
};

struct EnumElement : public Node {
    std::string name;
    std::vector<Expr*> expr;
};

struct EnumStatement : public Statement {
    std::string name;
    Type* extend;
    std::vector<EnumElement*> items;
};

struct DeleteStatement : public Statement {
    const std::string identifier;
    explicit DeleteStatement(const std::string& identifier): identifier(move(identifier)) {};
};

struct ForeignStatement: public Statement {
    std::string name;
    std::vector<Argument*> args;
    Type* type;
};