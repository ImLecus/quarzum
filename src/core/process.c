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

inline Process* start_process(const char* name){
    Process* process = (Process*) malloc(sizeof(Process));
    process->name = name;
    process->start = clock();
    return process;
}

inline void end_process(Process* process){
    process->end = clock();
    char buffer[100];
    sprintf(buffer,"%s finished in %f seconds.", process->name,  (float)(process->end - process->start)/CLOCKS_PER_SEC);
    debug(buffer);
    free(process);
}