/**
 * \file            x86_64.c
 * \brief           Quarzum x86_64 backend
 * \copyright       2024 Marcos González GPL3
 */
#include "x86_64.h"


static char* get_value(operand_t* operand){
    switch (operand->optype)
    {
    case CONSTANT: return operand->value.const_value;
    case REGISTER: return registers[operand->value.reg];
    case VARIABLE: return operand->value.varname;
    case POINTER:  
        char* child = get_value((operand_t*)operand->value.address);
        char* result = malloc(strlen(child) + 1);
        result[0] = '$';
        strncat(result, child, strlen(child));
        return result;
    default:
        printf("%d \n", operand->optype);
        return NULL;
    }
}

static char* get_size(char* operation, int size){

}

int assemble_x86_64(ir_t* ir){
    String* data_segment = init_string(512);
    String* text_segment = init_string(512);

    string_append(data_segment, ".section data\n");
    for(int i = 0; i < ir->data->len; ++i){

    }
    
    string_append(text_segment, ".section text\n");
    for(int i = 0; i < ir->instructions->len; ++i){
        instruction_t* ins = ir->instructions->value[i];
        switch (ins->opcode)
        {
        case MOV:
            string_append(text_segment, "mov ");
            string_append(text_segment, get_value(&ins->operands[0]));
            string_append(text_segment, ", ");
            string_append(text_segment, get_value(&ins->operands[1]));
            string_push(text_segment, '\n');
            break;
        case RET:
            string_append(text_segment, "ret\n");
            break;
        case LABEL:
            string_append(text_segment, get_value(&ins->operands[0]));
            string_append(text_segment,":\n");
            break;
        case JMP:
            string_append(text_segment, "jmp ");
            string_append(text_segment, get_value(&ins->operands[0]));
            string_push(text_segment, '\n');
            break;
        case SYSCALL:
            string_append(text_segment, "syscall\n");
            break;
        
        default:
            break;
        }
    }
    printf(text_segment->content);
    free(data_segment);
    free(text_segment);

}