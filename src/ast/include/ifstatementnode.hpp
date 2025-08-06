#pragma once
#include "expressionnode.hpp"
#include "statementnode.hpp"

class IfStatementNode: public StatementNode
{
private:
	ExpressionNode* condition;
	StatementNode* body;
public:
	IfStatementNode(ExpressionNode*, StatementNode*);
	~IfStatementNode();
};