#pragma once
#include "astnode.hpp"

class Identifier : public ASTNode {
public:
    std::string name;
    Identifier(std::string name) : name(name) {}
    const void print() override{
        std::cout << "Identifier: " << name << '\n';
    }
};

class Type : public ASTNode {
public:
    std::string type;
    Type(std::string type) : type(type) {}
    const void print() override{
        std::cout << "Type: " << type << '\n';
    }
};