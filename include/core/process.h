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

/**
 * @brief Defines a process.
 * Must have an end to avoid memory leaks.
 */
typedef struct {
    const char* name;
    long start;
    long end;
} Process;

/**
 * @brief Creates a `Process`.
 */
Process* startProcess(const char* name);
/**
 * @brief Ends a `Process` and prints a debug message with the time interval.
 */
void endProcess(Process* process);

#endif