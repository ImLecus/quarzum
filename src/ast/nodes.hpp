#pragma once
#include "astnode.hpp"

struct Type : public ASTNode {
    std::string type;
    Type(std::string type) : type(type) {}
    void print() override{
        std::cout << "Type: " << type << '\n';
    }
};

struct Symbol : public ASTNode {
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


struct FunctionCall : public ASTNode {
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

struct Argument : public ASTNode {
    Symbol* symbol;

    ASTNode* defaultValue = nullptr;
    Argument(Symbol* symbol): symbol(symbol) {}
    Argument(Symbol* symbol, ASTNode* defaultValue): symbol(symbol), defaultValue(defaultValue) {}
    void print() override{
        std::cout << "Argument:\n";
        symbol->print();
        if(defaultValue != nullptr){
            defaultValue -> print();
        }
    }
};

struct EnumElement : public ASTNode {
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