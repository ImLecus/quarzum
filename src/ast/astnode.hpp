#pragma once
#include <iostream>
#include <vector>

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void print() = 0;
};

struct Container : public ASTNode {
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

struct RootNode : public Container {
    RootNode(): Container(){}
};