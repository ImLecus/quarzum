/*
 * Quarzum Compiler - ir.c
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
#include "../../include/quarzum/ir.h"

InstructionList* createInstructionList(){
    InstructionList* list = (InstructionList*)malloc(sizeof(InstructionList));
    list->list = (Instruction**)malloc(DEFAULT_INSTRUCTION_LIST_SIZE*sizeof(Instruction*));
    list->size = 0;
    list->capacity = DEFAULT_INSTRUCTION_LIST_SIZE;
    return list;
}
void deleteInstructionList(InstructionList* list){
    free(list->list);
    free(list);
}
void addInstruction(InstructionList* list, Instruction* i){
    if(list->size >= list->capacity){
        // resize
    }
    list->list[++list->size] = i;
}

void generateIR(Node* node, InstructionList* list){
    switch (node->type)
    {
    case Root:
        for(unsigned long i = 0; i < node->childrenCount; ++i){
            generateIR(node->children[i], list);
        }
        break;
    case CallStmt:
        Instruction i = {iCall, {node->data}};
        addInstruction(list, &i);
        break;
    default:
        break;
    }
}