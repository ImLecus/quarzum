#include <utility>

#include "../include/functiondeclarationnode.hpp"

FunctionDeclarationNode::FunctionDeclarationNode(std::string  _name, std::string  _type, BlockNode* _body):
	name(std::move(_name)),
	type(std::move(_type)),
	body(_body) {}

FunctionDeclarationNode::~FunctionDeclarationNode()
{
	delete body;
}
