/*
 * Quarzum Compiler - node.h
 * Version 1.0, 02/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */

#ifndef NODE_H
#define NODE_H
#include <vector>

struct Node;
struct Statement;
struct Container;
struct RootNode;
struct Type; // TO-DO: merge with BaseType


struct Expr;
struct LiteralExpr;
struct IdentifierExpr;
struct ParenExpr;
struct UnaryExpr;
struct BinaryExpr;
struct TernaryExpr;
struct FunctionCallExpr;
struct ArrayIndexExpr;

struct Argument;

struct VarDeclaration;
struct VarRedec;

struct ModuleContainer;
struct IfContainer;
struct WhileContainer;
struct DoWhileContainer;
struct ForeachContainer;
struct ForContainer;
struct ClassContainer;
struct FunctionContainer;

struct ReturnStatement;
struct BreakStatement;
struct ContinueStatement;
struct ImportStatement;
struct EnumElement;
struct EnumStatement;
struct DeleteStatement;
struct ForeignStatement;

#endif