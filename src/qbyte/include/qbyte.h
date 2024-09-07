/**
 * Main QByte header file.
 */
#ifndef QBYTE_H
#define QBYTE_H

#include "bytecode.h"
#include <stdlib.h>

typedef struct {
    qbyte_function_t* functions;
    uint64_t functions_count;

    qbyte_data_t* data;
    uint64_t data_count;

    qbyte_instruction_t* instructions;
    uint64_t instructions_count;
} qbyte_code_t;

void qbyte_init();

void qbyte_add_function(qbyte_function_t func);

void qbyte_add_data(qbyte_data_t data);

void qbyte_add_instruction(qbyte_instruction_t inst);

void qbyte_free_bytecode();

int qbyte_writefile(const char* where);

#endif