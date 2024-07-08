/*
 * Quarzum Compiler - debug.h
 * Version 1.0, 08/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#ifndef DEBUG_H
#define DEBUG_H
#include <stdio.h>

/**
 * STYLE DEFINITIONSs
 */
#define RESET "\e[0m"

#define RED "\e[31m"
#define YELLOW "\e[33m"
#define GREEN "\e[32m"
#define CYAN "\e[36m"
#define MAGENTA "\e[35m"
#define GRAY "\e[90m"
#define ORANGE "\e[38;5;214m"

#define UNDERLINE "\e[4m"
#define BOLD "\e[1m"

/**
 * @brief Prints a message in console with the tag [LOG].
 */
void print(const char* message);
/**
 * @brief Prints a debug message in console with the tag [DEBUG].
 * `debugMode` flag must be activated.
 */
void debug(const char* message);
/**
 * @brief Throws a warning message in console with the tag [WARNING].
 * Can be desactivated with the `noWarningErrors` flag activated.
 */
void warn(const char* message);
/**
 * @brief Throws an error message in console with the tag [ERROR].
 * @param fatal Exits the program if the error is considered fatal.
 */
void err(const char* message, unsigned char fatal);


#endif