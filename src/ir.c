#include "quarzum.h"

static string* last_literal;

instruction* init_instruction(int type, char* dest, char* arg1, char* arg2, void* data){
    instruction* i = malloc(sizeof(instruction));
    i->type = type;
    i->dest = dest;
    i->arg1 = arg1;
    i->arg2 = arg2;
    i->data = data;
    return i;
}

static char* get_index(char type, uint32_t index){
    char buffer[10];
    sprintf(buffer,".%c%d",type,index);
    return strdup(buffer);
}

static void generate_instruction(vector* ir_list,node* n){
    static uint32_t b_index = 0; // branch index
    static uint32_t t_index = 0; // temporal expression index 
    static uint32_t c_index = 0; // close branch index
    static uint32_t s_index = 0; // string index
    if(!n){
        printf(ERROR_MSG("no node?"));
        return;
    }
    switch (n->type)
    {
    case N_CALL_EXPR:
        
        for(uint32_t i = 1; i < n->children->len; ++i){
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
        vector_push(ir_list, 
            init_instruction(I_MOV, "%rax", get_index('t', t_index), NULL,NULL)
        );
        last_literal->value = get_index('t', t_index++);
        break;
    case N_NULL_EXPR:
        break;
    case N_LITERAL:
    case N_IDENTIFIER:
        last_literal->value = n->children->value[0];
        break;
    case N_BINARY_EXPR:
        generate_instruction(ir_list,n->children->value[0]);
        char* left = string_copy(last_literal);
        generate_instruction(ir_list,n->children->value[1]);
        char* right = string_copy(last_literal);
        printf("%s + %s\n", left, right);
        vector_push(ir_list, 
            init_instruction(I_ADD, get_index('t', t_index), left, right, NULL)
        ); 
        last_literal->value = get_index('t', t_index++);
        break;

    case N_WHILE:
        node* c = n->children->value[0];

        BRANCH(get_index('b', b_index));
        // eval expr
        vector_push(ir_list, 
            init_instruction(I_NIF, "true", get_index('c', c_index), NULL, NULL)
        );        
        for(uint32_t i = 1; i < n->children->len && n->children->value[i]; ++i){
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
        uint32_t first_c_index = c_index;
        JMP(get_index('c', c_index++));
        BRANCH(get_index('b', b_index++));
        for(uint32_t i = 1; i < n->children->len && n->children->value[i]; ++i){
            generate_instruction(ir_list, n->children->value[i]);
        }
        BRANCH(get_index('c', first_c_index));
        break;


    case N_VAR:
        symbol* sy = n->children->value[0];
        node* expr = n->children->value[1];
        if(sy->scope == S_LOCAL){
            break;
        }
        if(expr->type == N_LITERAL){
            char* value = expr->children->value[0];
            printf("ASSIGN %s, %s (%d)\n",sy->name,value, sy->type->size);
            vector_push(ir_list, 
                init_instruction(I_ASSIGN, sy->name, value, NULL,sy->type)
            );
        }
        else{
            generate_instruction(ir_list, expr);
            vector_push(ir_list, 
                init_instruction(I_ASSIGN, sy->name, "0", NULL,sy->type)
            );
        }
        break;

    case N_ENUM:
        break;

    case N_CALL:
        for(uint32_t i = 1; i < n->children->len; ++i){
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
        symbol* s = (symbol*)(n->children->value[0]);
        if((s->type->flags & FOREIGN_FLAG) > 0){
            return;
        }
        function_info* info = s->type->info;
        int size = info->align * info->local_variables_len;
        char* size_str = malloc(3);
        sprintf(size_str,"%d",size);
        vector_push(ir_list, 
            init_instruction(I_FUNCTION,s->name,size_str, NULL,info->local_variables)
        );
        
        for(uint32_t i = 1; i < n->children->len; ++i){
            // TO-DO: check if it's a parameter (symbol*) or a child (node*)
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
    last_literal = init_string(10);
    vector* list = init_vector(INSTRUCTION_LIST_DEFAULT_SIZE);
    for(uint32_t i = 0; i < ast->children->len; ++i){
        generate_instruction(list,(node*)(vector_get(ast->children,i)));
    }
    return list;
}