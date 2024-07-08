/*
 * Quarzum Compiler - parser.c
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
#include "../../include/toolchain/parser.h"
#include <stdio.h>


Node* parse(TokenList* tokens){
    Node* root = createNode(Root,1);
    for(unsigned int i = 0; i < tokens->size; ++i){
        Node* statement = parseStatement(tokens, &i);
        if(statement != NULL){
            addChildNode(root, statement);
        }
        else {
            // err
        }
    }
    printf("%d statements created.\n", root->childrenCount);
    return root;
}

Node* parseStatement(PARSING_POS){
    Node* stmt = NULL;
    switch (getToken(tokens,*i).type)
    {
    case Break:
        EXPECT(Semicolon, "Expected semicolon")
        stmt = createNode(BreakStmt,0);
        break;
    case Continue:
        EXPECT(Semicolon, "Expected semicolon")
        stmt = createNode(ContinueStmt,0);
        break;
    case TypeKeyword:
        --(*i);
        stmt = parseDeclaration(tokens, i);
        break;
    default:
        break;
    }
    return stmt;
}

Node* parseDeclaration(PARSING_POS){
    Node* decl = NULL;
    EXPECT(TypeKeyword, "Expected type keyword.")
    Token id = getToken(tokens,*i);
    EXPECT(Identifier, "Expected identifier")
    switch (getToken(tokens, ++(*i)).type)
    {
    case Equal:
        pass;
        Node* expr = parseExpr(tokens, i);
        if(expr ==  NULL){
            err("Expected expression",0);
            break;
        }
        decl = createNode(VarStmt, 1);
        decl->data = id.value;
        addChildNode(decl, expr);
        break;
    case Semicolon:
        decl = createNode(VarStmt,1);
        decl->data = id.value;
        break;
    case LeftPar:
        pass;
        break;
    default:
        err("Expected semicolon or declaration", 0);
        break;
    }
    return decl;
}

