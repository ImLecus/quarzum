/*
 * Quarzum Compiler - config.h
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
#ifndef CONFIG_H
#define CONFIG_H

#define noWarningMessages 0x001
#define debugMode         0x010
#define showTokens        0x100

static int flags = 0;
static char* architecture = "x86_64";
static char* outputFileName = "program";

#endif