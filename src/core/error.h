// File: error.h
// Description: Quarzum compiler definitions for errors, warnings
// logs and debug messages.
#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>

#define MAX_ERRORS 100

typedef struct {
    uint32_t line;
    char message[256];
} error_t;

extern error_t ERROR_LIST[MAX_ERRORS];
extern int error_count;

// Throws a compilation error into ERROR_LIST. Only stores one
// error per line.
void throw_error(uint32_t line, uint32_t column, char* message);
// Prints all errors inside ERROR_LIST without modifying the list.
void print_errors();

#endif
