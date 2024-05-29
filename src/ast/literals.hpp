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

class CharLiteral : public Literal {
public:
    CharLiteral(std::string value) : Literal(value) {}
    void print() override{
        std::cout << "CharLiteral: " << getValue() << '\n';
    }
};

class StringLiteral : public Literal {
public:
    StringLiteral(std::string value) : Literal(value) {}
    void print() override{
        std::cout << "StringLiteral: " << getValue() << '\n';
    }
};

class BoolLiteral : public Literal {
public:
    BoolLiteral(std::string value) : Literal(value) {}
    void print() override{
        std::cout << "BoolLiteral: " << getValue() << '\n';
    }
};

class NullLiteral : public Literal {
public:
    NullLiteral() : Literal("null") {}
    void print() override{
        std::cout << "NullLiteral\n";
    }
};