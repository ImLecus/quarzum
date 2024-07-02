/*
 * Quarzum Compiler - check.h
 * Version 1.0, 01/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#ifndef CHECK_H
#define CHECK_H
#include "irinstruction.hpp"
#include "../parser/node.h"

IRInstruction check(const RootNode& root);

IRInstruction check(const FunctionCall& call);

IRInstruction check(const BreakStatement& break_stmt);
#endif