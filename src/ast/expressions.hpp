#pragma once
#include "astnode.hpp"

class UnaryExpression : public ASTNode {
public:
    std::string op; 
    ASTNode* operand;

    UnaryExpression(const std::string& oper, ASTNode* expr) : op(oper), operand(expr) {}

    void print() override {
        std::cout << "UnaryExpression: (" << op << ")\n\t";
        operand->print();
    }

    ~UnaryExpression() {
        delete operand;
    }
};

class BinaryExpression : public ASTNode {
public:
    std::string op;
    ASTNode* left; 
    ASTNode* right;

    BinaryExpression(const std::string& op, ASTNode* left, ASTNode* right): op(op), left(left), right(right) {}

    void print() override{
        std::cout << "BinaryExpression: (" << op << ")\n";
        std::cout << '\t';
        left->print();
        std::cout << '\t';
        right->print();
    }

    ~BinaryExpression() {
        delete left;
        delete right;
    }
};

class NullExpression : public ASTNode {
public:
    NullExpression(){}
    void print() override{
        std::cout << "NullExpression\n";
    }
};