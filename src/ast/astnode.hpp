#pragma once
#include <iostream>

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual const void print() = 0;
};