#pragma once
#include <iostream>
#include "../semantics/types.cpp"
#include <vector>

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual void print() = 0;
};

struct Statement : public ASTNode {
    virtual void check() = 0;
};

struct Expression : public ASTNode {
    void print() override {
        std::cout << "expr";
    }
    GenericType* type;
};

/**
 * @brief Containers creates identations and stores multiple nodes inside them.
*/
struct Container : public Statement {
    std::vector<Statement*> nodes;
    Container(): nodes{} {}
    ~Container(){
        for(auto& node : nodes){
            delete node;
        }
    }
    void print() override {
        printChildren();
    }
    void add(Statement* node) {
        nodes.push_back(node);
    }
    void printChildren(){
        for(auto& node : nodes){
            node->print();
        }
    }
    void check() override {
        
    }
};

struct RootNode : public Container {
    RootNode(): Container(){}
    void check() override {
        
    }
};