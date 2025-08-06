#pragma once
#include <vector>
#include "astnode.hpp"
#include "statementnode.hpp"

class BlockNode : public StatementNode
{
private:
	std::vector<ASTNode*> children;

public:
	BlockNode();
	explicit BlockNode(const std::vector<ASTNode*>&);
	~BlockNode();

	void addChild(ASTNode* child);
	std::vector<ASTNode*> getChildren();
};
