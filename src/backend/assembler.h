/**
 * \file            assembler.h
 * \brief           Compiler back-end entry point.
 * \copyright       2024 Marcos González GPL3
 */
#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "../core/string.h"
#include "../qbyte/qbyte.h"
/**
 * \brief           Defines the possible compiler target architectures.
 */
typedef enum {
    X86_64,
    X86_32
} architecture_t;

/**
 * \brief           Generates ASM code as a string in the selected 
 *                  target architecture.
 */
char* generate_asm(ir_code_t* ir, architecture_t arch);

/**
 * \brief           Generates ASM code as a string for the 
 *                  x86_64 architecture.
 */
char* x86_64(ir_code_t* ir);

#endif