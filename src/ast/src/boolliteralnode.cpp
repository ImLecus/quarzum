#include "../include/boolliteralnode.hpp"

BoolLiteralNode::BoolLiteralNode(const bool _value) :
	LiteralExpressionNode(_value ? "true" : "false"){}
