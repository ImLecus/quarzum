#pragma once
#include "astnode.hpp"

class VariableDeclaration : public ASTNode {
public:
    ASTNode* type;
    ASTNode* identifier;
    ASTNode* expression;

    VariableDeclaration(ASTNode* varType, ASTNode* varName, ASTNode* initValue): type(varType), identifier(varName), expression(initValue) {}

    void print() override  {
        std::cout << "VariableDeclaration:\n";
        std::cout <<'\t';
        type->print();
        std::cout <<'\t';
        identifier->print();
        if (expression) {
            std::cout <<'\t';
            expression->print();
        }
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