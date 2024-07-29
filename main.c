#include "src/quarzum.h"

int main(int argc, char** argv) {

    struct process task = start_process("Task");
    printf(DEBUG_MSG("Starting the compiler..."));
    // Lexical & Syntax analisys
    struct process lex = start_process("Parse phase");
    node* ast = parse();
    end_process(&lex);
    // // Semantic analisys
    // Process* checking = start_process("Check phase");
    // checkAST(ast);
    // end_process(checking);
    // // Intermediate representation
    // InstructionList* ir = createInstructionList();
    // generateIR(ast, ir);
    // printf("%d",ir->size);
    // ASM and output filed

    // free memory
    end_process(&task);

    return 0;
}