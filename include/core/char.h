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
#include "utils.h"
/**
 * @brief Returns 1 if the character is alphabetic (a-zA-Z),
 * 0 otherwise.
 */
bool isAlpha(char c);
/**
 * @brief Returns 1 if the character is a digit (0-9), 0 otherwise.
 */
bool isDigit(char c, char base);
/**
 * @brief Returns 1 if the character is a symbol, 0 otherwise.
 */
bool isSymbol(char c);
/**
 * @brief Returns 1 if the character is a digit (0-9) or an
 * alphabetic (a-zA-Z), 0 otherwise.
 */
bool isAlphaNumeric(char c);

#endif