/*
 * Quarzum Compiler - expr.cpp
 * Version 1.0, 02/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#pragma once
#include "parser.h"
unique_ptr<FunctionCallExpr> Parser::parseFunctionCallExpr(Token id){
    unique_ptr<FunctionCallExpr> call = make_unique<FunctionCallExpr>(id.value);
    Token t = tokens.front();
    if(t.type == right_par){
        tokens.pop_front();
        return move(call);
    }
    while(not tokens.empty()){
        auto expr = parseExpr();
        call->args.push_back(move(expr));
        Token next = pop();
        if(next.type == comma){
            continue;
        }
        if(next.type == right_par){
            break;
        }
        throwError("Expected ',' or ')'", next);
        return nullptr;
    }
    return move(call);
}

unique_ptr<ParenExpr> Parser::parseParenExpr(){
    auto expr = parseExpr();
    if(tokens.empty()){
        err("Mismatched '('");
        return nullptr;
    }
    EXPECT(pop(), right_par, "Expected ')'");
    return make_unique<ParenExpr>(move(expr));
}

unique_ptr<Expr> Parser::parsePrimaryExpr(){
    Token t = pop();
    if(t.type == ARITHMETIC_OPERATOR and (t.value == "+" or t.value == "-" or t.value == "not" or t.value == "++" or t.value == "--")){
        if(t.value == "--" or t.value == "++"){
            Token next = pop();
            EXPECT(next,identifier,"Increment or decrement operators are only compatible with identifiers")
            return make_unique<UnaryExpr>(t.value, make_unique<IdentifierExpr>(next.value));
        }
        auto operand = parsePrimaryExpr();
        return make_unique<UnaryExpr>(t.value,move(operand));
    }
    unique_ptr<Expr> expr = nullptr;

    switch (t.type)
    {
    case identifier:
        
        if(ask(left_par)){
            tokens.pop_front();
            expr = parseFunctionCallExpr(t);
            break;
        }
        if(ask(left_square)){
            tokens.pop_front();
            auto index = parseExpr();
            if(tokens.empty()){
                EXPECT(t, right_square, "Expected ']");
                return nullptr;
            }
            EXPECT(pop(), right_square, "Expected ']");
            expr = make_unique<ArrayIndexExpr>(t.value, move(index));
            break;
        }
        expr = make_unique<IdentifierExpr>(t.value);
        break;
    case int_literal:
    case numeric_literal:
    case string_literal:
    case char_literal:
    case bool_literal:
    case null_literal:
        expr = make_unique<LiteralExpr>(t);
        break;
    case left_par:
        expr = parseParenExpr();
        break;
    default:
        return nullptr;
    }
    Token suffix = tokens.front();
    if(suffix.type == ARITHMETIC_OPERATOR and (suffix.value=="--" or suffix.value == "++")){
        tokens.pop_front();
        if(t.type != identifier){
            throwError("Increment or decrement operators are only compatible with identifiers", suffix);
            return nullptr;
        }
        return make_unique<UnaryExpr>(t.value, make_unique<IdentifierExpr>(suffix.value), false);
    }
    return move(expr);
}

unique_ptr<Expr> Parser::parseExpr() {
    auto left = parsePrimaryExpr();
    while(not tokens.empty()){
        Token op = tokens.front();
        if(op.value == "?"){
            tokens.pop_front();
            auto ifTrue = parseExpr();
            EXPECT(pop(), ternary_separator, "Expected ternary expression")
            auto ifFalse = parseExpr();
            left = make_unique<TernaryExpr>(move(left), move(ifTrue), move(ifFalse));
            break;
        }
        if(not op.isOperator() or op.value == "++" or op.value == "--"){
            break;
        }
        tokens.pop_front();
        auto right = parseExpr();
        left = make_unique<BinaryExpr>(op.value,move(left), move(right));
    }
    return move(left);
}