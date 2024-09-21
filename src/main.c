#include "quarzum.h"
#include "qbyte/qbyte.h"
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

    vector_t* data = init_vector(32);
    vector_push(data, &(ir_data_t){
        .name = "examplevar",
        .type = INT,
        .size = 4,
        .value = "\"Hello world\""
    });
    vector_t* ins = init_vector(32);
    vector_push(ins, &(ir_instruction_t){
        .opcode = 1,
        .operand_num = 0,
        .operands = {}
    });
    vector_t* func = init_vector(32);
    vector_push(func, &(ir_function_t){
        .name = "out",
        .align=8,
        .arguments=2,
        .instructions = ins
    });
    ir_code_t ir;
    ir = (ir_code_t){
        .data = data,
        .functions = func
    };
    serialize(&ir, "example.qbyte");
    deserialize("example.qbyte",&ir);


    end_process(&task);
    printf(GREEN"[SUCCESS]"RESET" Process finished with exit code 0\n");
    //char* asm_code = generate_asm(&ir, X86_64);
    //printf(asm_code);

    return 0;
}
