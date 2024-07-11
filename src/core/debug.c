/*
 * Quarzum Compiler - debug.c
 * Version 1.0, 08/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#include "../../include/core/debug.h"


void print(const char* message){
    printf(CYAN "[LOG]" RESET " %s\n",message);
}

void debug(const char* message){
    if(1){//flags & debugMode == debugMode){
        printf(ORANGE "[DEBUG]" RESET " %s\n",message);
    }
}

void warn(const char* message){
    if(flags & noWarningMessages != noWarningMessages){
        printf(YELLOW "[WARNING]" RESET " %s\n",message);
    }
}

void err(const char* message, unsigned char fatal){
    printf(RED "[ERROR]" RESET " %s\n",message);
}