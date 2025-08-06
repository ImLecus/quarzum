#pragma once
#include <string>

#include "declarationnode.hpp"
#include "expressionnode.hpp"

class VarDeclarationNode : public DeclarationNode
{
private:
	std::string name;
	std::string type;
	bool is_const;
	ExpressionNode* expr;
public:
	VarDeclarationNode(const std::string&, const std::string&, bool, ExpressionNode*);
	~VarDeclarationNode();
};
