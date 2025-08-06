#pragma once
#include "expressionnode.hpp"
#include "statementnode.hpp"

class ReturnStatementNode : public StatementNode
{
private:
	ExpressionNode* expression;
public:
	explicit ReturnStatementNode(ExpressionNode*);
	~ReturnStatementNode();
};
