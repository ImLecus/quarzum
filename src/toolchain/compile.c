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
#include "../quarzum.h"

void compile(char* file){
    struct process task = start_process("Task");
    debug("Starting the compiler...");
    // Lexical analisys
    struct process lex = start_process("Lex phase");
    vector* t = tokenize(file);
    end_process(&lex);
    // Syntax analisys
    // Process* parsing = start_process("Parse phase");
    // node_t ast = parse(t);
    // end_process(parsing);
    // // Semantic analisys
    // Process* checking = start_process("Check phase");
    // checkAST(ast);
    // end_process(checking);
    // // Intermediate representation
    // InstructionList* ir = createInstructionList();
    // generateIR(ast, ir);
    // printf("%d",ir->size);
    // ASM and output file

    // free memory
    free_vector(t);
    //deleteInstructionList(ir);
    end_process(&task);
}

node_t getAST(char* file){
    struct process task = start_process("Import");
    struct process lex = start_process("Lex phase");
    vector* t = tokenize(file);
    end_process(&lex);
    // if(t != NULL){
    //     struct process parsing = start_process("Parse phase");
    //     node_t ast = parse(t);
    //     free_tlist(t);
    //     end_process(&parsing);
    //     end_process(&task);
    //     return ast;
    // }
    free_vector(t);
    end_process(&task);
    return NULL;
    
}