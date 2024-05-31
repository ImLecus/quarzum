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
        std::cout << "VariableDeclaration:\n";
        std::cout <<'\t';
        type->print();
        std::cout <<'\t';
        identifier->print();
        std::cout <<"\tconstant: " << constant << '\n';
        std::cout <<'\t';
        expression->print();
    }

    ~VariableDeclaration() {
        delete type;
        delete identifier;
        delete expression;
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