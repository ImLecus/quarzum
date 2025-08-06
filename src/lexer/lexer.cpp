#include "include/lexer.hpp"
#include <fstream>
#include <iostream>

Lexer::Lexer(const char* _file)
{
	this->file = _file;
	std::ifstream file_stream(file);
	std::string line;

	if (!file_stream.is_open())
	{
		std::cerr << "Failed to open file: " << file << std::endl;
		return;
	}

	while (std::getline(file_stream, line))
	{
		src.append(line).push_back('\n');
	}
}

char Lexer::peek(const uint8_t ahead = 0) const
{
	return idx + ahead > src.size() ? '\0' : src[idx + ahead];
}

void Lexer::advance()
{
	++idx;
}

Token* Lexer::createToken(const TokenType type, const bool useBuffer)
{
	const std::string data(buffer.begin(), buffer.end());
	const auto token = new Token(type, useBuffer ? data : "");
	buffer.clear();
	return token;
}

void Lexer::readDigitChain()
{
	while (isdigit(peek()))
	{
		buffer.push_back(peek());
		advance();
	}
}

void Lexer::readHexDigitChain()
{
	while (isxdigit(peek()))
	{
		buffer.push_back(peek());
		advance();
	}
}

const Token* Lexer::next()
{
	while (isspace(peek())) advance();

	if (peek(0) == '/' and peek(1) == '/')
	{
		while (peek() != '\n' and peek() != '\0')
		{
			advance();
		}
		advance();
		return next();
	}
	if (peek(0) == '/' and peek(1) == '*')
	{
		advance();
		advance();
		while (peek(0) != '*' and peek(1) != '/' and peek() != '\0') advance();
		advance();
		advance();
		return next();
	}
	const char c = peek();

	if (c == '\0') return &EMPTY_TOKEN;

	if (c == '"')
	{
		buffer.push_back(c);
		advance();
		while (peek() != '"' and peek() != '\0')
		{
			buffer.push_back(peek());
			advance();
		}
		buffer.push_back(peek());
		advance();
		return createToken(TokenType::STRING_LITERAL,true);
	}
	if (c == '\'')
	{
		buffer.push_back(c);
		advance();
		while (peek() != '\'' and peek() != '\0')
		{
			buffer.push_back(peek());
			advance();
		}
		buffer.push_back(peek());
		advance();
		return createToken(TokenType::CHAR_LITERAL,true);
	}
	if (isalpha(c) or c == '_')
	{
		while (isalnum(peek()) or peek() == '_')
		{
			buffer.push_back(peek());
			advance();
		}
		const std::string data(buffer.begin(), buffer.end());
		if (KEYWORDS.find(data) != KEYWORDS.end())
		{
			return createToken(KEYWORDS.at(data), false);
		}
		return createToken(TokenType::IDENTIFIER, true);
	}
	if (c == '#')
	{
		advance();
		readHexDigitChain();
		if (buffer.size() < 8)
		{
			buffer.push_back('f');
			buffer.push_back('f');
		}
		if (buffer.size() != 8)
		{
			std::cerr << "[LexicalError] Invalid format (" << buffer.data() << ")\n";
			// error
		}
		return createToken(TokenType::COLOR_LITERAL, true);
	}
	if (isdigit(c))
	{
		readDigitChain();
		if (peek() == '.')
		{
			buffer.push_back('.');
			advance();
			readDigitChain();
			return createToken(TokenType::FLOAT_LITERAL, true);
		}
		return createToken(TokenType::INTEGER_LITERAL, true);
	}

	while (ispunct(peek()))
	{
		buffer.push_back(peek());
		advance();
	}
	do
	{
		const std::string data(buffer.begin(), buffer.end());
		if (SYMBOLS.find(data) != SYMBOLS.end())
		{
			return createToken(SYMBOLS.at(data), false);
		}
		buffer.pop_back();
		--idx;
	}
	while (!buffer.empty());

	std::cerr << "[Lexical Error] Unexpected character " << peek() << std::endl;
	// error
	return &EMPTY_TOKEN;
}
