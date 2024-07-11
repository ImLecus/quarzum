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

void compile(const char* file){
    Process* task = startProcess("Task");
    debug("Starting the compiler...");
    Process* lex = startProcess("Lex phase");
    TokenList* t = tokenize(file);
    endProcess(lex);
    Process* parsing = startProcess("Parse phase");
    SymbolTable* symbolTable = createSymbolTable();
    // create type table
    // add symbol and type tables into parsing
    Node* ast = parse(t, symbolTable);
    endProcess(parsing);

    // generate IR
    InstructionList* ir = createInstructionList();

    // generate asm
    //createFile(outputFileName);

    // free memory
    deleteTokenList(t);
    deleteInstructionList(ir);
    deleteSymbolTable(symbolTable);
    endProcess(task);
}