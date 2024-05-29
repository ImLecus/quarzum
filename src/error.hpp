#include <string>
#include <iostream>
#define RED "\e[31m"
#define NOCOLOR "\e[0m"
#define YELLOW "\e[30;103m"
#define GREEN "\e[30;102m"

void throwError(const std::string message){
    std::cout << RED << "Error: " << NOCOLOR << message << '\n';
    exit(EXIT_FAILURE);
}

void throwLexicalError(const std::string message, size_t line){
    std::cout << RED << "LexicalError: " << NOCOLOR << message << " at line " << line << ".\n";
    exit(EXIT_FAILURE);
}

void throwSyntaxError(const std::string message){
    std::cout << RED << "SyntaxError: " << NOCOLOR << message << ".\n";
    exit(EXIT_FAILURE);
}

void throwWarning(const std::string message){
    std::cout << YELLOW << "Warning: " << NOCOLOR << message << '\n';
}