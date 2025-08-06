#pragma once
#include <string>

#include "blocknode.hpp"
#include "declarationnode.hpp"

class FunctionDeclarationNode : public DeclarationNode
{
private:
	std::string name;
	std::string type;
	// args
	BlockNode* body;

public:
	FunctionDeclarationNode(std::string , std::string , BlockNode*);
	~FunctionDeclarationNode();
};
