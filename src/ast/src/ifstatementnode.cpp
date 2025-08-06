#include "../include/ifstatementnode.hpp"

IfStatementNode::IfStatementNode(ExpressionNode* _condition, StatementNode* _body) :
	condition(_condition),
	body(_body) {}

IfStatementNode::~IfStatementNode()
{
	delete condition;
	delete body;
}

