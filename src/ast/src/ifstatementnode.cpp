#include "../include/ifstatementnode.hpp"

IfStatementNode::IfStatementNode(ExpressionNode* _condition, StatementNode* _body, StatementNode* _else_body) :
	condition(_condition),
	body(_body),
	else_body(_else_body) {}

IfStatementNode::~IfStatementNode()
{
	delete condition;
	delete body;
	delete else_body;
}

