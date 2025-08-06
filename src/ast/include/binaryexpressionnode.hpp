#pragma once
#include "expressionnode.hpp"

class BinaryExpressionNode : public ExpressionNode
{
private:
	ExpressionNode* left;
	ExpressionNode* right;
public:
	BinaryExpressionNode(ExpressionNode*, ExpressionNode*);
	~BinaryExpressionNode();
};