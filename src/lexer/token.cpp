#include <utility>
#include "include/token.hpp"

Token::Token(): type(TokenType::EMPTY) {}

Token::Token(const TokenType _type): type(_type) {}

Token::Token(const TokenType _type, std::string _value) : type(_type), value(std::move(_value)) {}

TokenType Token::getType() const
{
	return this->type;
}

const std::string& Token::getValue() const
{
	return this->value;
}

bool Token::empty() const
{
	return this->type == TokenType::EMPTY && this->value.empty();
}

std::ostream& operator <<(std::ostream& os, const Token& t)
{
	if (t.getValue().empty())
	{
		os << "Token(" << static_cast<unsigned>(t.getType()) << ")";
	}
	else
	{
		os << "Token(" << static_cast<unsigned>(t.getType()) << ", " << t.getValue() << ")";
	}
	return os;
}

bool operator==(const Token& lhs, const Token& rhs)
{
	return lhs.getType() == rhs.getType() && lhs.getValue() == rhs.getValue();
}