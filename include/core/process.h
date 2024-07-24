/*
 * Quarzum Compiler - process.h
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
#ifndef PROCESS_H
#define PROCESS_H
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "debug.h"
#include "defines.h"

/**
 * @brief Defines a process.
 * Must have an end to avoid memory leaks.
 */
typedef struct {
    const char* name;
    int64 start;
    int64 end;
} Process;

/**
 * @brief Creates a `Process`.
 */
Process* start_process(const char* name);
/**
 * @brief Ends a `Process` and prints a debug message with the time interval.
 */
void end_Process(Process* process);

#endif