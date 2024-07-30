#include "quarzum.h"


static char* get_type_by_size(unsigned int size){
    switch (size)
    {
    case 1: return ".byte ";
    case 2: return ".word ";
    case 4: return ".long ";
    case 8: return ".quad ";
    default: return ".space ";
    }
}

static void node_gen(node* n, asm_code* code){
    switch (n->type)
    {
    case N_VAR:
        symbol* s = (symbol*)(vector_get(n->children, 0));
        if(s->scope == S_GLOBAL){
            string_append(code->data_section, ".global ");
            string_append(code->data_section, s->name);
            string_push(code->data_section, '\n');
        }
        string_append(code->data_section, s->name);
        string_append(code->data_section, ": ");
        char* type = get_type_by_size(s->type->size);
        string_append(code->data_section, type);
        string_append(code->data_section, "0\n");
        break;
    
    default:
        break;
    }
}

asm_code* code_gen(node* ast){
    asm_code* code = (asm_code*)malloc(sizeof(asm_code));
    code->data_section =    init_string(256);
    string_append(code->data_section, ".data\n");
    code->bss_section =     init_string(256);
    string_append(code->bss_section, ".bss\n");
    code->text_section =    init_string(256);
    string_append(code->text_section, ".text\n.global _start\n_start:\n");

    for(unsigned int i = 0; i < ast->children->len; ++i){
        node* child = (node*)(vector_get(ast->children,i));
        node_gen(child, code);
    }


    return code;
}

