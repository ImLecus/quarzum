#include "quarzum.h"


static char* get_type_by_size(unsigned int size){
    switch (size)
    {
    case 1: return ".byte ";
    case 2: return ".word ";
    case 4: return ".long ";
    case 8: return ".quad ";
    default: return ".string ";
    }
}

static void node_gen(instruction* instruction, asm_code* code){
    switch (instruction->type)
    {
    case I_ASSIGN:
        string_append(code->data_section, ".global ");
        string_append(code->data_section, instruction->dest);
        string_push(code->data_section, '\n');
        string_append(code->data_section, instruction->dest);
        string_append(code->data_section, ": ");
        string_append(code->data_section, get_type_by_size(instruction->arg2));
        string_append(code->data_section, instruction->arg1);
        string_push(code->data_section, '\n');
        break;
    case I_LEAVERET:
        string_append(code->text_section, "leave\nret\n");
        break;
    // case I_ASSIGN:
    //     string_append(code->data_section, instruction->dest);
    //     string_append(code->data_section, ": .string ");
    //     string_append(code->data_section, instruction->arg1);
    //     string_push(code->data_section, '\n');
    //     break;
    case I_FUNCTION:
        string_append(code->text_section, instruction->dest);
        string_append(code->text_section, ":\n");
        break;  
    case I_PARAM:
        string_append(code->text_section, "movq ");
        string_append(code->text_section, instruction->dest);
        string_append(code->text_section, ",%rdi\n");
        break;
    case I_CALL:
        string_append(code->text_section, "call ");
        string_append(code->text_section, instruction->dest);
        string_push(code->text_section, '\n');
        break;
    default:
    printf("%d\n",instruction->type);
        break;
    }
}

asm_code* code_gen(vector* ir){
    asm_code* code = (asm_code*)malloc(sizeof(asm_code));
    code->data_section =    init_string(256);
    string_append(code->data_section, ".data\n");
    code->bss_section =     init_string(256);
    string_append(code->bss_section, ".bss\n");
    code->text_section =    init_string(1024);
    string_append(code->text_section, ".text\n.global _start\n_start:\ncall main\nmovq $60, %rax\nmovq $0, %rdi\nsyscall\n");

    for(unsigned int i = 0; i < ir->len; ++i){
        instruction* child = (instruction*)(vector_get(ir,i));
        node_gen(child, code);
    }


    return code;
}

