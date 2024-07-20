/*
 * Quarzum Compiler - compile.c
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
#include "../../include/toolchain/compile.h"

void compile(char* file){
    printf(resolvePath("code.qz\n"));
    printf(resolvePath("./math.qz\n"));

    Process* task = startProcess("Task");
    debug("Starting the compiler...");
    Process* lex = startProcess("Lex phase");
    TokenList* t = tokenize(file);
    endProcess(lex);
    Process* parsing = startProcess("Parse phase");
    // create type table
    // add symbol and type tables into parsing
    node_t ast = parse(t);
    endProcess(parsing);

    // generate IR
    InstructionList* ir = createInstructionList();

    // generate asm
    //createFile(outputFileName);

    // free memory
    deleteTokenList(t);
    deleteInstructionList(ir);
    endProcess(task);
}

node_t getAST(char* file){
    Process* task = startProcess("Import");
    Process* lex = startProcess("Lex phase");
    TokenList* t = tokenize(file);
    endProcess(lex);
    if(t != NULL){
        Process* parsing = startProcess("Parse phase");
        node_t ast = parse(t);
        deleteTokenList(t);
        endProcess(parsing);
        endProcess(task);
        return ast;
    }
    deleteTokenList(t);
    endProcess(task);
    return NULL;
    
}