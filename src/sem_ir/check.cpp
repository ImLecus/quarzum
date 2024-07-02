/*
 * Quarzum Compiler - check.cpp
 * Version 1.0, 01/07/2024
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
#include "check.h"

IRInstruction check(const RootNode& root){
    // for(auto& node : root.nodes){
    //     check(*node);
    // } 
    // return the IRInstruction eqivalent to "exit 0;"  
    return {};
}

IRInstruction check(const FunctionCall& call){
    // for(auto& arg: call.args){
    //     check(*arg);
    // }
    return {CALL, call.name};
}

IRInstruction check(const BreakStatement& break_stmt){
    return {};
}