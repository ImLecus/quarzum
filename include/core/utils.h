/*
 * Quarzum Compiler - utils.h
 * Version 1.0, 24/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#ifndef UTILS_H
#define UTILS_H

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "debug.h"
#include "utils.h"

//
//   Global variables
//

#define QUARZUM_COMPILER_VERSION "1.0"
#define QUARZUM_VERSION "1.0"
#define VERSION_TYPE "experimental"

//
// Booleans
//
typedef int bool;
#define true 1
#define false 0

//
//  Process related
//


//
// Binary search algorithm
//
int binary_search(const char* symbol, const char** list, unsigned int size);

#endif