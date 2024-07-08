/*
 * Quarzum Compiler - expr.h
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
#ifndef EXPR_H
#define EXPR_H
#include "ast.h"
#include "tokenlist.h"
#include "../core/debug.h"
#include <string.h>
#define PARSING_POS TokenList* tokens, unsigned int* i

/**
 * @brief Parses an expression. 
 */
Node* parseExpr(PARSING_POS);

Node* parsePrimaryExpr(PARSING_POS);

Node* parseParenExpr(PARSING_POS);

    
#endif