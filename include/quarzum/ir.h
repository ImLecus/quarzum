/*
 * Quarzum Compiler - ir.h
 * Version 1.0, 10/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#ifndef IR_H
#define IR_H

typedef enum {
    Call,
    Sum,
    Sub,
    Mul,
    Div,
    Mod,
    And,
    Or,
    Xor,
    CmpEq,
    CmpNeq,
    CmpGr,
    CmpGrEq,
    CmpLw,
    CmpLwEq,
    Assign,
    Load,
    Store,
    AssignConst,
    Jump,
    Return
} InstructionType;

typedef struct {
    InstructionType type;
    void*[5]        args;
} Instruction;

static unsigned int branchIndex;
static unsigned int temporalIndex;

typedef struct {
    Instruction** list;
} InstructionList;

InstructionList* createInstructionList();
void deleteInstructionList();
void addInstruction(InstructionList* list, Instruction* i);


#endif