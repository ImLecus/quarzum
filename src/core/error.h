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

extern error_t WARNING_LIST[MAX_ERRORS];
extern int warning_count;

// Throws a compilation error into ERROR_LIST. Only stores one
// error per line.
void throw_error(char* file, uint32_t line, uint32_t column, char* message);
// Throws a warning into WARNING_LIST. Only stores one warning per line.
void throw_warning(char* file, uint32_t line, uint32_t column, char* message);
// Prints all errors inside ERROR_LIST without modifying the list.
// Prints all the warnings inside WARNING_LIST too, eliminating them
// once they appear in console.
void print_errors();

// Prints all the errors and exits the program if error_count > 0
void check_errors();

// Throws a lexical error with the form "Unexpected token '[buffer]'"
void unexpected_token_err(char* file, uint32_t line, uint32_t column, char* buffer);
// Throws a lexical error with the form "Too many decimal points on numeric literal"
void invalid_decimal_err(char* file, uint32_t line, uint32_t column);

void expected_token_err(char* file, uint32_t line, uint32_t column, char* expected);

void duplicated_type_err(char* file, uint32_t line, uint32_t column, char* typename);

void undefined_type_err(char* file, uint32_t line, uint32_t column, char* typename);

void duplicated_flag_warning(char* file, uint32_t line, uint32_t column, char* flag);

void duplicated_symbol_err(char* file, uint32_t line, uint32_t column, char* symbol);

#endif
