#pragma once
#include "astnode.hpp"

class Return : public ASTNode {
public:
    ASTNode* value;
    Return(ASTNode* value): value(value) {}
    void print() override  {
        std::cout << "Return:\n\t";
        value->print();
    }
};

class Continue : public ASTNode {
public:
    Continue() {}
    void print() override  {
        std::cout << "Continue\n";
    }
};
class Break : public ASTNode {
public:
    Break() {}
    void print() override  {
        std::cout << "Break\n";
    }
};

class Exit : public ASTNode {
public:
    ASTNode* value;
    Exit(ASTNode* value): value(value) {}
    void print() override  {
        std::cout << "Exit:\n\t";
        value->print();
    }
};

class VariableDeclaration : public ASTNode {
public:
    ASTNode* type;
    ASTNode* identifier;
    ASTNode* expression;
    bool constant;

    VariableDeclaration(ASTNode* varType, ASTNode* varName, ASTNode* initValue, bool constant): 
        type(varType), identifier(varName), expression(initValue), constant(constant) {}

    void print() override  {
        std::cout << "VariableDeclaration:\n\t";
        type->print();
        std::cout <<'\t';
        identifier->print();
        std::cout <<"\tconstant: " << constant << "\n\t";
        expression->print();
    }

    ~VariableDeclaration() {
        delete type;
        delete identifier;
        delete expression;
    }
};

class ArrayDeclaration : public ASTNode {
public:
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

class VariableRedeclaration : public ASTNode {
public:
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