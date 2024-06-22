#pragma once
#include "astnode.hpp"

struct ElseContainer : public Container {
};
struct IfContainer: public Container {
    Expression* condition;
    ElseContainer* elseContainer = nullptr;
    IfContainer(Expression* condition): Container(), condition(condition) {}
    void print() override{
        std::cout << "if:\n\t";
        condition->print();
        printChildren();
    }
    void check() override {
        Container::check();
        condition->check();
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
        tIndex = 0;

        if(elseContainer != nullptr){
            elseContainer->generateIR();
        }
        
        ir.push_back(IRInstruction{GOTO, getCIndex()});

        ir.push_back(IRInstruction{LABEL, getLIndex()});
        lIndex++;

        Container::generateIR();

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
        condition->check();
        Container::check();
        if(condition->type->name != "bool"){
            throwTypeError("'if' condition must be a boolean");
        }
    }
    void generateIR() override {
        std::string initialIndex = getLIndex();
        ir.push_back(IRInstruction{LABEL, getLIndex()});
        lIndex++;
        condition->generateIR();
        if(auto l = dynamic_cast<Literal*>(condition)){
            ir.push_back(IRInstruction{GOTO, getLIndex(), condition->index, "literal", l->type->name});
        }
        else {
           ir.push_back(IRInstruction{GOTO, getLIndex(), condition->index}); 
        }
        tIndex = 0;
        
        
        ir.push_back(IRInstruction{GOTO, getCIndex()});

        ir.push_back(IRInstruction{LABEL, getLIndex()});
        lIndex++;
        Container::generateIR();
        
        ir.push_back(IRInstruction{GOTO, initialIndex});
        
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
    ModuleContainer(Identifier* identifier): Container(), identifier(identifier) {}
    void print() override{
        std::cout << "module:\n\t";
        identifier->print();
        printChildren();
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
        symbolTable.insert(identifier->value, {'f',identifier->value,type->name,"global"});
        symbolTable.enterScope();
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
        if(decl){
           decl->check(); 
        }
        if(condition){
           condition->check();
        }
        if(redec){
            redec->check();
        }
        Container::check();
    }
    void generateIR() override {
        if(decl){
            decl->generateIR();
        }
        std::string initialIndex = getLIndex();
        ir.push_back(IRInstruction{LABEL, getLIndex()});
        lIndex++;
        if(condition){
           condition->generateIR();
           ir.push_back(IRInstruction{GOTO, getLIndex(), condition->index}); 
        }
        ir.push_back(IRInstruction{GOTO, getCIndex()}); 
        ir.push_back(IRInstruction{LABEL, getLIndex()}); //AQUI
        lIndex++;
        Container::generateIR();
        if(redec){
            redec->generateIR();
        }
        ir.push_back(IRInstruction{GOTO, initialIndex}); 
        ir.push_back(IRInstruction{LABEL, getCIndex()});
        cIndex++;
    }
};