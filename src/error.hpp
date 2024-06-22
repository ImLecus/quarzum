#pragma once
#include <string>
#include <iostream>
#define RED "\e[31m"
#define NOCOLOR "\e[0m"
#define YELLOW "\e[30;103m"
#define GREEN "\e[30;102m"

inline void throwError(const std::string& message){
    std::cout << RED << "Error" << NOCOLOR << ": " << message << '\n';
    exit(EXIT_FAILURE);
}

inline void throwSyntaxError(const std::string& message,const size_t& line){
    std::cout << RED << "SyntaxError" << NOCOLOR << ": " << message << " at line " << line << ".\n";
    exit(EXIT_FAILURE);
}

inline void throwTypeError(const std::string& message){
    std::cout << RED << "TypeError" << NOCOLOR << ": " << message << '\n';
    exit(EXIT_FAILURE);
}

inline void throwWarning(const std::string& message){
    std::cout << YELLOW << "Warning" << NOCOLOR << ": " << message << '\n';
}