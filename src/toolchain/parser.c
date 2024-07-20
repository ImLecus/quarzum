/*
 * Quarzum Compiler - parser.c
 * Version 1.0, 10/07/2024
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

node_t parse(TokenList* tokens){
    node_t root = createNode(Root,1);
    for(unsigned int i = 0; i < tokens->size; ++i){
        node_t statement = parseStatement(tokens, &i);
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

node_t parseStatement(PARSING_POS){
    node_t stmt = NULL;
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
        stmt = parseDeclaration(tokens, i);
        break;
    case Import:
        stmt = parseImport(tokens, i);
    default:
        break;
    }
    return stmt;
}

node_t parseDeclaration(PARSING_POS){
    node_t decl = NULL;
    // to-do: add types into declarations
    Token type = getToken(tokens, *i);
    EXPECT(TypeKeyword, "Expected type keyword.")
    pass;
    Token id = getToken(tokens,*i);
    EXPECT(Identifier, "Expected identifier")
    pass;
    switch (getToken(tokens, *i).type)
    {
    case Equal:
        pass;
        node_t expr = parseExpr(tokens, i);
        if(expr ==  NULL){
            err("Expected expression",0);
            break;
        }
        decl = createNode(VarStmt, 1);
        decl->data = id.value;
        addChildNode(decl, expr);
        break;
    case Semicolon:
        pass;
        decl = createNode(VarStmt,1);
        decl->data = id.value;
        break;
    case LeftPar:
        pass;
        // parse arguments
        EXPECT(RightPar, "Expected ')'");
        pass;
        decl = createNode(FunctionStmt,2);
        decl->data = id.value;
        // semicolon for incompletion
        EXPECT(LeftCurly, "Expected 'function' body");
        pass;
        while(getToken(tokens, *i).type != RightPar){
            node_t statement = parseStatement(tokens, i);
            if(statement != NULL){
                addChildNode(decl, statement);
            }
            pass;
        }
        // parse identation
        break;
    default:
        err("Expected semicolon or declaration", 0);
        break;
    }
    Type* t = getTypeFromToken(type);
    if(t == NULL){
        // err
    }

    return decl;
}

node_t parseImport(PARSING_POS){
    pass;
    Token next = getToken(tokens, *i);
    pass;
    // Single import, merges the two AST.
    if(next.type == StringLiteral){
        EXPECT(Semicolon, "Expected semicolon");
        node_t importedAST = getAST(deleteQuotes(next.value));
        return importedAST;
    }
    err("Expected string literal or identifier",0);
    // Complex import, merges the selected nodes with the AST.
    return NULL;
}