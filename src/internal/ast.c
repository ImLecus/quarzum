/*
 * Quarzum Compiler - ast.c
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
#include "../../include/quarzum/ast.h"

inline node_t createNode(NodeType type, u_int32_t defaultChildren){
    node_t node = (node_t)malloc(sizeof(Node));
    node->type = type;
    node->childrenCount = 0;
    node->maxChildren = defaultChildren;
    node->children = (node_t*)malloc(sizeof(Node) * defaultChildren);
    node->data = NULL;
    return node;
}

inline void deleteNode(node_t node){
    for(u_int32_t i = 0; i < node->childrenCount; ++i){
        if(node->children[i] != NULL){
            deleteNode(node->children[i]);
        }
    }
    free(node->data);
    free(node);
}

void addChildNode(node_t parent, node_t child){
    if(parent->childrenCount == parent->maxChildren){
        if(parent->maxChildren == 0){
            parent->children = (node_t*)realloc(parent->children,sizeof(Node)); // 1 node
            parent->maxChildren = 1;
        }
        else{
            parent->children = (node_t*)realloc(parent->children,sizeof(Node) * parent->childrenCount * 2); 
            parent->maxChildren = parent->childrenCount * 2;
        }
    }
    parent->children[(parent->childrenCount)++] = child;
}