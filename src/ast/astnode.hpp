#pragma once
#include <iostream>
#include <vector>

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual void print() = 0;
};

struct Statement : public ASTNode {

};

struct Expression : public ASTNode {
    void print() override {
        std::cout << "expr";
    }
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
};

struct RootNode : public Container {
    RootNode(): Container(){}
};