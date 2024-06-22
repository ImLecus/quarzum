#pragma once
#include "astnode.hpp"

struct VariableDeclaration : public Statement {
    GenericType* type;
    Identifier* id;
    Expression* expression;

    VariableDeclaration(GenericType* type, Identifier* id, Expression* initValue): 
        type(type), id(id), expression(initValue) {}

    void print() override  {
        std::cout << "VariableDeclaration:\n\t" << type->name << "\n\t";
        id->print();
        std::cout <<"\tconstant: " << type->constant << "\n\t";
        expression->print();

    }

    void check() override {
        expression->check();
        if(symbolTable.find(id->value) != nullptr){
            throwError("Symbol " + id->value + " has been already declared");
        }
        symbolTable.insert(id->value, {'v',id->value,type->name,"global"});
    }

    void generateIR(){
        if(Literal* l = dynamic_cast<Literal*>(expression)){
            if(l->value == "true"){l->value="1";}
            if(l->value == "false" or l->value == "null"){l->value = "0";}
            ir.push_back(IRInstruction{ASSIGN,id->value,l->value,"literal",type->name});
            return;
        }
        expression->generateIR();
        ir.push_back(IRInstruction{ASSIGN,id->value,expression->index,"",type->name});
        tIndex = 0;

    }

    ~VariableDeclaration() {
        delete type;
        delete id;
        delete expression;
    }

};

enum Access: u_int8_t {
    PUBLIC = 0,
    PRIVATE = 1,
    PROTECTED = 2
};
struct AtributeDeclaration : public Statement {
    Access access;
    VariableDeclaration* var;
    AtributeDeclaration(Access access, VariableDeclaration* var): access(access), var(var) {}
    void print() override {
        std::cout << "atribute:\n\t"<<
        "access: " << std::to_string(access) << "\n\t";
        var->id->print();
        std::cout << var->type->name << "\n\t";
        var->expression->print();
        std::cout << "constant: " << var->type->constant << "\n";
    }
    void check() override {
        var->check();
    }
};

struct ArrayDeclaration : public Statement {
    ASTNode* type;
    ASTNode* identifier;
    std::vector<ASTNode*> elements;
    bool constant;
    ASTNode* size;

    ArrayDeclaration(ASTNode* varType, ASTNode* varName, std::vector<ASTNode*> elements, bool constant): 
        type(varType), identifier(varName), elements(elements), constant(constant) {
            size = new Literal(new UInteger(),std::to_string(elements.size()));
        }
    ArrayDeclaration(ASTNode* varType, ASTNode* varName, std::vector<ASTNode*> elements, bool constant, ASTNode* size): 
        type(varType), identifier(varName), elements(elements), constant(constant), size(size) {}

    void print() override  {
        std::cout << "ArrayDeclaration:\n\t";
        type->print();
        std::cout <<'\t';
        identifier->print();
        std::cout <<"\tconstant: " << constant << "\n\t";
        size->print();
        for(auto& element : elements){
            element->print();
        }
    }

    ~ArrayDeclaration() {
        delete type;
        delete identifier;
        for(auto& element: elements){
            delete element;
        }
        delete size;
    }
    void check() override {
        
    }
};

struct VariableRedeclaration : public Statement {
    Identifier* identifier;
    Expression* expression;

    VariableRedeclaration(Identifier* varName, Expression* value): identifier(varName), expression(value) {
        
    }

    void print() override  {
        std::cout << "redec:\n";
        std::cout <<'\t';
        identifier->print();
        if (expression) {
            std::cout <<'\t';
            expression->print();
        }
    }

    ~VariableRedeclaration() {
        delete identifier;
        delete expression;
    }
    void check() override {
       
        identifier->check();
        expression->check();
        if(symbolTable.find(identifier->value) == nullptr){
            throwError("Variable " + identifier->value + " was not defined");
        }
        if(expression->type->flag != identifier->type->flag){
            throwTypeError("Value of type " + expression->type->name + " can not be applied to variable " + identifier->type->name);
        }
        
    }

    void generateIR() override {
        
        if(Literal* l = dynamic_cast<Literal*>(expression)){
            if(l->value == "true"){l->value="1";}
            if(l->value == "false" or l->value == "null"){l->value = "0";}
            ir.push_back(IRInstruction{REASSIGN,identifier->value,l->index,"literal",identifier->type->name});
            return;
        }
        expression->generateIR();
        ir.push_back(IRInstruction{REASSIGN,identifier->value,expression->index,"", identifier->type->name});
        tIndex = 0;
    }
};