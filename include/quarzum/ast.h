/*
 * Quarzum Compiler - ast.h
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
#ifndef AST_H
#define AST_H
#include <stdlib.h>
typedef enum {
    Root,
    IfStmt,
    WhileStmt,
    VarStmt,
    ModuleStmt,
    BreakStmt,
    ContinueStmt,
    FunctionStmt,
    
    IdentifierNode,
    TypeNode,

    UnaryExpr,
    BinaryExpr,
    ParenExpr,
    TernaryExpr,
    Literal
} NodeType;

/**
 * @brief Defines an AST node.
 * A `Node` is composed by more nodes (or NULL).
 * Identifiers, expressions, and all the AST components are nodes. 
 */
typedef struct  {
    NodeType type;
    struct Node** children;
    u_int32_t childrenCount;
    u_int32_t maxChildren;
    void* data;

    struct Node* (*create)(NodeType type, u_int32_t defaultChildren);
} Node;
typedef Node* node_t;
node_t createNode(NodeType type, u_int32_t defaultChildren);
void deleteNode(node_t node);
void addChildNode(node_t parent, node_t child);



#endif