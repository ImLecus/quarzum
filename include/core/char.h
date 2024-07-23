/*
 * Quarzum Compiler - char.h
 * Version 1.0, 18/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#ifndef CHAR_H
#define CHAR_H
typedef unsigned char bool_t;
#include <string.h>
/**
 * @brief Returns 1 if the character is alphabetic (a-zA-Z),
 * 0 otherwise.
 */
bool_t isAlpha(char c);
/**
 * @brief Returns 1 if the character is a digit (0-9), 0 otherwise.
 */
bool_t isDigit(char c);
/**
 * @brief Returns 1 if the character is a symbol, 0 otherwise.
 */
bool_t isSymbol(char c);
/**
 * @brief Returns 1 if the character is a digit (0-9) or an
 * alphabetic (a-zA-Z), 0 otherwise.
 */
bool_t isAlphaNumeric(char c);
/**
 * @brief Returns 1 if the character is a whitespace (' ');
 */
bool_t isSpace(char c);

#endif