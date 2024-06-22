#pragma once
#include "astnode.hpp"
#include "../semantics/types.cpp"

struct Literal : public Expression {
    std::string value;
    Literal(GenericType* type,const std::string& value): value(value) {
        this->type = type;
        this->index = value;
    }
    void check() override {

    }
};

struct Identifier : public Literal {
    bool withScope = false;
    Identifier(const std::string& name) : Literal(new NullType(),name) {}
    void check() override {
        type = getTypeByName(symbolTable.find(value)->dataType);
    }
};

struct Element : public ASTNode {
    Identifier* id;
    Expression* value;
    Element(Identifier*& id): id(id), value(nullptr){}
    Element(Identifier*& id, Expression* value): id(id), value(value){}
};