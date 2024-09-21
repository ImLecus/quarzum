/**
 * \file            process.h
 * \brief           Process include file.
 * \copyright       2024 Marcos González GPL3
 */
#ifndef PROCESS_H
#define PROCESS_H
#include "core.h"
struct process {
    char* name;
    int start, end;
};

struct process start_process(char* name);
void end_process(struct process* process);

#endif /* PROCESS_H */
