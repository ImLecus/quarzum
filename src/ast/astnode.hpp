#pragma once
#include <iostream>
#include "../semantics/types.cpp"
#include "../ir/irinstruction.hpp"
#include "../symbols/symboltable.hpp"
#include <vector>

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual void generateIR(){};
};

struct Statement : public ASTNode {
    virtual inline void check() = 0;
};

struct Expression : public ASTNode {
    virtual inline void check() = 0;
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
    inline void add(Statement* node) {
        nodes.push_back(node);
    }
    inline void check() override {
        for(Statement* node : nodes){            
            node->check();
        }
    }
    inline void generateIR() override {
        for(Statement* node : nodes){
            node->generateIR();
        }
    }
    inline Statement* getLastObject() const noexcept{
        return nodes.back();
    }
    inline void deleteLastObject(){
        if(nodes.size() >= 1){
            nodes.pop_back();
        }
    }
};

struct RootNode : public Container {
    RootNode(): Container(){}
};