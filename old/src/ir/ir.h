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
    IMUL,
    MUL,
    IDIV,
    DIV,
    MOD,
    PUSH,
    POP,
    CALL,
    RET,
    JMP,
    LABEL,
    SYSCALL
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
    REP,
    RINVALID = 255
} reg_t;

typedef struct {
    enum {
        REGISTER,
        POINTER,
        CONSTANT,
        VARIABLE
    } optype;
    enum {
        S_BYTE,
        S_WORD,
        S_LONG,
        S_QUAD,
        S_ZERO
    } size; 
    union {
        char* varname;
        reg_t reg;
        struct operand_t* address;
        char* const_value;
    } value;
} operand_t;

typedef struct {
    opcode_t opcode;
    operand_t operands[3];
} instruction_t;

typedef struct {
    Vector* data;
    Vector* instructions;
} ir_t;

ir_t* ast_to_ir(AST* ast);

#endif /* IR_H */