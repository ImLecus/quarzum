#pragma once
#include "astnode.hpp"
#include "../semantics/types.cpp"

struct Literal : public Expression {
    std::string value;
    Literal(GenericType* type,std::string value): value(value) {
        this->type = type;
        this->index = value;
    }
    void print() override {
        std::cout << type->name << " (" << value << ")\n";
    }
};

struct Identifier : public Literal {
    Identifier(std::string name) : Literal(new NullType(),name) {}
    void print() override{
        std::cout << "Identifier: " << value << '\n';
    }
};

struct Element : public ASTNode {
    Identifier* id;
    Expression* value;
    Element(Identifier* id): id(id), value(nullptr){}
    Element(Identifier* id, Expression* value): id(id), value(value){}
    void print() override {
        std::cout << "element:";
        id->print();
        if(value){value->print();}
    }
};