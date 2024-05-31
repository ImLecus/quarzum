#pragma once
#include "astnode.hpp"

class Statement : public ASTNode {
public: 
    std::vector<ASTNode*> children;
    Statement(std::vector<ASTNode*> children): children(children) {}
};

class FunctionStatement : public Statement {
public:
    FunctionStatement(std::vector<ASTNode*> children): Statement(children) {}

};

class ModuleContainer : public Container {
public:
    ASTNode* identifier;
    bool isClass;
    ModuleContainer(ASTNode* identifier, bool isClass): Container(), identifier(identifier), isClass(isClass) {}
    void print() override{
        std::cout << "Module:\n";
        identifier->print();
        std::cout << "isClass: " << isClass << '\n';
        for(auto& node : nodes){
            std::cout << '\t';
            node -> print();
        }
    }
};

class IfContainer : public Container {
public:
    ASTNode* condition;

    IfContainer(ASTNode* condition): Container(), condition(condition) {}
    void print() override{
        std::cout << "If:\n\t";
        condition->print();
        for(auto& node : nodes){
            std::cout << '\t';
            node -> print();
        }
    }
};

class WhileContainer : public Container {
public:
    ASTNode* condition;

    WhileContainer(ASTNode* condition): Container(), condition(condition) {}
    void print() override{
        std::cout << "While:\n\t";
        condition->print();
        for(auto& node : nodes){
            std::cout << '\t';
            node -> print();
        }
    }
};

class FunctionContainer : public Container {
public:
    ASTNode* identifier;
    ASTNode* type;
    std::vector<ASTNode*> args;
    FunctionContainer(ASTNode* identifier,ASTNode* type, std::vector<ASTNode*> args): 
        Container(),identifier(identifier), type(type), args(args) {}
    void print() override{
        std::cout << "Function:\n";
        identifier->print();
        type->print();
        for(auto& arg : args){
            arg->print();
        }

        for(auto& node : nodes){
            std::cout << '\t';
            node -> print();
        }
    }
};