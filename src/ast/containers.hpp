#pragma once
#include "astnode.hpp"

struct ElseContainer : public Container {};
struct IfContainer: public Container {
    Expression* condition;
    ElseContainer* elseContainer = nullptr;
    IfContainer(Expression* condition): Container(), condition(condition) {}
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

struct DoContainer : public Container {};

struct ModuleContainer : public Container {
    Identifier* identifier;
    ModuleContainer(Identifier* identifier): Container(), identifier(identifier) {}
};

struct ForeachContainer : public Container {
    Identifier* identifier;
    Expression* iterable;
    GenericType* type;

    ForeachContainer(Identifier* identifier, Expression* iterable, GenericType* type): Container(), identifier(identifier), iterable(iterable), type(type) {}
};


struct FunctionContainer : public Container {
    Identifier* identifier;
    GenericType* type;
    std::vector<Argument*> args;
    FunctionContainer(Identifier*& identifier,GenericType*& type, std::vector<Argument*>& args): 
        Container(),identifier(identifier), type(type), args(args) {}

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
    MethodContainer(Access access, FunctionContainer*& func): access(access), func(func){}
};

struct ClassContainer : public Container {
    Identifier* identifier;
    GenericType* inherits;
    ClassContainer(Identifier*& id, GenericType*& inherits): identifier(id), inherits(inherits) {}
};

struct ForContainer : public Container {
    VariableDeclaration* decl;
    Expression* condition;
    VariableRedeclaration* redec;
    ForContainer(VariableDeclaration*& decl, Expression* condition, VariableRedeclaration*& redec): decl(decl), condition(condition), redec(redec){}
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