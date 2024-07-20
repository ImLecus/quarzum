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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PATH_MAX 4096

static char PATH[PATH_MAX];
/**
 * @brief Returns the current working directory.
 */
char* getCWD();
/**
 * @brief Converts a relative path into an absolute path.
 */
char* resolvePath(const char* relativePath);

#endif