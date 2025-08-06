#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "token.hpp"

const std::unordered_map<std::string, TokenType> KEYWORDS = {
	{"if", TokenType::IF},
	{"while", TokenType::WHILE},
	{"for", TokenType::FOR},
	{"loop", TokenType::LOOP},
	{"return", TokenType::RETURN},
	{"do", TokenType::DO},
	{"break", TokenType::BREAK},
	{"continue", TokenType::CONTINUE},
	{"switch", TokenType::SWITCH},
	{"match", TokenType::MATCH},
	{"case", TokenType::CASE},
	{"else", TokenType::ELSE},
	{"var", TokenType::VAR},
	{"const", TokenType::CONST},
	{"default", TokenType::DEFAULT},
	{"def", TokenType::DEF},
	{"import", TokenType::IMPORT},
	{"module", TokenType::MODULE},
	{"is", TokenType::IS},
	{"in", TokenType::IN},
	{"and", TokenType::AND},
	{"or", TokenType::OR},
	{"xor", TokenType::XOR},
	{"not", TokenType::NOT},
	{"true", TokenType::TRUE},
	{"false", TokenType::FALSE},
	{"null", TokenType::NULL_LITERAL},
	{"async", TokenType::ASYNC},
	{"await", TokenType::AWAIT},
	{"extern", TokenType::EXTERN},
	{"struct", TokenType::STRUCT},
	{"class", TokenType::CLASS},
	{"public", TokenType::PUBLIC},
	{"private", TokenType::PRIVATE},
	{"protected", TokenType::PROTECTED},
	{"as", TokenType::AS}
};

const std::unordered_map<std::string, TokenType> SYMBOLS = {
	{"+", TokenType::PLUS},
	{"+=", TokenType::PLUS_EQUAL},
	{"++", TokenType::PLUS_UNARY},
	{"-", TokenType::MINUS},
	{"-=", TokenType::MINUS_EQUAL},
	{"--", TokenType::MINUS_UNARY},
	{"*", TokenType::PRODUCT},
	{"*=", TokenType::PRODUCT_EQUAL},
	{"/", TokenType::DIVIDE},
	{"/=", TokenType::DIVIDE_EQUAL},
	{"%", TokenType::MODULO},
	{"%=", TokenType::MODULO_EQUAL},
	{".", TokenType::DOT},
	{",", TokenType::COMMA},
	{":", TokenType::COLON},
	{";", TokenType::SEMICOLON},
	{"(", TokenType::PAR_OPEN},
	{")", TokenType::PAR_CLOSE},
	{"[", TokenType::SQUARE_OPEN},
	{"]", TokenType::SQUARE_CLOSE},
	{"{", TokenType::CURLY_OPEN},
	{"}", TokenType::CURLY_CLOSE},
	{"=", TokenType::ASSIGN},
	{"==", TokenType::EQUAL},
	{"!=", TokenType::NOT_EQUAL},
	{"&", TokenType::BITWISE_AND},
	{"&=", TokenType::BITWISE_AND_EQ},
	{"|", TokenType::BITWISE_OR},
	{"|=", TokenType::BITWISE_OR_EQ},
	{"~", TokenType::BITWISE_NOT},
	{"~=", TokenType::BITWISE_NOT_EQ},
	{"<<", TokenType::BITWISE_SHL},
	{">>", TokenType::BITWISE_SHR},
	{"^", TokenType::BITWISE_XOR},
	{"^=", TokenType::BITWISE_XOR_EQ},
	{"<", TokenType::LESS},
	{"<=", TokenType::LESS_EQUAL},
	{">", TokenType::GREATER},
	{">=", TokenType::GREATER_EQUAL},
	{"?", TokenType::QUESTION}
};

class Lexer {
private:
	const char* file;
	std::string src;
	std::vector<char> buffer;
	size_t idx = 0;

	const Token EMPTY_TOKEN = Token();

	char peek(uint8_t) const;
	void advance();
	Token* createToken(TokenType, bool);
	void readDigitChain();
	void readHexDigitChain();

public:
	explicit Lexer(const char*);

	const Token* next();
};

