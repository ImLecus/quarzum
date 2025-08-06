#pragma once
#include "literalexpressionnode.hpp"

class BoolLiteralNode: public LiteralExpressionNode
{
private:
	bool value;
public:
	explicit BoolLiteralNode(bool);
};
