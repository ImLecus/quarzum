#include "quarzum.h"

instruction* init_instruction(int type, char* dest, char* arg1, char* arg2, void* data){
    instruction* i = (instruction*)malloc(sizeof(instruction));
    i->type = type;
    i->dest = dest;
    i->arg1 = arg1;
    i->arg2 = arg2;
    i->data = data;
    return i;
}

static char* get_index(char type, unsigned int index){
    char buffer[10];
    sprintf(buffer,".%c%d",type,index);
    return strdup(buffer);
}

static void generate_instruction(vector* ir_list,node* n){
    static unsigned int b_index = 0;
    static unsigned int t_index = 0;
    static unsigned int c_index = 0;

    switch (n->type)
    {
    case N_IF:
        // operate the condition
        // compare the result with true (1)
        // je b(x)
        // [else]
        // jmp c(x)x
        // b(x):
        // [if]
        // c(x):


        node* condition = n->children->value[0];
        printf("IF [condition] JMP b0\n");
        vector_push(ir_list, 
            init_instruction(I_IF, "true", get_index('b', b_index), NULL, NULL)
        );
        // else zone

        vector_push(ir_list, 
            init_instruction(I_JMP, get_index('c', c_index), NULL , NULL, NULL)
        );
        vector_push(ir_list, 
            init_instruction(I_BRANCH, get_index('b', b_index), NULL , NULL, NULL)
        );
        printf("JMP c0\nb0:\n");
        for(unsigned int i = 1; i < n->children->len; ++i){
            generate_instruction(ir_list, n->children->value[i]);
        }
        vector_push(ir_list, 
            init_instruction(I_BRANCH, get_index('c', c_index), NULL , NULL, NULL)
        );
        printf("c0:\n");
    break;


    case N_VAR:
        symbol* sy = n->children->value[0];
        node* expr = n->children->value[1];
        if(expr->type == N_LITERAL){
            char* value = expr->children->value[0];
            printf("ASSIGN %s, %s (%d)\n",sy->name,value, sy->type->size);
            vector_push(ir_list, 
                init_instruction(I_ASSIGN, sy->name, value, NULL,sy->type)
            );
        }
        break;
    
    case N_CALL:
        for(unsigned int i = 1; i < n->children->len; ++i){
            node* param = (node*)(n->children->value[i]);
            char* param_name = param->children->value[0];
            if(param_name[0] == '"'){
                printf("ASSIGN _s0, %s\n",param_name);
                vector_push(ir_list, 
                    init_instruction(I_ASSIGN, "_s0", param_name, NULL,ty_string)
                );
                printf("PARAM %s\n","$_s0");
                vector_push(ir_list, 
                    init_instruction(I_PARAM, "$_s0", NULL, NULL,NULL)
                );
                break;
            }

            printf("PARAM %s\n",param->children->value[0]);
            vector_push(ir_list, 
                init_instruction(I_PARAM, param->children->value[0], NULL, NULL,NULL)
            );
        }
        printf("CALL %s\n",n->children->value[0]);
        vector_push(ir_list, 
            init_instruction(I_CALL, n->children->value[0], NULL, NULL,NULL)
        );

        break;

    case N_FUNCTION:
        symbol* s =  (symbol*)(n->children->value[0]);
        printf("FUNCTION %s\n", s->name);
        vector_push(ir_list, 
            init_instruction(I_FUNCTION,s->name, NULL, NULL,NULL)
        );
        for(unsigned int i = 1; i < n->children->len; ++i){
            generate_instruction(ir_list,(node*)(vector_get(n->children,i)));
        }
        vector_push(ir_list, 
            init_instruction(I_LEAVERET,NULL, NULL, NULL,NULL)
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