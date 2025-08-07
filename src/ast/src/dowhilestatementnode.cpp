#include "../include/dowhilestatementnode.hpp"

DoWhileStatementNode::DoWhileStatementNode(StatementNode* _body, ExpressionNode* _condition) :
	body(_body),
	condition(_condition) {}

DoWhileStatementNode::~DoWhileStatementNode()
{
	delete condition;
	delete body;
}

