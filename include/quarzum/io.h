/*
 * Quarzum Compiler - io.h
 * Version 1.0, 06/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#ifndef IO_H
#define IO_H
#include <stdio.h>
#include "../core/buffer.h"

/**
 * @brief Returns the content of a file as a `Buffer`.
 */
Buffer* readFile(const char* filename);
/**
 * @brief Creates a new file in read+write mode.
 */
FILE* createFile(const char* filename);
/**
 * @brief Writes content in a file.
 */
void writeFile(FILE* file, char* content);

#endif
