#include "../include/binaryexpressionnode.hpp"

BinaryExpressionNode::BinaryExpressionNode(ExpressionNode* _left, ExpressionNode* _right):
	left(_left),
	right(_right) {}

BinaryExpressionNode::~BinaryExpressionNode()
{
	delete left;
	delete right;
}
