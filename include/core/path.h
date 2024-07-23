/*
 * Quarzum Compiler - path.h
 * Version 1.0, 19/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#ifndef PATH_H
#define PATH_H
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define PATH_MAX 4096
#define LIBRARY_PATH "/usr/lib/quarzum/"
#define LIB_PATH_LEN 18

static char PATH[PATH_MAX];
/**
 * @brief Returns the current working directory.
 */
char* getCWD();
/**
 * @brief Detects when a route starting with '@' and
 * transforms it into the library route. Also removes the
 * quotes.
 */
char* resolvePath(char* path);

#endif