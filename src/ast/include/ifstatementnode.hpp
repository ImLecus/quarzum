#pragma once
#include "expressionnode.hpp"
#include "statementnode.hpp"

class IfStatementNode: public StatementNode
{
private:
	ExpressionNode* condition;
	StatementNode* body;
	StatementNode* else_body;
public:
	IfStatementNode(ExpressionNode*, StatementNode*, StatementNode*);
	~IfStatementNode();
};