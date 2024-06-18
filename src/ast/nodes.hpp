#pragma once
#include "astnode.hpp"

struct Type : public ASTNode {
    std::string type;
    bool constant;
    Type(std::string type, bool constant = false) : type(type), constant(constant) {}
    void print() override{
        std::cout << "Type: " << type << '\n';
    }
};
struct Argument : public ASTNode {
    Type* type;
    Identifier* id;

    Expression* defaultValue = nullptr;
    Argument(Type* type, Identifier* id): type(type), id(id) {}
    Argument(Type* type, Identifier* id, Expression* defaultValue): type(type), id(id), defaultValue(defaultValue) {}
    void print() override{
        std::cout << "Argument:\n";
        type->print();
        id->print();
        if(defaultValue){
            defaultValue -> print();
        }
    }
};