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
std::unique_ptr<FunctionCallExpr> Parser::parseFunctionCallExpr(Token id){
    std::unique_ptr<FunctionCallExpr> call = std::make_unique<FunctionCallExpr>(id.value);
    Token t = tokens.front();
    if(t.type == right_par){
        tokens.pop_front();
        return std::move(call);
    }
    while(not tokens.empty()){
        auto expr = parseExpr();
        call->args.push_back(std::move(expr));
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
    return std::move(call);
}

std::unique_ptr<ParenExpr> Parser::parseParenExpr(){
    auto expr = parseExpr();
    if(tokens.empty()){
        err("Mismatched '('");
        return nullptr;
    }
    EXPECT(pop(), right_par, "Expected ')'");
    return std::make_unique<ParenExpr>(std::move(expr));
}

std::unique_ptr<Expr> Parser::parsePrimaryExpr(){
    Token t = pop();
    if(t.type == ARITHMETIC_OPERATOR and (t.value == "+" or t.value == "-" or t.value == "not" or t.value == "++" or t.value == "--")){
        if(t.value == "--" or t.value == "++"){
            Token next = pop();
            EXPECT(next,identifier,"Increment or decrement operators are only compatible with identifiers")
            return std::make_unique<UnaryExpr>(t.value, std::make_unique<IdentifierExpr>(next.value));
        }
        auto operand = parsePrimaryExpr();
        return std::make_unique<UnaryExpr>(t.value,std::move(operand));
    }
    std::unique_ptr<Expr> expr = nullptr;

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
            expr = std::make_unique<ArrayIndexExpr>(t.value, std::move(index));
            break;
        }
        expr = std::make_unique<IdentifierExpr>(t.value);
        break;
    case LITERAL:
        expr = std::make_unique<LiteralExpr>(t.value);
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
        return std::make_unique<UnaryExpr>(t.value, std::make_unique<IdentifierExpr>(suffix.value), false);
    }
    return std::move(expr);
}

std::unique_ptr<Expr> Parser::parseExpr() {
    auto left = parsePrimaryExpr();
    while(not tokens.empty()){
        Token op = tokens.front();
        if(op.value == "?"){
            tokens.pop_front();
            auto ifTrue = parseExpr();
            EXPECT(pop(), ternary_separator, "Expected ternary expression")
            auto ifFalse = parseExpr();
            left = std::make_unique<TernaryExpr>(std::move(left), std::move(ifTrue), std::move(ifFalse));
            break;
        }
        if(not op.isOperator() or op.value == "++" or op.value == "--"){
            break;
        }
        tokens.pop_front();
        auto right = parseExpr();
        left = std::make_unique<BinaryExpr>(op.value,std::move(left), std::move(right));
    }
    return std::move(left);
}