/*
 * Quarzum Compiler - main.c
 * Version 1.0, 11/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#include "include/quarzum.h"
#include "quarzum.h"

int main(int argc, char** argv) {
    flags |= debugMode;

    struct process task = start_process("Task");
    debug("Starting the compiler...");
    // Lexical analisys
    struct process lex = start_process("Lex phase");
    lexer* lexer = init_lexer(read_file("code.qz")->value);

    token* tok = next_token(lexer);
    printf(lexer->buffer->value);

    end_process(&lex);
    // Syntax analisys
    // struct process parsing = start_process("Parse phase");
    // node* ast = parse(t);
    // end_process(&parsing);

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
    free(lexer);
    end_process(&task);

    return 0;
}