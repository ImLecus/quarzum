/**
 * \file            x86_64.c
 * \brief           x86_64 traduction for the Quarzum compiler.
 * \copyright       2024 Marcos González GPL3
 */
#include "assembler.h"

static char* data(ir_data_t* d){
    string_t* code = init_string(32);
    string_append(code, d->name);
    string_append(code, ": ");
    char* types[8] = {".byte ", ".word ", ".long ", ".quad "};
    if(d->type == 1) { // int
        string_append(code, types[8/d->size]);
        
        //string_append(code, d->value.ptr_value);
    }
    // type
    // value

    string_push(code, '\n');

    return code->content;
}

char* x86_64(ir_code_t* ir){
    char* registers[5] = {"rax", "rdi", "rcx", "rdx", "r8"};
    char* stack_registers[2] = {"rbp", "rsp"};
    string_t* data_section = init_string(512);
    string_t* code_section = init_string(1024);

    string_append(data_section, ".data:\n");
    string_append(code_section, ".text:\n");

    for(size_t i = 0; i < ir->data->len; ++i){
        string_append(data_section, data((ir_data_t*)ir->data->value[i]));
    }


    string_append(data_section, code_section->content);
    return data_section->content;
}