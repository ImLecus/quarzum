#pragma once
#include "astnode.hpp"

struct Literal : public Expression {
    std::string value;
    Literal(std::string value): value(value) {}
    std::string getValue(){
        return value;
    }
    
};

struct Identifier : public Literal {
    Identifier(std::string name) : Literal(name) {}
    void print() override{
        std::cout << "Identifier: " << getValue() << '\n';
    }
};

struct IntegerLiteral : public Literal {
    IntegerLiteral(std::string value): Literal(value) {}
    void print() override{
        std::cout << "IntegerLiteral: " << getValue() << '\n';
    }
};

struct NumericLiteral : public Literal {
    NumericLiteral(std::string value) : Literal(value) {}
    void print() override{
        std::cout << "NumericLiteral: " << getValue() << '\n';
    }
};

struct CharLiteral : public Literal {
    CharLiteral(std::string value) : Literal(value) {}
    void print() override{
        std::cout << "CharLiteral: " << getValue() << '\n';
    }
};

struct StringLiteral : public Literal {
    StringLiteral(std::string value) : Literal(value) {}
    void print() override{
        std::cout << "StringLiteral: " << getValue() << '\n';
    }
};

struct BoolLiteral : public Literal {
    BoolLiteral(std::string value) : Literal(value) {}
    void print() override{
        std::cout << "BoolLiteral: " << getValue() << '\n';
    }
};

struct NullLiteral : public Literal {
    NullLiteral() : Literal("null") {}
    void print() override{
        std::cout << "NullLiteral\n";
    }
};