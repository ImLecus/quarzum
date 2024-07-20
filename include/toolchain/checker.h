/*
 * Quarzum Compiler - checker.h
 * Version 1.0, 20/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#ifndef CHECKER_H
#define CHECKER_H
#include "../quarzum/ast.h"
#include "../quarzum/symbol.h"
#include "../core/debug.h"
/**
 * @brief Checks an AST, in addition to the creation of the
 * SymbolTable and the type conversions.
 * Will end the program if it finishes with errors.
 */
void checkAST(node_t ast);
/**
 * @brief Checks a Node depending on its type.
 */
void checkNode(node_t node, SymbolTable* symbols);

#endif