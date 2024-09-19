/**
 * \file            file.h
 * \brief           Quarzum I/O include file
 * \copyright       2024 Marcos González GPL3
 */
#ifndef FILE_H
#define FILE_H
#include "string.h"
#include "error.h"

/**
 * \brief           Returns the file extension.
 * \return          The file last extension
 */
char* get_extension(char* file);

/**
 * \brief           Reads a file and returns its content.
 * \returns         A pointer to a `string_t` with the content
 *                  of a file.
 * \param           filename: the target file path
 * \warning         If the file can not be opened, returns `NULL`.
 */
string_t* read_file(char* filename);

/**
 * \brief           Writes content inside a file.
 * \warning         The file must be opened and exist.
 */
void write_file(FILE* file, char* content);

#endif