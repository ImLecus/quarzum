#include "src/quarzum.h"

int main(int argc, char** argv) {

    struct process task = start_process("Task");
    printf(DEBUG_MSG("Starting the compiler..."));
    // Lexical & Syntax analisys
    struct process lex = start_process("Parse phase");
    node* ast = parse();
    end_process(&lex);
    // Semantic analisys
    struct process checking = start_process("Check phase");
    // checkAST(ast);
    end_process(&checking);
    // Code generation
    struct process codegn = start_process("Code generation phase");
    
    end_process(&codegn);

    // free memory
    end_process(&task);

    return 0;
}