#include "src/quarzum.h"

int main(int argc, char** argv) {

    struct process task = start_process("Task");
    printf(DEBUG_MSG("Starting the compiler..."));
    // Lexical & Syntax analisys
    struct process lex = start_process("Parse phase");
    node* ast = parse("code.qz");
    end_process(&lex);
    // Semantic analisys
    struct process checking = start_process("Check phase");
    // checkAST(ast);

    vector* ir_list = generate_ir(ast);
    end_process(&checking);

    // Code generation
    // struct process codegn = start_process("Code generation phase");
    // asm_code* assembly = code_gen(ast);
    // end_process(&codegn);

    // printf("%s\n%s\n%s\n", 
    // assembly->data_section->value, 
    // assembly->bss_section->value,
    // assembly->text_section->value
    // );


    // free memory
    free_vector(ir_list);
    free_vector(ast->children);
    free(ast);
    end_process(&task);

    return 0;
}