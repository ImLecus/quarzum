#include "quarzum.h"

instruction* init_instruction(int type, char* dest, char* arg1, char* arg2){
    instruction* i = (instruction*)malloc(sizeof(instruction));
    i->type = type;
    i->dest = dest;
    i->arg1 = arg1;
    i->arg2 = arg2;
    return i;
}

static void generate_instruction(vector* ir_list,node* n){
    switch (n->type)
    {
    case N_CALL:
        for(unsigned int i = 1; i < n->children->len; ++i){
            node* param = (node*)(n->children->value[i]);
            char* param_name = param->children->value[0];
            if(param_name[0] == '"'){
                printf("ASSIGN _s0, %s\n",param_name);
                vector_push(ir_list, 
                    init_instruction(I_ASSIGN, "_s0", param_name, NULL)
                );
                printf("PARAM %s\n","$_s0");
                vector_push(ir_list, 
                    init_instruction(I_PARAM, "$_s0", NULL, NULL)
                );
                break;
            }

            printf("PARAM %s\n",param->children->value[0]);
            vector_push(ir_list, 
                init_instruction(I_PARAM, param->children->value[0], NULL, NULL)
            );
        }
        printf("CALL %s\n",n->children->value[0]);
        vector_push(ir_list, 
            init_instruction(I_CALL, n->children->value[0], NULL, NULL)
        );

        break;

    case N_FUNCTION:
        symbol* s =  (symbol*)(n->children->value[0]);
        printf("FUNCTION %s\n", s->name);
        vector_push(ir_list, 
            init_instruction(I_FUNCTION,s->name, NULL, NULL)
        );
        for(unsigned int i = 1; i < n->children->len; ++i){
            generate_instruction(ir_list,(node*)(vector_get(n->children,i)));
        }
        vector_push(ir_list, 
            init_instruction(I_LEAVERET,NULL, NULL, NULL)
        );
        break;
    
    default:
        break;
    }
}

vector* generate_ir(node* ast){
    vector* list = init_vector(INSTRUCTION_LIST_DEFAULT_SIZE);
    for(unsigned int i = 0; i < ast->children->len; ++i){
        generate_instruction(list,(node*)(vector_get(ast->children,i)));
    }
    return list;
}