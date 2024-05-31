#pragma once
#include "astnode.hpp"

class Statement : public ASTNode {
public: 
    std::vector<ASTNode*> children;
    Statement(std::vector<ASTNode*> children): children(children) {}
};

class EnumStatement : public Statement {
public:
    ASTNode* name;
    ASTNode* extend;
    EnumStatement(std::vector<ASTNode*> children,ASTNode* name, ASTNode* extend) :Statement(children), name(name),extend(extend)  {}
    void print () override {
        std::cout << "EnumStatement:\n\t";
        name->print();
        extend->print();
        for(auto& element : children){
            element->print();
        }
    }
};

class ImportStatement : public Statement {
public:
    std::string path;
    ImportStatement(std::vector<ASTNode*> imports, std::string path): Statement(imports), path(path) {}
    void print() override {
        std::cout << "Import: \n\t";
        std::cout << "Path: " << this->path << "\n\t";
        for(auto& import : children){
            import ->print();
        }
    }
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