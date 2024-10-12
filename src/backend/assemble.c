/**
 * \file            assemble.c
 * \brief           Quarzum Compiler Backend entry point
 * \copyright       2024 Marcos González GPL3
 */
#include "assemble.h"

int assemble(ir_t* ir, arch_t arch){
    if(arch == X86_64){
        return assemble_x86_64(ir);
    }
    return 0;
}