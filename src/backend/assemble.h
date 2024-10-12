/**
 * \file            assemble.h
 * \brief           Quarzum Compiler back-end entry point
 * \copyright       2024 Marcos González GPL3
 */
#ifndef ASSEMBLE_H
#define ASSEMBLE_H
#include "x86_64.h"
#include "../ir/ir.h"


typedef enum {
    X86_64,
    X86_32
} arch_t;

int assemble(ir_t* ir, arch_t arch);

#endif /* ASSEMBLE_H */