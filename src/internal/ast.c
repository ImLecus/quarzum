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

Node* createNode(NodeType type, unsigned int defaultChildren){
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = type;
    node->childrenCount = 0;
    node->maxChildren = defaultChildren;
    node->children = (Node**)malloc(sizeof(Node) * defaultChildren);
    node->data = NULL;
    return node;
}

void deleteNode(Node* node){
    for(unsigned int i = 0; i < node->childrenCount; ++i){
        if(node->children[i] != NULL){
            deleteNode(node->children[i]);
        }
    }
    free(node->data);
    free(node);
}

void addChildNode(Node* parent, Node* child){
    if(parent->childrenCount == parent->maxChildren){
        if(parent->maxChildren == 0){
            parent->children = (Node**)realloc(parent->children,sizeof(Node)); // 1 node
            parent->maxChildren = 1;
        }
        else{
            parent->children = (Node**)realloc(parent->children,sizeof(Node) * parent->childrenCount * 2); 
            parent->maxChildren = parent->childrenCount * 2;
        }
    }
    parent->children[(parent->childrenCount)++] = child;
}