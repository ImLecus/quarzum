/**
 * \file            ir.c
 * \brief           Quarzum Intermediate Representation
 * \copyright       2024 Marcos González GPL3
 */
#include "ir.h"

static void convert_if_stmt(node_t* node, vector_t* instructions){

}

static void convert_function(node_t* node, vector_t* instructions){
    
}

ir_t* ast_to_ir(ast_t* ast){
    ir_t* ir = malloc(sizeof(ir_t));
    if(ir == NULL) return NULL;
    ir->data = init_vector(32);
    ir->instructions = init_vector(32);

    

    return ir;
}