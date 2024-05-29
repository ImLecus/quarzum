#pragma once
#include "astnode.hpp"

class Literal : public ASTNode {
public:
    Literal(std::string value): value(value) {}
    std::string getValue(){
        return this->value;
    }
private:
    std::string value;
};

class IntegerLiteral : public Literal {
public:
    IntegerLiteral(std::string value): Literal(value) {}
    void print() override{
        std::cout << "IntegerLiteral: " << getValue() << '\n';
    }
};

class NumericLiteral : public Literal {
public:
    NumericLiteral(std::string value) : Literal(value) {}
    void print() override{
        std::cout << "NumericLiteral: " << getValue() << '\n';
    }
};

class NullLiteral : public Literal {
public:
    NullLiteral() : Literal("null") {}
    void print() override{
        std::cout << "NullLiteral\n";
    }
};