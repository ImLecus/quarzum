/*
 * Quarzum Compiler - cli.h
 * Version 1.0, 09/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#ifndef CLI_H
#define CLI_H
#include "../toolchain/compile.h"
#include "../core/bsearch.h"
#include "../core/debug.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#define FLAG_NUMBER 12

void printUsage();

void CLI(int argc, char** argv);

static const char* flagNames[FLAG_NUMBER] = {
    "-h",
    "--help",
    "-d",
    "--debug",
    "-v",
    "--version",
    "-a",
    "--arch",
    "-o",
    "--output",
    "--no-warnings",
    "--show-tokens"
};

#endif