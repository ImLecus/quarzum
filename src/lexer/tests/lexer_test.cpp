#include "../include/lexer.hpp"

#include <cassert>
#include <iostream>

const std::vector<Token> EXPECTED_TOKENS = {
	Token(TokenType::DEF),
	Token(TokenType::VAR),
	Token(TokenType::CONST),
	Token(TokenType::IF),
	Token(TokenType::ELSE),
	Token(TokenType::DO),
	Token(TokenType::WHILE),
	Token(TokenType::LOOP),
	Token(TokenType::FOR),
	Token(TokenType::IN),
	Token(TokenType::RETURN),
	Token(TokenType::BREAK),
	Token(TokenType::CONTINUE),
	Token(TokenType::IMPORT),
	Token(TokenType::MODULE),
	Token(TokenType::SWITCH),
	Token(TokenType::MATCH),
	Token(TokenType::DEFAULT),
	Token(TokenType::CASE),
	Token(TokenType::ASYNC),
	Token(TokenType::AWAIT),
	Token(TokenType::AS),
	Token(TokenType::IS),
	Token(TokenType::EXTERN),
	Token(TokenType::STRUCT),
	Token(TokenType::CLASS),
	Token(TokenType::PUBLIC),
	Token(TokenType::PRIVATE),
	Token(TokenType::PROTECTED),
	Token(TokenType::TRUE),
	Token(TokenType::FALSE),
	Token(TokenType::NULL_LITERAL),
	Token(TokenType::IDENTIFIER, "identifier"),
	Token(TokenType::INTEGER_LITERAL, "1264"),
	Token(TokenType::FLOAT_LITERAL, "3.1415926535"),
	Token(TokenType::STRING_LITERAL, "\"Hello world from Quarzum!\""),
	Token(TokenType::CHAR_LITERAL, "'h'"),
	Token(TokenType::COLOR_LITERAL, "ffffffff"),
	Token(TokenType::COLOR_LITERAL, "5a66b903"),
	Token(TokenType::PLUS),
	Token(TokenType::PLUS_EQUAL),
	Token(TokenType::PLUS_UNARY),
	Token(TokenType::MINUS),
	Token(TokenType::MINUS_EQUAL),
	Token(TokenType::MINUS_UNARY),
	Token(TokenType::PRODUCT),
	Token(TokenType::PRODUCT_EQUAL),
	Token(TokenType::DIVIDE),
	Token(TokenType::DIVIDE_EQUAL),
	Token(TokenType::MODULO),
	Token(TokenType::MODULO_EQUAL),
	Token(TokenType::AND),
	Token(TokenType::OR),
	Token(TokenType::XOR),
	Token(TokenType::NOT),
	Token(TokenType::BITWISE_AND),
	Token(TokenType::BITWISE_AND_EQ),
	Token(TokenType::BITWISE_OR),
	Token(TokenType::BITWISE_OR_EQ),
	Token(TokenType::BITWISE_NOT),
	Token(TokenType::BITWISE_NOT_EQ),
	Token(TokenType::BITWISE_XOR),
	Token(TokenType::BITWISE_XOR_EQ),
	Token(TokenType::BITWISE_SHL),
	Token(TokenType::BITWISE_SHR),
	Token(TokenType::ASSIGN),
	Token(TokenType::EQUAL),
	Token(TokenType::NOT_EQUAL),
	Token(TokenType::GREATER),
	Token(TokenType::GREATER_EQUAL),
	Token(TokenType::LESS),
	Token(TokenType::LESS_EQUAL),
	Token(TokenType::DOT),
	Token(TokenType::COMMA),
	Token(TokenType::COLON),
	Token(TokenType::SEMICOLON),
	Token(TokenType::PAR_OPEN),
	Token(TokenType::PAR_CLOSE),
	Token(TokenType::SQUARE_OPEN),
	Token(TokenType::SQUARE_CLOSE),
	Token(TokenType::CURLY_OPEN),
	Token(TokenType::CURLY_CLOSE),
	Token(TokenType::QUESTION),
	Token()
};

int main()
{
	size_t index = 0;

	const auto lexer = new Lexer("../src/lexer/tests/lexer_test.qz");
	const Token* token;
	do
	{
		token = lexer->next();
		const Token& expected = EXPECTED_TOKENS[index];
		std::cout << *token << " - " << expected << std::endl;
		assert(*token == expected);
		++index;
	}
	while (!token->empty());
	assert(index == EXPECTED_TOKENS.size());
	std::cout << "\033[32m" << "--- Test passed ---" << "\033[0m" << std::endl;
	return EXIT_SUCCESS;
}