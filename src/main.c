#include "quarzum.h"

int main(int argc, char** argv) {
    struct process task = start_process("Task");

    ast_t* ast = compile("code.qz");
    free(ast);

    end_process(&task);
    printf(GREEN"[SUCCESS]"RESET" Process finished with exit code 0\n");
    
    return 0;
}
