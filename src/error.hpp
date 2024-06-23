#pragma once
#include <string>
#include <iostream>


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