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
#include <stdlib.h>
#include "ast.h"
/**
 * @brief Defines the instruction set in the intermediate representation.
 */
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
    Ret
} InstructionType;
/**
 * @brief Defines an instruction with 5 arguments.
 */
typedef struct {
    InstructionType type;
    void*           args[5];
} Instruction;
/**
 * @brief Defines the branchIndex.
 * When a branch is generated, the index will grow.
 * If the branch ends, the index will NOT return to its original value.
 */
static unsigned int branchIndex = 0;
/**
 * @brief Defines the temporalIndex.
 * When a large operation is defined, results must be temporally stored
 * in a register. temporalIndex defines which is the first free temporal
 * register to use. When the operation ends, the index returns to 0.
 */
static unsigned int temporalIndex = 0;
/**
 * @brief Defines an `Instruction` list. Used by the asm code generator
 * as its input.
 */
typedef struct {
    Instruction** list;
    unsigned long size;
    unsigned long capacity;
} InstructionList;

#define DEFAULT_INSTRUCTION_LIST_SIZE 50
/**
 * @brief Creates a new `InstructionList` with size `DEFAULT_INSTRUCTION_LIST_SIZE`.
 */
InstructionList* createInstructionList();
/**
 * @brief Deletes an `InstructionList`.
 */
void deleteInstructionList(InstructionList* list);
/**
 * @brief Adds a new element to an `InstructionList`. 
 * The list will be reallocated if it's full.
 */
void addInstruction(InstructionList* list, Instruction* i);
/**
 * @brief Reads an AST generating zero or more `Instruction`
 * per `Node`, adding them to an `InstructionList`.
 */
void generateIR(Node* ast, InstructionList* list);

#endif