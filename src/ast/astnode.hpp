#pragma once
#include <iostream>
#include "../semantics/types.cpp"
#include "../ir/irinstruction.hpp"
#include <vector>

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual void print() = 0;
    virtual void generateIR(){};
};

struct Statement : public ASTNode {
    virtual void check() = 0;
};

struct Expression : public ASTNode {
    void print() override {
        std::cout << "expr";
    }
    GenericType* type;
    std::string index; 
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
        for(Statement* node : nodes){
            node->check();
        }
    }

};

struct RootNode : public Container {
    RootNode(): Container(){}
    void generateIR() override {
        for(Statement* node : nodes){
            node->generateIR();
        }
    }
};