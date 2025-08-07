#pragma once
#include <map>

#include "expressionnode.hpp"
#include "statementnode.hpp"

class SwitchStatementNode : public StatementNode
{
private:
	ExpressionNode* eval_expression;
	std::map<ExpressionNode*, StatementNode*> cases;
public:
	SwitchStatementNode(ExpressionNode*, std::map<ExpressionNode*, StatementNode*>&);
	~SwitchStatementNode();
};