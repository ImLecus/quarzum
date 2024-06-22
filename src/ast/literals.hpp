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
    void check() override {

    }
};

struct Identifier : public Literal {
    bool withScope = false;
    Identifier(std::string name) : Literal(new NullType(),name) {}
    void print() override {
        std::cout << "Identifier: " << value << '\n';
    }
    void check() override {
        type = getTypeByName(symbolTable.find(value)->dataType);
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