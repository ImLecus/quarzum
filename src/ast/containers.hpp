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
    void check() override {
        Container::check();
        if(condition->type->name != "bool"){
            throwTypeError("'if' condition must be a boolean");
        }
    }
    void generateIR() override {
        condition->generateIR();
        std::cout << "GOTO " << getLIndex() << " IF " << condition->index << '\n';
        ir.push_back(IRInstruction{GOTO, getLIndex(), condition->index});
        std::cout << getLIndex() << ":\n";
        
        ir.push_back(IRInstruction{LABEL, getLIndex()});
        lIndex++;
        Container::generateIR();
        
        
        lIndex = 0;
        std::cout << getCIndex() << ":\n";
        ir.push_back(IRInstruction{LABEL, getCIndex()});
        cIndex++;
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
    void check() override {
        Container::check();
        if(condition->type->name != "bool"){
            throwTypeError("'if' condition must be a boolean");
        }
    }
    void generateIR() override {
        std::string initialIndex = getLIndex();
        std::cout << getLIndex() << ":\n";
        ir.push_back(IRInstruction{LABEL, getLIndex()});
        lIndex++;

        std::cout << "GOTO " << getLIndex() << " IF " << condition->index << '\n';
        ir.push_back(IRInstruction{GOTO, getLIndex(), condition->index});
        
        std::cout << "GOTO " << getCIndex() << '\n';
        ir.push_back(IRInstruction{GOTO, getCIndex()});

        std::cout << getLIndex() << ":\n";
        ir.push_back(IRInstruction{LABEL, getLIndex()});
        lIndex++;
        Container::generateIR();
        
        std::cout << "GOTO " << initialIndex << '\n';
        ir.push_back(IRInstruction{GOTO, initialIndex});
        
        lIndex = 0;
        std::cout << getCIndex() << ":\n";
        ir.push_back(IRInstruction{LABEL, getCIndex()});
        cIndex++;
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
    void check() override {
        Container::check();
    }
};

struct ForeachContainer : public Container {
    Identifier* identifier;
    Expression* iterable;
    GenericType* type;

    ForeachContainer(Identifier* identifier, Expression* iterable, GenericType* type): Container(), identifier(identifier), iterable(iterable), type(type) {}
    void print() override{
        std::cout << "foreach:\n\t";
        identifier->print();
        std::cout << "Argument:\n\t"<<type->name<<"\n\t";
        iterable -> print();
        printChildren();
    }
    void check() override {
        Container::check();
    }
};


struct FunctionContainer : public Container {
    Identifier* identifier;
    GenericType* type;
    std::vector<Argument*> args;
    FunctionContainer(Identifier* identifier,GenericType* type, std::vector<Argument*> args): 
        Container(),identifier(identifier), type(type), args(args) {}
    void print() override{
        std::cout << "function:\n";
        identifier->print();
        std::cout << "Argument:\n\t"<<type->name<<"\n\t";
        for(auto& arg : args){
            arg->print();
        }
        printChildren();
    }
    void check() override {
        Container::check();
    }
    void generateIR() override {
        std::cout << identifier->value << ":\n";
        ir.push_back(IRInstruction{LABEL, identifier->value});
        for(Argument* arg: args){
            std::cout << "PARAM " << arg->id->value << '\n';
            ir.push_back(IRInstruction{PARAM, arg->id->value});
        }
        Container::generateIR();
        std::cout << '_' << identifier->value << ":\n";
        ir.push_back(IRInstruction{LABEL, '_' + identifier->value });
    }
};

struct MethodContainer : public Container {
    Access access;
    FunctionContainer* func;
    MethodContainer(Access access, FunctionContainer* func): access(access), func(func){}
    void print () override {
        std::cout << "method:\n\taccess:" << std::to_string(access) << "\n\t";
        std::cout << "Argument:\n\t"<<func->type->name<<"\n\t";
        func->identifier->print();
        for(auto& arg: func->args){
            arg->print();
        }
        for(auto& node: nodes){
            node->print();
        }
    }
    void check() override {
        Container::check();
    }
};

struct ClassContainer : public Container {
    Identifier* identifier;
    GenericType* inherits;
    ClassContainer(Identifier* id, GenericType* inherits): identifier(id), inherits(inherits) {}
    void print() override{
        std::cout << "class:\n";
        identifier->print();
        if(inherits){std::cout << "Argument:\n\t"<<inherits->name<<"\n\t";}
        printChildren();
    }
    void check() override {
        Container::check();
    }
};

struct ForContainer : public Container {
    VariableDeclaration* decl;
    Expression* condition;
    VariableRedeclaration* redec;
    ForContainer(VariableDeclaration* decl, Expression* condition, VariableRedeclaration* redec): decl(decl), condition(condition), redec(redec){}
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
    void check() override {
        Container::check();
    }
};