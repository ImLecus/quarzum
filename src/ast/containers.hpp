#pragma once
#include "astnode.hpp"

struct ElseContainer : public Container {};
struct IfContainer: public Container {
    Expression* condition;
    ElseContainer* elseContainer;
    IfContainer(Expression* condition, ElseContainer* elseContainer): 
    Container(), condition(condition), elseContainer(elseContainer) {}
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
        if(auto l = dynamic_cast<Literal*>(condition)){
            ir.push_back(IRInstruction{GOTO, getLIndex(), condition->index, "literal"});
        }
        else{
            ir.push_back(IRInstruction{GOTO, getLIndex(), condition->index});
        }

        if(elseContainer != nullptr){
            elseContainer->generateIR();
        }
        
        ir.push_back(IRInstruction{GOTO, getCIndex()});

        ir.push_back(IRInstruction{LABEL, getLIndex()});
        lIndex++;

        Container::generateIR();
        
        lIndex = 0;

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
        ir.push_back(IRInstruction{LABEL, getLIndex()});
        lIndex++;

        if(auto l = dynamic_cast<Literal*>(condition)){
            ir.push_back(IRInstruction{GOTO, getLIndex(), condition->index, "literal"});
        }
        else {
           ir.push_back(IRInstruction{GOTO, getLIndex(), condition->index}); 
        }
        
        
        ir.push_back(IRInstruction{GOTO, getCIndex()});

        ir.push_back(IRInstruction{LABEL, getLIndex()});
        lIndex++;
        Container::generateIR();
        
        ir.push_back(IRInstruction{GOTO, initialIndex});
        
        lIndex = 0;
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

        ir.push_back(IRInstruction{LABEL, identifier->value});
        for(Argument* arg: args){

            ir.push_back(IRInstruction{PARAM, arg->id->value});
        }
        Container::generateIR();

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