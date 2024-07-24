/*
 * Quarzum Compiler - defines.h
 * Version 1.0, 23/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#ifndef DEFINES_H
#define DEFINES_H
#include "stdlib.h"
#define QUARZUM_COMPILER_VERSION "1.0"
#define QUARZUM_VERSION "1.0"
#define VERSION_TYPE "experimental"

#define _create_object(object) (object)malloc(sizeof(object))

#define _create_array(object, size) (object*)malloc(sizeof(object) * size)

// boolean
typedef unsigned char bool;

#endif