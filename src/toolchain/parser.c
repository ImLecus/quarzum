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
    switch (get_token(tokens,*i)->type)
    {
    case T_BREAK:
        EXPECT(T_SEMICOLON, "Expected semicolon")
        return createNode(BreakStmt,0);
    case T_CONTINUE:
        EXPECT(T_SEMICOLON, "Expected semicolon")
        return createNode(ContinueStmt,0);
    case T_TYPE_KEYWORD:
        return parseDeclaration(tokens, i);
    case T_IMPORT:
        return parseImport(tokens, i);
    case T_IDENTIFIER:
        if(get_token(tokens, *i + 1)->type == T_LEFT_PAR){
            return parseFunctionCall(tokens,i);
        }
    default:
        return stmt;
    }
}

node_t parseFunctionCall(PARSING_POS){
    struct token* id = get_token(tokens, *i);
    pass;
    pass;
    node_t call = createNode(CallStmt,0);
    call->data = id->value;
    // args
    while(*i < tokens->size){
        if(get_token(tokens, *i)->type == T_RIGHT_PAR){
            pass;
            break;
        }
        node_t expr = parseExpr(tokens, i);
        if(expr == NULL){
            err("Expected expression", 0);
            pass;
        }
        addChildNode(call, expr);
        if(get_token(tokens, *i)->type == T_COMMA){
            pass;
            continue;
        }
    }
    EXPECT(T_SEMICOLON, "Expected semicolon");
    pass;
    return call;
}

node_t parseDeclaration(PARSING_POS){
    node_t decl = NULL;
    // to-do: add types into declarations
    struct token* type = get_token(tokens, *i);
    EXPECT(T_TYPE_KEYWORD, "Expected type keyword.")
    pass;
    struct token* id = get_token(tokens,*i);
    EXPECT(T_IDENTIFIER, "Expected identifier")
    pass;
    switch (get_token(tokens, *i)->type)
    {
    case T_EQUAL:
        pass;
        node_t expr = parseExpr(tokens, i);
        if(expr ==  NULL){
            err("Expected expression",0);
            break;
        }
        decl = createNode(VarStmt, 1);
        decl->data = id->value;
        addChildNode(decl, expr);
        break;
    case T_SEMICOLON:
        pass;
        decl = createNode(VarStmt,1);
        decl->data = id->value;
        break;
    case T_LEFT_PAR:
        pass;
        // parse arguments
        EXPECT(T_RIGHT_PAR, "Expected ')'");
        pass;
        decl = createNode(FunctionStmt,2);
        decl->data = id->value;
        // semicolon for incompletion
        EXPECT(T_LEFT_CURLY, "Expected 'function' body");
        pass;
        while(get_token(tokens, *i)->type != T_RIGHT_PAR){
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
    struct token* next = get_token(tokens, *i);
    pass;
    // Single import, merges the two AST.
    if(next->type == T_STRING_LITERAL){
        EXPECT(T_SEMICOLON, "Expected semicolon");
        print(resolve_path(next->value));
        node_t importedAST = getAST(resolve_path(next->value));
        return importedAST;
    }
    err("Expected string literal or identifier",0);
    // Complex import, merges the selected nodes with the AST.
    return NULL;
}