/**
 * \file            process.h
 * \brief           Process include file.
 * \copyright       2024 Marcos González GPL3
 */
#ifndef PROCESS_H
#define PROCESS_H
#include "core.h"
typedef struct {
    char* name;
    clock_t start, end;
} process_t;

process_t start_process(char* name);
void end_process(process_t process);

#endif /* PROCESS_H */
