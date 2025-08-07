#include "../include/switchstatementnode.hpp"

SwitchStatementNode::SwitchStatementNode(ExpressionNode* _eval_expression, std::map<ExpressionNode*, StatementNode*>& _cases):
	eval_expression(_eval_expression),
	cases(_cases) {}

SwitchStatementNode::~SwitchStatementNode()
{
	delete eval_expression;
	for (auto case_ : cases)
	{
		delete case_.first;
		delete case_.second;
	}
}
