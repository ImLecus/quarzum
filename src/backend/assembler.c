/**
 * \file            assembler.c
 * \brief           Backend entry point.
 * \copyright       2024 Marcos González GPL3
 */
#include "assembler.h"

char* generate_asm(ir_code_t* ir, architecture_t arch){
    switch (arch)
    {
    case X86_64: return x86_64(ir);
    
    default: return NULL;
    }
}