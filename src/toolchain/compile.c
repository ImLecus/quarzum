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
    Process* task = start_process("Task");
    debug("Starting the compiler...");
    // Lexical analisys
    Process* lex = start_process("Lex phase");
    TokenList* t = tokenize(file);
    end_Process(lex);
    // Syntax analisys
    Process* parsing = start_process("Parse phase");
    node_t ast = parse(t);
    end_Process(parsing);
    // Semantic analisys
    Process* checking = start_process("Check phase");
    checkAST(ast);
    end_Process(checking);
    // Intermediate representation
    InstructionList* ir = createInstructionList();
    generateIR(ast, ir);
    printf("%d",ir->size);
    // ASM and output file

    // free memory
    deleteTokenList(t);
    deleteInstructionList(ir);
    end_Process(task);
}

node_t getAST(char* file){
    Process* task = start_process("Import");
    Process* lex = start_process("Lex phase");
    TokenList* t = tokenize(file);
    end_Process(lex);
    if(t != NULL){
        Process* parsing = start_process("Parse phase");
        node_t ast = parse(t);
        deleteTokenList(t);
        end_Process(parsing);
        end_Process(task);
        return ast;
    }
    deleteTokenList(t);
    end_Process(task);
    return NULL;
    
}