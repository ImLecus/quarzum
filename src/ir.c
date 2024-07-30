#include "quarzum.h"

static instruction* generate_instruction(node* n){
    switch (n->type)
    {
    
    default:
        break;
    }

    return NULL;
}

vector* generate_ir(node* ast){
    vector* list = init_vector(INSTRUCTION_LIST_DEFAULT_SIZE);
    for(unsigned int i = 0; i < ast->children->len; ++i){
        vector_push(list, 
            generate_instruction(
                (node*)(vector_get(ast->children,i))
            )
        );
    }
    return list;
}