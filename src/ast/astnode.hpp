#pragma once
#include <iostream>
#include <vector>

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void print() = 0;
};

class Container : public ASTNode {
public:
    std::vector<ASTNode*> nodes;
    Container(): nodes{}{}
    ~Container(){
        for(auto& node : nodes){
            delete node;
        }
    }
    void print() override {
        for(auto& node : nodes){
            node->print();
        }
    }
    void add(ASTNode* node) {
        nodes.push_back(node);
    }
};

class RootNode : public Container {
public:
    RootNode(): Container(){}

};