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

class FunctionContainer : public Container {
public:
    // std::vector<ASTNode*> args; TO BE IMPLEMENTED
    ASTNode* identifier;
    ASTNode* type;
    FunctionContainer(ASTNode* identifier,ASTNode* type): Container(),identifier(identifier), type(type) {}
    void print() override{
        std::cout << "Function:\n";
        identifier->print();
        type->print();

        for(auto& node : nodes){
            std::cout << '\t';
            node -> print();
        }
    }
};

class FunctionCall : public ASTNode {
public:
    ASTNode* identifier;
    FunctionCall(ASTNode* identifier): identifier(identifier) {}
    void print() override{
        std::cout << "FunctionCall:\n";
        identifier->print();
    }
};