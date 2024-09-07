#include "../include/qbyte.h"

qbyte_code_t bytecode;

void qbyte_init(){
    bytecode.functions = NULL;
    bytecode.functions_count = 0;
    bytecode.data = NULL;
    bytecode.data_count = 0;
    bytecode.instructions = NULL;
    bytecode.instructions_count = 0;
}

void qbyte_add_function(qbyte_function_t func){
    bytecode.functions = realloc(bytecode.functions, sizeof(qbyte_function_t) * (bytecode.functions_count + 1));
    bytecode.functions[bytecode.functions_count++] = func;
}

void qbyte_add_data(qbyte_data_t data){
    bytecode.data = realloc(bytecode.data, sizeof(qbyte_data_t) * (bytecode.data_count + 1));
    bytecode.data[bytecode.data_count++] = data;
}

void qbyte_add_instruction(qbyte_instruction_t inst){
    bytecode.instructions = realloc(bytecode.instructions, sizeof(qbyte_instruction_t) * (bytecode.instructions_count + 1));
    bytecode.instructions[bytecode.instructions_count++] = inst;
}

void qbyte_free_bytecode(){
    free(bytecode.data);
    free(bytecode.functions);
    free(bytecode.instructions);
    bytecode.data_count = 0;
    bytecode.functions_count = 0;
    bytecode.instructions_count = 0;
}