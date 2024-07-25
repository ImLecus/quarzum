/*
 * Quarzum Compiler - main.c
 * Version 1.0, 11/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#include "include/quarzum.h"
#include "quarzum.h"

int main(int argc, char** argv) {
    flags |= debugMode;
    compile(argv[1]);
    return 0;
}