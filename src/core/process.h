/**
 * \file            process.h
 * \brief           Process include file.
 * \copyright       2024 Marcos González GPL3
 */
#ifndef PROCESS_H
#define PROCESS_H
#include "core.h"
typedef struct Process {
    const char* name;
    clock_t start, end;
} Process;

Process start_process(const char* name);
void end_process(Process process);

#endif /* PROCESS_H */
