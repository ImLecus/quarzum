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
    print("Starting the compiler...");
    Buffer* b = read(file);

    Process* lex = startProcess("Lex phase");
    TokenList* t = tokenize(b);
    endProcess(lex);
    Process* parsing = startProcess("Parse phase");
    Node* ast = parse(t);
    endProcess(parsing);


    // free memory
    deleteTokenList(t);
    deleteBuffer(b);
    endProcess(task);
}