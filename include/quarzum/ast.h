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
    
    IdentifierNode,
    ExprNode,
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
    unsigned int childrenCount;
    unsigned int maxChildren;
    void* data;
} Node;

Node* createNode(NodeType type, unsigned int defaultChildren);
void deleteNode(Node* node);
void addChildNode(Node* parent, Node* child);



#endif