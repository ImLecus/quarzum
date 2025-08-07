#pragma once
#include "primaryexpressionnode.hpp"
#include <string>
class LiteralExpressionNode : public PrimaryExpressionNode
{
protected:
	std::string value;
public:
	explicit LiteralExpressionNode(const std::string& value) : value(value) {}
};
