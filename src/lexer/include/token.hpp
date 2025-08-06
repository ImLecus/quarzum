#pragma once
#include <string>
#include <ostream>

enum class TokenType
{
	EMPTY,
	INTEGER_LITERAL,
	FLOAT_LITERAL,
	STRING_LITERAL,
	CHAR_LITERAL,
	IDENTIFIER,
	TRUE,
	FALSE,
	NULL_LITERAL,
	COLOR_LITERAL,
	PLUS,
	PLUS_EQUAL,
	PLUS_UNARY,
	MINUS,
	MINUS_EQUAL,
	MINUS_UNARY,
	PRODUCT,
	PRODUCT_EQUAL,
	DIVIDE,
	DIVIDE_EQUAL,
	MODULO,
	MODULO_EQUAL,
	NOT,
	AND,
	OR,
	XOR,
	BITWISE_NOT,
	BITWISE_AND,
	BITWISE_OR,
	BITWISE_XOR,
	BITWISE_SHL,
	BITWISE_SHR,
	BITWISE_NOT_EQ,
	BITWISE_AND_EQ,
	BITWISE_OR_EQ,
	BITWISE_XOR_EQ,
	DEF,
	VAR,
	CONST,
	DEFAULT,
	IMPORT,
	MODULE,
	RETURN,
	IF,
	ELSE,
	WHILE,
	FOR,
	IN,
	LOOP,
	DO,
	BREAK,
	CONTINUE,
	SWITCH,
	MATCH,
	CASE,
	ASYNC,
	AWAIT,
	EQUAL,
	NOT_EQUAL,
	GREATER,
	GREATER_EQUAL,
	LESS,
	LESS_EQUAL,
	IS,
	EXTERN,
	STRUCT,
	CLASS,
	PUBLIC,
	PRIVATE,
	PROTECTED,
	AS,
	DOT,
	COMMA,
	PAR_OPEN,
	PAR_CLOSE,
	SQUARE_OPEN,
	SQUARE_CLOSE,
	CURLY_OPEN,
	CURLY_CLOSE,
	COLON,
	SEMICOLON,
	ASSIGN,
	QUESTION
};


class Token {
private:
	const TokenType type;
	const std::string value;

public:
	Token();
	explicit Token(TokenType);
	Token(TokenType, std::string );

	TokenType getType() const;
	const std::string& getValue() const;
	bool empty() const;
};

std::ostream& operator << (std::ostream&, const Token&);
bool operator==(const Token&, const Token&);