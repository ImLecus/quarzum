#pragma once
#include "statementnode.hpp"
#include "blocknode.hpp"

class LoopStatementNode: public StatementNode
{
private:
	BlockNode* body;
public:
	explicit LoopStatementNode(BlockNode*);
	~LoopStatementNode();
};