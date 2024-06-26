#pragma once

inline void throwError(const std::string& message){
    std::cout << RED << "Error" << NOCOLOR << ": " << message << '\n';
    exit(EXIT_FAILURE);
}

inline void throwTypeError(const std::string& message){
    std::cout << RED << "TypeError" << NOCOLOR << ": " << message << '\n';
    exit(EXIT_FAILURE);
}