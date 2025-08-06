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

    instruction_t* ins = malloc(sizeof(instruction_t)); 
    *ins = (instruction_t){
        .opcode = MOV,
        .operands = {
            (operand_t){
                .optype = REGISTER,
                .value = {
                    .reg = R1
                }
            },
            (operand_t){
                .optype = CONSTANT,
                .value = {
                    .const_value = "42"
                }
            }
        }
    };
    vector_push(ir->instructions, ins);
    operand_t* ptr = malloc(sizeof(operand_t));
    *ptr =  (operand_t){
                .optype = REGISTER,
                .value = {
                    .reg = R4
                }
            };
    instruction_t* in2 = malloc(sizeof(instruction_t)); 
    *in2 = (instruction_t){
        .opcode = MOV,
        .operands = {
            (operand_t){
                .optype = REGISTER,
                .value = {
                    .reg = R2
                }
            },
            (operand_t){
                .optype = POINTER,
                .value = {
                    .address = (struct operand_t*)ptr
                }
            }
        }
    };
    vector_push(ir->instructions, in2);

    return ir;
}