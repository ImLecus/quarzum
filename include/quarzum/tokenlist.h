/*
 * Quarzum Compiler - tokenlist.h
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
#ifndef TOKENLIST_H
#define TOKENLIST_H
#include "token.h"
#include <stdlib.h>


/**
 * @brief Defines a list of tokens.
 * Used by the tokenizer and the parser.
 */
typedef struct {
    Token* content;
    unsigned long size;
    unsigned long capacity;
} TokenList;
typedef TokenList* tlist_t;

/**
 * @brief Creates a new `TokenList` with a determined size.
 */
tlist_t createTokenList(unsigned long size);
/**
 * @brief Deletes a `TokenList`.
 */
void deleteTokenList(tlist_t list);
/**
 * @brief Adds a new `Token` into the list.
 */
void addToTokenList(tlist_t list, Token token);
/**
 * @brief Returns the token at the list[i] position.
 * If the position is out of bounds, it will return an ErrorToken.
 */
Token getToken(tlist_t list, unsigned int i);
#endif