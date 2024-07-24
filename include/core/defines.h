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

#define QUARZUM_COMPILER_VERSION "1.0"
#define QUARZUM_VERSION "1.0"
#define VERSION_TYPE "experimental"

// Signed integers
typedef char int8;
typedef short int16;
typedef int int32;
typedef long int64;
// Unsigned integers
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;
// boolean
typedef uint8 bool;

#ifndef NULL
#define NULL ((void*)0)
#endif

// process flags
typedef uint8 process_flags;
#define PROCESS_START   0b00000001
#define PROCESS_PAUSE   0b00000010
#define PROCESS_RUNNING 0b00000100
#define PROCESS_END     0b00001000
#define MAKE_PROCESS process_flags __flags

#endif