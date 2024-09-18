/**
 * QByte Bytecode header file.
 */
#ifndef BYTECODE_H
#define BYTECODE_H
#include <stdint.h>
typedef unsigned char byte_t;

typedef enum { 
    O_LOAD,
    O_STORE,
    O_MOV,
    O_JMP,
    O_JE,
    O_JNE,
    O_JG,
    O_JGE,
    O_JL,
    O_JLE,
    O_ADD,
    O_SUB,
    O_MUL,
    O_DIV,
    O_MOD,
    O_FUNC,
    O_CALL,
    O_RET,
    O_CMP,
    O_NGT,
    O_PUSH,
    O_POP,
    O_AND,
    O_OR,
    O_XOR,
    O_NOT,
    O_ADVANCE,
    O_REF,
    O_DEREF
} opcode_t;

typedef struct {
    opcode_t opcode;
    // mode represents the format of each operand in 2 bits.
    // data = 0b01, constant = 0b10, register = 0b11, stack-base offset = 0b00
    byte_t mode; 
    uint64_t op1, op2, op3;
} qbyte_instruction_t;

typedef struct {
    char* name;
    byte_t params;
    byte_t local_vars;
} qbyte_function_t;

typedef struct {
    char* name;
    byte_t type;
    char* value;
} qbyte_data_t;

#endif