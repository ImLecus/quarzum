/*
 * Quarzum Compiler - defs.h
 * Version 1.0, 07/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#ifndef DEFS_H
#define DEFS_H
#include "token.h"
#include "../core/bsearch.h"

#define KEYWORDS_SIZE 54
#define SYMBOLS_SIZE 39


/**
 * @brief Searches in the `keywords` array using binary search.
 * If the element is not a keyword, it will return an `Identifier` type.
 */
const TokenType keywordToType(const char* keyword);


/**
 * @brief Converts a symbol string to a TokenType using binary search.
 * If the symbol is not found, it will return a `TokenError`.
 */
const TokenType symbolToType(const char* symbol);

#endif