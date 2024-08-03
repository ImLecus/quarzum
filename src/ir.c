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
    static unsigned int b_index = 0; // branch index
    static unsigned int t_index = 0; // temporal expression index 
    static unsigned int c_index = 0; // close branch index
    static unsigned int s_index = 0; // string index
    if(!n){
        printf(ERROR_MSG("no node?"));
        return;
    }
    switch (n->type)
    {
    case N_WHILE:
        node* c = n->children->value[0];

        BRANCH(get_index('b', b_index));
        // eval expr
        vector_push(ir_list, 
            init_instruction(I_NIF, "true", get_index('c', c_index), NULL, NULL)
        );        
        for(unsigned int i = 1; i < n->children->len && n->children->value[i]; ++i){
            generate_instruction(ir_list, n->children->value[i]);
        }
        JMP(get_index('b', b_index++));
        BRANCH(get_index('c', c_index++));

        break;
    case N_IF:
        node* condition = n->children->value[0];
        // eval expr
        vector_push(ir_list, 
            init_instruction(I_IF, "true", get_index('b', b_index), NULL, NULL)
        );
        // else zone
        unsigned int first_c_index = c_index;
        JMP(get_index('c', c_index++));
        BRANCH(get_index('b', b_index++));
        for(unsigned int i = 1; i < n->children->len && n->children->value[i]; ++i){
            generate_instruction(ir_list, n->children->value[i]);
        }
        BRANCH(get_index('c', first_c_index));
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
                vector_push(ir_list, 
                    init_instruction(I_ASSIGN, get_index('s',s_index), param_name, NULL,ty_string)
                );
                vector_push(ir_list, 
                    init_instruction(I_PARAM,get_index('s',s_index++), NULL, NULL,NULL)
                );
                break;
            }
            vector_push(ir_list, 
                init_instruction(I_PARAM, param->children->value[0], NULL, NULL,NULL)
            );
        }
        vector_push(ir_list, 
            init_instruction(I_CALL, n->children->value[0], NULL, NULL,NULL)
        );

        break;

    case N_FUNCTION:
        symbol* s =  (symbol*)(n->children->value[0]);
        if((s->type->flags & FOREIGN_FLAG) > 0){
            return;
        }
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