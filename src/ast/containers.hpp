#pragma once
#include "astnode.hpp"

struct IfContainer: public Container {
    Expression* condition;
    IfContainer(Expression* condition): Container(), condition(condition) {}
    void print() override{
        std::cout << "if:\n\t";
        condition->print();
        printChildren();
    }
};

struct WhileContainer : public Container {
    Expression* condition;

    WhileContainer(Expression* condition): Container(), condition(condition) {}
    void print() override{
        std::cout << "while:\n\t";
        condition->print();
        printChildren();
    }
};

struct DoContainer : public Container {
    void print() override{
        std::cout << "do:\n\t";
        printChildren();
    }
};

struct ModuleContainer : public Container {
    Identifier* identifier;
    bool isClass;
    ModuleContainer(Identifier* identifier, bool isClass): Container(), identifier(identifier), isClass(isClass) {}
    void print() override{
        std::cout << "module:\n\t";
        identifier->print();
        std::cout << "\tclass: " << isClass << '\n';
        printChildren();
    }
};

struct ForeachContainer : public Container {
    Identifier* identifier;
    Expression* iterable;
    Type* type;

    ForeachContainer(Identifier* identifier, Expression* iterable, Type* type): Container(), identifier(identifier), iterable(iterable), type(type) {}
    void print() override{
        std::cout << "foreach:\n\t";
        identifier->print();
        type -> print();
        iterable -> print();
        printChildren();
    }
};


struct FunctionContainer : public Container {
    Identifier* identifier;
    Type* type;
    std::vector<ASTNode*> args;
    FunctionContainer(Identifier* identifier,Type* type, std::vector<ASTNode*> args): 
        Container(),identifier(identifier), type(type), args(args) {}
    void print() override{
        std::cout << "function:\n";
        identifier->print();
        type->print();
        for(auto& arg : args){
            arg->print();
        }
        printChildren();
    }
};

struct MethodContainer : public Container {
    Access access;
    FunctionContainer* func;
    MethodContainer(Access access, FunctionContainer* func): access(access), func(func){}
    void print () override {
        std::cout << "method:\n\taccess:" << std::to_string(access) << "\n\t";
        func->type->print();
        func->identifier->print();
        for(auto& arg: func->args){
            arg->print();
        }
        for(auto& node: nodes){
            node->print();
        }
    }
    
};

struct ClassContainer : public Container {
    Identifier* identifier;
    Type* inherits;
    ClassContainer(Identifier* id, Type* inherits): identifier(id), inherits(inherits) {}
    void print() override{
        std::cout << "class:\n";
        identifier->print();
        if(inherits){inherits->print();}
        printChildren();
    }
};

struct ForContainer : public Container {
    ASTNode* decl;
    ASTNode* condition;
    ASTNode* redec;
    ForContainer(ASTNode* decl, ASTNode* condition, ASTNode* redec): decl(decl), condition(condition), redec(redec){}
    void print() override{
        std::cout << "for:\n";
        if(decl){
           decl->print(); 
        }
        if(condition){
           condition->print();
        }
        if(redec){
            redec->print();
        }
        printChildren();
    }
};