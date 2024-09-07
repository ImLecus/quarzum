/**
 * QByte Bytecode header file.
 */
#ifndef BYTECODE_H
#define BYTECODE_H
#include <stdint.h>
typedef unsigned char byte_t;

enum { 
    OPCODE_LOAD
};

typedef struct {
    byte_t opcode;
    byte_t operands;
} qbyte_instruction_t;

typedef struct {
    byte_t id;
    byte_t params;
    unsigned int offset;
    byte_t local_vars;
} qbyte_function_t;

typedef struct {
    byte_t type;
    unsigned int length;
    char* content;
} qbyte_data_t;

#endif