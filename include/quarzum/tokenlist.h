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
#include "defs.h"

// TokenList structure
typedef struct {
    struct token** content;
    unsigned int size;
    unsigned int capacity;
} TokenList;

TokenList* init_tlist(unsigned int size);

void free_tlist(TokenList* list);

void push_tlist(TokenList* list, struct token* Token);

// Returns list[i], NULL if is not found.
struct token* get_token(TokenList* list, unsigned int i);


#endif