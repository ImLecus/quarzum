/*
 * Quarzum Compiler - compile.h
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
#ifndef COMPILE_H
#define COMPILE_H
#include "../core/debug.h"
#include "../core/process.h"
#include "../core/path.h"
#include "../quarzum/config.h"
#include "../quarzum/symbol.h"
#include "../quarzum/ir.h"
#include "tokenizer.h"
#include "parser.h"
/**
 * @brief Compiles a file. 
 * Compiler options are not managed by this function.
 */
void compile(char* file);
/**
 * @brief Tokenizes and parses a file, returning the AST.
 */
node_t getAST(char* file);

#endif