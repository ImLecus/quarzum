#pragma once
#include "astnode.hpp"

struct UnaryExpression : public Expression {
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

struct BinaryExpression : public Expression {
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

struct NullExpression : public Expression {
    NullExpression(){}
    void print() override{
        std::cout << "NullExpression\n";
    }
};