/*
 * Quarzum Compiler - tokenlist.h
 * Version 1.0, 06/07/2024
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

typedef struct {
    Token* content;
    unsigned long size;
    unsigned long capacity;
} TokenList;

TokenList* createTokenList(unsigned long size);
void deleteTokenList(TokenList* list);
void addToTokenList(TokenList* list, Token token);

#endif