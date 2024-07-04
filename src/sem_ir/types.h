/*
 * Quarzum Compiler - types.h
 * Version 1.0, 29/06/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */

#ifndef TYPES_H
#define TYPES_H
#include <cmath>

/**
 * @brief Defines a Type.
 */
struct BaseType;

/**
 * @brief Defines a text Type and does not have a fixed size.
 * Derivated types: char, string
 */
struct TextType;
/**
 * @brief Defines a single character. Can be converted to int, uint, num and string.
 */
struct Char;
/**
 * @brief Defines a string. Cannot be converted.
 */
struct String;

/**
 * @brief Defines a numeric Type. Does have a fixed size and numeric vaule ranges.
 * Derivated types: int, uint, bool, num, decimal
 */
struct NumericType;
/**
 * @brief Defines a number with floating point precision.
 * Sizes: 16, 32, 64
 */
struct Num;
long double getNumMinValue(uint8 bits);
long double getNumMaxValue(uint8 bits);
/**
 * @brief Defines an integer. 
 * Sizes: 8, 16, 32, 64
 */
struct Int;
/**
 * @brief Defines an unsigned integer. 
 * Sizes: 8, 16, 32, 64
 */
struct Uint;
/**
 * @brief Defines a logical type, which can be true (1) or false (0).
 * Size: 1
 */
struct Bool;
/**
 * @brief Defines a function without return value.
 */
struct Function;

/**
 * @brief Converts a string into a BaseType.
 */
BaseType strToType(const string& str);

#endif

