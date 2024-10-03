/**
 * \file            ir.c
 * \brief           Quarzum Intermediate Representation
 * \copyright       2024 Marcos González GPL3
 */
#include "ir.h"

static void convert_if_stmt(Node* node, Vector* instructions){

}

static void convert_function(Node* node, Vector* instructions){
    
}

ir_t* ast_to_ir(AST* ast){
    ir_t* ir = malloc(sizeof(ir_t));
    if(ir == NULL) return NULL;
    ir->data = init_vector(32);
    ir->instructions = init_vector(32);

    

    return ir;
}