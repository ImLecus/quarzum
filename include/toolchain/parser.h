/*
 * Quarzum Compiler - parser.h
 * Version 1.0, 10/07/2024
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
#include "../quarzum/symbol.h"
#include "../quarzum/tokenlist.h"
#include "../core/debug.h"
#include "compile.h"

#define EXPECT(t,errMessage) if(getToken(tokens, *i).type != t){err(errMessage,0); return NULL;}

/**
 * @brief Discards the actual Token to check next.
 */
#define pass ++(*i)
/**
 * @brief General parsing function.
 * It will generate a `Node` with type Root and wll call
 * `parseStatement` until the `TokenList` is fully iterated.
 */
node_t parse(TokenList* tokens, SymbolTable* symbolTable);
/**
 * @brief Statement parsing function.
 */
node_t parseStatement(PARSING_POS);
/**
 * @brief Variable and function declaration parsing function.
 */
node_t parseDeclaration(PARSING_POS);

node_t parseImport(PARSING_POS);

#endif