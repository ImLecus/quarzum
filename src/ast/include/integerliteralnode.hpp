#pragma once
#include "literalexpressionnode.hpp"

class IntegerLiteralNode : public LiteralExpressionNode
{
	public:
	explicit IntegerLiteralNode(const std::string& value) : LiteralExpressionNode(value) {}
};