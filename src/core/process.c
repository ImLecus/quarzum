/*
 * Quarzum Compiler - process.c
 * Version 1.0, 08/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#include "../../include/core/process.h"

Process* startProcess(const char* name){
    Process* process = (Process*) malloc(sizeof(Process));
    process->name = name;
    process->start = clock();
    return process;
}

void endProcess(Process* process){
    process->end = clock();
    printf("%s finished in %f microseconds.\n", process->name, 
    (float)(process->end - process->start)*1000000/CLOCKS_PER_SEC);
    free(process);
}