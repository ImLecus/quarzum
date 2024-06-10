#pragma once
#include "astnode.hpp"

struct Statement : public ASTNode {
    std::vector<ASTNode*> children;
    Statement(std::vector<ASTNode*> children): children(children) {}
};

struct EnumStatement : public Statement {
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

struct ImportStatement : public Statement {
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

struct ModuleContainer : public Container {
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

struct IfContainer : public Container {
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

struct WhileContainer : public Container {
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

struct ForeachContainer : public Container {
    ASTNode* identifier;
    ASTNode* iterable;
    ASTNode* type;

    ForeachContainer(ASTNode* identifier, ASTNode* iterable, ASTNode* type): Container(), identifier(identifier), iterable(iterable), type(type) {}
    void print() override{
        std::cout << " Foreach:\n\t";
        identifier->print();
        type -> print();
        iterable -> print();
        for(auto& node : nodes){
            std::cout << '\t';
            node -> print();
        }
    }
};

struct FunctionContainer : public Container {
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