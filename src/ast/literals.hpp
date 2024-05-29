#include "astnode.hpp"

class IntegerLiteral : public ASTNode {
public:
    int value;
    IntegerLiteral(int value): value(value) {}
    const void print() override{
        std::cout << "Integer Literal: " << value << '\n';
    }
};

class NumericLiteral : public ASTNode {
public:
    double value;
    NumericLiteral(double value) : value(value) {}
    const void print() override{
        std::cout << "Numeric Literal: " << value << '\n';
    }
};