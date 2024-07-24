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
        return createNode(BreakStmt,0);
    case Continue:
        EXPECT(Semicolon, "Expected semicolon")
        return createNode(ContinueStmt,0);
    case TypeKeyword:
        return parseDeclaration(tokens, i);
    case Import:
        return parseImport(tokens, i);
    case Identifier:
        if(getToken(tokens, *i + 1).type == LeftPar){
            return parseFunctionCall(tokens,i);
        }
    default:
        return stmt;
    }
}

node_t parseFunctionCall(PARSING_POS){
    Token id = getToken(tokens, *i);
    pass;
    pass;
    node_t call = createNode(CallStmt,0);
    call->data = id.value;
    // args
    while(*i < tokens->size){
        if(getToken(tokens, *i).type == RightPar){
            pass;
            break;
        }
        node_t expr = parseExpr(tokens, i);
        if(expr == NULL){
            err("Expected expression", 0);
            pass;
        }
        addChildNode(call, expr);
        if(getToken(tokens, *i).type == Comma){
            pass;
            continue;
        }
    }
    EXPECT(Semicolon, "Expected semicolon");
    pass;
    return call;
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
        print(resolve_path(next.value));
        node_t importedAST = getAST(resolve_path(next.value));
        return importedAST;
    }
    err("Expected string literal or identifier",0);
    // Complex import, merges the selected nodes with the AST.
    return NULL;
}