#pragma once
#include "astnode.hpp"

class Type : public ASTNode {
public:
    std::string type;
    Type(std::string type) : type(type) {}
    void print() override{
        std::cout << "Type: " << type << '\n';
    }
};

class Symbol : public ASTNode {
public:
    Type* type;
    Identifier* id;
    bool constant;
    bool array;
    bool sizeDefined;
    ASTNode* arrayLength;
    Symbol(Type* type, Identifier* id, bool constant, bool array, bool sizeDefined, ASTNode* arrayLength): 
    type(type), id(id), constant(constant), array(array), sizeDefined(sizeDefined), arrayLength(arrayLength) {}

    Symbol(Type* type, Identifier* id, bool constant): 
    type(type), id(id), constant(constant) {}
    void print() override{
        type->print();
        id->print();
        std::cout << "Constant: " << constant << '\n';
    }
};


class FunctionCall : public ASTNode {
public:
    ASTNode* identifier;
    std::vector<ASTNode*> args;
    FunctionCall(ASTNode* identifier, std::vector<ASTNode*> args): identifier(identifier), args(args) {}
    void print() override{
        std::cout << "FunctionCall:\n";
        identifier->print();
        for(auto& arg: args){
            arg->print();
        }
    }
};

class Argument : public ASTNode {
public:
    ASTNode* type;
    ASTNode* identifier;
    ASTNode* defaultValue = nullptr;
    Argument(ASTNode* type, ASTNode* identifier): type(type), identifier(identifier) {}
    Argument(ASTNode* type, ASTNode* identifier, ASTNode* defaultValue): 
    type(type), identifier(identifier), defaultValue(defaultValue) {}
    void print() override{
        std::cout << "Argument:\n";
        type->print();
        identifier->print();
        if(defaultValue != nullptr){
            defaultValue -> print();
        }
    }
};

class EnumElement : public ASTNode {
public:
    ASTNode* identifier;
    ASTNode* defaultValue = nullptr;
   
    EnumElement(ASTNode* identifier, ASTNode* defaultValue): 
    identifier(identifier), defaultValue(defaultValue) {}
    void print() override{
        std::cout << "EnumElement:\n";
        identifier->print();
        if(defaultValue != nullptr){
            defaultValue -> print();
        }
    }
};