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
/**
 * @brief Defines a list of tokens.
 * Used by the tokenizer and the parser.
 */
typedef struct {
    Token** content;
    u_int64_t size;
    u_int64_t capacity;
} TokenList;
typedef TokenList tlist_t;

/**
 * @brief Creates a new `TokenList` with a determined size.
 */
tlist_t* init_tlist(u_int64_t size);
/**
 * @brief Deletes a `TokenList`.
 */
void delete_tlist(tlist_t* list);
/**
 * @brief Adds a new `Token` into the list.
 */
void push_tlist(tlist_t* list, Token* token);
/**
 * @brief Returns the token at the list[i] position.
 * If the position is out of bounds, it will return an ErrorToken.
 */
Token* get_token(tlist_t* list, u_int32_t i);
#endif