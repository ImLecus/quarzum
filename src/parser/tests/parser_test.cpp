#include "parser.hpp"
#include <iostream>

int main()
{
	auto lexer = Lexer("../src/parser/tests/parser_test.qz");
	const auto parser = new Parser(lexer);
	const auto root = parser->parse();
	delete root;
	delete parser;
	std::cout << "\033[32m" << "--- Test passed ---" << "\033[0m" << std::endl;
	return EXIT_SUCCESS;
}
