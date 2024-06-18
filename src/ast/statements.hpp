#pragma once
#include "astnode.hpp"

struct ReturnStatement: public Statement {
    Expression* expression;
    ReturnStatement(Expression* expr): expression(expr) {};
    void print() override {
        std::cout << "ret ";
        expression->print();
    }
};
struct ExitStatement: public Statement {
    Expression* expression;
    ExitStatement(Expression* expr): expression(expr) {};
    void print() override {
        std::cout << "exit ";
        expression->print();
    }
};
struct WhileStatement: public Statement {
    Expression* condition;
    WhileStatement(Expression* condition): condition(condition) {};
    void print() override {
        std::cout << "while ";
        condition->print();
    }
};
struct BreakStatement: public Statement {
    void print() override {
        std::cout << "break\n";
    }
};
struct ContinueStatement: public Statement {
    void print() override {
        std::cout << "continue\n";
    }
};

struct EnumStatement : public Statement {
    Identifier* name;
    Type* extend;
    std::vector<Element*> children;
    EnumStatement(std::vector<Element*> children,Identifier* name, Type* extend) :children(children), name(name),extend(extend)  {}
    void print () override {
        std::cout << "enum:\n\t";
        name->print();
        extend->print();
        for(auto& element : children){
            element->print();
        }
    }
};

struct ImportStatement : public Statement {
    std::string path;
    std::vector<Identifier*> imports;
    ImportStatement(std::vector<Identifier*> imports, std::string path): imports(imports), path(path) {}
    void print() override {
        std::cout << "import: \n\t";
        std::cout << "path: " << this->path << "\n\t";
        for(auto& import : imports){
            import ->print();
        }
    }
};

struct FunctionCall : public Statement {
    ASTNode* identifier;
    std::vector<ASTNode*> args;
    FunctionCall(ASTNode* identifier, std::vector<ASTNode*> args): identifier(identifier), args(args) {}
    void print() override{
        std::cout << "FunctionCall:\n";
        identifier->print();
        for(auto& arg: args){
            arg->print();
        }
    }
};
