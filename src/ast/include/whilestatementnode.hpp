#pragma once
#include "expressionnode.hpp"
#include "statementnode.hpp"

class WhileStatementNode : public StatementNode
{
private:
	ExpressionNode* condition;
	StatementNode* body;
public:
	WhileStatementNode(ExpressionNode*, StatementNode*);
	~WhileStatementNode();
};
