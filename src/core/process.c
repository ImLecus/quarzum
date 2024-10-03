/**
 * \file            process.c
 * \brief           Process implementation file.
 * \copyright       2024 Marcos González GPL3
 */
#include "process.h"

inline process_t start_process(char* name){
    return (process_t){
        name, clock()
    };
}

inline void end_process(process_t process){
    process.end = clock();
    char buffer[100];
    sprintf(buffer,MAGENTA"[DEBUG] " RESET"%s finished in %f seconds\n", process.name,  (float)(process.end - process.start)/CLOCKS_PER_SEC);
    printf(buffer);
}