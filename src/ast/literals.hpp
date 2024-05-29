#pragma once
#include "astnode.hpp"

class IntegerLiteral : public ASTNode {
public:
    int value;
    IntegerLiteral(int value): value(value) {}
    const void print() override{
        std::cout << "IntegerLiteral: " << value << '\n';
    }
};

class NumericLiteral : public ASTNode {
public:
    double value;
    NumericLiteral(double value) : value(value) {}
    const void print() override{
        std::cout << "NumericLiteral: " << value << '\n';
    }
};