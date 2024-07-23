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
    Process* task = startProcess("Task");
    debug("Starting the compiler...");
    // Lexical analisys
    Process* lex = startProcess("Lex phase");
    TokenList* t = tokenize(file);
    endProcess(lex);
    // Syntax analisys
    Process* parsing = startProcess("Parse phase");
    node_t ast = parse(t);
    endProcess(parsing);
    // Semantic analisys
    Process* checking = startProcess("Check phase");
    checkAST(ast);
    endProcess(checking);
    // Intermediate representation
    InstructionList* ir = createInstructionList();
    generateIR(ast, ir);
    printf("%d",ir->size);
    // ASM and output file

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