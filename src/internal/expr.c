/*
 * Quarzum Compiler - expr.c
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
#include "../../include/quarzum/expr.h"

Node* parseExpr(PARSING_POS){
    Node* left = parsePrimaryExpr(tokens, i,symbolTable);
    
    while (getToken(tokens, *i).value != NULL)
    {
        Token op = getToken(tokens, *i);

        if(strcmp(op.value,"?") == 0){
            ++(*i);
            Node* ifTrue = parseExpr(tokens,i,symbolTable);
            if(getToken(tokens, (*i)++).type != TernarySeparator){
                err("Expected ternary expression.",0);
                return NULL;
            }
            Node* ifFalse = parseExpr(tokens,i,symbolTable);
            Node* expr = createNode(TernaryExpr, 3);
            addChildNode(expr, left);
            addChildNode(expr, ifTrue);
            addChildNode(expr, ifFalse);
            return expr;        
        }
        
        if(op.type != ArithmeticOperator && op.type != ComparationOperator){
            break;
        }

        ++(*i);
        Node* right = parseExpr(tokens, i,symbolTable);
        Node* expr;
        expr->create(BinaryExpr, 2);
        expr->data = op.value;
        addChildNode(expr, left);
        addChildNode(expr, right);
    }
    return left;
}

Node* parseParenExpr(PARSING_POS){

    Node* expr = parseExpr(tokens,i,symbolTable);

    if(getToken(tokens, *i).type != RightPar){
        err("Expected ')'",0);
        return NULL;
    }
    Node* parenExpr = createNode(ParenExpr,0);
    parenExpr->data = expr;
    return parenExpr;
}

Node* parsePrimaryExpr(PARSING_POS){
    Token t = getToken(tokens,(*i)++);
    if(t.type == ArithmeticOperator && 
    (strcmp(t.value, "+") == 0 || strcmp(t.value, "-") == 0 || strcmp(t.value, "++") == 0 || strcmp(t.value, "--") == 0) ){
        if(strcmp(t.value, "++") == 0 || strcmp(t.value, "--") == 0){
            Token next = getToken(tokens,*i);
            if(getToken(tokens, *i + 1).type != Identifier){
                err("Increment or decrement operators are only compatible with identifiers", 0);
                return NULL;
            }
            Node* unaryExpr = createNode(UnaryExpr,1);
            unaryExpr->data = t.value;
            Node* id = createNode(IdentifierNode, 0);
            id->data = next.value;
            addChildNode(unaryExpr, id);
            return unaryExpr;
        }
        Node* unaryExpr = createNode(UnaryExpr,1);
        unaryExpr->data = t.value;
        addChildNode(unaryExpr, parsePrimaryExpr(tokens,i,symbolTable));
        return unaryExpr;
    }
    Node* expr = NULL;
    switch (t.type)
    {
    case Identifier:
        expr = createNode(IdentifierNode,0);
        expr->data = t.value;
        break;
    case IntLiteral:
    case StringLiteral:
    case NumericLiteral:
    case CharLiteral:
    case BoolLiteral:
    case NullLiteral:
        expr = createNode(Literal,0);
        expr->data = t.value;
        break;
    case LeftPar:
        expr = parseParenExpr(tokens,i,symbolTable);
        break;
    default:
        return NULL;
    }
    Token suffix = getToken(tokens, *i);
    if(suffix.type == ArithmeticOperator && (suffix.value == "--" || suffix.value == "++")){
        ++(*i);
        if(t.type != Identifier){
            err("Increment or decrement operators are only compatible with identifiers", 0);
            return NULL;
        }
        Node* unaryExpr = createNode(UnaryExpr, 1);
        unaryExpr->data = suffix.value;
        Node* id = createNode(Identifier, 0);
        id->data = suffix.value;
        addChildNode(unaryExpr, id);
        return unaryExpr;
    }

    return expr;
}