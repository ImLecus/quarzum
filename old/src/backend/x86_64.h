/**
 * \file            x86_64.h
 * \brief           QByte to x86_64 asm include file.
 * \copyright       2024 Marcos González GPL3  
 */
#ifndef ASM_X86_64_H
#define ASM_X86_64_H
#include "../ir/ir.h"
#include "assemble.h"

static char* registers[6] = {"rax", "rdi", "rsi","rcx", "rdx", "rbx"};

int assemble_x86_64(ir_t* ir);

#endif