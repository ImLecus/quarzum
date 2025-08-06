#include "../include/loopstatementnode.hpp"

LoopStatementNode::LoopStatementNode(BlockNode* _body):
	body(_body) {}

LoopStatementNode::~LoopStatementNode()
{
	delete body;
}

