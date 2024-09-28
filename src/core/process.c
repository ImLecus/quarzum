/**
 * \file            process.c
 * \brief           Process implementation file.
 * \copyright       2024 Marcos González GPL3
 */
#include "process.h"

inline struct process start_process(char* name){
    return (struct process){
        name, clock()
    };
}

inline void end_process(struct process* process){
    process->end = clock();
    char buffer[100];
    sprintf(buffer,"%s finished in %f seconds", process->name,  (float)(process->end - process->start)/CLOCKS_PER_SEC);
    printf(DEBUG_MSG(buffer));
}