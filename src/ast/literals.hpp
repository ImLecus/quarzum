#pragma once
#include "astnode.hpp"

class IntegerLiteral : public ASTNode {
public:
    std::string value;
    IntegerLiteral(std::string value): value(value) {}
    void print() override{
        std::cout << "IntegerLiteral: " << value << '\n';
    }
};

class NumericLiteral : public ASTNode {
public:
    std::string value;
    NumericLiteral(std::string value) : value(value) {}
    void print() override{
        std::cout << "NumericLiteral: " << value << '\n';
    }
};