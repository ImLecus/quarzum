/**
 * \file            process.c
 * \brief           Process implementation file.
 * \copyright       2024 Marcos González GPL3
 */
#include "process.h"

inline Process start_process(const char* name){
    return (Process){
        name, clock()
    };
}

inline void end_process(Process process){
    process.end = clock();
    char buffer[100];
    sprintf(buffer,MAGENTA"[DEBUG] " RESET"%s finished in %f seconds\n", process.name,  (float)(process.end - process.start)/CLOCKS_PER_SEC);
    printf(buffer);
}