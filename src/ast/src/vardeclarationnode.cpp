#include "../include/vardeclarationnode.hpp"

#include "lexer.hpp"

VarDeclarationNode::VarDeclarationNode(const std::string& _name, const std::string& _type, const bool _is_const, ExpressionNode* _expr):
	name(_name),
	type(_type),
	is_const(_is_const),
	expr(_expr) {}

VarDeclarationNode::~VarDeclarationNode()
{
	delete expr;
}
