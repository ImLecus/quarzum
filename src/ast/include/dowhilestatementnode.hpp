#pragma once
#include "expressionnode.hpp"
#include "statementnode.hpp"

class DoWhileStatementNode : public StatementNode
{
private:
	StatementNode* body;
	ExpressionNode* condition;
public:
	DoWhileStatementNode(StatementNode*,ExpressionNode*);
	~DoWhileStatementNode();
};
