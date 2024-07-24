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

node_t parseExpr(PARSING_POS){
    node_t left = parsePrimaryExpr(tokens, i);
    
    while (get_token(tokens, *i)->value != NULL)
    {
        struct token* op = get_token(tokens, *i);

        if(strcmp(op->value,"?") == 0){
            ++(*i);
            node_t ifTrue = parseExpr(tokens,i);
            if(get_token(tokens, (*i)++)->type != T_TERNARY_SEPARATOR){
                err("Expected ternary expression.",0);
                return NULL;
            }
            node_t ifFalse = parseExpr(tokens,i);
            node_t expr = createNode(TernaryExpr, 3);
            addChildNode(expr, left);
            addChildNode(expr, ifTrue);
            addChildNode(expr, ifFalse);
            return expr;        
        }
        
        if(op->type != T_ARITHMETIC_OPERATOR && op->type != T_COMPARATION_OPERATOR){
            break;
        }

        ++(*i);
        node_t right = parseExpr(tokens, i);
        node_t expr = createNode(BinaryExpr, 2);
        expr->data = op->value;
        addChildNode(expr, left);
        addChildNode(expr, right);
    }
    return left;
}

node_t parseParenExpr(PARSING_POS){

    node_t expr = parseExpr(tokens,i);

    if(get_token(tokens, *i)->type != T_RIGHT_PAR){
        err("Expected ')'",0);
        return NULL;
    }
    node_t parenExpr = createNode(ParenExpr,0);
    parenExpr->data = expr;
    return parenExpr;
}

node_t parsePrimaryExpr(PARSING_POS){
    struct token* t = get_token(tokens,(*i)++);
    if(t->type == T_ARITHMETIC_OPERATOR && 
    (strcmp(t->value, "+") == 0 || strcmp(t->value, "-") == 0 || strcmp(t->value, "++") == 0 || strcmp(t->value, "--") == 0) ){
        if(strcmp(t->value, "++") == 0 || strcmp(t->value, "--") == 0){
            struct token* next = get_token(tokens,*i);
            if(get_token(tokens, *i + 1)->type != T_IDENTIFIER){
                err("Increment or decrement operators are only compatible with identifiers", 0);
                return NULL;
            }
            node_t unaryExpr = createNode(UnaryExpr,1);
            unaryExpr->data = t->value;
            node_t id = createNode(IdentifierNode, 0);
            id->data = next->value;
            addChildNode(unaryExpr, id);
            return unaryExpr;
        }
        node_t unaryExpr = createNode(UnaryExpr,1);
        unaryExpr->data = t->value;
        addChildNode(unaryExpr, parsePrimaryExpr(tokens,i));
        return unaryExpr;
    }
    node_t expr = NULL;
    switch (t->type)
    {
    case T_IDENTIFIER:
        expr = createNode(IdentifierNode,0);
        expr->data = t->value;
        break;
    case T_INT_LITERAL:
    case T_STRING_LITERAL:
    case T_NUMERIC_LITERAL:
    case T_CHAR_LITERAL:
    case T_BOOL_LITERAL:
    case T_NULL_LITERAL:
        expr = createNode(Literal,0);
        expr->data = t->value;
        break;
    case T_LEFT_PAR:
        expr = parseParenExpr(tokens,i);
        break;
    default:
        return NULL;
    }
    struct token* suffix = get_token(tokens, *i);
    if(suffix->type == T_ARITHMETIC_OPERATOR && (suffix->value == "--" || suffix->value == "++")){
        ++(*i);
        if(t->type != T_IDENTIFIER){
            err("Increment or decrement operators are only compatible with identifiers", 0);
            return NULL;
        }
        node_t unaryExpr = createNode(UnaryExpr, 1);
        unaryExpr->data = suffix->value;
        node_t id = createNode(T_IDENTIFIER, 0);
        id->data = suffix->value;
        addChildNode(unaryExpr, id);
        return unaryExpr;
    }

    return expr;
}