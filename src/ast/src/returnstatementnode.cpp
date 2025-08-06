#include "../include/returnstatementnode.hpp"

ReturnStatementNode::ReturnStatementNode(ExpressionNode* _expression):
	expression(_expression) {}

ReturnStatementNode::~ReturnStatementNode()
{
	delete expression;
}
