#include "../include/whilestatementnode.hpp"

WhileStatementNode::WhileStatementNode(ExpressionNode* _condition, StatementNode* _body):
	condition(_condition),
	body(_body) {}

WhileStatementNode::~WhileStatementNode()
{
	delete condition;
	delete body;
}

