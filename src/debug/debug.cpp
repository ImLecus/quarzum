#pragma once
#include "../Quarzum.h"
#define RED "\e[31m"
#define NOCOLOR "\e[0m"
#define YELLOW "\e[33m"
#define GREEN "\e[32m"
#define CYAN "\e[36m"
#define MAGENTA "\e[35m"
#include <iostream>
#include <chrono>
#include <string>


namespace quarzum::debug {
    /**
     * `TO-DO`: Activate debugMode by terminal arguments.
     * 
     * Activates log and debug messages in console.
     * Can be activated by using the --debug flag in the terminal.
    */
    bool debugMode = true;

    /**
     * @brief Displays a log in the console.
     * `debugMode` must be true.
    */
    inline void log(const std::string& message) {
        if(debugMode){
            std::cout << MAGENTA << "[LOG]" << NOCOLOR << ' ' << message << '\n';
        }
    }
    /**
     * @brief Displays a debug message in the console.
     * `debugMode` must be true.
    */
    inline void debug(const std::string& message) {
        if(debugMode){
            std::cout << CYAN << "[DEBUG]" << NOCOLOR << ' ' << message << '\n';
        }
    }
    /**
     * @brief Displays a warn message in the console.
     * Not affected by `debugMode`.
    */
    inline void warn(const std::string& message) {
        std::cout << YELLOW << "[WARNING]" << NOCOLOR << ' ' << message << '\n';
    }
    /**
     * @brief Displays am error message in the console.
     * Not affected by `debugMode`.
    */
    inline void err(const std::string& message) {
        std::cout << RED << "[ERROR]" << NOCOLOR << ' ' << message << '\n';
    }

    void throwLexicalError(const std::string& message, auto& token) {
        err(message + " at line " + std::to_string(token.line) + '.');
    }
    /**
     * @brief Displays the difference of time between two points
     * in the time. The message is categorized as a debug message.
    */
    inline void debugTime(std::chrono::_V2::system_clock::time_point start, const char* taskName = "Task"){
        std::chrono::duration<float> duration = (std::chrono::high_resolution_clock::now() - start)*1000000;
        std::string message = taskName;
        message += " completed in ";
        message += std::to_string(duration.count());
        message += " microseconds from the start.";
        debug(message.c_str());
    }
}