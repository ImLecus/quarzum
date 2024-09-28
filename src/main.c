#include "quarzum.h"

int main(int argc, char** argv) {
    struct process task = start_process("Task");
    // Lexical & Syntax analisys
    node_t* ast = parse("code.qz");
    check_errors();
    // Semantic analisys
    check_ast(ast);
    check_errors();
    free(type_map);
    free(symbol_table);
    free(ast);

    end_process(&task);
    printf(GREEN"[SUCCESS]"RESET" Process finished with exit code 0\n");
    
    return 0;
}
