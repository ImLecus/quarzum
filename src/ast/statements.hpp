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