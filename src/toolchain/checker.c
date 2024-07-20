/*
 * Quarzum Compiler - checker.c
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
#include "../../include/toolchain/checker.h"

void checkAST(node_t ast){
    SymbolTable* symbols = createSymbolTable();
    for(u_int32_t i = 0; i < ast->childrenCount; ++i){
        if(ast->children[i] != NULL){
           checkNode(ast->children[i], symbols);
        }
    }
}

void checkNode(node_t node, SymbolTable* symbols){
    switch (node->type)
    {
    // Secondary RootNodes may appear in simple import statements
    case Root:
        for(u_int32_t i = 0; i < node->childrenCount; ++i){
            if(node->children[i] != NULL){
                checkNode(node->children[i], symbols);
            }
        }
        break;
    case VarStmt:
        print(node->data);
        // Check if the node->data (symbol id) is in the table. If not, add it
        // Check types
        break;    

    default:
        break;
    }
}