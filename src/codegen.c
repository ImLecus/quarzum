#include "quarzum.h"

static char* param_registers[6] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
static char* registers[6] = {"%r10", "%r11", "%r12", "%r13", "%r14", "%r15"};

static int get_free_reg(){
    static int i;
    return i < 6? i++ : -1;
}


static int param_index = 0;
static int get_free_param(){
    return param_index < 6? param_index++: -1;
}

static char* get_type(type* type){
    if(type->size == 1){
        return ".byte ";
    }
    if(type->type == TY_PTR && strcmp(type->name, "char*") == 0){ 
        return ".string ";
    }
    if(type->type == TY_NUM){
        return type->size == 8 ? ".double " : ".float ";
    }
    if(type->size == 2){
        return ".word ";
    }
    if(type->size == 4){
        return ".long ";
    }
    if(type->size == 8){
        return ".quad ";
    }
    return ".space ";
}

static void node_gen(instruction* instruction, asm_code* code){
    switch (instruction->type)
    {
    case I_BRANCH:
        string_append(code->text_section, instruction->dest);
        string_append(code->text_section, ":\n");
        break;
    case I_ADD:
        if(instruction->dest[0] == '.'){
            instruction->dest = registers[instruction->dest[strlen(instruction->dest)- 1]-'0'];
        }
        if(instruction->arg1[0] == '.'){
            instruction->arg1 = registers[instruction->arg1[strlen(instruction->arg1)- 1]-'0'];
        }

        string_append(code->text_section, "mov ");
        string_append(code->text_section, instruction->dest);
        string_push(code->text_section, ',');
        string_append(code->text_section, instruction->arg1);
        string_push(code->text_section, '\n');

        string_append(code->text_section, "add ");
        string_append(code->text_section, instruction->dest);
        string_push(code->text_section, ',');
        string_append(code->text_section, instruction->arg2);
        string_push(code->text_section, '\n');
        break;
    case I_JMP:
        string_append(code->text_section, "jmp ");
        string_append(code->text_section, instruction->dest);
        string_push(code->text_section, '\n');
        break;
    case I_IF:
        string_append(code->text_section, "cmpb $1, ");
        string_append(code->text_section, instruction->dest);
        string_append(code->text_section, "\nje ");
        string_append(code->text_section, instruction->arg1);
        string_push(code->text_section, '\n');
        break;
    case I_NIF:
        string_append(code->text_section, "cmpb $1, ");
        string_append(code->text_section, instruction->dest);
        string_append(code->text_section, "\njne ");
        string_append(code->text_section, instruction->arg1);
        string_push(code->text_section, '\n');
        break;
    case I_ASSIGN:
        type* assign_type = (type*)instruction->data;
        string_append(code->data_section, ".global ");
        string_append(code->data_section, instruction->dest);
        string_push(code->data_section, '\n');
        string_append(code->data_section, instruction->dest);
        string_append(code->data_section, ": ");
        string_append(code->data_section, get_type(assign_type));
        string_append(code->data_section, instruction->arg1);
        string_push(code->data_section, '\n');
        break;
    case I_MOV:
        if(instruction->arg1[0] == '.'){
            instruction->arg1 = registers[instruction->arg1[strlen(instruction->arg1)- 1]-'0'];
        }
        string_append(code->text_section, "mov ");
        string_append(code->text_section, instruction->dest);
        string_append(code->text_section, ", ");
        string_append(code->text_section, instruction->arg1);
        string_push(code->text_section, '\n');
        break;
    case I_LEAVERET:
        string_append(code->text_section, "leave\nret\n");
        break;
    case I_FUNCTION:
        string_append(code->text_section, instruction->dest);
        string_append(code->text_section, ":\nenter $");
        string_append(code->text_section, instruction->arg1);
        string_append(code->text_section, ", $0\n");
        break;  
    case I_PARAM:
        string_append(code->text_section, "movq $");
        string_append(code->text_section, instruction->dest);
        string_push(code->text_section, ',');
        string_append(code->text_section, param_registers[get_free_param()]);
        string_push(code->text_section, '\n');
        break;
    case I_CALL:
        string_append(code->text_section, "call ");
        string_append(code->text_section, instruction->dest);
        string_push(code->text_section, '\n');
        param_index = 0;
        break;
    default:
        printf("%d\n",instruction->type);
            break;
        }
}

asm_code* code_gen(vector* ir){
    asm_code* code = malloc(sizeof(asm_code));
    code->data_section =    init_string(256);
    string_append(code->data_section, ".data\n");
    code->bss_section =     init_string(256);
    string_append(code->bss_section, ".bss\n");
    code->text_section =    init_string(1024);
    string_append(code->text_section, ".text\n.global _start\n_start:\ncall main\nmovq $60, %rax\nmovq $0, %rdi\nsyscall\n");

    for(uint32_t i = 0; i < ir->len; ++i){
        instruction* child = (instruction*)(vector_get(ir,i));
        node_gen(child, code);
    }
    return code;
}

