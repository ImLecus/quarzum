/*
 * Quarzum Compiler - parser.h
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
#ifndef PARSER_H
#define PARSER_H
#include "../quarzum/expr.h"
#include "../quarzum/ast.h"
#include "../quarzum/tokenlist.h"
#include "../core/debug.h"

#define EXPECT(t,errMessage) if(getToken(tokens, ++(*i)).type != t){err(errMessage,0); return NULL;}

/**
 * @brief Discards the actual Token to check next.
 */
#define pass ++(*i)
/**
 * @brief General parsing function.
 * It will generate a `Node` with type Root and wll call
 * `parseStatement` until the `TokenList` is fully iterated.
 */
Node* parse(TokenList* tokens);
/**
 * @brief Statement parsing function.
 */
Node* parseStatement(PARSING_POS);
/**
 * @brief Variable and function declaration parsing function.
 */
Node* parseDeclaration(PARSING_POS);

#endif