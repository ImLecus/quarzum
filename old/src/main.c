#include "quarzum.h"

int main(int argc, char** argv) {
    
    int error_code = 0;
    Process task = start_process("Task");
    AST* ast;
    ir_t* ir;


    error_code = compile("code.qz", ast);
    ir = ast_to_ir(ast);
    assemble(ir, X86_64);
    
    free(ast);
    free(ir);
    end_process(task);

    printf(GREEN"[SUCCESS]"RESET" Process finished with exit code 0\n");

    return error_code;
}
