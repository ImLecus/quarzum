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
const char* get_extension(const char* file);

/**
 * \brief           Reads a file and returns its content.
 * \returns         A pointer to a `String` with the content
 *                  of a file.
 * \param           filename: the target file path
 * \warning         If the file can not be opened, returns `NULL`.
 */
String* const read_file(const char* const filename);

/**
 * \brief           Writes content inside a file.
 * \warning         The file must be opened and exist.
 */
void write_file(FILE* file, char* const content);

#endif