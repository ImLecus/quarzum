#include "../include/blocknode.hpp"

BlockNode::BlockNode(): children(std::vector<ASTNode*>()) {}
BlockNode::BlockNode(const std::vector<ASTNode*>& _children) : children(_children) {}
BlockNode::~BlockNode()
{
	for (const auto child : children) delete child;
}

void BlockNode::addChild(ASTNode* child)
{
	this->children.push_back(child);
}

std::vector<ASTNode*> BlockNode::getChildren()
{
	return this->children;
}
