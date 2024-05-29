#pragma once
#include <iostream>
#include <vector>

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual const void print() = 0;
};

class RootNode : public ASTNode {
public:
    std::vector<ASTNode*> nodes;
    RootNode(){}
    const void print() override {
        for(auto& node : nodes){
            node->print();
        }
    }
    void add(ASTNode* node) {
        nodes.push_back(node);
    }

};