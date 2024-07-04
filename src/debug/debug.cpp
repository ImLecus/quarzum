#pragma once
#include "../Quarzum.h"
#define RED "\e[31m"
#define NOCOLOR "\e[0m"
#define YELLOW "\e[33m"
#define GREEN "\e[32m"
#define CYAN "\e[36m"
#define MAGENTA "\e[35m"
#define GRAY "\e[90m"


namespace Quarzum::Debug {
    /**
     * `TO-DO`: Activate debugMode by terminal arguments.
     * 
     * Activates log and debug messages in console.
     * Can be activated by using the --debug flag in the terminal.
    */
    bool debugMode = false;

    unique_ptr<std::vector<char>> source; 

    void getLine(const uint32 line,const uint32 column){
        std::cout << GRAY << ' ';
        for(uint8 i = 0; i <= std::to_string(line).length(); ++i){
            std::cout << ' ';
        }
        std::cout << "|\n " << line << " | ";

        uint32 currentLine = 1;
        for(char c: *source){
            if(currentLine == line){
                std::cout << c;
            }
            if(c == '\n'){++currentLine;}
        }
        for(uint8 i = 0; i <= std::to_string(line).length(); ++i){
            std::cout << ' ';
        }
        std::cout << " | ";
        for(uint32 i = 0; i <= column; ++i){
            std::cout << ' ';
        }
        std::cout << RED << "^^^\n" << NOCOLOR;
    }

    /**
     * @brief Displays a log in the console.
     * `debugMode` must be true.
    */
    inline void log(const string& message) {
        if(debugMode){
            std::cout << MAGENTA << "[LOG]" << NOCOLOR << ' ' << message << '\n';
        }
    }
    /**
     * @brief Displays a debug message in the console.
     * `debugMode` must be true.
    */
    inline void debug(const string& message) {
        if(debugMode){
            std::cout << CYAN << "[DEBUG]" << NOCOLOR << ' ' << message << '\n';
        }
    }
    /**
     * @brief Displays a warn message in the console.
     * Not affected by `debugMode`.
    */
    inline void warn(const string& message) {
        std::cout << YELLOW << "[WARNING]" << NOCOLOR << ' ' << message << '\n';
    }
    /**
     * @brief Displays am error message in the console.
     * Not affected by `debugMode`.
    */
    inline void err(const string& message) {
        std::cout << RED << "[ERROR]" << NOCOLOR << ' ' << message << '\n';
    }

    void throwError(const string& message, auto token) {
        err(message + " at line " + std::to_string(token.line) + ':');
        uint8 lineStr = std::to_string(token.line).length();
        getLine(token.line, token.column);
    }
    /**
     * @brief Displays the difference of time between two points
     * in the time. The message is categorized as a debug message.
    */
    inline void debugTime(std::chrono::_V2::system_clock::time_point start, const char* taskName = "Task"){
        std::chrono::duration<float> duration = (std::chrono::high_resolution_clock::now() - start)*1000000;
        string message = taskName;
        message += " completed in ";
        message += std::to_string(duration.count());
        message += " microseconds from the start.";
        debug(message.c_str());
    }

    /**
     * @brief Finishes the program.
     * Should be called at the end of the compiler.
    */
    inline void exit(uint8 code){
        if(code == 0){
            log("The compiler exited with exit code 0 (success).");
        }
        else{
            err("The compiler exited with exit code " + std::to_string(code) + " (failure).");
        }
        std::exit(code);
    }
}