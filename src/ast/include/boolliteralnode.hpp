#pragma once
#include "literalexpressionnode.hpp"

class BoolLiteralNode: public LiteralExpressionNode
{
public:
	explicit BoolLiteralNode(bool);
};
