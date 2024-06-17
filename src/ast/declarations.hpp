#pragma once
#include "astnode.hpp"

struct VariableDeclaration : public Statement {
    Symbol* symbol;
    ASTNode* expression;

    VariableDeclaration(Symbol* symbol, ASTNode* initValue): 
        symbol(symbol), expression(initValue) {}

    void print() override  {
        std::cout << "VariableDeclaration:\n\t";
        symbol->type->print();
        std::cout <<'\t';
        symbol->id->print();
        std::cout <<"\tconstant: " << symbol->constant << "\n\t";
        expression->print();

    }

    void generateIR(){
        if(Literal* l = dynamic_cast<Literal*>(expression)){
            std::cout << symbol->id->getValue() << " = LOADI " << l->getValue() << '\n';
        }
        
    }

    ~VariableDeclaration() {
        delete symbol;
        delete expression;
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
            size = new IntegerLiteral(std::to_string(elements.size()));
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
};

struct VariableRedeclaration : public Statement {
    ASTNode* identifier;
    ASTNode* expression;

    VariableRedeclaration(ASTNode* varName, ASTNode* value): identifier(varName), expression(value) {}

    void print() override  {
        std::cout << "VariableRedeclaration:\n";
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
};