/**
 * \file            ir.h
 * \brief           Quarzum Intermediate Representation include file
 * \copyright       2024 Marcos González GPL3
 */
#ifndef IR_H
#define IR_H
#include "../core/vector.h"
#include "../frontend/parse.h"

typedef enum {
    MOV,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    PUSH,
    POP,
    CALL,
    RET,
    JMP
} opcode_t;

typedef enum {
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
    RSB,
    REP
} reg_t;

typedef struct {
    vector_t* data;
    vector_t* instructions;
} ir_t;

ir_t* ast_to_ir(ast_t* ast);

#endif /* IR_H */